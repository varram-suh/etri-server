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
#include <queue>
#include <vector>
#include "simple.pb-c.h"

class SensorPrint {
    public :
        std::string sensor_mac_address;
        uint32_t sensor_type;
        float temperature;
        float humidity;
        uint32_t eco2_ppm;
        uint32_t tvoc_ppb;
        bool hall_state;
        bool hall_interrupt;
        bool pir_interrupt;
        uint32_t voltage;

        bool has_sensor_type = false;
        bool has_temperature = false;
        bool has_humidity = false;
        bool has_eco2_ppm = false;
        bool has_tvoc_ppb = false;
        bool has_hall_state = false;
        bool has_hall_interrupt = false;
        bool has_pir_interrupt = false;
        bool has_voltage = false;

        SensorPrint() {
            sensor_mac_address = "";
        }
        friend bool operator < ( const SensorPrint &s1, const SensorPrint &s2 ) {
            return s1.sensor_type < s2.sensor_type;
        }
};

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

    void on_moveNeckOriginButton_clicked();

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
    std::vector<SensorPrint> sensorList;
    void updateSensor(SensorData* s);

    // Set up the format, eg.
};

#endif // MAINWINDOW_H
