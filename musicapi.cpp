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
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply] () {onQuerySearchFinished(reply);});
}

void MusicApi::searchByGenre(const QString &genre)
{
    std::vector<Song> result;
    std::vector<QPointer<QNetworkReply>> replies;

    for(int page = 0; page <= 480; page += 48) {
        QString strUrl = "https://rus.hitmotop.com/genre/" + genre + "/start/" + QString::number(page);
        QNetworkRequest request(strUrl);
        QNetworkReply* reply = manager.get(request);
        replies.push_back(reply);
    }

    for(auto& reply : replies) {
        if(!reply)
            continue;

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if(reply && reply->error() == QNetworkReply::NoError) {
            std::vector<Song> pagesSongs = parseHtml(reply->readAll());
            result.insert(result.end(), pagesSongs.begin(), pagesSongs.end());
        }

        reply->deleteLater();
    }

    emit genreSearchFinished(result);
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

void MusicApi::onQuerySearchFinished(QNetworkReply* reply)
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

