#include "mdi-child.h"
#include "ui_mdichild.h"
#include <iostream>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "license-session.h"
#include "spdlogger.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <boost/lexical_cast.hpp>
#include <algorithm>


void MdiChild::updateSensor(SensorData* s) {
    bool isExist = false;
    for(auto &i : sensorList) {
        if( i.sensor_mac_address == s->sensor_mac_address ) {
            isExist = true;

            if( s->has_sensor_type )       { i.has_sensor_type = true; i.sensor_type = s->sensor_type; }
            if( s->has_shtc1_temperature ) { i.has_temperature = true; i.temperature = s->shtc1_temperature; }
            if( s->has_shtc1_humidity )    { i.has_humidity = true; i.humidity = s->shtc1_humidity; }
            if( s->has_sgp30_eco2_ppm )    { i.has_eco2_ppm = true; i.eco2_ppm = s->sgp30_eco2_ppm; }
            if( s->has_sgp30_tvoc_ppb )    { i.has_tvoc_ppb = true; i.tvoc_ppb = s->sgp30_tvoc_ppb; }
            if( s->has_hall_state )        { i.has_hall_state = true; i.hall_state = s->hall_state; }
            if( s->has_hall_interrupt )    { i.has_hall_interrupt = true; i.hall_interrupt = s->hall_interrupt; }
            if( s->has_pir_interrupt )     { i.has_pir_interrupt = true; i.pir_interrupt = s->pir_interrupt; }
            if( s->has_bq27441_voltage )   { i.has_voltage = true; i.voltage = s->bq27441_voltage; }
            break;
        }
    }

    if( isExist == false ) {
        SensorPrint i;

        i.sensor_mac_address = s->sensor_mac_address;
        if( s->has_sensor_type )       { i.has_sensor_type = true; i.sensor_type = s->sensor_type; }
        if( s->has_shtc1_temperature ) { i.has_temperature = true; i.temperature = s->shtc1_temperature; }
        if( s->has_shtc1_humidity )    { i.has_humidity = true; i.humidity = s->shtc1_humidity; }
        if( s->has_sgp30_eco2_ppm )    { i.has_eco2_ppm = true; i.eco2_ppm = s->sgp30_eco2_ppm; }
        if( s->has_sgp30_tvoc_ppb )    { i.has_tvoc_ppb = true; i.tvoc_ppb = s->sgp30_tvoc_ppb; }
        if( s->has_hall_state )        { i.has_hall_state = true; i.hall_state = s->hall_state; }
        if( s->has_hall_interrupt )    { i.has_hall_interrupt = true; i.hall_interrupt = s->hall_interrupt; }
        if( s->has_pir_interrupt )     { i.has_pir_interrupt = true; i.pir_interrupt = s->pir_interrupt; }
        if( s->has_bq27441_voltage )   { i.has_voltage = true; i.voltage = s->bq27441_voltage; }
        sensorList.push_back( i );
    }

    // set new text with sensor
    std::string newText = "";
    sort(sensorList.begin(), sensorList.end());
    for(auto &i : sensorList) {
        newText += i.sensor_mac_address;
        if( i.has_sensor_type )       { newText += " type : "           + boost::lexical_cast<std::string>( i.sensor_type ); }
        if( i.has_temperature )       { newText += " temperature : "    + boost::lexical_cast<std::string>( i.temperature ); }
        if( i.has_humidity )          { newText += " humidity : "       + boost::lexical_cast<std::string>( i.humidity ); }
        if( i.has_eco2_ppm )          { newText += " eco2 : "           + boost::lexical_cast<std::string>( i.eco2_ppm ); }
        if( i.has_tvoc_ppb )          { newText += " tvoc : "           + boost::lexical_cast<std::string>( i.tvoc_ppb ); }
        if( i.has_hall_state )        { newText += " hall_state : "     + boost::lexical_cast<std::string>( i.hall_state ); }
        if( i.has_hall_interrupt )    { newText += " hall_interrupt : " + boost::lexical_cast<std::string>( i.hall_interrupt ); }
        if( i.has_pir_interrupt )     { newText += " pir_interrupt : "  + boost::lexical_cast<std::string>( i.pir_interrupt ); }
        if( i.has_voltage )           { newText += " voltage : "        + boost::lexical_cast<std::string>( i.voltage ); }

        newText += "\n";
    }

    ui->textBrowser->setText(QString::fromStdString(newText));
}

MdiChild::MdiChild(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MdiChild)
{
    ui->setupUi(this);

    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        printf("no supported\n");
        return;
    }

    audio = new QAudioOutput(format, this);
    qb.open(QIODevice::ReadWrite);
    connect(audio,SIGNAL(stateChanged(QAudio::State)),SLOT(finishedPlaying(QAudio::State)));
    m_audioDevice = audio->start();
}
MdiChild::~MdiChild() {
}

void MdiChild::mySlotName(const QByteArray& message) {
    uchar *buff = (uchar*)message.data(); 
    // QPixmap pix;
    // pix.loadFromData(buff, message.size(), "JPG"); 
    // ui->label->setPixmap(pix);
    // ui->label->resize(pix.width(), pix.height()); 

    std::vector<char> vb(&buff[0], &buff[message.size()]);
    cv::Mat img = cv::imdecode(vb, 1);
    cvtColor(img, img, CV_BGR2RGB);
    cv::Mat imgGray;
    cv::cvtColor(img, imgGray, CV_RGB2GRAY );
	// cv::Mat img = cv::imread("Lena.jpg", CV_LOAD_IMAGE_COLOR);
    cv::Mat contours;
    cv::Canny(imgGray, // 그레이레벨 영상
              contours, // 결과 외곽선
              125,  // 낮은 경계값
              350);  // 높은 경계값

    // 넌제로 화소로 외곽선을 표현하므로 흑백 값을 반전
    cv::Mat contoursInv; // 반전 영상
    cv::threshold(contours, contoursInv, 128, 255, cv::THRESH_BINARY_INV);
    // 밝기 값이 128보다 작으면 255가 되도록 설정
    cvtColor(contoursInv, contoursInv, CV_GRAY2RGB);
    // ui->label->setPixmap(QPixmap::fromImage(QImage(contoursInv.data, contoursInv.cols, contoursInv.rows, contoursInv.step, QImage::Format_RGB888)));
    ui->label->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
}

//void MdiChild::showBandwidth(int download, int upload) {
//}

void MdiChild::mySlotName2(const QByteArray& message) {
}
void MdiChild::finishedPlaying(QAudio::State state) {
}

void MdiChild::onDisconnection() {
    delete this;
}
void MdiChild::showRecvBytes(const QString& message) {
    //ui->recvBandwidth->setText(message);
}

void MdiChild::showStatus(const QString& message) {
}

void MdiChild::on_retryButton_clicked()
{
}

void MdiChild::on_retryButton_clicked(bool checked)
{

}

void MdiChild::on_retryButton_pressed()
{

}

void MdiChild::on_savePositionButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_do_charge = 1;
    tohi.do_charge = 11;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_gotoPositionButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_do_charge = 1;
    tohi.do_charge = 12;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_initPositionButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_do_charge = 1;
    tohi.do_charge = 13;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_pwmChangeButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    PwmLed pwmLed = PWM_LED__INIT;
    tohi.pwm_led = &pwmLed;
    pwmLed.has_pwm = 1;
    pwmLed.pwm = rand() % 2;
    pwmLed.has_duty = 1;
    pwmLed.duty = rand() % 600;
    pwmLed.has_period = 1; // rand() % 1000;
    pwmLed.period = rand() % 1000;
    pwmLed.has_enable = 1;
    pwmLed.enable = 1;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_vaToggleButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_do_charge = 1;
    tohi.do_charge = 2;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_moveUpButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    ToSt tost = TO_ST__INIT;

    RoughMotorControl rmc = ROUGH_MOTOR_CONTROL__INIT;
    rmc.has_left_motor = 1;
    rmc.has_right_motor = 1;

    rmc.left_motor = 60;
    rmc.right_motor = 60;
    tost.rough_motor_control = &rmc;

    tohi.tost_bypass = &tost;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_moveLeftButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    ToSt tost = TO_ST__INIT;

    RoughMotorControl rmc = ROUGH_MOTOR_CONTROL__INIT;
    rmc.has_left_motor = 1;
    rmc.has_right_motor = 1;

    rmc.left_motor = 60;
    rmc.right_motor = -60;
    tost.rough_motor_control = &rmc;

    tohi.tost_bypass = &tost;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_moveOriginButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    ToSt tost = TO_ST__INIT;

    RoughMotorControl rmc = ROUGH_MOTOR_CONTROL__INIT;
    rmc.has_left_motor = 1;
    rmc.has_right_motor = 1;

    rmc.left_motor = 0;
    rmc.right_motor = 0;
    tost.rough_motor_control = &rmc;

    tohi.tost_bypass = &tost;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_moveRightButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    ToSt tost = TO_ST__INIT;

    RoughMotorControl rmc = ROUGH_MOTOR_CONTROL__INIT;
    rmc.has_left_motor = 1;
    rmc.has_right_motor = 1;

    rmc.left_motor = -60;
    rmc.right_motor = 60;
    tost.rough_motor_control = &rmc;

    tohi.tost_bypass = &tost;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_moveDownButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    ToSt tost = TO_ST__INIT;

    RoughMotorControl rmc = ROUGH_MOTOR_CONTROL__INIT;
    rmc.has_left_motor = 1;
    rmc.has_right_motor = 1;

    rmc.left_motor = -60;
    rmc.right_motor = -60;
    tost.rough_motor_control = &rmc;

    tohi.tost_bypass = &tost;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_moveNeckUp_clicked()
{
    ToHi tohi = TO_HI__INIT;
    ToSt tost = TO_ST__INIT;

    tost.has_neck_speed = 1;
    tost.neck_speed = 500;

    tohi.tost_bypass = &tost;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_moveNeckDown_clicked()
{
    ToHi tohi = TO_HI__INIT;
    ToSt tost = TO_ST__INIT;

    tost.has_neck_speed = 1;
    tost.neck_speed = -500;

    tohi.tost_bypass = &tost;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_moveNeckOriginButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    ToSt tost = TO_ST__INIT;

    tost.has_neck_speed = 1;
    tost.neck_speed = 0;

    tohi.tost_bypass = &tost;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}

void MdiChild::on_macText_textChanged()
{
    sensor_mac_recv = ui->macText->toPlainText().toStdString();
}

void MdiChild::on_periodText_textChanged()
{
    try {
        sensor_period_recv = (uint32_t)(ui->periodText->toPlainText().toInt());
    } catch( std::exception e ) {
        std::cerr << e.what() << std::endl;
        sensor_period_recv = 5000;
    }
}

void MdiChild::on_setSensorButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    SensorSet sensor_set = SENSOR_SET__INIT;
    sensor_set.sensor_mac_address = (char*)sensor_mac_recv.c_str();

    sensor_set.has_period_alarm = 1;
    sensor_set.period_alarm = 1;

    sensor_set.has_notify_period = 1;
    sensor_set.notify_period = sensor_period_recv;
    CONSOLE_INFO("MAC {} period {} notify {}",
                 sensor_set.sensor_mac_address,
                 sensor_set.period_alarm,
                 sensor_set.notify_period);

    tohi.sensor_set_bypass = &sensor_set;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
}
