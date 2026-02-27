#ifndef ARTISTSLISTWIDGET_H
#define ARTISTSLISTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>

#include "artistslistitem.h"

class ArtistsListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ArtistsListWidget(QWidget *parent = nullptr);

    void setArtists(const std::vector<ArtistData>& artistList);
    void clear();

private:

    QVBoxLayout* main_layout;
    QScrollArea* scrollArea;
    QWidget* scrollContent;
    QHBoxLayout* scroll_layout;

    std::vector<ArtistsListItem*> items;

private slots:

    void onFindArtistTracks(const QString& artistId);
    void onPlayArtistPlaylist(const QString& artistId);

signals:

    void findArtistTracks(const QString& artistId);
    void playArtistPlaylist(const QString& artistId);
};

#endif // ARTISTSLISTWIDGET_H
