#ifndef ARTISTSLISTITEM_H
#define ARTISTSLISTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QFrame>

#include "MusicGlobals.h"
#include "styles.h"

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

    QFrame* container;
    bool isHovered = false;

    QLabel* artistNameLabel;
    ArtistData data;

protected:

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:

    void findArtistTracks(const QString& id);
    void playArtistPlaylist(const QString& id);
};

#endif // ARTISTSLISTITEM_H
