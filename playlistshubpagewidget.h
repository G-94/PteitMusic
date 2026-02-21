#ifndef PLAYLISTSHUBPAGEWIDGET_H
#define PLAYLISTSHUBPAGEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include "musicapi.h"
#include <QDir>
#include <QTime>
#include <QByteArray>
#include <QRandomGenerator>
#include <fstream>

#include "genrelistwidget.h"
#include "artistslistwidget.h"
#include "MusicGlobals.h"

class PlaylistsHubPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaylistsHubPageWidget(QWidget *parent = nullptr);
    ~PlaylistsHubPageWidget();

    void incrementGenreCounter(int genreId);

    void updateListWithNewData();

private:

    QVBoxLayout* main_layout;

    QLabel* pageDescription;

    GenreListWidget* genreList;
    std::vector<GenreData> genresData;

    ArtistsListWidget* familiarArtistsList;

    QHBoxLayout* streams_layout;
    QPushButton* btnPlayFamiliarSongs;
    QPushButton* btnPlayLikedSongs;
    QPushButton* btnPlayDownloadedSongs;

    MusicApi api;

    QString JsonGenresPath = "Data/genres_rank.json";
    QString JsonFamiliarSongsPath = "Data/familiar_songs.json";
    QString JsonFamiliarArtistsPath = "Data/familiar_artists.json";

    void saveGenreInfoJson();
    void loadGenreInfoJson();

    void saveFamiliarSongsJson();
    void loadFamiliarSongsJson();

    void saveFamiliarArtistsJson();
    void loadFamiliarArtistsJson();

private slots:

    void onGenreSelected(int id);
    void onPlayFamiliarSongs();
    void onPlayLikedSongs();
    void onPlayDownloadedSongs();
    void onPlayFamiliarArtist(const QString& artistId);
    void onFindFamiliarArtistTracklits(const QString& artistId);
    void onFamiliarArtistSignalRecieved(std::vector<Song> tracklist, bool isForPlay);

signals:

    void playGenrePlaylist(std::vector<Song> tracklist, int ID, int genreId);
    void playFamiliarSongPlaylist(std::vector<Song> tracklist, int ID);
    void playLikedSongPlaylist(std::vector<Song> tracklist, int ID);
    void playDownloadedSongPlaylist(std::vector<Song> tracklist, int ID);
    void playFamiliarArtistPlaylist(std::vector<Song> tracklist, int ID);
    void findFamiliarArtistTracklits(std::vector<Song> tracklist);

};

#endif // PLAYLISTSHUBPAGEWIDGET_H
