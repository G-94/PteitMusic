#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <QObject>

#include "MusicGlobals.h"
#include "musicapi.h"

class PlayerService : public QObject
{
    Q_OBJECT
public:
    explicit PlayerService(MusicApi* api_);

    void getArtistDataBySongId(const QString& songID);

private:

    MusicApi* api;

signals:

    void searchArtistDataBySongIdFinished(const ArtistData& data);

};

#endif // PLAYERSERVICE_H
