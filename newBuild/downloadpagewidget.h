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
#include "MusicGlobals.h"

#include "scrollabletracklist.h"
#include "likesservice.h"
#include "downloadsservice.h"

class DownloadPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadPageWidget(LikesService* likesService_, DownloadsService* downloadsService_);

    void updateTracklist();

private:

    QVBoxLayout* main_layout;
    QLabel* page_description;
    QPushButton* btnPlayDownloadedSongs;
    ScrollableTrackList* tracklist_widget;

    LikesService* likesService;
    DownloadsService* downloadsService;

signals:

    void setPlayDownloadedPlaylist(std::vector<Song> tracklist, int ID);
    void setPlaySong(std::vector<Song> tracklist, int ID);
    void setLikedSong(std::vector<Song> tracklist, int ID);
    void setUnlikedSong(std::vector<Song> tracklist, int ID);

};

#endif // DOWNLOADPAGEWIDGET_H
