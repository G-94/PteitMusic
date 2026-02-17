#include "artistslistitem.h"

ArtistsListItem::ArtistsListItem(const ArtistData &data_) : data{data_}
{
    main_layout = new QVBoxLayout(this);

    artistNameLabel = new QLabel(QString::fromStdString(data.name));
    main_layout->addWidget(artistNameLabel);

    btn_layout = new QHBoxLayout();

    btnArtistFindTracks = new QPushButton("Find tracks");
    btn_layout->addWidget(btnArtistFindTracks);
    btnArtistPlayPlaylist = new QPushButton("Play volna");
    btn_layout->addWidget(btnArtistPlayPlaylist);

    main_layout->addLayout(btn_layout);

    connect(btnArtistFindTracks, &QPushButton::clicked, this, [this] () {
        emit findArtistTracks(QString::fromStdString(data.id));
    });

    connect(btnArtistPlayPlaylist, &QPushButton::clicked, this, [this] () {
        emit playArtistPlaylist(QString::fromStdString(data.id));
    });
}
