#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "videowindow.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void switchToVideo();
    void switchToMain();

private:
    Ui::MainWindow *ui;

    QTimer *m_timer;
    VideoWindow *videoWindow;
};
#endif // MAINWINDOW_H
