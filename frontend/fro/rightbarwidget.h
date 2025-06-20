#include <QScreen> //for Qscreen *screen = QApplication::primaryScreen();
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QPushButton>
// #include <QWebSocket>
#include <QMessageBox>
#include <QDir>
#include <QTabBar>


#include "mytrackswidget.h"

#ifndef RIGHTBARWIDGET_H
#define RIGHTBARWIDGET_H


class RightBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightBarWidget(QWidget *parent = nullptr, track *currentTrack = nullptr);
    void toggle_buttons();
    void on_toAuthor_clicked();
    void on_toLyrics_clicked();
    void on_followButton_clicked();
    //void set_author_text_tab();

private:
    track *currentTrack;
    QPushButton *toLyrics;
    QPushButton *toAuthor;
    bool isLyrics = 0;
    QTabWidget *author_lyrics_tab;
    QWidget *lyricsWidget;
    QWidget *authorWidget;
    QPushButton *followButton;
    bool isFollowed = 0;
};

#endif // RIGHTBARWIDGET_H

