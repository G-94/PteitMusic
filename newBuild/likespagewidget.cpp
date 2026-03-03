#include "likespagewidget.h"

LikesPageWidget::LikesPageWidget(LikesService* likesService_, DownloadsService* downloadsService_) : likesService{likesService_}, downloadsService{downloadsService_}
{
    main_layout = new QVBoxLayout(this);

    QFrame *mainFrame = new QFrame();

    mainFrame->setStyleSheet(Style::getQFrameWidgetStyle());

    QVBoxLayout *frame_layout = new QVBoxLayout(mainFrame);

    page_description = new QLabel("Понравившиеся песни");
    page_description->setStyleSheet(Style::getLabelStyle());
    page_description->setAlignment(Qt::AlignCenter);
    frame_layout->addWidget(page_description);

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

    likesService->loadLikesSongs();
    tracklist_widget->setTracklist(MusicGlobal::current_liked_tracklist);
}

void LikesPageWidget::updateTracklist()
{
    tracklist_widget->updateTracklist(MusicGlobal::current_liked_tracklist);
}

