#ifndef ARTISTSLISTITEM_H
#define ARTISTSLISTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "MusicGlobals.h"

class ArtistsListItem : public QWidget
{
    Q_OBJECT
public:
    ArtistsListItem(const ArtistData& data_);

private:

    QVBoxLayout* main_layout;

    QHBoxLayout* btn_layout;
    QPushButton* btnArtistFindTracks;
    QPushButton* btnArtistPlayPlaylist;

    QLabel* artistNameLabel;
    ArtistData data;

signals:

    void findArtistTracks(const QString& id);
    void playArtistPlaylist(const QString& id);
};

#endif // ARTISTSLISTITEM_H
