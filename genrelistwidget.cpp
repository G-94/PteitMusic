#include "genrelistwidget.h"

GenreListWidget::GenreListWidget(QWidget *parent)
    : QWidget{parent}
{
    main_layout = new QVBoxLayout(this);

    scrollContent = new QWidget();
    buttons_layout = new QHBoxLayout(scrollContent);
    buttons_layout->setSpacing(8);

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollContent);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    main_layout->addWidget(scrollArea);
    setFixedHeight(150);
}

void GenreListWidget::setGenres(const std::vector<GenreData> &buttons_data)
{
    QLayoutItem* item;
    while((item = buttons_layout->takeAt(0)) != nullptr) {
        if(item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    buttonsData.clear();

    for(const auto& genre : buttons_data) {
        buttonsData[genre.name] = genre.id;
        QPushButton* new_btn = new QPushButton(QString::fromStdString(genre.name));

        buttons_layout->addWidget(new_btn);
        connect(new_btn, &QPushButton::clicked, this, &GenreListWidget::onClickedGenreButton);
    }

    buttons_layout->addStretch();
}

void GenreListWidget::onClickedGenreButton()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;

    int genreId = buttonsData.at(btn->text().toStdString());

    emit genreSelected(genreId);

}
