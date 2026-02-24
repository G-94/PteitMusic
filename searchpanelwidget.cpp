#include "searchpanelwidget.h"

SearchPanelWidget::SearchPanelWidget(QWidget *parent)
    : QWidget{parent}
{
    main_layout = new QVBoxLayout();

    QHBoxLayout* search_layout = new QHBoxLayout();
    search_line = new QLineEdit();
    btnSearch = new QPushButton();

    search_layout->addWidget(search_line);
    search_layout->addWidget(btnSearch);

    main_layout->addLayout(search_layout);

    artists_search_result = new ArtistsListWidget();
    artists_search_result->setFixedHeight(140);
    artists_search_result->setVisible(false);
    main_layout->addWidget(artists_search_result);

    search_result = new ScrollableTrackList();
    main_layout->addWidget(search_result, 1);

    this->setLayout(main_layout);

    QObject::connect(btnSearch, &QPushButton::clicked, this, &SearchPanelWidget::onClickedSearchbtn);

    QObject::connect(search_line, &QLineEdit::returnPressed, this, &SearchPanelWidget::onClickedSearchbtn);

    QObject::connect(search_result, &ScrollableTrackList::setSongRequset, this, &SearchPanelWidget::setPlaySelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setLikeSong, this, &SearchPanelWidget::setLikedSelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setDownloadSong, this, &SearchPanelWidget::setDownloadedSelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setDeleteSong, this, &SearchPanelWidget::setDeleteSelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setUnlikeSong, this, &SearchPanelWidget::setUnlikedSelectedSong);

    QObject::connect(&api, &MusicApi::querySearchFinished, this, [this] (const std::vector<Song>& songs) {
        search_result->setTracklist(songs);
    });

    QObject::connect(artists_search_result, &ArtistsListWidget::findArtistTracks, this, &SearchPanelWidget::onFindArtistTracklist);
    QObject::connect(artists_search_result, &ArtistsListWidget::playArtistPlaylist, this, &SearchPanelWidget::onPlayArtistPlaylist);

    connect(&api, &MusicApi::artistSearchFinished, this, &SearchPanelWidget::setArtistList);
    QObject::connect(&api, &MusicApi::artistTracklistSearchFinished, this, &SearchPanelWidget::onArtistTracklistReceived);
}

void SearchPanelWidget::onClickedSearchbtn()
{
    QString query_text = search_line->text();
    api.searchByQuery(query_text);
    api.searchByArtist(query_text);
}

void SearchPanelWidget::setLikedSelectedSong(int ID)
{
    emit likeSelectedSong(search_result->getTracklist(), ID);
}

void SearchPanelWidget::setUnlikedSelectedSong(int ID)
{
    emit unlikeSelectedSong(search_result->getTracklist(), ID);
}

void SearchPanelWidget::setDownloadedSelectedSong(int ID)
{
    emit downloadSelectedSong(search_result->getTracklist(), ID);
}

void SearchPanelWidget::setDeleteSelectedSong(int ID)
{
    emit deleteSelectedSong(search_result->getTracklist(), ID);
}

void SearchPanelWidget::setPlaySelectedSong(int ID)
{
    emit playSelectedSong(search_result->getTracklist(), ID);
}

void SearchPanelWidget::setArtistList(const std::vector<ArtistData> &artist_data)
{
    if(!artist_data.empty()) {
        artists_search_result->setArtists(artist_data);
        artists_search_result->setVisible(true);

        disconnect(artists_search_result, &ArtistsListWidget::findArtistTracks,this, &SearchPanelWidget::onFindArtistTracklist);
        disconnect(artists_search_result, &ArtistsListWidget::playArtistPlaylist, this, &SearchPanelWidget::onPlayArtistPlaylist);

        connect(artists_search_result, &ArtistsListWidget::findArtistTracks, this, &SearchPanelWidget::onFindArtistTracklist);
        connect(artists_search_result, &ArtistsListWidget::playArtistPlaylist, this, &SearchPanelWidget::onPlayArtistPlaylist);
    } else {
        artists_search_result->setVisible(false);
    }
}

void SearchPanelWidget::onArtistTracklistReceived(const std::vector<Song> &tracklist, bool isForPlay)
{
    qDebug() << "recieved";
    if(isForPlay) {
        if(tracklist.empty()) return;
        int randomId = rand() % tracklist.size();
        emit playArtistPlaylist(tracklist, randomId);
    } else {
        artists_search_result->setVisible(false);
        search_result->setTracklist(tracklist);
    }
}

void SearchPanelWidget::onFindArtistTracklist(const QString& artistId)
{
    api.getTracklistByArtistId(artistId, false);
}

void SearchPanelWidget::onPlayArtistPlaylist(const QString& artistId)
{
    api.getTracklistByArtistId(artistId, true);
}

void SearchPanelWidget::updateTracklist()
{
    search_result->updateTracklist(search_result->getTracklist());
}

void SearchPanelWidget::setTracklist(const std::vector<Song> &tracklist)
{
    search_result->setTracklist(tracklist);
}
