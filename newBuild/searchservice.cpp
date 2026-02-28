#include "searchservice.h"

SearchService::SearchService(MusicApi* api_) : api{api_} {

    QObject::connect(api, &MusicApi::artistSearchFinished, this, [this] (const std::vector<ArtistData> &artist_data) {
        emit searchArtistsFinished(artist_data);
    });

    QObject::connect(api, &MusicApi::querySearchFinished, this, [this] (const std::vector<Song>& tracklist) {
        emit searchByQueryFinished(tracklist);
    });

    QObject::connect(api, &MusicApi::artistTracklistSearchFinished, this, [this] (const std::vector<Song> &songs, bool isForPlay) {
        emit searchSongsByArtistIdFinished(songs, isForPlay);
    });

    QObject::connect(api, &MusicApi::genreSearchFinished, this, [this] (const std::vector<Song> &songs) {
        emit searchSongsByGenreFinished(songs);
    });
}

void SearchService::searchByQuery(const QString &query)
{
    api->searchByQuery(query);
}

void SearchService::searchArtists(const QString &query)
{
    api->searchArtists(query);
}

void SearchService::searchSongsByArtistId(const QString &id, bool isForPlay)
{
    api->getTracklistByArtistId(id, isForPlay);
}

void SearchService::searchSongsByGenre(const QString &genre)
{
    api->searchByGenre(genre);
}


