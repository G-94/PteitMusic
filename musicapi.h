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

using json = nlohmann::json;
using Song = std::map<std::string, std::string>;

class MusicApi : public QObject {
    Q_OBJECT
public:
    explicit MusicApi(QObject* parent = nullptr);

    void searchByQuery(const QString& query);
    void searchByGenre(const QString& genre);
    QByteArray downloadSong(const QString& url);

private slots:

    void onQuerySearchFinished(QNetworkReply* reply);

signals:

    void querySearchFinished(const std::vector<Song>& songs);
    void genreSearchFinished(const std::vector<Song>& songs);
    void error(const QString& e);

private:

    QNetworkAccessManager manager;
    std::vector<Song> parseHtml(const QByteArray& data);

};

#endif // MUSICAPI_H
