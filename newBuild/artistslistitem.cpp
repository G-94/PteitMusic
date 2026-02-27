#include "artistslistitem.h"

ArtistsListItem::ArtistsListItem(const ArtistData &data_) : data{data_}
{
    main_layout = new QVBoxLayout(this);
    this->setStyleSheet(Style::getArtistListItemStyle());

    container = new QFrame(this);
    container->setStyleSheet(Style::getQFrameWithRadiusStyle());

    QVBoxLayout* container_layout = new QVBoxLayout(container);

    artistNameLabel = new QLabel(QString::fromStdString(data.name));
    container_layout->addWidget(artistNameLabel);

    btn_layout = new QHBoxLayout();
    btnArtistFindTracks = new QPushButton("Найти треки");
    btnArtistFindTracks->setStyleSheet(Style::getArtistListItemButtonStyle());
    btn_layout->addWidget(btnArtistFindTracks);
    btnArtistPlayPlaylist = new QPushButton("Поточек");
    btnArtistPlayPlaylist->setStyleSheet(Style::getArtistListItemButtonStyle());
    btn_layout->addWidget(btnArtistPlayPlaylist);

    container_layout->addLayout(btn_layout);
    main_layout->addWidget(container);

    connect(btnArtistFindTracks, &QPushButton::clicked, this, [this] () {
        emit findArtistTracks(QString::fromStdString(data.id));
    });

    connect(btnArtistPlayPlaylist, &QPushButton::clicked, this, [this] () {
        emit playArtistPlaylist(QString::fromStdString(data.id));
    });
}

void ArtistsListItem::enterEvent(QEnterEvent *event)
{
    isHovered = true;

    container->setStyleSheet(R"(
        QFrame {
        background-color: #3d3d3d;
        border-radius: 15px;
        margin: 2px 8px;
        }
        )"
    );

    QWidget::enterEvent(event);
}

void ArtistsListItem::leaveEvent(QEvent *event)
{
    isHovered = false;

    container->setStyleSheet(R"(
        QFrame {
        background-color: #2d2d2d;
        border-radius: 15px;
        margin: 2px 8px;
        }
        )"
    );

    QWidget::leaveEvent(event);
}

