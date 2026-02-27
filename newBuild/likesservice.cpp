#include "likesservice.h"

LikesService::LikesService(QObject *parent)
    : QObject{parent}
{
    QFile likesFile(likesPath);
    if(!likesFile.exists()) {
        if(likesFile.open(QIODevice::WriteOnly)) {
            likesFile.write("[]");
            likesFile.close();
            qDebug() << "Created empty likes file:" << likesPath;
        }
    }
}

void LikesService::addSongToLikes(const Song &song)
{
    try {
        for(const Song& existing_song : MusicGlobal::current_liked_tracklist) {
            if(song.at("id") == existing_song.at("id")) {
                qDebug() << "Song already in likes";
                return;
            }
        }

        MusicGlobal::current_liked_tracklist.push_back(song);

        saveLikedSongs();
        emit likesSongUpdated();
    } catch (...) {
        qDebug() << "something went wrong while adding song to Likes";
    }
}

void LikesService::removeSongFromLikes(const std::string &songID)
{
    try {
        auto it = std::remove_if(MusicGlobal::current_liked_tracklist.begin(), MusicGlobal::current_liked_tracklist.end(),
                                 [&songID] (Song& song) {return song.at("id") == songID;});

        if(it != MusicGlobal::current_liked_tracklist.end()) {
            MusicGlobal::current_liked_tracklist.erase(it);
            saveLikedSongs();
            emit likesSongUpdated();

            qDebug() << "Song removed from likes";
        }
    } catch (...) {
        qDebug() << "error while removing song from likes";
    }
}

void LikesService::loadLikesSongs()
{
    QFile file(likesPath);

    if(!file.exists()) {
        MusicGlobal::current_liked_tracklist.clear();
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "failed to read json";
        return;
    }

    QTextStream in(&file);
    QString jsonStr = in.readAll();
    file.close();

    try {
        json j = json::parse(jsonStr.toStdString());

        MusicGlobal::current_liked_tracklist.clear();

        for(const auto& item : j) {
            if(item.is_object()) {
                Song song = jsonToSong(item);
                MusicGlobal::current_liked_tracklist.push_back(song);
            }
        }

        qDebug() << "loaded" << MusicGlobal::current_liked_tracklist.size() << "liked songs";
    } catch (const json::exception& e) {
        qDebug() << e.what();
    } catch (const std::exception& e) {
        qDebug() << e.what();
    }
}

void LikesService::saveLikedSongs()
{
    try {
        json j = json::array();

        for(const auto& song : MusicGlobal::current_liked_tracklist) {
            j.push_back(songToJson(song));
        }

        std::string jsonStr = j.dump(4);

        QFile file(likesPath);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "openning file error in saveLikedSongsToFile";
            return;
        }

        QTextStream out(&file);
        out << QString::fromStdString(jsonStr);
        file.close();

        qDebug() << "Saved" << MusicGlobal::current_liked_tracklist.size() << "songs to likes file";
    } catch (...) {
        qDebug() << "error while saving liked songs to file";
    }
}
