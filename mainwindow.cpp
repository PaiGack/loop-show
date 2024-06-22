#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow::MainWindow init";

    ui->setupUi(this);
    this->showFullScreen();

    QString m_filepath = "";
    if (!QFile::exists(m_filepath)) {
        qDebug() << "MainWindow::MainWindow not found file.";
        return;
    }

    videoWindow = new VideoWindow(m_filepath);
    connect(videoWindow, &VideoWindow::videoFinished, this, &MainWindow::switchToMain);

    // 设置定时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::switchToVideo);
    m_timer->start(60000); // 60秒后切换到视频窗口
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchToVideo()
{
    hide();

    videoWindow->show();
    videoWindow->play();
}

void MainWindow::switchToMain()
{
    videoWindow->hide();
    videoWindow->stop();

    show();
    m_timer->start(60000);
}
