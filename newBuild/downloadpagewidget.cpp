#include "downloadpagewidget.h"

DownloadPageWidget::DownloadPageWidget(LikesService* likesService_, DownloadsService* downloadsService_)
    : likesService{likesService_}, downloadsService{downloadsService_}
{
    main_layout = new QVBoxLayout(this);

    QFrame* mainFrame = new QFrame();
    QVBoxLayout* frame_layout = new QVBoxLayout(mainFrame);
    mainFrame->setStyleSheet(Style::getQFrameWidgetStyle());

    QHBoxLayout* title_layout = new QHBoxLayout();
    page_description = new QLabel("Скачанные песни");
    page_description->setStyleSheet(Style::getLabelStyle());
    page_description->setAlignment(Qt::AlignCenter);
    title_layout->addWidget(page_description);

    title_layout->addStretch();

    btnPlayDownloadedSongs = new QPushButton("Поток по скачанным");
    btnPlayDownloadedSongs->setStyleSheet(Style::getStreamButtonStyle());
    title_layout->addWidget(btnPlayDownloadedSongs);

    frame_layout->addLayout(title_layout);

    tracklist_widget = new ScrollableTrackList();
    frame_layout->addWidget(tracklist_widget);

    main_layout->addWidget(mainFrame);

    QObject::connect(tracklist_widget, &ScrollableTrackList::setSongRequset, this, [this] (int ID) {
        emit setPlaySong(MusicGlobal::current_downloaded_tracklist, ID);
    });

    QObject::connect(tracklist_widget, &ScrollableTrackList::setLikeSong, this, [this] (int ID) {
        Song song = MusicGlobal::current_downloaded_tracklist.at(ID);
        likesService->addSongToLikes(song);
    });

    QObject::connect(tracklist_widget, &ScrollableTrackList::setUnlikeSong, this, [this] (int ID) {
        std::string songID = MusicGlobal::current_downloaded_tracklist.at(ID).at("id");
        likesService->removeSongFromLikes(songID);
    });

    QObject::connect(tracklist_widget, &ScrollableTrackList::setDeleteSong, [this] (int index) {
        if(index >= 0 && index < MusicGlobal::current_downloaded_tracklist.size()) {
            std::string song_id = MusicGlobal::current_downloaded_tracklist[index].at("id");
            downloadsService->removeSongFromDownloads(song_id);
        }
    });

    QObject::connect(btnPlayDownloadedSongs, &QPushButton::clicked, this, [this] () {
        if(MusicGlobal::current_downloaded_tracklist.empty()) return;
        int randomInt = rand() % MusicGlobal::current_downloaded_tracklist.size();
        emit setPlayDownloadedPlaylist(MusicGlobal::current_downloaded_tracklist, randomInt);
    });

    downloadsService->loadDownloadsSongs();
    tracklist_widget->setTracklist(MusicGlobal::current_downloaded_tracklist);
}

void DownloadPageWidget::updateTracklist()
{
    tracklist_widget->updateTracklist(MusicGlobal::current_downloaded_tracklist);
}
