#ifndef DOWNLOADPAGEWIDGET_H
#define DOWNLOADPAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <fstream>

#include <include/nlohmann/json/json.hpp>
#include "scrollabletracklist.h"
#include "musicapi.h"
#include "MusicGlobals.h"

using json = nlohmann::json;
using Song = std::map<std::string, std::string>;

class DownloadPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadPageWidget(QWidget *parent = nullptr);

    void addSongToDownloads(const Song& song);
    void removeSongFromDownloads(const std::string& songID);
    void loadDownloadsSongs();

private:

    std::map<std::string, std::string> current_downloaded_paths;

    QVBoxLayout* main_layout;
    QLabel* page_description;
    ScrollableTrackList* tracklist_widget;

    MusicApi api;

    QString downloadsDir = "Downloads";
    QString downloadsJsonPath = "Data/DownloadsList/downloads.json";

    void saveDownloadsSongsToJson();
    Song jsonToSong(const json& j);
    json songToJson(const Song& song);

signals:

    void downloadedSongsUpdated();
    void setPlaySong(const Song& song);
    void setLikedSong(const Song& song);


};

#endif // DOWNLOADPAGEWIDGET_H
