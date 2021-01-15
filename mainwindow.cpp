#include "fouriertransform.h"
#include "mainwindow.h"
#include <iostream>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QFileDialog>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      originLabel          (new QLabel(this)),
      transformLabel       (new QLabel(this)),
      addButton            (new QPushButton("Add picture", this)),
      transformButton      (new QPushButton("Fourier transform", this)),
      fullOriginButton     (new QPushButton("Show Full Size", this)),
      fullTransformedButton(new QPushButton("Show Full Size", this)),
      exitButton           (new QPushButton("Exit", this))
{
    QHBoxLayout *originButtonsBox = new QHBoxLayout();
    originButtonsBox->addWidget(addButton);
    originButtonsBox->addWidget(fullOriginButton);
    originButtonsBox->setAlignment(Qt::AlignLeft);

    QVBoxLayout *originBox = new QVBoxLayout();
    originBox->addLayout(originButtonsBox);
    originBox->addWidget(originLabel);

    QHBoxLayout *transformButtonsBox = new QHBoxLayout();
    transformButtonsBox->addWidget(transformButton);
    transformButtonsBox->addWidget(fullTransformedButton);
    transformButtonsBox->setAlignment(Qt::AlignLeft);

    QVBoxLayout *transformBox = new QVBoxLayout();
    transformBox->addLayout(transformButtonsBox);
    transformBox->addWidget(transformLabel);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(originBox);
    mainLayout->addLayout(transformBox);
    mainLayout->addWidget(exitButton);

    setLayout(mainLayout);

    begin();

    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addPicture()));
    connect(transformButton, SIGNAL(clicked(bool)), this, SLOT(transformPicture()));
    connect(fullOriginButton, SIGNAL(clicked(bool)), this, SLOT(showFullOrigin()));
    connect(fullTransformedButton, SIGNAL(clicked(bool)), this, SLOT(showFullTransformed()));
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}


MainWindow::~MainWindow()
{
    delete originLabel;
    delete transformLabel;
    delete addButton;
    delete transformButton;
    delete fullOriginButton;
    delete fullTransformedButton;
    delete exitButton;

    std::cout<< "Bye-bye app!" <<std::endl;
}


void MainWindow::begin()
{
    addButton            ->setEnabled(true);
    fullOriginButton     ->setEnabled(false);
    transformButton      ->setEnabled(false);
    fullTransformedButton->setEnabled(false);
    exitButton           ->setEnabled(true);

    exitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    originLabel   ->setScaledContents(true);
    transformLabel->setScaledContents(true);
}


void MainWindow::addPicture()
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Add Picture", "~/", "*.jpg *.png");
    if(str.isEmpty())
        return;

    originImg = QImage(str);

    originLabel->setPixmap(QPixmap::fromImage(originImg).scaled(SCALLED_IMG_SIZE, SCALLED_IMG_SIZE, Qt::KeepAspectRatioByExpanding));

    if(!fullOriginButton->isEnabled())
    {
        fullOriginButton->setEnabled(true);
        transformButton->setEnabled(true);
    }
}


void MainWindow::transformPicture()
{
    transformedImg = originImg.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat img = cv::Mat(transformedImg.height(), transformedImg.width(), CV_8U, (uchar*)transformedImg.bits(), transformedImg.bytesPerLine()).clone();

    cv::Mat dst;
    fourierTransform(img, dst);

    transformedImg = QImage((uchar*)dst.data, dst.cols, dst.rows, dst.step, QImage::Format_Grayscale8);
    transformLabel->setPixmap(QPixmap::fromImage(transformedImg.scaled(SCALLED_IMG_SIZE, SCALLED_IMG_SIZE, Qt::KeepAspectRatioByExpanding)));


    if(!fullTransformedButton->isEnabled())
        fullTransformedButton->setEnabled(true);
}


void MainWindow::showFullOrigin()
{
    QLabel *secW = new QLabel(this);
    secW->setWindowFlags(Qt::Window);
    secW->setPixmap(QPixmap::fromImage(originImg));
    secW->show();
}


void MainWindow::showFullTransformed()
{
    QLabel *secW = new QLabel(this);
    secW->setWindowFlags(Qt::Window);
    secW->setPixmap(QPixmap::fromImage(transformedImg));
    secW->show();
}
