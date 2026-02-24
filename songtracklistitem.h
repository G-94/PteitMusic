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
#include <QEvent>
#include <QPainter>
#include <QStackedWidget>
#include <QLabel>

#include "MusicGlobals.h"
#include "styles.h"

using Song = std::map<std::string, std::string>;

class SongTracklistItem : public QWidget
{
    Q_OBJECT
public:
    explicit SongTracklistItem(Song song, int ID, const std::vector<QString>& params = {});

    ~SongTracklistItem();

    int getSongID() const;

private:

    QHBoxLayout* main_layout;
    QPushButton* btnLike;
    QPushButton* btnDownload;
    QPushButton* btnPlay;

    QFrame *container;
    bool isHovered = false;

    int songID;

protected:

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:

    void onClickedbtnLike();
    void onClickedbtnDownload();
    void onClickedbtnDelete();
    void onClickedbtnPlay();
    void onClickedbtnUnlike();

signals:

    void likeSongRequest(int songID);
    void downloadSongRequest(int songID);
    void deleteSongRequest(int SongID);
    void settingSongRequest(int songID);
    void unlikeSongRequest(int songID);
};

#endif // SONGTRACKLISTITEM_H
