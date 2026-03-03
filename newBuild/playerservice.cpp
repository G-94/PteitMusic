#include "playerservice.h"

PlayerService::PlayerService(MusicApi* api_) : api{api_}
{
    QObject::connect(api, &MusicApi::artistIdSearchBySongFinished, this, [this] (const ArtistData& data) {
        bool found = false;
        for(auto& item : MusicGlobal::familiarArtists) {
            if(data.id == item.id) {
                ++item.playCounter;
                found = true;
                break;
            }
        }

        if(!found) {
            ArtistData newArtist = data;
            newArtist.playCounter = 1;
            MusicGlobal::familiarArtists.push_back(newArtist);
        }

        emit searchArtistDataBySongIdFinished(data);
    });

    QObject::connect(api, &MusicApi::error, this, [this] (const QString& ex) {
        qDebug() << ex;
    });
}

void PlayerService::getArtistDataBySongId(const QString& songID)
{
    api->getArtistDataBySongId(songID);
}
