#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPixmap>
#include <QTextEdit>


#ifndef CREATEWIDGET_H
#define CREATEWIDGET_H

class CreateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateWidget(QWidget *parent = nullptr);

private slots:
    void chooseImage();
    void on_typeButton_clicked();
    void addAuthor(const QString &name);
    void addNewTrackButton();
private:
    QLabel *imageLabel;
    QHBoxLayout *authorLayout;
    QLineEdit *nameEdit;
    QTextEdit *descriptionEdit;
    QTextEdit *lyricsEdit;
    bool isSingle = 1;
    QPushButton *typeButton;
    QVBoxLayout *trackLayout;
    QWidget *trackContainer;
};

#endif // CREATEWIDGET_H
