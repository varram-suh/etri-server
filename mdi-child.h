#ifndef MDIMAINWINDOW_H
#define MDIMAINWINDOW_H

#include <QMainWindow>
#include <QBuffer> 
#include <QAudioOutput>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <chrono>
#include "json.hpp"
#include <queue>

namespace Ui {
class MdiChild;
}
#include "ui_mdichild.h"

class LicenseSession;
class MdiChild : public QMainWindow
{
    Q_OBJECT

public:
    explicit MdiChild(QWidget *parent = nullptr) ;
    ~MdiChild() ;

    bool isConnected() {
        return false;
    }


private slots:
    void on_pushButton_clicked();
    void mySlotName(const QByteArray& message);
    void mySlotName2(const QByteArray& message);
    void applyResults(const QByteArray& message);
    void showBandwidth(int download, int upload);
    void finishedPlaying(QAudio::State state);
    void onDisconnection();
    void showRecvBytes(const QString& message);
    void showStatus(const QString& message);
    void on_micSlider_valueChanged(int value);

    void on_speakerSlider_valueChanged(int value);

    void on_chkHorizonalInvert_stateChanged(int arg1);

    void on_chkVerticalInvert_stateChanged(int arg1);

    void on_chkIrCut1_stateChanged(int arg1);

    void on_chkIrCut2_stateChanged(int arg1);

    void on_playPcm_clicked();

    void on_stopPcm_clicked();


    void on_retryButton_clicked();

    void on_retryButton_clicked(bool checked);

    void on_retryButton_pressed();

    void on_startRecordButton_clicked();

    void on_closeRecordButton_clicked();

public:
    Ui::MdiChild *ui;
    QAudioOutput* audio; 
    std::queue<QBuffer*> buffers;
    int m_state = 0;
    QBuffer qb;
    QIODevice *	m_audioDevice;
    boost::weak_ptr<LicenseSession> m_session;
    QVector<double> realtimeX, realtimeY;
    QVector<double> playlistX, playlistY;
    FILE *pcmVolumeOutput = nullptr;
    int pcm_record_count = 0;
    std::chrono::system_clock::time_point startTimePoint;

    bool isPlayPcmNow = false;

    void pcmPlayRecordStop();
    int m_hz_division = 0;
    bool mic_loopback_on = false;

    // Set up the format, eg.
};

#endif // MAINWINDOW_H
