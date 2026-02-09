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
        setTracklist(tracklist_, {});
    }

}

void ScrollableTrackList::setTracklist(const std::vector<Song>& tracklist_, const std::vector<Song>& liked_songs, const QString& params)
{
    clear();

    qDebug() << "SetTracklist";

    current_tracklist = tracklist_;

    int counter = 0;
    for(auto song : current_tracklist) {

        SongTracklistItem* item;

        bool isLiked = false;

        if(params.isEmpty()) {
            for(auto& liked_song : liked_songs) {
                if(song.at("id") == liked_song.at("id")) {
                    isLiked = true;
                    break;
                }
            }
        } else if(params == "liked_list") {
            isLiked = true;
        }

        if(!isLiked) {
            item = new SongTracklistItem(song, counter);
        } else {
            item = new SongTracklistItem(song, counter, "liked");
        }

        trackItems.append(item);
        list_layout->addWidget(item);
        ++counter;

        QObject::connect(item, &SongTracklistItem::settingSongRequest, this, &ScrollableTrackList::onPlayRequest);
        QObject::connect(item, &SongTracklistItem::likeSongRequest, this, &ScrollableTrackList::onLikeRequest);
        QObject::connect(item, &SongTracklistItem::downloadSongRequest, this, &ScrollableTrackList::onDownloadRequest);
        QObject::connect(item, &SongTracklistItem::unlikeSongRequest, this, &ScrollableTrackList::onUnlikeRequest);
    }

    list_layout->addStretch();
    listWidget->adjustSize();

}

void ScrollableTrackList::updateTracklist(const std::vector<Song>& tracklist_, const std::vector<Song>& liked_songs, const QString &params)
{
    QScrollBar* scrollBar = scrollArea->verticalScrollBar();
    int scrollPosition = scrollBar->value();

    setTracklist(tracklist_, liked_songs, params);

    QMetaObject::invokeMethod(this, [scrollBar, scrollPosition]() {
        scrollBar->setValue(scrollPosition);
    }, Qt::QueuedConnection);

}

void ScrollableTrackList::clear()
{
    for(auto* item : trackItems) {
        list_layout->removeWidget(item);
        delete item;
    }

    trackItems.clear();

    QLayoutItem *child;
    while ((child = list_layout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }
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

void ScrollableTrackList::onPlayRequest(int ID)
{
    emit setSongRequset(ID);
}

void ScrollableTrackList::onUnlikeRequest(int ID)
{
    emit setUnlikeSong(ID);
}
