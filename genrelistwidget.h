#ifndef GENRELISTWIDGET_H
#define GENRELISTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>

#include "MusicGlobals.h"

class GenreListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GenreListWidget(QWidget *parent = nullptr);

    void setGenres(const std::vector<GenreData>& buttons_data);

private:

    QVBoxLayout* main_layout;
    QWidget* scrollContent;
    QScrollArea* scrollArea;
    QHBoxLayout* buttons_layout;

    std::map<std::string, int> buttonsData;

private slots:

    void onClickedGenreButton();

signals:

    void genreSelected(int id);
};

#endif // GENRELISTWIDGET_H
