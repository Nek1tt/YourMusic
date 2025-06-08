#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#include "mytrackswidget.h"
#include "myalbumswidget.h"
#ifndef ALBUMWIDGET_H
#define ALBUMWIDGET_H

class AlbumTrackButton : public QPushButton {
    Q_OBJECT
public:
    explicit AlbumTrackButton(const track &trackData, QString number, QWidget *parent = nullptr);
    explicit AlbumTrackButton(QWidget *parent = nullptr);
    //QString getTrackName();
    //void resize_trackbutton(int width);
    QString getTrackName();
    track* getTrack();
    int getTrackId();

private:
    track trackData;
    QString number;
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
signals:
    void trackButtonClicked(track *trackData);
    void trackNameButtonClicked();
    void trackAuthorButtonClicked();
};

class AlbumWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumWidget(const album &albumData, QWidget *parent = nullptr);
    AlbumWidget(const track &trackData, QWidget *parent = nullptr);
    void onTrackdoubleClicked(track *trackData);
    void loadCover(const QString& url, QLabel *label);

private:
    album albumData;

signals:
    void trackButtonClicked(track *trackData);
    void trackNameButtonClicked(int trackId);
    void authorButtonClickedByTrackId(int trackid);
    void authorButtonClicked(QString authorUsertag);
};

#endif // ALBUMWIDGET_H
