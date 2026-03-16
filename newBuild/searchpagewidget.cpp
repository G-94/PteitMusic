#include "searchpagewidget.h"

SearchPageWidget::SearchPageWidget(LikesService* likesService_, DownloadsService* downloadsService_, SearchService* searchService_)
    : likesService{likesService_}, downloadsService{downloadsService_}, searchService{searchService_}
{
    main_layout = new QVBoxLayout(this);

    QFrame* mainFrame = new QFrame();
    QVBoxLayout* frame_layout = new QVBoxLayout(mainFrame);
    mainFrame->setStyleSheet(Style::getQFrameWidgetStyle());

    QHBoxLayout* search_layout = new QHBoxLayout();
    search_line = new QLineEdit();
    search_line->setStyleSheet(Style::getLineEditStyle());
    search_line->setPlaceholderText("Введите название трека или артиста");
    btnSearch = new QPushButton("⌕");
    btnSearch->setStyleSheet(Style::getCircleButtonStyle());

    search_layout->addWidget(search_line);
    search_layout->addWidget(btnSearch);

    frame_layout->addLayout(search_layout);

    artists_search_result = new ArtistsListWidget();
    artists_search_result->setFixedHeight(160);
    artists_search_result->setVisible(false);
    frame_layout->addWidget(artists_search_result);

    search_result = new ScrollableTrackList();
    frame_layout->addWidget(search_result, 1);

    main_layout->addWidget(mainFrame);

    QObject::connect(btnSearch, &QPushButton::clicked, this, &SearchPageWidget::onClickedSearchbtn);
    QObject::connect(search_line, &QLineEdit::returnPressed, this, &SearchPageWidget::onClickedSearchbtn);

    QObject::connect(search_result, &ScrollableTrackList::setSongRequset, this, &SearchPageWidget::setPlaySelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setLikeSong, this, &SearchPageWidget::setLikedSelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setDownloadSong, this, &SearchPageWidget::setDownloadedSelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setDeleteSong, this, &SearchPageWidget::setDeleteSelectedSong);
    QObject::connect(search_result, &ScrollableTrackList::setUnlikeSong, this, &SearchPageWidget::setUnlikedSelectedSong);

    QObject::connect(searchService, &SearchService::searchByQueryFinished, this, [this] (const std::vector<Song>& songs) {search_result->setTracklist(songs);});
    QObject::connect(searchService, &SearchService::searchArtistsFinished, this, &SearchPageWidget::setArtistList);
    QObject::connect(searchService, &SearchService::searchSongsByArtistIdFinished, this, &SearchPageWidget::onArtistTracklistReceived);

    QObject::connect(artists_search_result, &ArtistsListWidget::findArtistTracks, this, &SearchPageWidget::onFindArtistTracklist);
    QObject::connect(artists_search_result, &ArtistsListWidget::playArtistPlaylist, this, &SearchPageWidget::onPlayArtistPlaylist);

}

void SearchPageWidget::onClickedSearchbtn()
{
    QString query_text = search_line->text();
    searchService->searchByQuery(query_text);
    searchService->searchArtists(query_text);
}

void SearchPageWidget::setLikedSelectedSong(int ID)
{
    Song song = search_result->getTracklist().at(ID);
    likesService->addSongToLikes(song);
}

void SearchPageWidget::setUnlikedSelectedSong(int ID)
{
    std::string songID = search_result->getTracklist().at(ID).at("id");
    likesService->removeSongFromLikes(songID);
}

void SearchPageWidget::setDownloadedSelectedSong(int ID)
{
    Song song = search_result->getTracklist().at(ID);
    downloadsService->addSongToDownloads(song);
}

void SearchPageWidget::setDeleteSelectedSong(int ID)
{
    std::string songID = search_result->getTracklist().at(ID).at("id");
    downloadsService->removeSongFromDownloads(songID);
}

void SearchPageWidget::setPlaySelectedSong(int ID)
{
    emit playSelectedSong(search_result->getTracklist(), ID);
}

void SearchPageWidget::setArtistList(const std::vector<ArtistData> &artist_data)
{
    if(!artist_data.empty()) {
        artists_search_result->setArtists(artist_data);
        artists_search_result->setVisible(true);

        disconnect(artists_search_result, &ArtistsListWidget::findArtistTracks,this, &SearchPageWidget::onFindArtistTracklist);
        disconnect(artists_search_result, &ArtistsListWidget::playArtistPlaylist, this, &SearchPageWidget::onPlayArtistPlaylist);

        connect(artists_search_result, &ArtistsListWidget::findArtistTracks, this, &SearchPageWidget::onFindArtistTracklist);
        connect(artists_search_result, &ArtistsListWidget::playArtistPlaylist, this, &SearchPageWidget::onPlayArtistPlaylist);
    } else {
        artists_search_result->setVisible(false);
    }
}

void SearchPageWidget::onArtistTracklistReceived(const std::vector<Song> &tracklist, bool isForPlay)
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

void SearchPageWidget::onFindArtistTracklist(const QString& artistId)
{
    searchService->searchSongsByArtistId(artistId, false);
}

void SearchPageWidget::onPlayArtistPlaylist(const QString& artistId)
{
    searchService->searchSongsByArtistId(artistId, true);
}

void SearchPageWidget::updateTracklist()
{
    search_result->updateTracklist(search_result->getTracklist());
}

void SearchPageWidget::setTracklist(const std::vector<Song> &tracklist)
{
    if(tracklist.empty()) {

    }
    search_result->setTracklist(tracklist);
}
