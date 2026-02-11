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
#include "musicapi.h"
#include "MusicGlobals.h"

class SearchPanelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchPanelWidget(QWidget *parent = nullptr);

    void setLikedSongsTracklist(std::vector<Song> new_tracklist);
    void updateTracklist(const std::vector<Song>& songs);

private:

    QVBoxLayout* main_layout;

    QLineEdit* search_line;
    QPushButton* btnSearch;

    ScrollableTrackList* search_result;
    MusicApi api;

private slots:

    void onClickedSearchbtn();
    void setLikedSelectedSong(int ID);
    void setUnlikedSelectedSong(int ID);
    void setDownloadedSelectedSong(int ID);
    void setDeleteSelectedSong(int ID);
    void setPlaySelectedSong(int ID);

signals:

    void searchRequest(QString query);
    void likeSelectedSong(std::vector<Song> tracklist, int ID);
    void unlikeSelectedSong(std::vector<Song> tracklist, int ID);
    void downloadSelectedSong(std::vector<Song> tracklist, int ID);
    void deleteSelectedSong(std::vector<Song> tracklist, int ID);
    void playSelectedSong(std::vector<Song> tracklist, int ID);

};

#endif // SEARCHPANELWIDGET_H
