#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QString>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QList>
#include <QStackedWidget>
#include <QLabel>
#include <QDir>

#include "include/nlohmann/json/json.hpp"
#include "searchpanelwidget.h"
#include "playerwidget.h"
#include "likespagewidget.h"
#include "downloadpagewidget.h"
#include "playlistshubpagewidget.h"
#include "MusicGlobals.h"

using Song = std::map<std::string, std::string>;

class homepage : public QWidget
{
    Q_OBJECT
public:
    explicit homepage(QWidget *parent = nullptr);

private:

    QHBoxLayout* main_layout;
    QVBoxLayout* right_panel_layout;
    QFrame* left_pages_panel;
    QWidget *right_panel;

    QStackedWidget* pages;
    SearchPanelWidget* pageSearch;
    LikesPageWidget* pageLikes;
    DownloadPageWidget* pageDownloads;
    PlaylistsHubPageWidget* pagePlaylists;

    QPushButton* btnPlaylistPage;
    QPushButton* btnSearchPage;
    QPushButton* btnLikePage;
    QPushButton* btnDownloadedPage;

    PlayerWidget* player;

private slots:

    void setPlaySelectedSong(std::vector<Song> tracklist, int ID);

    void setPlaySelectedSongFromSource(std::vector<Song> tracklist, int ID);

    void setPlaySelectedGenrePlaylist(std::vector<Song> tracklist, int ID, int genreId);

    void setLikeSelectedSong(std::vector<Song> tracklist, int ID);

    void setUnlikeSelectedSong(std::vector<Song> tracklist, int ID);

    void setDownloadSelectedSong(std::vector<Song> tracklist, int ID);

    void setDeleteSelectedSong(std::vector<Song> tracklist, int ID);

    void onLikesSongsUpdated();

    void onDownloadsSongsUpdated();

signals:
};

#endif // HOMEPAGE_H
