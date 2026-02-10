#ifndef SCROLLABLETRACKLIST_H
#define SCROLLABLETRACKLIST_H

#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QList>

#include "songtracklistitem.h"
#include "MusicGlobals.h"

using Song = std::map<std::string, std::string>;

class ScrollableTrackList : public QWidget
{
    Q_OBJECT
public:
    explicit ScrollableTrackList(const std::vector<Song>& tracklist_ = std::vector<Song>());

    void setTracklist(const std::vector<Song>& tracklist_, const std::vector<Song>& liked_songs, const QString& params = "");
    void updateTracklist(const std::vector<Song>& tracklist_, const std::vector<Song>& liked_songs, const QString& params = "");

    void clear();

    std::vector<Song> getTracklist() const;

private:

    QScrollArea* scrollArea;

    QWidget* listWidget;
    QVBoxLayout* list_layout;

    std::vector<Song> current_tracklist;
    QVector<SongTracklistItem*> trackItems;

private slots:

    void onLikeRequest(int ID);
    void onDownloadRequest(int ID);
    void onPlayRequest(int ID);
    void onUnlikeRequest(int ID);

signals:

    void setLikeSong(int ID);
    void setUnlikeSong(int ID);
    void setDownloadSong(int ID);
    void setSongRequset(int ID);
};

#endif // SCROLLABLETRACKLIST_H
