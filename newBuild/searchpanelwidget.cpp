#include "searchpanelwidget.h"

SearchPanelWidget::SearchPanelWidget(LikesService* likesService_, DownloadsService* downloadsService_, SearchService* searchService_)
    : likesService{likesService_}, downloadsService{downloadsService_}, searchService{searchService_}
{
    main_layout = new QVBoxLayout(this);

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, QColor(45, 45, 45));
    this->setPalette(pal);
    this->setAutoFillBackground(true);

    QHBoxLayout* search_layout = new QHBoxLayout();
    search_line = new QLineEdit();
    search_line->setStyleSheet(Style::getLineEditStyle());
    btnSearch = new QPushButton("⌕");
    btnSearch->setStyleSheet(Style::getCircleButtonStyle());

    search_layout->addWidget(search_line);
    search_layout->addWidget(btnSearch);

    main_layout->addLayout(search_layout);

    artists_search_result = new ArtistsListWidget();
    artists_search_result->setFixedHeight(140);
    artists_search_result->setVisible(false);
    main_layout->addWidget(artists_search_result);

    search_result = new ScrollableTrackList();
    main_layout->addWidget(search_result, 1);

    QObject::connect(btnSearch, &QPushButton::clicked, this, &SearchPanelWidget::onClickedSearchbtn);
    QObject::connect(search_line, &QLineEdit::returnPressed, this, &SearchPanelWidget::onClickedSearchbtn);

    QObject::connect(search_result, &ScrollableTrackList::setSongRequset, this, &SearchPanelWidget::setPlaySelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setLikeSong, this, &SearchPanelWidget::setLikedSelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setDownloadSong, this, &SearchPanelWidget::setDownloadedSelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setDeleteSong, this, &SearchPanelWidget::setDeleteSelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setUnlikeSong, this, &SearchPanelWidget::setUnlikedSelectedSong);

    QObject::connect(searchService, &SearchService::searchByQueryFinished, this, [this] (const std::vector<Song>& songs) {search_result->setTracklist(songs);});
    QObject::connect(searchService, &SearchService::searchArtistsFinished, this, &SearchPanelWidget::setArtistList);
    QObject::connect(searchService, &SearchService::searchSongsByArtistIdFinished, this, &SearchPanelWidget::onArtistTracklistReceived);

    QObject::connect(artists_search_result, &ArtistsListWidget::findArtistTracks, this, &SearchPanelWidget::onFindArtistTracklist);
    QObject::connect(artists_search_result, &ArtistsListWidget::playArtistPlaylist, this, &SearchPanelWidget::onPlayArtistPlaylist);

}

void SearchPanelWidget::onClickedSearchbtn()
{
    QString query_text = search_line->text();
    searchService->searchByQuery(query_text);
    searchService->searchArtists(query_text);
}

void SearchPanelWidget::setLikedSelectedSong(int ID)
{
    Song song = search_result->getTracklist().at(ID);
    likesService->addSongToLikes(song);
}

void SearchPanelWidget::setUnlikedSelectedSong(int ID)
{
    std::string songID = search_result->getTracklist().at(ID).at("id");
    likesService->removeSongFromLikes(songID);
}

void SearchPanelWidget::setDownloadedSelectedSong(int ID)
{
    Song song = search_result->getTracklist().at(ID);
    downloadsService->addSongToDownloads(song);
}

void SearchPanelWidget::setDeleteSelectedSong(int ID)
{
    std::string songID = search_result->getTracklist().at(ID).at("id");
    downloadsService->removeSongFromDownloads(songID);
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
    searchService->searchSongsByArtistId(artistId, false);
}

void SearchPanelWidget::onPlayArtistPlaylist(const QString& artistId)
{
    searchService->searchSongsByArtistId(artistId, true);
}

void SearchPanelWidget::updateTracklist()
{
    search_result->updateTracklist(search_result->getTracklist());
}

void SearchPanelWidget::setTracklist(const std::vector<Song> &tracklist)
{
    search_result->setTracklist(tracklist);
}
