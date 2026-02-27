#ifndef LIKESSERVICE_H
#define LIKESSERVICE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

#include "MusicGlobals.h"

class LikesService : public QObject
{
    Q_OBJECT
public:
    explicit LikesService(QObject *parent = nullptr);

    void addSongToLikes(const Song &song);

    void removeSongFromLikes(const std::string &songID);

    void loadLikesSongs();

    void saveLikedSongs();

private:

    QString likesPath = "Data/liked_songs.json";

signals:

    void likesSongUpdated();

};

#endif // LIKESSERVICE_H
