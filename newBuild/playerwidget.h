#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include "musicapi.h"
#include <QDir>
#include <QTime>
#include <QByteArray>
#include <QRandomGenerator>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "MusicGlobals.h"

#include "likesservice.h"
#include "downloadsservice.h"
#include "playlistsservice.h"
#include "historyservice.h"
#include "playerservice.h"

class PlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWidget(LikesService* likesService_, DownloadsService* downloadsService_, PlayerService* playerService_,
                            HistoryService* historyService_, PlaylistsService* playlistsService_
    );

    void SetSong(const std::vector<Song>& playlist, int current_song_index, const QString& param = "");

    void checkLikes();
    void checkDownloads();

private:

    QMediaPlayer* player;
    QAudioOutput* aOutput;

    QSlider* trackTimePos;

    QPushButton *btnPlay;
    QPushButton *btnPlayNext;
    QPushButton *btnPlayPrev;
    QPushButton *btnLike;
    QPushButton *btnDownload;
    QRadioButton *btnVolume;

    QLabel* track_title;
    QLabel *track_dur;

    QVBoxLayout* main_layout;

    QString current_param;
    std::vector<Song> currentPlaylist;
    int current_song_index = -1;

    HistoryService* historyService;
    LikesService* likesService;
    DownloadsService* downloadsService;
    PlaylistsService* playlistsService;
    PlayerService* playerService;

    void checkNextAndPrev();

private slots:

    void onClickedbtnPlay();
    void onClickedbtnPause();
    void onClickedbtnPlayNext();
    void onClickedbtnPlayPrev();
    void onClickedbtnLike();
    void onClickedbtnUnlike();
    void onClickedbtnDownload();
    void onClickedbtnDelete();
    void onClickedbtnVolume(bool Muted);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onArtistIdSearchBySong(const ArtistData& data);

signals:

    void songPlayed();
};

#endif // PLAYERWIDGET_H
