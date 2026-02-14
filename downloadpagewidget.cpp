#include "downloadpagewidget.h"

DownloadPageWidget::DownloadPageWidget(QWidget *parent)
    : QWidget{parent}
{
    QDir dir(downloadsDir);
    if(!dir.exists()) {
        dir.mkpath(".");
    }

    QFile jsonFile(downloadsJsonPath);
    if(!jsonFile.exists()) {
        if(jsonFile.open(QIODevice::WriteOnly)) {
            jsonFile.write("[]");
            jsonFile.close();
            qDebug() << "Created empty downloads.json";
        }
    }

    main_layout = new QVBoxLayout(this);

    page_description = new QLabel("This is downloads list");
    main_layout->addWidget(page_description);

    tracklist_widget = new ScrollableTrackList();
    main_layout->addWidget(tracklist_widget);

    QObject::connect(tracklist_widget, &ScrollableTrackList::setSongRequset, this, [this] (int ID) {
        emit setPlaySong(MusicGlobal::current_downloaded_tracklist, ID);
    });

    QObject::connect(tracklist_widget, &ScrollableTrackList::setLikeSong, this, [this] (int ID) {
        emit setLikedSong(MusicGlobal::current_downloaded_tracklist, ID);
    });

    QObject::connect(tracklist_widget, &ScrollableTrackList::setUnlikeSong, this, [this] (int ID) {
        emit setUnlikedSong(MusicGlobal::current_downloaded_tracklist, ID);
    });

    QObject::connect(tracklist_widget, &ScrollableTrackList::setDeleteSong, [this] (int index) {
        if(index >= 0 && index < MusicGlobal::current_downloaded_tracklist.size()) {
            std::string real_song_id = MusicGlobal::current_downloaded_tracklist[index].at("id");
            removeSongFromDownloads(real_song_id);
        }
    });

    connect(&deleteWatcher, &QFutureWatcher<bool>::finished, this, [this]() {
        if(deleteWatcher.result()) {
            qDebug() << "File deleted successfully";
        }
    });

    loadDownloadsSongs();

    qDebug() << "download widget created";
}

void DownloadPageWidget::addSongToDownloads(const Song &song)
{
    for(const Song& existing_song : MusicGlobal::current_downloaded_tracklist) {
        if(existing_song.at("id") == song.at("id")) return;
    }

    QString path = downloadsDir + "/" +
                   QString::fromStdString(clearPathName(song.at("artist")) + " - " +
                                          clearPathName(song.at("title")) + ".mp3");

    Song addedSong = song;
    addedSong["path"] = path.toStdString();

    MusicGlobal::current_downloaded_tracklist.push_back(addedSong);
    tracklist_widget->updateTracklist(MusicGlobal::current_downloaded_tracklist);
    saveDownloadsSongsToJson();
    emit downloadedSongsUpdated();

    QFutureWatcher<QByteArray>* watcher = new QFutureWatcher<QByteArray>(this);

    connect(watcher, &QFutureWatcher<QByteArray>::finished, this, [this, watcher, song, path]() {
        QByteArray data = watcher->result();

        if(data.isEmpty()) {
            removeSongFromDownloads(song.at("id"));
        } else {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly)) {
                file.write(data);
                file.close();
                qDebug() << "File saved:" << path;
            }
        }

        watcher->deleteLater();
    });

    watcher->setFuture(QtConcurrent::run([=]() {
        return api.downloadSong(QString::fromStdString(song.at("url")));
    }));
}

void DownloadPageWidget::removeSongFromDownloads(std::string songID)
{
    std::string artist;
    std::string title;

    for(const Song& song : MusicGlobal::current_downloaded_tracklist) {
        if(song.at("id") == songID) {
            artist = song.at("artist");
            title = song.at("title");
            break;
        }
    }

    connect(&deleteWatcher, &QFutureWatcher<bool>::finished, this, [this] () {
        if(deleteWatcher.result()) {
            qDebug() << "file delete succesfully";
        }
    });

    MusicGlobal::current_downloaded_tracklist.erase(
        std::remove_if(MusicGlobal::current_downloaded_tracklist.begin(),
                       MusicGlobal::current_downloaded_tracklist.end(), [this, songID] (const Song& song) {
                           return song.at("id") == songID;
                       }),
        MusicGlobal::current_downloaded_tracklist.end()
        );

    tracklist_widget->updateTracklist(MusicGlobal::current_downloaded_tracklist);
    saveDownloadsSongsToJson();
    emit downloadedSongsUpdated();

    deleteWatcher.setFuture(QtConcurrent::run([=]() {
        QString path = downloadsDir + "/" +
                       QString::fromStdString(clearPathName(artist) + " - " +
                                              clearPathName(title) + ".mp3");

        return QFile::remove(path);
    }));
}

void DownloadPageWidget::loadDownloadsSongs()
{
    QFile file(downloadsJsonPath);

    if(!file.exists()) {
        MusicGlobal::current_downloaded_tracklist.clear();
        tracklist_widget->setTracklist(MusicGlobal::current_downloaded_tracklist);
        return;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "error on open file in loadDownloadsSongs";
        return;
    }

    MusicGlobal::current_downloaded_tracklist.clear();

    QTextStream in(&file);
    QString strData = in.readAll();
    file.close();

    try {
        json j = json::parse(strData.toStdString());

        for(const auto& item : j) {

            bool isExists = false;
            Song song = jsonToSong(item);

            if(item.is_object()) {
                for(const Song& existing_song : MusicGlobal::current_downloaded_tracklist) {
                    if(existing_song.at("id") == song.at("id")) {
                        isExists = true;
                        break;
                    }
                }
            }

            if(!isExists) {
                MusicGlobal::current_downloaded_tracklist.push_back(song);
            }
        }

        tracklist_widget->setTracklist(MusicGlobal::current_downloaded_tracklist);
        qDebug() << "downloaded tracklist build correct";
    } catch (const json::exception& e) {
        qDebug() << "catch json exc in loadDownloadedSongs" << e.what();
    } catch (const std::exception& e) {
        qDebug() << "catch std::exception exc in loadDownloadedSongs" << e.what();
    }

}

void DownloadPageWidget::updateTracklist()
{
    tracklist_widget->updateTracklist(MusicGlobal::current_downloaded_tracklist);
}

void DownloadPageWidget::saveDownloadsSongsToJson()
{
    try {
        json j = json::array();

        std::set<std::string> existingIds;
        for(const auto& song : MusicGlobal::current_downloaded_tracklist) {
            std::string songID = song.at("id");
            if(existingIds.find(songID) == existingIds.end()) {
                existingIds.insert(songID);
                j.push_back(songToJson(song));
            }
        }

        std::string jsonStr = j.dump(4);

        QFile file(downloadsJsonPath);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            qDebug() << "openning file error in saveDownloadsSongsToFile";
            return;
        }

        QTextStream out(&file);
        out << QString::fromStdString(jsonStr);
        file.close();

        qDebug() << "Saved" << MusicGlobal::current_downloaded_tracklist.size() << "songs to downloads file";
    } catch (...) {
        qDebug() << "error while saving downloads songs to file";
    }
}

Song DownloadPageWidget::jsonToSong(const json &j)
{
    Song song;

    if(j.is_object()) {
        for(auto it = j.begin(); it != j.end(); ++it) {
            song[it.key()] = it.value().get<std::string>();
        }
    }

    return song;
}

json DownloadPageWidget::songToJson(const Song &song)
{
    json j;

    for(const auto& [key, value] : song) {
        j[key] = value;
    }

    return j;
}

std::string DownloadPageWidget::clearPathName(const std::string &path)
{
    std::string r = path;
    for(char& c : r) {
        if(c == '/' || c == '\\' || c == ':' || c == '*' ||
            c == '?' || c == '"' || c == '<' || c == '>' || c == '|')
            c = '_';
    }
    return r;

}
