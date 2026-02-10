#ifndef MUSICGLOBALS_H
#define MUSICGLOBALS_H

#include <map>
#include <vector>
#include <string>

using Song = std::map<std::string, std::string>;

namespace MusicGlobal {

inline std::vector<Song> current_liked_tracklist;
inline std::vector<Song> current_downloaded_tracklist;

}

#endif // MUSICGLOBALS_H
