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
            MusicGlobal::familiarArtists.push_back(data);
        }
    });
}

void PlayerService::getArtistDataBySongId(const QString& songID)
{
    api->getArtistDataBySongId(songID);
}
