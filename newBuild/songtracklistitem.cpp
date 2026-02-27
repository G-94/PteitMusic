#include "songtracklistitem.h"

SongTracklistItem::SongTracklistItem(Song song, int ID, const std::vector<QString>& params) : songID{ID}
{
    main_layout = new QHBoxLayout(this);
    QVBoxLayout* song_title_layout = new QVBoxLayout();

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, QColor(45, 45, 45));
    this->setPalette(pal);
    this->setAutoFillBackground(false);

    container = new QFrame(this);
    container->setStyleSheet(Style::getQFrameWithRadiusStyle());

    QHBoxLayout *container_layout = new QHBoxLayout(container);
    container_layout->setContentsMargins(12, 8, 12, 8);
    container_layout->setSpacing(10);

    this->setProperty("class", "track-item");
    this->setAutoFillBackground(true);
    this->setStyleSheet(Style::getSongTracklistItemStyle());

    this->setAttribute(Qt::WA_Hover, true);

    QLabel *song_title = new QLabel(QString::fromStdString(song["title"]));
    song_title_layout->addWidget(song_title);

    QLabel *song_author = new QLabel(QString::fromStdString(song["artist"]));
    song_title_layout->addWidget(song_author);

    QLabel *song_dur = new QLabel(QString::fromStdString(song["duration"]));
    song_title_layout->addWidget(song_dur);

    song_title_layout->setSpacing(2);

    container_layout->addLayout(song_title_layout);
    container_layout->addStretch();

    if (std::find(params.begin(), params.end(), "liked") != params.end()) {
        btnLike = new QPushButton("♥");
        connect(btnLike, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnUnlike);
    } else {
        btnLike = new QPushButton("♡");
        connect(btnLike, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnLike);
    }

    if (std::find(params.begin(), params.end(), "downloaded") != params.end()) {
        btnDownload = new QPushButton("✕");
        connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDelete);
    } else {
        btnDownload = new QPushButton("⤓");
        connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDownload);
    }

    btnDownload->setStyleSheet(Style::getCircleButtonStyle());
    btnLike->setStyleSheet(Style::getCircleButtonStyle());

    container_layout->addWidget(btnLike);
    container_layout->addWidget(btnDownload);

    btnPlay = new QPushButton("▶");
    btnPlay->setStyleSheet(Style::getCircleButtonStyle());
    container_layout->addWidget(btnPlay);

    main_layout->addWidget(container);

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
void SongTracklistItem::enterEvent(QEnterEvent *event)
{
    isHovered = true;

    container->setStyleSheet(R"(
        QFrame {
        background-color: #3d3d3d;
        border-radius: 15px;
        margin: 2px 8px;
        })"
    );

    this->update();
    QWidget::enterEvent(event);
}

void SongTracklistItem::leaveEvent(QEvent *event)
{
    isHovered = false;

    container->setStyleSheet(R"(
        QFrame {
        background-color: #2d2d2d;
        border-radius: 15px;
        margin: 2px 8px;
        })"
    );

    this->update();
    QWidget::leaveEvent(event);
}

void SongTracklistItem::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void SongTracklistItem::onClickedbtnLike()
{
    disconnect(btnLike, nullptr, nullptr, nullptr);
    connect(btnLike, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnUnlike);
    emit likeSongRequest(songID);
}

void SongTracklistItem::onClickedbtnDownload()
{
    emit downloadSongRequest(songID);
    disconnect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDownload);
    connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDelete);
}

void SongTracklistItem::onClickedbtnDelete()
{
    emit deleteSongRequest(songID);
    disconnect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDelete);
    connect(btnDownload, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnDownload);
}

void SongTracklistItem::onClickedbtnPlay()
{
    emit settingSongRequest(songID);
}

void SongTracklistItem::onClickedbtnUnlike()
{
    disconnect(btnLike, nullptr, nullptr, nullptr);
    connect(btnLike, &QPushButton::clicked, this, &SongTracklistItem::onClickedbtnLike);
    emit unlikeSongRequest(songID);
}
