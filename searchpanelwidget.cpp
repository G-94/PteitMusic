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
}

void SearchPanelWidget::onClickedSearchbtn()
{
    QString query_text = search_line->text();
    api.searchByQuery(query_text);
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

void SearchPanelWidget::updateTracklist()
{
    search_result->updateTracklist(search_result->getTracklist());
}
