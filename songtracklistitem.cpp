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

    btnPlay = new QPushButton("Play");
    main_layout->addWidget(btnPlay);

    qDebug() << "SongItemConstrucvt";

    connect(btnPlay, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnPlay);
}

SongTracklistItem::~SongTracklistItem()
{
    disconnect();
    qDebug() << "SongTracklistItem deleted for ID:" << songID;
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
    btnDownload->setText("Delete");
    disconnect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDownload);
    connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDelete);
}

void SongTracklistItem::onClickedbtnDelete()
{
    emit deleteSongRequest(songID);
    btnDownload->setText("Download");
    disconnect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDelete);
    connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDownload);
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
