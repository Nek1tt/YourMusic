#ifndef ALBUMWIDGET_H
#define ALBUMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#include "mytrackswidget.h"
#include "myalbumswidget.h"

class AlbumTrackButton : public QPushButton {
    Q_OBJECT

public:
    explicit AlbumTrackButton(const track &trackData, bool isLiked, QString number, QWidget *parent = nullptr);
    explicit AlbumTrackButton(QWidget *parent = nullptr);

    QString getTrackName();
    track* getTrack();
    QString* getTrackAuthorUsertag();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void trackButtonClicked(track *trackData);
    void trackNameButtonClicked();
    void trackAuthorButtonClicked();
    void albumTrackLiked();

private:
    track trackData;
    QString number;
    QLabel *likeTrackButtonLabel;
    bool liked;
};

class AlbumWidget : public QWidget {
    Q_OBJECT

public:
    explicit AlbumWidget(const album &albumData, QVector<int> likedTracks, QWidget *parent = nullptr);
    AlbumWidget(const track &trackData, QWidget *parent = nullptr);

    void onTrackdoubleClicked(track *trackData);
    void loadCover(const QString& url, QLabel *label);

signals:
    void trackButtonClicked(track *trackData);
    void trackNameButtonClicked(track *trackData);
    void authorButtonClickedByTrackId(QString *trackUsertag);
    void authorButtonClicked(QString *trackUsertag);
    void albumTrackLiked(track *trackData);

private:
    album albumData;
};

#endif // ALBUMWIDGET_H
