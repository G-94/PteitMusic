#ifndef MUSICGLOBALS_H
#define MUSICGLOBALS_H

#include <map>
#include <vector>
#include <string>
#include "include/nlohmann/json/json.hpp"

using Song = std::map<std::string, std::string>;
using json = nlohmann::json;

///

struct GenreData {
    std::string name;
    int id;
    int playCounter;

    GenreData() : id{0}, playCounter{0} {};
    GenreData(std::string name_, int id_, int playCounter_) : name{std::move(name_)}, id{id_}, playCounter{playCounter_} {};

    json toJson() const {
        return json {
            {"name", name},
            {"id", id},
            {"playCounter", playCounter}
        };
    }

    static GenreData fromJson (const json& j) {
        GenreData genre;
        genre.name = j.at("id");
        genre.id = j.at("name");
        genre.playCounter = j.at("playCounter").get<int>();
        return genre;
    }
};

inline bool compareGenresByPlayCount(const GenreData& a, const GenreData& b) {
    return a.playCounter > b.playCounter;
}

///

struct ArtistData {
    std::string name;
    std::string id;
    int playCounter;

    ArtistData() : name{}, id{} {};
    ArtistData(std::string name_, std::string id_) : name{std::move(name_)}, id{std::move(id_)}, playCounter{0} {};
    ArtistData(std::string name_, std::string id_, int playCounter_) : name{std::move(name_)}, id{std::move(id_)}, playCounter{playCounter_} {};

    json toJson() const {
        return json {
            {"name", name},
            {"id", id},
            {"playCounter", playCounter}
        };
    }

    static ArtistData fromJson(const json& j) {
        ArtistData data;
        data.id = j.at("id");
        data.name = j.at("name");
        data.playCounter = j.at("playCounter").get<int>();
        return data;
    }
};

inline bool compareArtistsByPlayCount(const ArtistData& a, const ArtistData& b) {
    return a.playCounter > b.playCounter;
}

///

namespace MusicGlobal {

inline std::vector<Song> current_liked_tracklist;
inline std::vector<Song> current_downloaded_tracklist;
inline std::vector<ArtistData> familiarArtists;
inline std::vector<GenreData> current_genres_data;
inline std::vector<Song> familiarSongs;

}

///

inline Song jsonToSong(const json &j)
{
    Song song;

    if(j.is_object()) {
        for(auto it = j.begin(); it != j.end(); ++it) {
            song[it.key()] = it.value().get<std::string>();
        }
    }

    return song;
}

inline json songToJson(const Song &song)
{
    json j;

    for(const auto& [key, value] : song) {
        j[key] = value;
    }

    return j;
}

///

inline std::vector<GenreData> GENRES_DATA = {
    {"Нью-эйдж", 51, 0},
    {"Медитация", 59, 0},
    {"Латиноамериканская", 32, 0},
    {"Джаз", 39, 0},
    {"Поп", 2, 10},
    {"Другое", 13, 0},
    {"Иностранный рэп", 99, 10},
    {"Рок", 6, 10},
    {"Кантри", 42, 0},
    {"Электроника", 8, 0},
    {"Старая классика", 28, 0},
    {"Альтернатива", 7, 0},
    {"Саундтреки", 4, 0},
    {"Музыка мира", 9, 0},
    {"Эмбиент", 194, 0},
    {"Блюз", 17, 0},
    {"Иностранная авторская песня", 101, 0},
    {"Соул", 24, 0},
    {"Лёгкая музыка", 16, 0},
    {"Современная классика", 92, 0},
    {"Восточная", 73, 0},
    {"Детская", 15, 0},
    {"Хаус", 20, 0},
    {"Лаундж", 33, 0},
    {"Эстрада и ретро", 5, 0},
    {"Панк", 12, 0},
    {"Африканская", 62, 0},
    {"Танцевальная", 11, 0},
    {"Регги", 30, 0},
    {"Современный", 63, 0},
    {"Техно", 45, 0},
    {"Экспериментальная", 55, 0},
    {"Инди", 18, 0},
    {"R&&B", 19, 10},
    {"Русский рэп", 75, 10},
    {"Рок-н-ролл", 61, 0},
    {"Вокал", 52, 0},
    {"Транс", 1, 0},
    {"Европейская", 26, 0},
    {"Аудиокниги", 107, 0},
    {"Традиционный", 70, 0},
    {"Реггетон", 43, 0},
    {"Из фильмов", 82, 0},
    {"Прогрессивный рок", 50, 0},
    {"Русская поп-музыка", 88, 0},
    {"Спортивная", 36, 0},
    {"Драм-н-бэйс", 31, 0},
    {"Американская", 57, 0},
    {"Русский рок", 76, 0},
    {"Шансон", 14, 0},
    {"Экстрим", 97, 0},
    {"Современная классика", 47, 0},
    {"Фанк", 46, 0},
    {"K-pop", 106, 0},
    {"Русская", 87, 0},
    {"Праздничная", 48, 0},
    {"Хард-рок", 105, 0},
    {"Металл", 10, 0},
    {"Фолк-рок", 102, 0},
    {"Рэп и хип-хоп", 3, 0},
    {"Дабстеп", 29, 0},
    {"Хардкор", 44, 0},
    {"Индастриал", 34, 0},
    {"Кельтская", 68, 0},
    {"Местное", 78, 0},
    {"Ска", 25, 0},
    {"Фолк", 216, 0},
    {"Балканская", 69, 0},
    {"Болливуд", 38, 0},
    {"Звуки природы и шум города", 158, 0},
    {"Даб", 56, 0},
    {"Мюзиклы", 65, 0},
    {"New Wave", 49, 0},
    {"Авторская песня", 21, 0},
    {"Пост-хардкор", 100, 0},
    {"R&B и Урбан", 60, 0},
    {"Русская", 79, 0},
    {"Ню-метал", 93, 0},
    {"Вокальный джаз", 201, 0},
    {"Эпический", 96, 0},
    {"Для детей", 91, 0},
    {"Из сериалов", 83, 0},
    {"Бибоп", 200, 0},
    {"Прогрессив", 95, 0},
    {"J-pop", 154, 0},
    {"Турецкая народная музыка", 172, 0},
    {"Диско", 22, 0},
    {"Еврейская", 74, 0},
    {"Аргентинское танго", 103, 0},
    {"Пост-рок", 80, 0},
    {"Шедевры мировой классики", 171, 0},
    {"Из видеоигр", 85, 0},
    {"Кавказская", 89, 0},
    {"Фонк", 209, 0},
    {"Исламская музыка", 221, 0},
    {"Из мультфильмов", 84, 0},
    {"Русская", 67, 0},
    {"Узбекская поп-музыка", 113, 0},
    {"Аудиосказки", 90, 0},
    {"Турецкая поп-музыка", 109, 0},
    {"Фолк", 94, 0},
    {"Дэт-метал", 196, 0},
    {"Татарская", 86, 0},
    {"Арабский поп", 217, 0},
    {"Аниме", 174, 0},
    {"Турецкий рэп и хип-хоп", 111, 0},
    {"Блэк-метал", 197, 0},
    {"Украинский рок", 77, 0},
    {"Стоунер-рок", 104, 0},
    {"Смус-джаз", 202, 0},
    {"Армянская музыка", 152, 0},
    {"Металкор", 192, 0},
    {"Турецкая классическая музыка", 169, 0},
    {"Арабеска", 170, 0},
    {"Романсы", 81, 0},
    {"EDM", 212, 0},
    {"Биг бэнды", 203, 0},
    {"Израильский рок", 168, 0},
    {"Альтернативный метал", 199, 0},
    {"Израильский рэп и хип-хоп", 112, 0},
    {"Трэш-метал", 195, 0},
    {"Художественная литература", 164, 0},
    {"Аудиокниги на иностранном языке", 181, 0},
    {"Азербайджанская музыка", 151, 0},
    {"Турецкий рок", 155, 0},
    {"Гиперпоп", 208, 0},
    {"Раи", 235, 0},
    {"Египетский поп", 219, 0},
    {"Готический", 189, 0},
    {"Арабский рэп и хип-хоп", 222, 0},
    {"Грузинская музыка", 153, 0},
    {"Дум-метал", 198, 0},
    {"Турецкая альтернативная музыка", 110, 0},
    {"Израильская поп-музыка", 167, 0},
    {"Азербайджанская поп-музыка", 173, 0},
    {"Казахская эстрада", 225, 0},
    {"Колыбельные", 214, 0},
    {"Брейкбит", 193, 0},
    {"Иракский поп", 234, 0},
    {"UK Garage", 205, 0},
    {"Пост-метал", 191, 0},
    {"Казахская народная музыка", 233, 0},
    {"Сладж", 190, 0},
    {"Пост-панк", 213, 0},
};


#endif // MUSICGLOBALS_H
