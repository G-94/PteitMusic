#include "playlistshubpagewidget.h"
#include "MusicGlobals.h"

PlaylistsHubPageWidget::PlaylistsHubPageWidget(QWidget *parent)
    : QWidget{parent}
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

    main_layout = new QVBoxLayout(this);

    pageDescription = new QLabel("this is yout volna page");
    main_layout->addWidget(pageDescription);

    genreList = new GenreListWidget();

    loadGenreInfoJson();
    loadFamiliarArtistsJson();
    loadFamiliarSongsJson();

    if(genresData.empty()) {
        genresData = GENRES_DATA;
        saveGenreInfoJson();
    }

    genreList->setGenres(genresData);
    main_layout->addWidget(genreList);

    familiarArtistsList = new ArtistsListWidget();
    familiarArtistsList->setArtists(MusicGlobal::familiarArtists);
    main_layout->addWidget(familiarArtistsList);

    volna_btns_layout = new QHBoxLayout();
    btnPlayFamiliarSongs = new QPushButton("play familiar songs");
    volna_btns_layout->addWidget(btnPlayFamiliarSongs);
    main_layout->addLayout(volna_btns_layout);

    connect(genreList, &GenreListWidget::genreSelected, this, &PlaylistsHubPageWidget::onGenreSelected);
    connect(btnPlayFamiliarSongs, &QPushButton::clicked, this, &PlaylistsHubPageWidget::onPlayFamiliarSongs);

    connect(familiarArtistsList, &ArtistsListWidget::playArtistPlaylist, this, &PlaylistsHubPageWidget::onPlayFamiliarArtist);
    connect(familiarArtistsList, &ArtistsListWidget::findArtistTracks, this, &PlaylistsHubPageWidget::onFindFamiliarArtistTracklits);

    connect(&api, &MusicApi::artistTracklistSearchFinished, this, &PlaylistsHubPageWidget::onFamiliarArtistSignalRecieved);
}

PlaylistsHubPageWidget::~PlaylistsHubPageWidget()
{
    saveGenreInfoJson();
    saveFamiliarArtistsJson();
    saveFamiliarSongsJson();
}

void PlaylistsHubPageWidget::incrementGenreCounter(int genreId)
{
    for(auto& genre : genresData) {
        if(genre.id == genreId) {
            ++genre.playCounter;
            break;
        }
    }
}

void PlaylistsHubPageWidget::updateListWithNewData()
{
    familiarArtistsList->setArtists(MusicGlobal::familiarArtists);

    disconnect(familiarArtistsList, &ArtistsListWidget::playArtistPlaylist, this, &PlaylistsHubPageWidget::onPlayFamiliarArtist);
    disconnect(familiarArtistsList, &ArtistsListWidget::findArtistTracks, this, &PlaylistsHubPageWidget::onFindFamiliarArtistTracklits);

    connect(familiarArtistsList, &ArtistsListWidget::playArtistPlaylist, this, &PlaylistsHubPageWidget::onPlayFamiliarArtist);
    connect(familiarArtistsList, &ArtistsListWidget::findArtistTracks, this, &PlaylistsHubPageWidget::onFindFamiliarArtistTracklits);
}

void PlaylistsHubPageWidget::saveGenreInfoJson()
{
    json j = json::array();

    try {
        for(const auto& item : genresData) {
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

void PlaylistsHubPageWidget::loadGenreInfoJson()
{
    QFile file(JsonGenresPath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error on opening jsongenre file";
        return;
    }

    genresData.clear();

    QByteArray data = file.readAll();
    file.close();

    try {
        json j = json::parse(data.toStdString());

        for(const auto& item : j) {
            if(item.is_object()) {
                genresData.push_back(GenreData::fromJson(item));
            }
        }

        std::sort(genresData.begin(), genresData.end(), compareGenresByPlayCount);
    } catch (json::exception& e) {
        qDebug() << "catch json exc while loadGenreInfo" << e.what();
    } catch (std::exception& e) {
        qDebug() << "catch std::exception while loadGenreInfo" << e.what();
    }

}

void PlaylistsHubPageWidget::saveFamiliarSongsJson()
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

void PlaylistsHubPageWidget::loadFamiliarSongsJson()
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

void PlaylistsHubPageWidget::saveFamiliarArtistsJson()
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

void PlaylistsHubPageWidget::loadFamiliarArtistsJson()
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

void PlaylistsHubPageWidget::onGenreSelected(int id)
{
    disconnect(&api, &MusicApi::genreSearchFinished, this, nullptr);

    connect(&api, &MusicApi::genreSearchFinished, this, [=](const std::vector<Song>& tracklist) {
        if(tracklist.empty()) {
            qDebug() << "No tracks for genre" << id;
            return;
        }
        int randomInt = rand() % 480;
        emit playGenrePlaylist(tracklist, randomInt, id);
    });

    qDebug() << "Searching genre:" << id;
    api.searchByGenre(QString::number(id));
}

void PlaylistsHubPageWidget::onPlayFamiliarSongs()
{
    if (MusicGlobal::familiarSongs.empty()) return;

    int randomId = rand() % MusicGlobal::familiarSongs.size();
    emit playFamiliarSongPlaylist(MusicGlobal::familiarSongs, randomId);
}

void PlaylistsHubPageWidget::onPlayFamiliarArtist(const QString &artistId)
{
    api.getTracklistByArtistId(artistId, true);
}

void PlaylistsHubPageWidget::onFindFamiliarArtistTracklits(const QString &artistId)
{
    api.getTracklistByArtistId(artistId, false);
}

void PlaylistsHubPageWidget::onFamiliarArtistSignalRecieved(std::vector<Song> tracklist, bool isForPlay)
{
    if(isForPlay) {
        int randomId = rand() & tracklist.size();
        emit playFamiliarArtistPlaylist(tracklist, randomId);
    } else {
        emit findFamiliarArtistTracklits(tracklist);
    }
}
