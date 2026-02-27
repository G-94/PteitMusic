#ifndef LIKESPAGEWIDGET_H
#define LIKESPAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDir>
#include <QFile>

#include "scrollabletracklist.h"
#include "MusicGlobals.h"

#include "likesservice.h"
#include "downloadsservice.h"

class LikesPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LikesPageWidget(LikesService* likesService_, DownloadsService* downloadsService_);

    void updateTracklist();

private:

    QVBoxLayout* main_layout;
    ScrollableTrackList* tracklist_widget;
    QLabel* page_discription;

    QString likesPath = "Data/liked_songs.json";

    LikesService* likesService;
    DownloadsService* downloadsService;

signals:

    void likesSongUpdated();
    void setPlaySong(std::vector<Song> tracklist, int ID);
    void setDownloadSong(std::vector<Song> tracklist, int ID);
    void setDeleteSong(std::vector<Song> tracklist, int ID);

};

#endif // LIKESPAGEWIDGET_H
