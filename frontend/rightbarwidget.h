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
#include "websocketclient.h"

#ifndef RIGHTBARWIDGET_H
#define RIGHTBARWIDGET_H


class RightBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightBarWidget(WebSocketClient *webSocket, int initialScreenWidth, int initialScreenHeight, QWidget *parent = nullptr, track *currentTrack = nullptr);
    void toggle_buttons();
    void on_toAuthor_clicked();
    void on_toLyrics_clicked();
    void on_followButton_clicked();
    void on_Play_clicked();
    void on_Pause_clicked();
    void resizeBarWidget(int width);
    int get_barWidth();
    void setNewCurrentTrack(const track &trackData);
    //void set_author_text_tab();

private:
    WebSocketClient *webSocket;
    int initialScreenWidth;
    int initialScreenHeight;
    int barSize;
    int coverSize;
    QWidget *scrollWidget;
    QPixmap originalCover;
    QPushButton *currentAlbumCoverButton;
    QPushButton *currentAlbumNameButton;
    QPushButton *currentTrackNameButton;
    QPushButton *currentTrackAuthorButton;
    QLabel *avaLabel;
    QPushButton *authorNameButton;
    QLabel *currentAlbumCoverLabel;
    QLabel *lyricsLabel;
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

