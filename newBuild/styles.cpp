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
    }

    QPushButton:focus {
        outline: none;
        border: none;
    }

    )";
}

QString Style::getSongTracklistItemStyle()
{
    return R"(
    [class="track-item"] QLabel {
        background-color: transparent;
        color: white;
        border: none;
        outline: none;
    }

    [class="track-item"] QLabel:focus {
        border: none;
        outline: none;
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

        QPushButton:focus {
            outline: none;
            border: none;
        }
    )";
}

QString Style::getArtistListItemStyle()
{
    return R"(
        QLabel {
            color: white;
            border: none;
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

QString Style::getLineEditStyle()
{
    return R"(
    QLineEdit {
        background-color: #3d3d3d !important;
        color: white !important;
        border: 2px solid #4d4d4d !important;
        border-radius: 25px !important;
        padding: 8px 16px !important;
        font-size: 14px !important;
        font-family: 'Arial', 'Segoe UI', sans-serif !important;
        selection-background-color: #ca6f35 !important;
        selection-color: white !important;
        min-height: 20px !important;
        max-height: 40px !important;
    }

    QLineEdit:focus {
        border: 2px solid #5d5d5d !important;
        background-color: #454545 !important;
    }

    QLineEdit:hover {
        background-color: #454545 !important;
        border: 2px solid #6d6d6d !important;
    }

    QLineEdit::placeholder {
        color: #888888 !important;
        font-style: italic !important;
    }

    QLineEdit:disabled {
        background-color: #2d2d2d !important;
        color: #666666 !important;
        border: 2px solid #3d3d3d !important;
    }
    )";
}

QString Style::getGenreListWidgetStyle()
{
    return R"(
    GenreListWidget {
        background-color: #2d2d2d;
        border-radius: 12px;
        border: 1px solid #404040;
    }

    QScrollArea {
        background-color: transparent;
        border: none;
        border-radius: 12px;
    }

    QWidget {
        background-color: transparent;
    }

    QPushButton {
        background-color: #333333;
        color: #ffffff;
        border: none;
        border-radius: 8px;
        padding: 10px 18px;
        font-size: 14px;
        font-weight: bold;
        font-family: 'Arial', 'Segoe UI', sans-serif;
    }

    QPushButton:hover {
        background-color: #404040;
    }

    QPushButton:pressed {
        background-color: #4d4d4d;
        padding-top: 11px;
        padding-left: 19px;
    }

    QPushButton:focus {
        outline: none;
        border: none;
    }

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

QString Style::getNavButtonStyle()
{
    return R"(
    QPushButton {
        background-color: #333333;
        color: #ffffff;
        border: none;
        border-radius: 25px;
        padding: 12px 0px;
        font-size: 14px;
        font-weight: bold;
        font-family: 'Arial', 'Segoe UI', sans-serif;
        min-width: 120px;
        max-width: 140px;
        min-height: 50px;
        max-height: 50px;
        text-align: center;
        margin: 5px 10px;
    }

    QPushButton:hover {
        background-color: #404040;
    }

    QPushButton:pressed {
        background-color: #4d4d4d;
        padding-top: 13px;
    }

    QPushButton:checked {
        background-color: #4d4d4d;
        font-weight: bold;
    }

    QPushButton:focus {
        outline: none;
        border: none;
    }
    )";
}

QString Style::getLabelStyle()
{
    return R"(
    QLabel {
        color: #ffffff;
        background-color: transparent;
        border: none;
        font-size: 14px;
        font-family: 'Arial', 'Segoe UI', sans-serif;
    }
    )";
}

QString Style::getQFrameWidgetStyle()
{
    return R"(
    QFrame {
        background-color: #2d2d2d;
        border-radius: 15px;
        border: 1px solid #404040;
        margin: 10px;
    }
    )";
}

QString Style::getStreamButtonStyle()
{
    return R"(
    QPushButton {
        background-color: #333333;
        color: #ffffff;
        border: none;
        border-radius: 8px;
        padding: 12px 20px;
        font-size: 14px;
        font-weight: bold;
        font-family: 'Arial', 'Segoe UI', sans-serif;
        min-width: 170px;
        max-width: 190px;
        outline: none;
    }

    QPushButton:hover {
        background-color: #404040;
    }

    QPushButton:pressed {
        background-color: #4d4d4d;
        padding-top: 13px;
        padding-left: 21px;
    }

    QPushButton:focus {
        outline: none;
        border: none;
    }
    )";
}

QString Style::getPlayerWidgetStyle()
{
    return R"(
    PlayerWidget {
        background-color: #1a1a1a;
        border-top: 1px solid #333333;
    }

    PlayerWidget QLabel {
        color: #ffffff;
        background-color: transparent;
        border: none;
    }

    PlayerWidget QLabel#trackTitle {
        font-size: 16px;
        font-weight: bold;
        color: #ffffff;
        padding: 5px 0px;
    }

    PlayerWidget QSlider::groove:horizontal {
        border: none;
        height: 4px;
        background: #333333;
        border-radius: 2px;
    }

    PlayerWidget QSlider::handle:horizontal {
        background: #ca6f35;
        border: none;
        width: 12px;
        height: 12px;
        margin: -4px 0;
        border-radius: 6px;
    }

    PlayerWidget QSlider::handle:horizontal:hover {
        background: #ca6f35;
    }

    PlayerWidget QSlider::handle:horizontal:pressed {
        background: #ca6f35;
    }

    PlayerWidget QSlider::sub-page:horizontal {
        background: #ca6f35;
        border-radius: 2px;
    }

    PlayerWidget QPushButton {
        background-color: #333333;
        color: #ffffff;
        border: none;
        border-radius: 25px;
        font-size: 18px;
        font-weight: bold;
        min-width: 40px;
        max-width: 40px;
        min-height: 40px;
        max-height: 40px;
        text-align: center;
        margin: 0px 5px;
        outline: none;
    }

    PlayerWidget QPushButton:focus {
        outline: none;
    }

    PlayerWidget QPushButton:hover {
        background-color: #404040;
    }

    PlayerWidget QPushButton:pressed {
        background-color: #4d4d4d;
        padding-top: 1px;
        padding-left: 1px;
    }

    PlayerWidget QPushButton:disabled {
        background-color: #2a2a2a;
        color: #666666;
    }

    PlayerWidget QPushButton#btnPlay {
        background-color: #1db954;
        font-size: 20px;
    }

    PlayerWidget QPushButton#btnPlay:hover {
        background-color: #1ed760;
    }

    PlayerWidget QPushButton#btnPlay:pressed {
        background-color: #1aa34a;
    }

    PlayerWidget QPushButton#btnLike[text="♥"] {
        background-color: #ff4d4d;
    }

    PlayerWidget QPushButton#btnLike[text="♡"] {
        background-color: #333333;
    }

    PlayerWidget QPushButton#btnLike[text="♥"]:hover {
        background-color: #ff6666;
    }

    PlayerWidget QPushButton#btnDownload[text="✕"] {
        background-color: #ff4d4d;
    }

    PlayerWidget QPushButton#btnDownload[text="⤓"] {
        background-color: #333333;
    }

    PlayerWidget QPushButton#btnDownload[text="✕"]:hover {
        background-color: #ff6666;
    }

    PlayerWidget QRadioButton {
        color: #ffffff;
        background-color: #333333;
        border: none;
        border-radius: 20px;
        padding: 8px 12px;
        font-size: 12px;
        min-width: 80px;
        max-height: 30px;
        spacing: 5px;
    }

    PlayerWidget QRadioButton::indicator {
        width: 16px;
        height: 16px;
        border-radius: 8px;
        background-color: #666666;
    }

    PlayerWidget QRadioButton:focus {
        outline: none;
    }

    PlayerWidget QRadioButton::indicator:checked {
        background-color: #ca6f35;
    }

    PlayerWidget QRadioButton:hover {
        background-color: #404040;
    }

    PlayerWidget QRadioButton:pressed {
        background-color: #4d4d4d;
    }

    PlayerWidget QFrame#buttonsContainer {
        background-color: transparent;
        border: none;
        margin: 5px 0px;
    }
    )";
}
