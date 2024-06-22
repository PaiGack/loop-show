#include "videowindow.h"

#include <QDebug>

#include "vlc/vlc.h"

VideoWindow::VideoWindow(const QString &filepath, QWidget *parent)
    : QWidget{parent}, m_filepath(filepath)
{
    qDebug() << "VideoWindow::VideoWindow init";

    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_label->setAttribute(Qt::WA_OpaquePaintEvent);

    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_label);
    setLayout(m_layout);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    this->showFullScreen();
    this->hide();

    loadVlc();
}

VideoWindow::~VideoWindow()
{
    freeVlc();
}

// 播放
void VideoWindow::play()
{
    if (libvlc_Ended == libvlc_media_get_state(m_media))
    {
        stop(); // vlc这个设定太奇怪了，当自然播放结束，重新播放还得主动调用一次stop();
    }
    libvlc_media_player_play(m_mediaPlayer);
}

// 停止
void VideoWindow::stop()
{
    m_bPlaying = false;
    libvlc_media_player_stop(m_mediaPlayer);
}

// 加载
bool VideoWindow::loadVlc()
{
    m_vlcInstance = libvlc_new(0, NULL);
    m_media = libvlc_media_new_path(m_vlcInstance, m_filepath.toUtf8().data());
    if (!m_media)
    {
        freeVlc();
        return m_media;
    }
    qDebug() << "loadVlc" << qPrintable(m_filepath);
    m_mediaPlayer = libvlc_media_player_new_from_media(m_media);
    if (!m_mediaPlayer)
    {
        freeVlc();
        return false;
    }
    libvlc_media_parse(m_media);
    libvlc_event_manager_t *em = libvlc_media_player_event_manager(m_mediaPlayer);
    libvlc_event_attach(em, libvlc_MediaPlayerTimeChanged, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerEndReached, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerStopped, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerPlaying, vlcEvents, this);
    libvlc_event_attach(em, libvlc_MediaPlayerPaused, vlcEvents, this);

    libvlc_media_player_set_hwnd(m_mediaPlayer, (void *)this->m_label->winId());
    return true;
}

void VideoWindow::onVlcEvent(const libvlc_event_t *ev)
{
    qDebug() << "VideoWindow::onVlcEvent" << ev;
    switch (ev->type)
    {
    case libvlc_MediaPlayerEndReached: // 不能在回调里调用stop();否则会卡死
        qDebug() << "libvlc_MediaPlayerEndReached";
        videoFinished();
        break;
    case libvlc_MediaPlayerStopped:
        qDebug() << "libvlc_MediaPlayerStopped";
        videoFinished();
        break;
    }

    qDebug() << "VideoWindow::onVlcEvent >-<";
}

// VLC事件
void VideoWindow::vlcEvents(const libvlc_event_t *ev, void *param)
{
    qDebug() << "VideoWindow::vlcEvents" << ev;

    Q_UNUSED(param);
    auto window = static_cast<VideoWindow *>(param);
    window->onVlcEvent(ev);

    qDebug() << "VideoWindow::vlcEvents >-<";
}

// 释放
void VideoWindow::freeVlc()
{
    if (m_media)
    {
        libvlc_media_release(m_media);
        m_media = Q_NULLPTR;
    }
    if (m_mediaPlayer)
    {
        libvlc_media_player_stop(m_mediaPlayer);
        libvlc_media_player_release(m_mediaPlayer);
        m_mediaPlayer = Q_NULLPTR;
    }
    if (m_vlcInstance)
    {
        libvlc_release(m_vlcInstance);
        m_vlcInstance = Q_NULLPTR;
    }
}
