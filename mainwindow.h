#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QImage>

#define SCALLED_IMG_SIZE 256

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void begin();
    void addPicture();
    void transformPicture();
    void showFullOrigin();
    void showFullTransformed();

protected:
    QLabel *originLabel;
    QLabel *transformLabel;
    QPushButton *addButton;
    QPushButton *transformButton;
    QPushButton *fullOriginButton;
    QPushButton *fullTransformedButton;
    QPushButton *exitButton;
    QImage originImg;
    QImage transformedImg;
};

#endif // MAINWINDOW_H
