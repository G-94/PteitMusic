#ifndef HISTORYSERVICE_H
#define HISTORYSERVICE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

#include "MusicGlobals.h"

class HistoryService : public QObject
{
    Q_OBJECT
public:
    explicit HistoryService(QObject *parent = nullptr);

    void saveFamiliarSongsJson();
    void loadFamiliarSongsJson();
    void saveFamiliarArtistsJson();
    void loadFamiliarArtistsJson();

private:

    QString JsonFamiliarSongsPath = "Data/familiar_songs.json";
    QString JsonFamiliarArtistsPath = "Data/familiar_artists.json";

signals:
};

#endif // HISTORYSERVICE_H
