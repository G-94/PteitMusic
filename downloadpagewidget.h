#ifndef DOWNLOADPAGEWIDGET_H
#define DOWNLOADPAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <fstream>
#include <QtConcurrent>


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
    void removeSongFromDownloads(std::string songID);
    void loadDownloadsSongs();

    void updateTracklist();

private:

    std::map<std::string, std::string> current_downloaded_paths;

    QVBoxLayout* main_layout;
    QLabel* page_description;
    ScrollableTrackList* tracklist_widget;

    MusicApi api;

    QFutureWatcher<QByteArray> downloadWatcher;
    QFutureWatcher<bool> deleteWatcher;

    QString downloadsDir = "Downloads";
    QString downloadsJsonPath = "Data/DownloadsList/downloads.json";
    QString pendingDownloadUrl;

    void saveDownloadsSongsToJson();
    Song jsonToSong(const json& j);
    json songToJson(const Song& song);

    std::string clearPathName(const std::string& path);

signals:

    void downloadedSongsUpdated();
    void setPlaySong(std::vector<Song> tracklist, int ID);
    void setLikedSong(std::vector<Song> tracklist, int ID);
    void setUnlikedSong(std::vector<Song> tracklist, int ID);

};

#endif // DOWNLOADPAGEWIDGET_H
