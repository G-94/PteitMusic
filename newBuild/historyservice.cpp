#include "historyservice.h"

HistoryService::HistoryService(QObject *parent)
    : QObject{parent}
{
    QDir dir("Data");
    if(!dir.exists()) {
        dir.mkdir(".");
    }

    QFile familiarSongsJson(JsonFamiliarSongsPath);
    if(!familiarSongsJson.exists()) {
        if(familiarSongsJson.open(QIODevice::WriteOnly)) {
            familiarSongsJson.write("[]");
            familiarSongsJson.close();
            qDebug() << "Created empty familiar_songs.json";
        }
    }

    QFile familiarArtistsJson(JsonFamiliarArtistsPath);
    if(!familiarArtistsJson.exists()) {
        if(familiarArtistsJson.open(QIODevice::WriteOnly)) {
            familiarArtistsJson.write("[]");
            familiarArtistsJson.close();
            qDebug() << "Created empty familiar_artists.json";
        }
    }
}

void HistoryService::saveFamiliarSongsJson()
{
    json j = json::array();

    try {
        for(const auto& item : MusicGlobal::familiarSongs) {
            j.push_back(songToJson(item));
        }

        std::string jsonStr = j.dump(4);

        QFile file(JsonFamiliarSongsPath);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qDebug() << "error on openning json in save Familiar songs";
            return;
        }

        file.write(jsonStr.c_str(), jsonStr.size());
        file.close();
    } catch (json::exception& e) {
        qDebug() << "catch json exc while saveFamiliarSongs" << e.what();
    } catch (std::exception& e) {
        qDebug() << "catch std::exception while saveFamiliarSongs" << e.what();
    }

}

void HistoryService::loadFamiliarSongsJson()
{
    QFile file(JsonFamiliarSongsPath);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "error on openning json in load Familiar songs";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    try {
        json j = json::parse(data.toStdString());

        for(const auto& item : j) {
            if(item.is_object()) {
                MusicGlobal::familiarSongs.push_back(jsonToSong(item));
            }
        }
    } catch (json::exception& e) {
        qDebug() << "catch json exc while loadFamiliarSongs" << e.what();
    } catch (std::exception& e) {
        qDebug() << "catch std::exception while loadFamiliarSongs" << e.what();
    }
}

void HistoryService::saveFamiliarArtistsJson()
{
    json j = json::array();

    try {
        for(const auto& item : MusicGlobal::familiarArtists) {
            j.push_back(item.toJson());
        }

        std::string jsonStr = j.dump(4);

        QFile file(JsonFamiliarArtistsPath);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qDebug() << "error on openning json in save Familiar songs";
            return;
        }

        file.write(jsonStr.c_str(), jsonStr.size());
        file.close();
    } catch (json::exception& e) {
        qDebug() << "catch json exc while saveFamiliarArtists" << e.what();
    } catch (std::exception& e) {
        qDebug() << "catch std::exception while saveFamiliarArtists" << e.what();
    }

}

void HistoryService::loadFamiliarArtistsJson()
{
    QFile file(JsonFamiliarArtistsPath);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "error on openning json in load Familiar songs";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    try {
        json j = json::parse(data.toStdString());

        for(const auto& item : j) {
            if(item.is_object()) {
                MusicGlobal::familiarArtists.push_back(ArtistData::fromJson(item));
            }
        }

        std::sort(MusicGlobal::familiarArtists.begin(), MusicGlobal::familiarArtists.end(), compareArtistsByPlayCount);
    } catch (json::exception& e) {
        qDebug() << "catch json exc while loadFamiliarArtists" << e.what();
    } catch (std::exception& e) {
        qDebug() << "catch std::exception while loadFamiliarArtists" << e.what();
    }
}
