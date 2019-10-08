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
    void finishedPlaying(QAudio::State state);
    void onDisconnection();
    void showRecvBytes(const QString& message);
    void showStatus(const QString& message);

    void on_retryButton_clicked();

    void on_retryButton_clicked(bool checked);

    void on_retryButton_pressed();

    void on_savePositionButton_clicked();

    void on_gotoPositionButton_clicked();

    void on_initPositionButton_clicked();

    void on_pwmChangeButton_clicked();

    void on_vaToggleButton_clicked();

    void on_moveUpButton_clicked();

    void on_moveLeftButton_clicked();

    void on_moveOriginButton_clicked();

    void on_moveRightButton_clicked();

    void on_moveDownButton_clicked();

    void on_moveNeckUp_clicked();

    void on_moveNeckDown_clicked();

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
    std::chrono::system_clock::time_point startTimePoint;

    int m_hz_division = 0;
    bool mic_loopback_on = false;

    // Set up the format, eg.
};

#endif // MAINWINDOW_H
