#include "playerwidget.h"

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget{parent}
{

    QDir dir("Data/current_songs");
    if (!dir.exists()) {
        QDir().mkpath("Data/current_songs");
        dir.mkdir(".");
    }

    main_layout = new QVBoxLayout();

    track_title = new QLabel("Playlist is empty");
    main_layout->addWidget(track_title);

    QHBoxLayout* slider = new QHBoxLayout();
    trackTimePos = new QSlider(Qt::Orientation::Horizontal);
    trackTimePos->setPageStep(1);
    track_dur = new QLabel("00:00");
    slider->addWidget(trackTimePos);
    slider->addWidget(track_dur);
    main_layout->addLayout(slider);

    QHBoxLayout* buttons_layout = new QHBoxLayout();

    btnPlay = new QPushButton("Play");
    btnPlay->setEnabled(false);
    btnPlayNext = new QPushButton("Next");
    btnPlayNext->setEnabled(false);
    btnPlayPrev = new QPushButton("Previous");
    btnPlayPrev->setEnabled(false);
    btnLike = new QPushButton("Like");
    btnLike->setEnabled(false);
    btnDownload = new QPushButton("Download");
    btnDownload->setEnabled(false);
    btnVolume = new QRadioButton("Volume off");

    buttons_layout->addWidget(btnLike);
    buttons_layout->addWidget(btnPlayPrev);
    buttons_layout->addWidget(btnPlay);
    buttons_layout->addWidget(btnPlayNext);
    buttons_layout->addWidget(btnDownload);
    buttons_layout->addWidget(btnVolume);

    main_layout->addLayout(buttons_layout);

    this->setLayout(main_layout);
    this->setFixedSize(1100, 130);

    player = new QMediaPlayer();
    aOutput = new QAudioOutput();
    player->setAudioOutput(aOutput);

    connect(btnPlay, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnPlay);
    connect(btnPlayNext, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnPlayNext);
    connect(btnPlayPrev, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnPlayPrev);
    connect(btnLike, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnLike);
    connect(btnDownload, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnDownload);
    connect(btnVolume, &QRadioButton::toggled, this, &PlayerWidget::onClickedbtnVolume);

    QObject::connect(player, &QMediaPlayer::playbackStateChanged, this, &PlayerWidget::onPlaybackStateChanged);

    QObject::connect(player, &QMediaPlayer::positionChanged, this, [this] (qint64 pos) {
        if (player->duration() > 0) {
            int progress = static_cast<int>((pos * 100) / player->duration());
            trackTimePos->setValue(progress);

            QTime currentTime(0, 0, 0);
            currentTime = currentTime.addMSecs(static_cast<int>(pos));
            track_dur->setText(currentTime.toString("mm:ss"));

            qDebug() << static_cast<QString>(currentPlaylist[current_song_index]["duration"].c_str()) << track_dur->text();
        }
    });

    QObject::connect(player, &QMediaPlayer::durationChanged, this, [this] (qint64 dur) {
        trackTimePos->setRange(0, 100);
    });

    QObject::connect(trackTimePos, &QSlider::sliderReleased, this, [this]() {
        if (player && player->duration() > 0) {
            int value = trackTimePos->value();
            qint64 newPos = (static_cast<qint64>(value) * player->duration()) / 100;
            player->setPosition(newPos);
        }
    });

    QObject::connect(player, &QMediaPlayer::mediaStatusChanged, this, &PlayerWidget::onMediaStatusChanged);

}

void PlayerWidget::SetSong(const std::vector<Song> &playlist, int temp_current_song_index, std::vector<Song> &likedSongs)
{
    currentPlaylist = playlist;
    MusicGlobal::current_liked_tracklist = likedSongs;
    current_song_index = temp_current_song_index;

    if(currentPlaylist.empty()) {
        btnPlay->setEnabled(false);
        btnPlayNext->setEnabled(false);
        btnPlayPrev->setEnabled(false);
        btnLike->setEnabled(false);
        btnDownload->setEnabled(false);
        trackTimePos->setValue(0);
        track_dur->setText("00:00");
        track_title->setText("Uknown");

        return;
    }

    if(current_song_index < 0 || current_song_index >= playlist.size()) {
        qDebug() << "Wrong current song index";
        return;
    }

    Song song = currentPlaylist[current_song_index];

    checkLikes(MusicGlobal::current_liked_tracklist);

    player->setSource(QUrl());

    try {
        std::string songString_url = song["url"];
        QByteArray songData = api.downloadSong(QString::fromStdString(songString_url));

        if(!songData.isEmpty()) {
            QString temp_file_location = "Data/current_songs/temp_" + QString::number(1) + ".mp3";

            std::ofstream song_file(temp_file_location.toStdString(), std::ios::binary);
            if(song_file.is_open()) {
                song_file.write(songData.constData(), songData.size());
                song_file.close();

                std::string song_title = song["title"] + " - " + song["artist"];
                track_title->setText(QString::fromStdString(song_title));

                player->setSource(QUrl::fromLocalFile(temp_file_location));
                player->setPosition(0);
                btnLike->setEnabled(true);
                btnPlay->setEnabled(true);
                track_dur->setText("00:00");
                onClickedbtnPlay();
            }
            else {
                qDebug() << "Song file wasnt open";
            }

        }
    } catch(...) {
        qDebug() << "Exception was catch while trying to connect server";

    }

}

void PlayerWidget::checkNextAndPrev()
{
    if(current_song_index > 0) {
        btnPlayPrev->setEnabled(true);
        if(current_song_index < currentPlaylist.size() - 1) {
            btnPlayNext->setEnabled(true);
        }
        else {
            btnPlayNext->setEnabled(false);
        }
    }
    else {
        btnPlayPrev->setEnabled(false);
        btnPlayNext->setEnabled(true);
    }
}

void PlayerWidget::checkLikes(const std::vector<Song>& likedTracklist)
{
    if(currentPlaylist.empty() || current_song_index < 0) return;
    Song song = currentPlaylist[current_song_index];

    disconnect(btnLike, nullptr, nullptr, nullptr);

    bool isLiked = false;
    for(const auto& liked_song : likedTracklist) {
        if(song.at("id") == liked_song.at("id")) {
            isLiked = true;
        }
    }

    if(isLiked) {
        btnLike->setText("Unlike");
        connect(btnLike, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnUnlike);
    } else {
        btnLike->setText("Like");
        connect(btnLike, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnLike);
    }

}

void PlayerWidget::onClickedbtnPlay()
{
    if (player->source().isEmpty()) {
        qDebug() << "No source to play";
        return;
    }
    player->play();
    btnPlay->setText("Pause");
    disconnect(btnPlay, nullptr, nullptr, nullptr);
    connect(btnPlay, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnPause);
    qDebug() << "Playing";

}

void PlayerWidget::onClickedbtnPause()
{
    player->pause();
    btnPlay->setText("Play");
    disconnect(btnPlay, nullptr, nullptr, nullptr);
    connect(btnPlay, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnPlay);
    qDebug() << "Paused";
}

void PlayerWidget::onClickedbtnPlayNext()
{
    if(currentPlaylist.empty() || current_song_index < 0) return;

    int nextSongIndex = current_song_index + 1;
    if (nextSongIndex < currentPlaylist.size()) {
        SetSong(currentPlaylist, nextSongIndex, MusicGlobal::current_liked_tracklist);
        onClickedbtnPlay();
    }
    else qDebug() << "No next track in playlist";
}

void PlayerWidget::onClickedbtnPlayPrev()
{
    if(currentPlaylist.empty() || current_song_index < 0) return;

    int nextSongIndex = current_song_index - 1;
    if (nextSongIndex >= 0) {
        SetSong(currentPlaylist, nextSongIndex, MusicGlobal::current_liked_tracklist);
        onClickedbtnPlay();
    }
    else qDebug() << "No next track in playlist";
}

void PlayerWidget::onClickedbtnLike()
{
    qDebug() << "Like button clicked";
    btnLike->setText("Unlike");
    disconnect(btnLike, nullptr, nullptr, nullptr);
    connect(btnLike, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnUnlike);
    emit likeSongRequest(currentPlaylist, current_song_index);
}

void PlayerWidget::onClickedbtnUnlike()
{
    qDebug() << "Unlike button clicked";
    btnLike->setText("Like");
    disconnect(btnLike, nullptr, nullptr, nullptr);
    connect(btnLike, &QPushButton::clicked, this, &PlayerWidget::onClickedbtnLike);
    emit unlikeSongRequest(currentPlaylist, current_song_index);
}

void PlayerWidget::onClickedbtnDownload()
{
    qDebug() << "Download button clicked";
    // Здесь должна быть логика скачивания трека в постоянное хранилище
    // Например, копирование из temp файла в папку с музыкой пользователя
}

void PlayerWidget::onClickedbtnVolume(bool Muted)
{
    player->audioOutput()->setMuted(Muted);
}

void PlayerWidget::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    switch(state) {
    case QMediaPlayer::PlaybackState::PlayingState:
        checkNextAndPrev();
        break;
    case QMediaPlayer::PlaybackState::PausedState:
        checkNextAndPrev();
        break;
    }
}

void PlayerWidget::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    switch(status) {
    case QMediaPlayer::EndOfMedia:
        if(current_song_index + 1 < currentPlaylist.size()) {
            this->SetSong(currentPlaylist, current_song_index + 1, MusicGlobal::current_liked_tracklist);
        }
    }
}
