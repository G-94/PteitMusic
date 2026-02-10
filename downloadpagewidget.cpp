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
    for(const auto& downloaded_song : current_downloaded_tracklist) {
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

            current_downloaded_tracklist.push_back(song);
            tracklist_widget->updateTracklist(current_downloaded_tracklist, MusicGlobal::current_liked_tracklist, "downloads_list");
        }


    } catch (...) {

    }

}

void DownloadPageWidget::removeSongFromDownloads(const std::string &songID)
{

}

void DownloadPageWidget::loadDownloadsSongs()
{

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
