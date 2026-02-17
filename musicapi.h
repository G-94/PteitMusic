#ifndef MUSICAPI_H
#define MUSICAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QPointer>
#include <QStringConverter>
#include <vector>
#include <string>
#include <map>

#include "include/nlohmann/json/json.hpp"
#include "MusicGlobals.h"

using json = nlohmann::json;
using Song = std::map<std::string, std::string>;

class MusicApi : public QObject {
    Q_OBJECT
public:
    explicit MusicApi(QObject* parent = nullptr);

    void searchByQuery(const QString& query);
    void searchByGenre(const QString& genre);
    void searchByArtist(const QString& artist);
    void getTracklistByArtistId(const QString& id, bool isForPlay);
    void getArtistDataBySongId(QString id);
    QByteArray downloadSong(const QString& url);


private slots:

    void onQuerySearchFinished(QNetworkReply* reply);

signals:

    void querySearchFinished(const std::vector<Song>& songs);
    void genreSearchFinished(const std::vector<Song>& songs);
    void artistSearchFinished(const std::vector<ArtistData>& artists_list);
    void artistTracklistSearchFinished(const std::vector<Song>& songs, bool isForPlay);
    void artistIdSearchBySongFinished(const ArtistData& artist_data);
    void error(const QString& e);

private:

    QNetworkAccessManager manager;
    std::vector<Song> parseHtml(const QByteArray& data);

    std::vector<Song> accumulatedResults;
    int pendingReplies;

};

#endif // MUSICAPI_H
