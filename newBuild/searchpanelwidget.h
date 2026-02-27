#ifndef SEARCHPANELWIDGET_H
#define SEARCHPANELWIDGET_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QList>
#include <QListWidgetItem>
#include <QKeyEvent>

#include "scrollabletracklist.h"
#include "artistslistwidget.h"
#include "musicapi.h"
#include "searchservice.h"
#include "likesservice.h"
#include "downloadsservice.h"
#include "MusicGlobals.h"

class SearchPanelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchPanelWidget(LikesService* likesService_, DownloadsService* downloadsService_, SearchService* searchService_);

    void updateTracklist();
    void setTracklist(const std::vector<Song>& tracklist);

private:

    QVBoxLayout* main_layout;

    QLineEdit* search_line;
    QPushButton* btnSearch;

    ArtistsListWidget* artists_search_result;
    ScrollableTrackList* search_result;

    LikesService* likesService;
    DownloadsService* downloadsService;
    SearchService* searchService;

private slots:

    void onClickedSearchbtn();
    void setLikedSelectedSong(int ID);
    void setUnlikedSelectedSong(int ID);
    void setDownloadedSelectedSong(int ID);
    void setDeleteSelectedSong(int ID);
    void setPlaySelectedSong(int ID);

    void setArtistList(const std::vector<ArtistData>& artist_data);
    void onArtistTracklistReceived(const std::vector<Song>& tracklist, bool isForPlay);

    void onFindArtistTracklist(const QString& artistId);
    void onPlayArtistPlaylist(const QString& artistId);

signals:

    void playSelectedSong(std::vector<Song> tracklist, int ID);
    void playArtistPlaylist(std::vector<Song> tracklist, int ID);

};

#endif // SEARCHPANELWIDGET_H
