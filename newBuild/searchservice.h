#ifndef SEARCHSERVICE_H
#define SEARCHSERVICE_H

#include <QObject>

#include "MusicGlobals.h"
#include "musicapi.h"

class SearchService : public QObject
{
    Q_OBJECT
public:
    explicit SearchService(MusicApi* api_);

    void searchByQuery(const QString& query);
    void searchArtists(const QString& query);
    void searchSongsByArtistId(const QString &id, bool isForPlay);
    void searchSongsByGenre(const QString& genre);

private:

    MusicApi* api;

signals:

    void searchByQueryFinished(const std::vector<Song>& tracklist);
    void searchArtistsFinished(const std::vector<ArtistData> &artist_data);
    void searchSongsByArtistIdFinished(const std::vector<Song>& tracklist, bool isForPlay);
    void searchSongsByGenreFinished(const std::vector<Song>& tracklist);

};

#endif // SEARCHSERVICE_H
