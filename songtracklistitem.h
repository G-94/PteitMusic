#ifndef SONGTRACKLISTITEM_H
#define SONGTRACKLISTITEM_H

#include <QWidget>
#include <QString>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QList>
#include <QStackedWidget>
#include <QLabel>

using Song = std::map<std::string, std::string>;

class SongTracklistItem : public QWidget
{
    Q_OBJECT
public:
    explicit SongTracklistItem(Song song, int ID, const QString& params = "");

    int getSongID() const;

private:

    QHBoxLayout* main_layout;
    QPushButton* btnLike;
    QPushButton* btnDownload;
    QPushButton* btnPlay;

    int songID;

private slots:

    void onClickedbtnLike();
    void onClickedbtnDownload();
    void onClickedbtnPlay();
    void onClickedbtnUnlike();

signals:

    void likeSongRequest(int songID);
    void downloadSongRequest(int songID);
    void settingSongRequest(int songID);
    void unlikeSongRequest(int songID);
};

#endif // SONGTRACKLISTITEM_H
