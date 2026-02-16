#include <QApplication>
#include <QDir>
#include "homepage.h"
#include "musicapi.h"
#include "playerwidget.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    MusicGlobal::current_liked_tracklist = {};
    MusicGlobal::current_downloaded_tracklist = {};

    homepage pl;
    pl.setFixedSize(1280, 720);

    pl.show();



    return app.exec();
}
