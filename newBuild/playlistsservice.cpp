#include "playlistsservice.h"

PlaylistsService::PlaylistsService()
{
    QDir dir("Data");
    if(!dir.exists()) {
        dir.mkdir(".");
    }

    QFile genresJson(JsonGenresPath);
    if(!genresJson.exists()) {
        if(genresJson.open(QIODevice::WriteOnly)) {
            genresJson.write("[]");
            genresJson.close();
            qDebug() << "Created empty genres.json";
        }
    }
}

void PlaylistsService::incrementGenreCounter(int genreId)
{
    for(auto& genre : MusicGlobal::current_genres_data) {
        if(genre.id == genreId) {
            ++genre.playCounter;
            break;
        }
    }
}

void PlaylistsService::saveGenreInfoJson()
{
    json j = json::array();

    try {
        for(const auto& item : MusicGlobal::current_genres_data) {
            j.push_back(item.toJson());
        }

        std::string jsonStr = j.dump(4);

        QFile file(JsonGenresPath);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qDebug() << "Error on opening jsongenre file";
            return;
        }

        file.write(jsonStr.c_str(), jsonStr.size());
        file.close();

        qDebug() << "dave genre info success";
    } catch (json::exception& e) {
        qDebug() << "catch json exc while saveGenreInfo" << e.what();
    } catch (std::exception& e) {
        qDebug() << "catch std::exception while saveGenreInfo" << e.what();
    }
}

void PlaylistsService::loadGenreInfoJson()
{
    QFile file(JsonGenresPath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error on opening jsongenre file";
        return;
    }

    MusicGlobal::current_genres_data.clear();

    QByteArray data = file.readAll();
    file.close();

    try {
        json j = json::parse(data.toStdString());

        for(const auto& item : j) {
            if(item.is_object()) {
                MusicGlobal::current_genres_data.push_back(GenreData::fromJson(item));
            }
        }

        std::sort(MusicGlobal::current_genres_data.begin(), MusicGlobal::current_genres_data.end(), compareGenresByPlayCount);
    } catch (json::exception& e) {
        qDebug() << "catch json exc while loadGenreInfo" << e.what();
    } catch (std::exception& e) {
        qDebug() << "catch std::exception while loadGenreInfo" << e.what();
    }

}
