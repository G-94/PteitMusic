#include "likespagewidget.h"

LikesPageWidget::LikesPageWidget(LikesService* likesService_, DownloadsService* downloadsService_) : likesService{likesService_}, downloadsService{downloadsService_}
{
    main_layout = new QVBoxLayout(this);

    QFrame *mainFrame = new QFrame();

    mainFrame->setStyleSheet(Style::getQFrameWidgetStyle());

    QVBoxLayout *frame_layout = new QVBoxLayout(mainFrame);

    QHBoxLayout* title_layout = new QHBoxLayout();
    page_description = new QLabel("Понравившиеся песни");
    page_description->setStyleSheet(Style::getLabelStyle());
    page_description->setAlignment(Qt::AlignCenter);
    title_layout->addWidget(page_description);

    title_layout->addStretch();

    btnPlayLikedSongs = new QPushButton("Поток по понравившимся");
    btnPlayLikedSongs->setStyleSheet(Style::getStreamButtonStyle());
    title_layout->addWidget(btnPlayLikedSongs);

    frame_layout->addLayout(title_layout);

    tracklist_widget = new ScrollableTrackList();
    frame_layout->addWidget(tracklist_widget);

    main_layout->addWidget(mainFrame);

    QObject::connect(tracklist_widget, &ScrollableTrackList::setSongRequset, [this] (int songID) {
        emit setPlaySong(MusicGlobal::current_liked_tracklist, songID);
    });

    QObject::connect(tracklist_widget, &ScrollableTrackList::setDownloadSong, [this] (int songID) {
        Song addedSong = MusicGlobal::current_liked_tracklist.at(songID);
        downloadsService->addSongToDownloads(addedSong);
    });

    QObject::connect(tracklist_widget, &ScrollableTrackList::setDeleteSong, [this] (int songID) {
        std::string id_toRemove = MusicGlobal::current_liked_tracklist.at(songID).at("id");
        downloadsService->removeSongFromDownloads(id_toRemove);
    });

    QObject::connect(tracklist_widget, &ScrollableTrackList::setUnlikeSong, [this] (int songID) {
        Song song = tracklist_widget->getTracklist()[songID];
        likesService->removeSongFromLikes(song.at("id"));
        tracklist_widget->updateTracklist(MusicGlobal::current_liked_tracklist);
    });

    QObject::connect(btnPlayLikedSongs, &QPushButton::clicked, this, [this] () {
        if(MusicGlobal::current_liked_tracklist.empty()) return;
        int randomInt = rand() % MusicGlobal::current_liked_tracklist.size();
        emit setPlayLikedSongsPlaylist(MusicGlobal::current_liked_tracklist, randomInt);
    });

    likesService->loadLikesSongs();
    tracklist_widget->setTracklist(MusicGlobal::current_liked_tracklist);
}

void LikesPageWidget::updateTracklist()
{
    tracklist_widget->updateTracklist(MusicGlobal::current_liked_tracklist);
}

