#include "homepage.h"

homepage::homepage(QWidget *parent)
    : QWidget{parent}
{
    main_layout = new QHBoxLayout();

    ///

    //
    left_pages_panel = new QFrame();
    btnHomePage = new QPushButton();
    btnLikePage = new QPushButton();
    btnDownloadedPage = new QPushButton();

    QVBoxLayout *left_panel_layout = new QVBoxLayout();
    left_panel_layout->addWidget(btnHomePage);
    left_panel_layout->addWidget(btnLikePage);
    left_panel_layout->addWidget(btnDownloadedPage);

    left_pages_panel->setLayout(left_panel_layout);
    left_pages_panel->setFixedWidth(150);

    ///

    main_layout->addWidget(left_pages_panel);

    ///

    right_panel = new QWidget();
    pages = new QStackedWidget();

    pageSearch = new SearchPanelWidget();
    pages->addWidget(pageSearch);

    pageLikes = new LikesPageWidget();
    pages->addWidget(pageLikes);

    pages->setCurrentWidget(pageSearch);

    player = new PlayerWidget();

    QVBoxLayout* right_panel_layout = new QVBoxLayout();
    right_panel_layout->addWidget(pages);
    right_panel_layout->addWidget(player);
    right_panel->setLayout(right_panel_layout);

    ///

    main_layout->addWidget(right_panel);

    this->setLayout(main_layout);

    QObject::connect(btnHomePage, &QPushButton::clicked, this, [this] () {
        pages->setCurrentWidget(pageSearch);
    });

    QObject::connect(btnLikePage, &QPushButton::clicked, this, [this] () {
        pages->setCurrentWidget(pageLikes);
    });


    QObject::connect(pageSearch, &SearchPanelWidget::playSelectedSong, this, &homepage::setPlaySelectedSong);
    QObject::connect(pageSearch, &SearchPanelWidget::downloadSelectedSong, this, &homepage::setDownloadSelectedSong);
    QObject::connect(pageSearch, &SearchPanelWidget::likeSelectedSong, this, &homepage::setLikeSelectedSong);
    QObject::connect(pageSearch, &SearchPanelWidget::unlikeSelectedSong, this, &homepage::setUnlikeSelectedSong);

    QObject::connect(pageLikes, &LikesPageWidget::setPlaySong, this, &homepage::setPlaySelectedSong);
    QObject::connect(pageLikes, &LikesPageWidget::likesSongUpdated, this, &homepage::onLikesSongsUpdated);

    QObject::connect(player, &PlayerWidget::likeSongRequest, this, &homepage::setLikeSelectedSong);
    QObject::connect(player, &PlayerWidget::unlikeSongRequest, this, &homepage::setUnlikeSelectedSong);

}

void homepage::setPlaySelectedSong(std::vector<Song> tracklist, int ID)
{
    qDebug() << "onplaySelected";
    player->SetSong(tracklist, ID, MusicGlobal::current_liked_tracklist);
}

void homepage::setLikeSelectedSong(std::vector<Song> tracklist, int ID)
{
    if (tracklist.empty() || ID < 0) return;

    Song currentSong = tracklist[ID];

    pageLikes->addSongToLikes(currentSong);
}

void homepage::setDownloadSelectedSong(std::vector<Song> tracklist, int ID)
{
    qDebug() << "doqnloadSelectedHOmepage";

}

void homepage::setUnlikeSelectedSong(std::vector<Song> tracklist, int ID)
{
    if (tracklist.empty() || ID < 0) return;

    Song currentSong = tracklist[ID];

    pageLikes->removeSongFromLikes(currentSong.at("id"));
}

void homepage::onLikesSongsUpdated(std::vector<Song> tracklist)
{
    MusicGlobal::current_liked_tracklist = tracklist;
    pageSearch->setLikedSongsTracklist(MusicGlobal::current_liked_tracklist);
    pageSearch->updateTracklist(tracklist);
    player->checkLikes(MusicGlobal::current_liked_tracklist);
}




