#include "mdi-child.h"
#include "ui_mdichild.h"
#include <iostream>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "license-session.h"
#include "spdlogger.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>




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
