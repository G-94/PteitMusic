#include "artistslistwidget.h"

ArtistsListWidget::ArtistsListWidget(QWidget *parent)
{
    main_layout = new QVBoxLayout(this);

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, QColor(45, 45, 45));
    this->setPalette(pal);
    this->setAutoFillBackground(true);

    scrollContent = new QWidget();
    scroll_layout = new QHBoxLayout(scrollContent);

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollContent);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollArea->setStyleSheet(Style::getArtistListWidgetStyle());

    main_layout->addWidget(scrollArea);
    setFixedHeight(150);
}

void ArtistsListWidget::setArtists(const std::vector<ArtistData> &artistList)
{
    clear();

    for(const auto& artist : artistList) {
        ArtistsListItem* item = new ArtistsListItem(artist);
        items.push_back(item);
        scroll_layout->addWidget(item);

        connect(item, &ArtistsListItem::findArtistTracks, this, &ArtistsListWidget::onFindArtistTracks);
        connect(item, &ArtistsListItem::playArtistPlaylist, this, &ArtistsListWidget::onPlayArtistPlaylist);
    }

    scroll_layout->addStretch();
}

void ArtistsListWidget::clear()
{
    for(auto* item : items) {
        scroll_layout->removeWidget(item);
        delete item;
    }
    items.clear();
}

void ArtistsListWidget::onFindArtistTracks(const QString &artistId)
{
    emit findArtistTracks(artistId);
}

void ArtistsListWidget::onPlayArtistPlaylist(const QString &artistId)
{
    emit playArtistPlaylist(artistId);
}
