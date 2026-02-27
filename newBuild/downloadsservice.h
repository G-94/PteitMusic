#ifndef DOWNLOADSSERVICE_H
#define DOWNLOADSSERVICE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QtConcurrent>

#include "MusicGlobals.h"
#include "musicapi.h"

class DownloadsService : public QObject
{
    Q_OBJECT
public:
    explicit DownloadsService(MusicApi* api_);

    void addSongToDownloads(const Song &song);

    void removeSongFromDownloads(std::string songID);

    void loadDownloadsSongs();

    void saveDownloadsSongsToJson();

    std::string clearPathName(const std::string &path);

private:

    MusicApi* api;

    QFutureWatcher<QByteArray> downloadWatcher;
    QFutureWatcher<bool> deleteWatcher;

    QString downloadsDir = "Downloads";
    QString downloadsJsonPath = "Data/downloads.json";
    QString pendingDownloadUrl;

signals:

    void downloadedSongsUpdated();

};

#endif // DOWNLOADSSERVICE_H
