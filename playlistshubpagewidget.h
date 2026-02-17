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

private:

    QVBoxLayout* main_layout;

    QLabel* pageDescription;

    QLineEdit* artistsSearchLine;

    GenreListWidget* genreList;

    std::vector<GenreData> genresData;

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

signals:

    void playGenrePlaylist(std::vector<Song> tracklist, int ID, int genreId);

};

#endif // PLAYLISTSHUBPAGEWIDGET_H
