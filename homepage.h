#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QString>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QList>
#include <QStackedWidget>
#include <QLabel>
#include <QDir>

#include "include/nlohmann/json/json.hpp"
#include "searchpanelwidget.h"
#include "playerwidget.h"
#include "likespagewidget.h"

using Song = std::map<std::string, std::string>;

class homepage : public QWidget
{
    Q_OBJECT
public:
    explicit homepage(QWidget *parent = nullptr);

private:

    QHBoxLayout* main_layout;
    QVBoxLayout* right_panel_layout;
    QFrame* left_pages_panel;
    QWidget *right_panel;

    QStackedWidget* pages;
    SearchPanelWidget* pageSearch;
    LikesPageWidget* pageLikes;

    QPushButton* btnHomePage;
    QPushButton* btnLikePage;
    QPushButton* btnDownloadedPage;

    PlayerWidget* player;

    std::vector<Song> liked_tracklist;
    std::vector<Song> downloaded_tracklist;

private slots:

    void setPlaySelectedSong(std::vector<Song> tracklist, int ID);

    void setLikeSelectedSong(std::vector<Song> tracklist, int ID);

    void setDownloadSelectedSong(std::vector<Song> tracklist, int ID);

    void setUnlikeSelectedSong(std::vector<Song> tracklist, int ID);

    void onLikesSongsUpdated(std::vector<Song> tracklist);




signals:
};

#endif // HOMEPAGE_H
