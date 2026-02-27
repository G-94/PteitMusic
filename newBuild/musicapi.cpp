#include "musicapi.h"

MusicApi::MusicApi(QObject *parent)
{}

void MusicApi::searchByQuery(const QString &query)
{
    QUrl url("https://rus.hitmotop.com/search");
    QUrlQuery params;
    params.addQueryItem("q", query);
    url.setQuery(params);

    QNetworkRequest request(url);
    QNetworkReply* reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply] () {onSearchByQueryFinished(reply);});
}

void MusicApi::searchByGenre(const QString &genre)
{
    accumulatedResults.clear();
    pendingReplies = 0;

    for(int page = 0; page <= 480; page += 48) {
        QString strUrl = "https://rus.hitmotop.com/genre/" + genre + "/start/" + QString::number(page);
        QNetworkRequest request(strUrl);
        QNetworkReply* reply = manager.get(request);

        ++pendingReplies;
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {onSearchByGenreFinished(reply);});
    }
}

void MusicApi::searchArtists(const QString &artist)
{
    QUrl url("https://rus.hitmotop.com/search");
    QUrlQuery param;
    param.addQueryItem("q", artist);
    url.setQuery(param);

    QNetworkRequest request(url);
    QNetworkReply* reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply] () {onSearchArtistsFinished(reply);});
}

void MusicApi::getTracklistByArtistId(const QString &id, bool isForPlay)
{
    accumulatedResults.clear();
    pendingReplies = 0;

    for(int page = 0; page <= 480; page += 48) {
        QUrl url("https://rus.hitmotop.com/artist/" + id + "/start/" + QString::number(page));
        QNetworkRequest request(url);
        QNetworkReply* reply = manager.get(request);

        ++pendingReplies;
        connect(reply, &QNetworkReply::finished, this, [this, reply, isForPlay] () {onTracklistByArtistIdFinished(reply, isForPlay);});
    }
}

void MusicApi::getArtistDataBySongId(QString id)
{
    if(id.startsWith("t")) {
        id = id.mid(9);
    }

    QUrl url("https://rus.hitmotop.com/song/" + id);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply] () {onArtistDataBySongIdFinished(reply);});
}

QByteArray MusicApi::downloadSong(const QString &url)
{
    QNetworkRequest request(url);
    QNetworkReply* reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    QByteArray data;
    if(reply->error() == QNetworkReply::NoError) {
        data = reply->readAll();
    } else {
        qDebug() << "Error while downloading song" << reply->errorString();
    }

    reply->deleteLater();
    return data;
}

void MusicApi::onSearchByQueryFinished(QNetworkReply* reply)
{
    std::vector<Song> result;

    if(reply->error() == QNetworkReply::NoError) {
        result = parseHtml(reply->readAll());
        emit querySearchFinished(result);
    } else {
        emit error(reply->errorString());
    }

    reply->deleteLater();
}

void MusicApi::onSearchByGenreFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError) {
        std::vector<Song> pagesSongs = parseHtml(reply->readAll());
        accumulatedResults.insert(accumulatedResults.end(),
                                  pagesSongs.begin(), pagesSongs.end());
    }
    reply->deleteLater();

    --pendingReplies;
    if(pendingReplies == 0) {
        emit genreSearchFinished(accumulatedResults);
    }
}

void MusicApi::onSearchArtistsFinished(QNetworkReply *reply)
{
    QString strReply = reply->readAll();
    std::vector<ArtistData> result;

    if(strReply.contains("<ul class=\"singers-list album-list\">")) {
        auto artists_el = strReply.split("<ul class=\"singers-list album-list\">")[1].split("</ul>")[0].split("<li");
        artists_el.pop_front();

        for(const auto& el : artists_el) {
            ArtistData artist;
            artist.id = el.split(" <a href=\"/artist/")[1].split("\" class")[0].toStdString();
            artist.name = el.split("album-title\">")[1].split("</span>")[0].toStdString();

            result.push_back(artist);
        }
    }

    emit artistSearchFinished(result);
}

void MusicApi::onTracklistByArtistIdFinished(QNetworkReply *reply, bool isForPlay)
{
    if(reply->error() == QNetworkReply::NoError) {
        std::vector<Song> pageSongs = parseHtml(reply->readAll());
        accumulatedResults.insert(accumulatedResults.end(),
                                  pageSongs.begin(), pageSongs.end());
    }
    reply->deleteLater();

    --pendingReplies;
    if(pendingReplies == 0) {
        emit artistTracklistSearchFinished(accumulatedResults, isForPlay);
    }
}

void MusicApi::onArtistDataBySongIdFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError) {
        ArtistData result;

        QString strReply = reply->readAll();
        strReply = strReply.split("play-attr__link\" href=\"/artist/")[1];

        result.id = strReply.split("\">")[0].toStdString();
        result.name = strReply.split("\">")[1].split("</a>")[0].toStdString();
        emit artistIdSearchBySongFinished(result);
    }
}

std::vector<Song> MusicApi::parseHtml(const QByteArray &data)
{
    std::vector<Song> result;
    std::string html = data.toStdString();

    size_t start = 0;
    while((start = html.find("data-musmeta='", start)) != std::string::npos) {
        start += 14;
        size_t end = html.find("'>", start);
        if(end == std::string::npos)
            break;

        std::string jsonStr = html.substr(start, end-start);

        try {
            json j = json::parse(jsonStr);
            Song song;

            for(const auto& [key, value] : j.items()) {
                song[key] = value;
            }

            size_t durStart = html.find("<div class=\"track__fulltime\">", end);
            if(durStart != std::string::npos) {
                durStart += 30;
                size_t durEnd = html.find("</div>", durStart);

                if(durEnd != std::string::npos) {
                    song["duration"] = html.substr(durStart, durEnd - durStart);
                }
            }

            result.push_back(song);
        } catch (...) {
            qDebug() << "Error on parseHtml in api";
        }

        start = end;
    }

    return result;

}

