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

    MusicApi* api;

    void searchByQuery(const QString& query);
    void searchArtists(const QString& query);
    void searchSongsByArtistId(const QString &id, bool isForPlay);

signals:

    void searchByQueryFinished(const std::vector<Song>& tracklist);
    void searchArtistsFinished(const std::vector<ArtistData> &artist_data);
    void searchSongsByArtistIdFinished(const std::vector<Song>& tracklist, bool isForPlay);

};

#endif // SEARCHSERVICE_H
