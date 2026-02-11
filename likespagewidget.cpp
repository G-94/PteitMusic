#include "likespagewidget.h"

LikesPageWidget::LikesPageWidget(QWidget *parent)
    : QWidget{parent}
{
    QDir dir(likesDir);
    if(!dir.exists()) {
        QDir().mkpath(likesDir);
        dir.mkdir(".");
        qDebug() << "Created likes directory:" << likesDir;
    } else {
        qDebug() << "Likes directory exists:" << likesDir;
    }

    main_layout = new QVBoxLayout(this);

    page_discription = new QLabel("There is your liked songs");
    main_layout->addWidget(page_discription);

    tracklist_widget = new ScrollableTrackList();
    main_layout->addWidget(tracklist_widget);

    loadLikesSongs();

    QObject::connect(tracklist_widget, &ScrollableTrackList::setSongRequset, [this] (int songID) {
        emit setPlaySong(MusicGlobal::current_liked_tracklist, songID);
    });

    QObject::connect(tracklist_widget, &ScrollableTrackList::setUnlikeSong, [this] (int songID) {
        Song song = tracklist_widget->getTracklist()[songID];
        removeSongFromLikes(song.at("id"));
    });
}

void LikesPageWidget::addSongToLikes(const Song &song)
{
    try {
        for(const Song& existing_song : MusicGlobal::current_liked_tracklist) {
            if(song.at("id") == existing_song.at("id")) {
                qDebug() << "Song already in likes";
                return;
            }
        }

        MusicGlobal::current_liked_tracklist.push_back(song);
        tracklist_widget->updateTracklist(MusicGlobal::current_liked_tracklist);
        saveLikedSongsToFile();

        emit likesSongUpdated(MusicGlobal::current_liked_tracklist);
    } catch (...) {
        qDebug() << "something went wrong while adding song to Likes";
    }
}

void LikesPageWidget::removeSongFromLikes(const std::string &songID)
{
    try {
        auto it = std::remove_if(MusicGlobal::current_liked_tracklist.begin(), MusicGlobal::current_liked_tracklist.end(),
                                 [&songID] (Song& song) {return song.at("id") == songID;});

        if(it != MusicGlobal::current_liked_tracklist.end()) {
            MusicGlobal::current_liked_tracklist.erase(it);
            tracklist_widget->updateTracklist(MusicGlobal::current_liked_tracklist);
            saveLikedSongsToFile();
            emit likesSongUpdated(MusicGlobal::current_liked_tracklist);

            qDebug() << "Song removed from likes";
        }

    } catch (...) {
        qDebug() << "error while removing song from likes";
    }
}

void LikesPageWidget::loadLikesSongs()
{
    QFile file(likesPath);

    if(!file.exists()) {
        MusicGlobal::current_liked_tracklist.clear();
        tracklist_widget->setTracklist(MusicGlobal::current_liked_tracklist);
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "failed to read json";
        return;
    }

    QTextStream in(&file);
    QString jsonStr = in.readAll();
    file.close();

    try {
        json j = json::parse(jsonStr.toStdString());

        MusicGlobal::current_liked_tracklist.clear();

        for(const auto& item : j) {
            if(item.is_object()) {
                Song song = jsonToSong(item);
                MusicGlobal::current_liked_tracklist.push_back(song);
            }
        }

        tracklist_widget->setTracklist(MusicGlobal::current_liked_tracklist);
        qDebug() << "loaded" << MusicGlobal::current_liked_tracklist.size() << "liked songs";
    } catch (const json::exception& e) {
        qDebug() << e.what();
    } catch (const std::exception& e) {
        qDebug() << e.what();
    }
}

std::vector<Song> LikesPageWidget::getLikedSongs()
{
    return MusicGlobal::current_liked_tracklist;
}

void LikesPageWidget::saveLikedSongsToFile()
{
    try {
        json j = json::array();

        for(const auto& song : MusicGlobal::current_liked_tracklist) {
            j.push_back(songToJson(song));
        }

        std::string jsonStr = j.dump(4);

        QFile file(likesPath);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "openning file error in saveLikedSongsToFile";
            return;
        }

        QTextStream out(&file);
        out << QString::fromStdString(jsonStr);
        file.close();

        qDebug() << "Saved" << MusicGlobal::current_liked_tracklist.size() << "songs to likes file";
    } catch (...) {
        qDebug() << "error while saving liked songs to file";
    }
}

Song LikesPageWidget::jsonToSong(const json &j)
{
    Song song;

    if(j.is_object()) {
        for(auto it = j.begin(); it != j.end(); ++it) {
            song[it.key()] = it.value().get<std::string>();
        }
    }

    return song;
}

json LikesPageWidget::songToJson(const Song &song)
{
    json j;

    for(const auto& [key, value] : song) {
        j[key] = value;
    }

    return j;
}
