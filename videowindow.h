#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>

struct libvlc_media_track_info_t;
struct libvlc_media_t;
struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;

class VideoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWindow(const QString &filepath, QWidget *parent = nullptr);
    ~VideoWindow();

    void play(); // 播放
    void stop(); // 停止

private:
    bool loadVlc(); // 加载
    void freeVlc(); // 释放

protected:
    static void vlcEvents(const libvlc_event_t *ev, void *param); // VLC事件
    void onVlcEvent(const libvlc_event_t *event);

signals:
    void videoFinished();

private:
    QLabel *m_label;
    QVBoxLayout *m_layout;

    QString m_filepath; // 视频文件路径（全路径）

    libvlc_instance_t *m_vlcInstance = nullptr;
    libvlc_media_t *m_media = nullptr;
    libvlc_media_player_t *m_mediaPlayer = nullptr;

    bool m_bPlaying = false;
};

#endif // VIDEOWINDOW_H
