#ifndef LIKESPAGEWIDGET_H
#define LIKESPAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDir>
#include <QFile>

#include <include/nlohmann/json/json.hpp>
#include "scrollabletracklist.h"

using json = nlohmann::json;
using Song = std::map<std::string, std::string>;

class LikesPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LikesPageWidget(QWidget *parent = nullptr);

    void addSongToLikes(const Song& song);
    void removeSongFromLikes(const std::string& songID);
    void loadLikesSongs();

    std::vector<Song> getLikedSongs();

private:

    std::vector<Song> current_liked_tracklist;
    QVBoxLayout* main_layout;
    ScrollableTrackList* tracklist_widget;
    QLabel* page_discription;

    QString likesDir = "Data/Likes";
    QString likesPath = "Data/Likes/liked_songs.json";

    void saveLikedSongsToFile();
    Song jsonToSong(const json& j);
    json songToJson(const Song& song);

signals:

    void likesSongUpdated(std::vector<Song> tracklist);
    void setPlaySong(std::vector<Song> tracklist, int ID);
    void setDownloadSong(std::vector<Song> tracklist, int ID);

};

#endif // LIKESPAGEWIDGET_H
