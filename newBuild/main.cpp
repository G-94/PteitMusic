#include <QApplication>
#include <QDir>
#include <QtPlugin>

#include "homepage.h"

#include <QCoreApplication>
#include <QLibraryInfo>
#include <QDir>
#include <QDebug>
#include <QMediaDevices>
#include <QAudioDevice>

int main(int argc, char* argv[]) {

    qputenv("QT_FFMPEG_DEBUG", "1");
    QLoggingCategory::setFilterRules("qt.multimedia.ffmpeg.*=true");

    QApplication app(argc, argv);

    qDebug() << "=== ДИАГНОСТИКА ===";
    qDebug() << "Application path:" << QCoreApplication::applicationDirPath();

    // Проверяем multimedia плагин
    QDir multimediaDir(QCoreApplication::applicationDirPath() + "/multimedia");
    if (multimediaDir.exists()) {
        qDebug() << "Multimedia plugins:" << multimediaDir.entryList(QStringList() << "*.dll");
    }

    // Проверяем наличие FFmpeg DLL
    QDir appDir(QCoreApplication::applicationDirPath());
    QStringList ffmpegDlls = appDir.entryList(QStringList() << "av*.dll" << "libmp3*.dll");
    qDebug() << "FFmpeg DLLs found:" << ffmpegDlls;

    // Проверяем доступные бэкенды
    qDebug() << "Default audio output:" << QMediaDevices::defaultAudioOutput().description();
    qDebug() << "=== КОНЕЦ ДИАГНОСТИКИ ===";

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    MusicGlobal::current_liked_tracklist = {};
    MusicGlobal::current_downloaded_tracklist = {};

    homepage pl;
    pl.setMinimumSize(1280, 720);

    pl.show();


    return app.exec();
}
