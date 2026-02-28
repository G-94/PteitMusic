#ifndef PLAYLISTSSERVICE_H
#define PLAYLISTSSERVICE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

#include "MusicGlobals.h"

class PlaylistsService : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistsService();
    void incrementGenreCounter(int genreId);
    void saveGenreInfoJson();
    void loadGenreInfoJson();

private:

    QString JsonGenresPath = "Data/genres_rank.json";

signals:
};

#endif // PLAYLISTSSERVICE_H
