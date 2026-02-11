#include "songtracklistitem.h"

SongTracklistItem::SongTracklistItem(Song song, int ID, const std::vector<QString>& params) : songID{ID}
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

    if (std::find(params.begin(), params.end(), "liked") != params.end()) {
        btnLike = new QPushButton("Unlike");
        connect(btnLike, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnUnlike);
    } else {
        btnLike = new QPushButton("Like");
        connect(btnLike, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnLike);
    }

    if (std::find(params.begin(), params.end(), "downloaded") != params.end()) {
        btnDownload = new QPushButton("Delete");
        connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDelete);
    } else {
        btnDownload = new QPushButton("Download");
        connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDownload);
    }

    main_layout->addWidget(btnLike);

    main_layout->addWidget(btnDownload);

    btnPlay = new QPushButton("PLay");
    main_layout->addWidget(btnPlay);

    qDebug() << "SongItemConstrucvt";

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
    btnDownload->setText("Delete");
    disconnect(btnDownload, nullptr, nullptr, nullptr);
    connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDelete);
    emit downloadSongRequest(songID);
}

void SongTracklistItem::onClickedbtnDelete()
{
    btnDownload->setText("Download");
    disconnect(btnDownload, nullptr, nullptr, nullptr);
    connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDownload);
    emit deleteSongRequest(songID);
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
