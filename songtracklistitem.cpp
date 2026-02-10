#include "songtracklistitem.h"

SongTracklistItem::SongTracklistItem(Song song, int ID, const QString& params) : songID{ID}
{
    main_layout = new QHBoxLayout(this);
    QVBoxLayout* song_title_layout = new QVBoxLayout();

    QLabel *song_title = new QLabel(QString::fromStdString(song["title"]));
    song_title_layout->addWidget(song_title);

    QLabel *song_author = new QLabel(QString::fromStdString(song["artist"]));
    song_title_layout->addWidget(song_author);

    QLabel *song_dur = new QLabel(QString::fromStdString(song["duration"]));
    song_title_layout->addWidget(song_dur);

    song_title_layout->setSpacing(2);

    main_layout->addLayout(song_title_layout);

    if(params.isEmpty()) {
        btnLike = new QPushButton("Like");
    } else if (params == "liked") {
        btnLike = new QPushButton("Unlike");
    }
    main_layout->addWidget(btnLike);

    btnDownload = new QPushButton("Download");
    main_layout->addWidget(btnDownload);

    btnPlay = new QPushButton("PLay");
    main_layout->addWidget(btnPlay);

    qDebug() << "SongItemConstrucvt";

    if(params.isEmpty()) {
        connect(btnLike, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnLike);
    } else if (params == "liked") {
        connect(btnLike, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnUnlike);
    }

    connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDownload);
    connect(btnPlay, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnPlay);

}

int SongTracklistItem::getSongID() const
{
    return songID;
}

void SongTracklistItem::onClickedbtnLike()
{
    btnLike->setText("Unlike");
    disconnect(btnLike, nullptr, nullptr, nullptr);
    connect(btnLike, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnUnlike);
    emit likeSongRequest(songID);
}

void SongTracklistItem::onClickedbtnDownload()
{
    emit downloadSongRequest(songID);
}

void SongTracklistItem::onClickedbtnPlay()
{
    emit settingSongRequest(songID);
}

void SongTracklistItem::onClickedbtnUnlike()
{
    btnLike->setText("Like");
    disconnect(btnLike, nullptr, nullptr, nullptr);
    connect(btnLike, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnLike);
    emit unlikeSongRequest(songID);
}
