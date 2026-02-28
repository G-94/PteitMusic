#include "homepage.h"

homepage::homepage(QWidget *parent)
    : QWidget{parent}, downloadsService{&api}, searchService{&api}, playerService(&api)
{
    main_layout = new QHBoxLayout();

    ///

    //
    left_pages_panel = new QFrame();
    btnPlaylistPage = new QPushButton();
    btnSearchPage = new QPushButton();
    btnLikePage = new QPushButton();
    btnDownloadedPage = new QPushButton();

    QVBoxLayout *left_panel_layout = new QVBoxLayout();
    left_panel_layout->addWidget(btnPlaylistPage);
    left_panel_layout->addWidget(btnSearchPage);
    left_panel_layout->addWidget(btnLikePage);
    left_panel_layout->addWidget(btnDownloadedPage);

    left_pages_panel->setLayout(left_panel_layout);
    left_pages_panel->setFixedWidth(150);

    ///

    main_layout->addWidget(left_pages_panel);

    ///

    right_panel = new QWidget();
    pages = new QStackedWidget();

    pagePlaylists = new PlaylistsHubPageWidget(&playlistsService, &historyService, &searchService);
    pages->addWidget(pagePlaylists);

    pageSearch = new SearchPanelWidget(&likesService, &downloadsService, &searchService);
    pages->addWidget(pageSearch);

    pageLikes = new LikesPageWidget(&likesService, &downloadsService);
    pages->addWidget(pageLikes);

    pageDownloads = new DownloadPageWidget(&likesService, &downloadsService);
    pages->addWidget(pageDownloads);

    pages->setCurrentWidget(pageSearch);

    player = new PlayerWidget(&likesService, &downloadsService, &playerService, &historyService, &playlistsService);

    QVBoxLayout* right_panel_layout = new QVBoxLayout();
    right_panel_layout->addWidget(pages);
    right_panel_layout->addWidget(player);
    right_panel->setLayout(right_panel_layout);

    ///

    main_layout->addWidget(right_panel);

    this->setLayout(main_layout);

    QObject::connect(btnPlaylistPage, &QPushButton::clicked, this, [this] () {
        pages->setCurrentWidget(pagePlaylists);
    });

    QObject::connect(btnSearchPage, &QPushButton::clicked, this, [this] () {
        pages->setCurrentWidget(pageSearch);
    });

    QObject::connect(btnLikePage, &QPushButton::clicked, this, [this] () {
        pages->setCurrentWidget(pageLikes);
    });

    QObject::connect(btnDownloadedPage, &QPushButton::clicked, this, [this] () {
        pages->setCurrentWidget(pageDownloads);
    });

    QObject::connect(pageSearch, &SearchPanelWidget::playSelectedSong, this, &homepage::setPlaySelectedSong);
    QObject::connect(pageSearch, &SearchPanelWidget::playArtistPlaylist, this, &homepage::setPlayPlaylistInRandomOrder);

    QObject::connect(pageLikes, &LikesPageWidget::setPlaySong, this, &homepage::setPlaySelectedSong);

    QObject::connect(pageDownloads, &DownloadPageWidget::setPlaySong, this, &homepage::setPlaySelectedSongFromSource);

    QObject::connect(pagePlaylists, &PlaylistsHubPageWidget::playGenrePlaylist, this, &homepage::setPlaySelectedGenrePlaylist);
    QObject::connect(pagePlaylists, &PlaylistsHubPageWidget::playFamiliarArtistPlaylist, this, &homepage::setPlayPlaylistInRandomOrder);
    QObject::connect(pagePlaylists, &PlaylistsHubPageWidget::playLikedSongPlaylist, this, &homepage::setPlayPlaylistInRandomOrder);
    QObject::connect(pagePlaylists, &PlaylistsHubPageWidget::playFamiliarSongPlaylist, this, &homepage::setPlayPlaylistInRandomOrder);
    QObject::connect(pagePlaylists, &PlaylistsHubPageWidget::playDownloadedSongPlaylist, this, &homepage::setPlaySongPlaylistFromSourceInRandomOrder);
    QObject::connect(pagePlaylists, &PlaylistsHubPageWidget::findFamiliarArtistTracklits, this, &homepage::onFindArtistTracklist);

    QObject::connect(player, &PlayerWidget::songPlayed, this, &homepage::onPlayedNewSong);

    QObject::connect(&likesService, &LikesService::likesSongUpdated, this, &homepage::onLikesSongsUpdated);
    QObject::connect(&downloadsService, &DownloadsService::downloadedSongsUpdated, this, &homepage::onDownloadsSongsUpdated);
}

void homepage::setPlaySelectedSong(std::vector<Song> tracklist, int ID)
{
    player->SetSong(tracklist, ID);
}

void homepage::setPlaySelectedSongFromSource(std::vector<Song> tracklist, int ID)
{
    player->SetSong(tracklist, ID, "from_source");
}

void homepage::setPlaySongPlaylistFromSourceInRandomOrder(std::vector<Song> tracklist, int ID)
{
    player->SetSong(tracklist, ID, "from_source_random_order");
}

void homepage::setPlaySelectedGenrePlaylist(std::vector<Song> tracklist, int ID, int genreId)
{
    pagePlaylists->incrementGenreCounter(genreId);
    player->SetSong(tracklist, ID, "random_order");
}

void homepage::setPlayPlaylistInRandomOrder(std::vector<Song> tracklist, int ID)
{
    player->SetSong(tracklist, ID, "random_order");
}

void homepage::onFindArtistTracklist(std::vector<Song> tracklist)
{
    pageSearch->setTracklist(tracklist);
    pages->setCurrentWidget(pageSearch);
}

void homepage::onLikesSongsUpdated()
{
    pageSearch->updateTracklist();
    pageLikes->updateTracklist();
    pageDownloads->updateTracklist();
    player->checkLikes();
}

void homepage::onDownloadsSongsUpdated()
{
    pageSearch->updateTracklist();
    pageDownloads->updateTracklist();
    pageLikes->updateTracklist();
    player->checkDownloads();
}

void homepage::onPlayedNewSong()
{
    pagePlaylists->updateListWithNewData();
}




