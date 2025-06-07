#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPixmap>
#include <QTextEdit>
#include "websocketclient.h"

#ifndef CREATEWIDGET_H
#define CREATEWIDGET_H

class TrackWidget : public QWidget {
    Q_OBJECT
public:
    explicit TrackWidget(QString fileName, int index, QWidget *parent = nullptr);
    void addAuthor(const QString &name);
    QLineEdit *getTitleEdit();
    QVector<QString> *getAuthors();
    int getIndex();
    void setIndex(int newIndex);
    void setAddButtonStyle(QString color);
    void resetLineEditStyle(QLineEdit *lineEdit);
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    QString fileName;
    QTextEdit *tracLyricsEdit;
    QHBoxLayout *authorLayout;
    int trackIndex;
    QVector<QString> authors;
    QLineEdit *titleEdit;
    QLabel *numberLabel;
    QPushButton *addAuthorButton;
signals:
    void removeButtonClicked();
};


class CreateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateWidget(QString *usertag, WebSocketClient *webSocketStas, QWidget *parent = nullptr);

private slots:
    void chooseImage();
    void on_typeButton_clicked();
    void addAuthor(const QString &name, int index);
    void addNewTrackButton();
    void on_upload_clicked();
    void resetLineEditStyle(QLineEdit *lineEdit);
    void onTextMessageReceived(const QString &type, const QJsonObject &data);
private:
    QLabel *imageLabel;
    QString *usertag;
    QHBoxLayout *authorLayout;
    QLineEdit *nameEdit;
    QTextEdit *descriptionEdit;
    QTextEdit *lyricsEdit;
    QString typeOfObject = "single";
    QPushButton *typeButton;
    QVBoxLayout *trackLayout;
    QWidget *trackContainer;
    QPushButton *addTrackBtn;
    int trackIndex = 0;
    QVector<TrackWidget*> trackWidgets;
    QVector<QVector<QString>*> authors;
    QVector<QLineEdit*> titles;
    QVector<QPushButton*> addAuthorButtons;
    QVector<QLabel*> numberLabels;
    WebSocketClient *webSocketStas;
    QString fileCoverName;

};

#endif // CREATEWIDGET_H
