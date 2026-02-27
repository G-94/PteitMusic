#ifndef PLAYLISTSSERVICE_H
#define PLAYLISTSSERVICE_H

#include <QObject>

class PlaylistsService : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistsService(QObject *parent = nullptr);

signals:
};

#endif // PLAYLISTSSERVICE_H
