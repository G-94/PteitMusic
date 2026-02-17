#include "homepage.h"

homepage::homepage(QWidget *parent)
    : QWidget{parent}
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

    pagePlaylists = new PlaylistsHubPageWidget();
    pages->addWidget(pagePlaylists);

    pageSearch = new SearchPanelWidget();
    pages->addWidget(pageSearch);

    pageLikes = new LikesPageWidget();
    pages->addWidget(pageLikes);

    pageDownloads = new DownloadPageWidget();
    pages->addWidget(pageDownloads);

    pages->setCurrentWidget(pageSearch);

    player = new PlayerWidget();

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
    QObject::connect(pageSearch, &SearchPanelWidget::downloadSelectedSong, this, &homepage::setDownloadSelectedSong);
    QObject::connect(pageSearch, &SearchPanelWidget::deleteSelectedSong, this, &homepage::setDeleteSelectedSong);
    QObject::connect(pageSearch, &SearchPanelWidget::likeSelectedSong, this, &homepage::setLikeSelectedSong);
    QObject::connect(pageSearch, &SearchPanelWidget::unlikeSelectedSong, this, &homepage::setUnlikeSelectedSong);
    QObject::connect(pageSearch, &SearchPanelWidget::playArtistPlaylist, this, &homepage::setPlayPlaylistInRandomOrder);

    QObject::connect(pageLikes, &LikesPageWidget::setPlaySong, this, &homepage::setPlaySelectedSong);
    QObject::connect(pageLikes, &LikesPageWidget::likesSongUpdated, this, &homepage::onLikesSongsUpdated);
    QObject::connect(pageLikes, &LikesPageWidget::setDeleteSong, this, &homepage::setDeleteSelectedSong);
    QObject::connect(pageLikes, &LikesPageWidget::setDownloadSong, this, &homepage::setDownloadSelectedSong);

    QObject::connect(pageDownloads, &DownloadPageWidget::setPlaySong, this, &homepage::setPlaySelectedSongFromSource);
    QObject::connect(pageDownloads, &DownloadPageWidget::downloadedSongsUpdated, this, &homepage::onDownloadsSongsUpdated);
    QObject::connect(pageDownloads, &DownloadPageWidget::setLikedSong, this, &homepage::setLikeSelectedSong);
    QObject::connect(pageDownloads, &DownloadPageWidget::setUnlikedSong, this, &homepage::setUnlikeSelectedSong);

    QObject::connect(pagePlaylists, &PlaylistsHubPageWidget::playGenrePlaylist, this, &homepage::setPlaySelectedGenrePlaylist);

    QObject::connect(player, &PlayerWidget::likeSongRequest, this, &homepage::setLikeSelectedSong);
    QObject::connect(player, &PlayerWidget::unlikeSongRequest, this, &homepage::setUnlikeSelectedSong);
    QObject::connect(player, &PlayerWidget::downloadSongRequest, this, &homepage::setDownloadSelectedSong);
    QObject::connect(player, &PlayerWidget::deleteSongRequest, this, &homepage::setDeleteSelectedSong);
}

void homepage::setPlaySelectedSong(std::vector<Song> tracklist, int ID)
{
    player->SetSong(tracklist, ID);
    qDebug() << "onplaySelected";
}

void homepage::setPlaySelectedSongFromSource(std::vector<Song> tracklist, int ID)
{
    player->SetSong(tracklist, ID, "from_source");
    qDebug() << "onplayDownloaded";
}

void homepage::setPlaySelectedGenrePlaylist(std::vector<Song> tracklist, int ID, int genreId)
{
    pagePlaylists->incrementGenreCounter(genreId);
    player->SetSong(tracklist, ID, "random_order");
    qDebug() << "onplaygenre";
}

void homepage::setPlayPlaylistInRandomOrder(std::vector<Song> tracklist, int ID)
{
    player->SetSong(tracklist, ID, "random_order");
    qDebug() << "onplayrandom";
}

void homepage::setLikeSelectedSong(std::vector<Song> tracklist, int ID)
{
    if (tracklist.empty() || ID < 0) return;

    Song currentSong = tracklist[ID];

    pageLikes->addSongToLikes(currentSong);
}

void homepage::setDownloadSelectedSong(std::vector<Song> tracklist, int ID)
{
    if (tracklist.empty() || ID < 0) return;

    Song currentSong = tracklist[ID];

    pageDownloads->addSongToDownloads(currentSong);
}

void homepage::setDeleteSelectedSong(std::vector<Song> tracklist, int ID)
{
    if (tracklist.empty() || ID < 0) return;

    Song currentSong = tracklist[ID];

    pageDownloads->removeSongFromDownloads(currentSong.at("id"));
}

void homepage::setUnlikeSelectedSong(std::vector<Song> tracklist, int ID)
{
    if (tracklist.empty() || ID < 0) return;

    Song currentSong = tracklist[ID];

    pageLikes->removeSongFromLikes(currentSong.at("id"));
}

void homepage::onLikesSongsUpdated()
{
    pageSearch->updateTracklist();
    pageDownloads->updateTracklist();
    player->checkLikes();
}

void homepage::onDownloadsSongsUpdated()
{
    pageSearch->updateTracklist();
    pageLikes->updateTracklist();
    player->checkDownloads();
}




