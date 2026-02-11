#include "downloadpagewidget.h"

DownloadPageWidget::DownloadPageWidget(QWidget *parent)
    : QWidget{parent}
{
    QDir dir(downloadsDir);
    if(dir.exists()) {
        dir.mkdir(".");
    }

    main_layout = new QVBoxLayout(this);

    page_description = new QLabel("This is downloads list");
    main_layout->addWidget(page_description);

    tracklist_widget = new ScrollableTrackList();

}

void DownloadPageWidget::addSongToDownloads(const Song &song)
{
    for(const auto& downloaded_song : MusicGlobal::current_downloaded_tracklist) {
        if(song.at("id") == downloaded_song.at("id")) {
            qDebug() << "Song already in downloaded";
            return;
        }
    }

    try {
        std::string downloaded_song_path = downloadsDir.toStdString() + "/" + song.at("artist") + " - " + song.at("title");
        std::ofstream song_file(downloaded_song_path, std::ios::binary);
        if(song_file.is_open()) {
            QByteArray song_data = api.downloadSong(QString::fromStdString(song.at("url")));
            song_file.write(song_data.constData(), song_data.size());
            song_file.close();

            MusicGlobal::current_downloaded_tracklist.push_back(song);
            tracklist_widget->updateTracklist(MusicGlobal::current_downloaded_tracklist);
            loadDownloadsSongs();

            emit downloadedSongsUpdated();
        }


    } catch (...) {

    }

}

void DownloadPageWidget::removeSongFromDownloads(const std::string &songID)
{

}

void DownloadPageWidget::loadDownloadsSongs()
{

    QFile file(downloadsJsonPath);

    if(!file.exists()) {
        MusicGlobal::current_downloaded_tracklist.clear();
        tracklist_widget->setTracklist(MusicGlobal::current_downloaded_tracklist);
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "failed to read json";
        return;
    }

    QTextStream in(&file);
    QString strData = in.readAll();
    file.close();

    try {
        json j = json::parse(strData.toStdString());

        for(const auto& item : j) {
            if(item.is_object()) {
                Song song = jsonToSong(item);
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

void DownloadPageWidget::saveDownloadsSongsToJson()
{

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
