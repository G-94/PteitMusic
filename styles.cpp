#include "styles.h"

Style::Style() {}

QString Style::getCircleButtonStyle()
{
    return R"(
    QPushButton {
        background-color: rgba(128, 128, 128, 0.7);
        color: white;
        border: none;
        border-radius: 25px;
        font-size: 20px;
        font-weight: bold;
        font-family: 'Arial', 'Segoe UI', sans-serif;

        min-width: 50px;
        max-width: 50px;
        min-height: 50px;
        max-height: 50px;

        text-align: center;
    }

    QPushButton:hover {
        background-color: rgba(128, 128, 128, 0.9);
    }

    QPushButton:pressed {
        background-color: rgba(100, 100, 100, 0.9);
        padding-top: 2px;
        padding-left: 2px;
    }

    QPushButton:disabled {
        background-color: rgba(80, 80, 80, 0.3);
        color: rgba(255, 255, 255, 0.5);
    })";
}

QString Style::getSongTracklistItemStyle()
{
    return R"(
    [class="track-item"] QLabel {
        background-color: transparent;
        color: white;
    }
    )";
}

QString Style::getScrollableTracklistStyle() {
    return R"(

    QScrollArea {
        background-color: transparent;
        border: none;
    }

    QWidget#scrollAreaWidgetContents {
        background-color: transparent;
    }

    QScrollBar:vertical {
        background-color: #2d2d2d;
        width: 8px;
        border-radius: 4px;
    }

    QScrollBar::handle:vertical {
        background-color: #666666;
        border-radius: 4px;
        min-height: 30px;
    }

    QScrollBar::handle:vertical:hover {
        background-color: #888888;
    }

    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
        border: none;
        background: none;
        height: 0px;
    }
    )";
}

QString Style::getQFrameWithRadiusStyle()
{
    return R"(
        QFrame {
           background-color: #2d2d2d;
           border-radius: 15px;
           margin: 2px 8px;
        }
    )";
}

QString Style::getArtistListItemButtonStyle()
{
    return R"(
        QPushButton {
            background-color: rgba(128, 128, 128, 0.7);
            color: white;
            border: none;
            border-radius: 20px;
            padding: 2px 16px;
            font-size: 13px;
            font-weight: 500;
            min-width: 80px;
            min-height: 36px;
            font-family: 'Arial', 'Segoe UI', sans-serif;
        }

        QPushButton:hover {
            background-color: rgba(128, 128, 128, 0.9);
        }

        QPushButton:pressed {
            background-color: rgba(100, 100, 100, 0.9);
            padding-top: 5px;
            padding-left: 17px;
        }
    )";
}

QString Style::getArtistListItemStyle()
{
    return R"(
        QLabel {
            color: white;
            font-size: 16px;
            font-weight: bold;
            background-color: transparent;
        }
    )";

}

QString Style::getArtistListWidgetStyle()
{
    return R"(
    QScrollArea {
        background-color: transparent;
        border: none;
    }

    QWidget {
        background-color: transparent;
    }

    /* Стиль для горизонтального скролла */
    QScrollBar:horizontal {
        background-color: #2d2d2d;
        height: 8px;
        border-radius: 4px;
    }

    QScrollBar::handle:horizontal {
        background-color: #666666;
        border-radius: 4px;
        min-width: 30px;
    }

    QScrollBar::handle:horizontal:hover {
        background-color: #888888;
    }

    QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
        border: none;
        background: none;
        width: 0px;
    }
    )";
}


