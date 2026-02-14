#include "scrollabletracklist.h"

ScrollableTrackList::ScrollableTrackList(const std::vector<Song>& tracklist_) : current_tracklist{tracklist_}
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);

    list_layout = new QVBoxLayout();
    listWidget = new QWidget();

    listWidget->setLayout(list_layout);

    scrollArea->setWidget(listWidget);
    main_layout->addWidget(scrollArea);

    if(!tracklist_.empty()) {
        setTracklist(tracklist_);
    }

}

void ScrollableTrackList::setTracklist(const std::vector<Song>& tracklist_)
{
    clear();

    qDebug() << "SetTracklist";

    current_tracklist = tracklist_;

    int counter = 0;
    for(auto song : current_tracklist) {

        std::vector<QString> params_for_item = {};

        for(auto& liked_song : MusicGlobal::current_liked_tracklist) {
            if(song.at("id") == liked_song.at("id")) {
                params_for_item.push_back("liked");
                break;
            }
        }

        for(auto& downloaded_song : MusicGlobal::current_downloaded_tracklist) {
            if(song.at("id") == downloaded_song.at("id")) {
                params_for_item.push_back("downloaded");
                break;
            }
        }

        SongTracklistItem* item = new SongTracklistItem(song, counter, params_for_item);

        trackItems.append(item);
        list_layout->addWidget(item);
        ++counter;

        QObject::connect(item, &SongTracklistItem::settingSongRequest, this, &ScrollableTrackList::onPlayRequest);
        QObject::connect(item, &SongTracklistItem::likeSongRequest, this, &ScrollableTrackList::onLikeRequest);
        QObject::connect(item, &SongTracklistItem::downloadSongRequest, this, &ScrollableTrackList::onDownloadRequest);
        QObject::connect(item, &SongTracklistItem::deleteSongRequest, this, &ScrollableTrackList::onDeleteRequest);
        QObject::connect(item, &SongTracklistItem::unlikeSongRequest, this, &ScrollableTrackList::onUnlikeRequest);
    }

    list_layout->addStretch();
    listWidget->adjustSize();

}

void ScrollableTrackList::updateTracklist(const std::vector<Song>& tracklist_)
{
    QScrollBar* scrollBar = scrollArea->verticalScrollBar();
    int scrollPosition = scrollBar->value();

    setTracklist(tracklist_);

    QMetaObject::invokeMethod(this, [scrollBar, scrollPosition]() {
        scrollBar->setValue(scrollPosition);
    }, Qt::QueuedConnection);

}

void ScrollableTrackList::clear()
{
    qDebug() << "Clearing" << trackItems.size() << "items";

    while (list_layout->count() > 0) {
        QLayoutItem* item = list_layout->takeAt(0);
        if (item->spacerItem()) {
            delete item;
        } else if (item->widget()) {
            list_layout->removeWidget(item->widget());
            item->widget()->deleteLater();
            delete item;
        }
    }

    trackItems.clear();
}

std::vector<Song> ScrollableTrackList::getTracklist() const
{
    return current_tracklist;
}

void ScrollableTrackList::onLikeRequest(int ID)
{
    emit setLikeSong(ID);
}

void ScrollableTrackList::onDownloadRequest(int ID)
{
    emit setDownloadSong(ID);
}

void ScrollableTrackList::onDeleteRequest(int ID)
{
    emit setDeleteSong(ID);
}

void ScrollableTrackList::onPlayRequest(int ID)
{
    emit setSongRequset(ID);
}

void ScrollableTrackList::onUnlikeRequest(int ID)
{
    emit setUnlikeSong(ID);
}
