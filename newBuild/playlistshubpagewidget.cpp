#include "playlistshubpagewidget.h"
#include "MusicGlobals.h"

PlaylistsHubPageWidget::PlaylistsHubPageWidget(PlaylistsService* playlistsService_, HistoryService* historyService_, SearchService* searchService_)
    : playlistsService{playlistsService_},  historyService{historyService_}, searchService{searchService_}
{
    main_layout = new QVBoxLayout(this);

    pageDescription = new QLabel("this is yout stream page");
    main_layout->addWidget(pageDescription);

    genreList = new GenreListWidget();

    playlistsService->loadGenreInfoJson();
    historyService->loadFamiliarArtistsJson();
    historyService->loadFamiliarSongsJson();

    if(MusicGlobal::current_genres_data.empty()) {
        MusicGlobal::current_genres_data = GENRES_DATA;
        playlistsService->saveGenreInfoJson();
    }

    genreList->setGenres(MusicGlobal::current_genres_data);
    main_layout->addWidget(genreList);

    familiarArtistsList = new ArtistsListWidget();
    familiarArtistsList->setArtists(MusicGlobal::familiarArtists);
    main_layout->addWidget(familiarArtistsList);

    streams_layout = new QHBoxLayout();

    btnPlayFamiliarSongs = new QPushButton("play familiar songs");
    streams_layout->addWidget(btnPlayFamiliarSongs);

    btnPlayLikedSongs = new QPushButton("Play liked songs");
    streams_layout->addWidget(btnPlayLikedSongs);

    btnPlayDownloadedSongs = new QPushButton("Play downloaded songs");
    streams_layout->addWidget(btnPlayDownloadedSongs);

    main_layout->addLayout(streams_layout);

    connect(genreList, &GenreListWidget::genreSelected, this, &PlaylistsHubPageWidget::onGenreSelected);
    connect(btnPlayFamiliarSongs, &QPushButton::clicked, this, &PlaylistsHubPageWidget::onPlayFamiliarSongs);
    connect(btnPlayLikedSongs, &QPushButton::clicked, this, &PlaylistsHubPageWidget::onPlayLikedSongs);
    connect(btnPlayDownloadedSongs, &QPushButton::clicked, this, &PlaylistsHubPageWidget::onPlayDownloadedSongs);

    connect(familiarArtistsList, &ArtistsListWidget::playArtistPlaylist, this, &PlaylistsHubPageWidget::onPlayFamiliarArtist);
    connect(familiarArtistsList, &ArtistsListWidget::findArtistTracks, this, &PlaylistsHubPageWidget::onFindFamiliarArtistTracklits);

    connect(searchService, &SearchService::searchSongsByArtistIdFinished, this, &PlaylistsHubPageWidget::onFamiliarArtistSignalRecieved);
}

PlaylistsHubPageWidget::~PlaylistsHubPageWidget()
{
    playlistsService->saveGenreInfoJson();
    historyService->saveFamiliarArtistsJson();
    historyService->saveFamiliarSongsJson();
}

void PlaylistsHubPageWidget::incrementGenreCounter(int genreId)
{
    playlistsService->incrementGenreCounter(genreId);
}

void PlaylistsHubPageWidget::updateListWithNewData()
{
    std::sort(MusicGlobal::familiarArtists.begin(),
              MusicGlobal::familiarArtists.end(),
              compareArtistsByPlayCount);

    familiarArtistsList->setArtists(MusicGlobal::familiarArtists);
}

void PlaylistsHubPageWidget::onGenreSelected(int id)
{
    disconnect(searchService, &SearchService::searchSongsByGenreFinished, this, nullptr);

    connect(searchService, &SearchService::searchSongsByGenreFinished, this, [=](const std::vector<Song>& tracklist) {
        if(tracklist.empty()) {
            qDebug() << "No tracks for genre" << id;
            return;
        }
        int randomInt = rand() % 480;
        emit playGenrePlaylist(tracklist, randomInt, id);
    });

    qDebug() << "Searching genre:" << id;
    searchService->searchSongsByGenre(QString::number(id));
}

void PlaylistsHubPageWidget::onPlayFamiliarSongs()
{
    if (MusicGlobal::familiarSongs.empty()) return;
    int randomId = rand() % MusicGlobal::familiarSongs.size();
    emit playFamiliarSongPlaylist(MusicGlobal::familiarSongs, randomId);
}

void PlaylistsHubPageWidget::onPlayLikedSongs()
{
    if(MusicGlobal::current_liked_tracklist.empty()) return;
    int randomInt = rand() % MusicGlobal::current_liked_tracklist.size();
    emit playLikedSongPlaylist(MusicGlobal::current_liked_tracklist, randomInt);
}

void PlaylistsHubPageWidget::onPlayDownloadedSongs()
{
    if(MusicGlobal::current_downloaded_tracklist.empty()) return;
    int randomInt = rand() % MusicGlobal::current_downloaded_tracklist.size();
    emit playLikedSongPlaylist(MusicGlobal::current_downloaded_tracklist, randomInt);
}

void PlaylistsHubPageWidget::onPlayFamiliarArtist(const QString &artistId)
{
    searchService->searchSongsByArtistId(artistId, true);
}

void PlaylistsHubPageWidget::onFindFamiliarArtistTracklits(const QString &artistId)
{
    searchService->searchSongsByArtistId(artistId, false);
}

void PlaylistsHubPageWidget::onFamiliarArtistSignalRecieved(const std::vector<Song>& tracklist, bool isForPlay)
{
    if(isForPlay) {
        int randomId = rand() & tracklist.size();
        emit playFamiliarArtistPlaylist(tracklist, randomId);
    } else {
        emit findFamiliarArtistTracklits(tracklist);
    }
}
