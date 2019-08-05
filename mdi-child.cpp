#include "mdi-child.h"
#include "ui_mdichild.h"
#include "json.hpp"
#include <iostream>
using nlohmann::json;
#include <QMdiArea>
#include <QMdiSubWindow>
#include "debugger.h"
#include "license-session.h"
#include "spdlogger.h"
#include <opencv2/opencv.hpp>

#include <opencv2/imgproc/types_c.h>




MdiChild::MdiChild(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MdiChild)
{
    ui->setupUi(this);

    { // set graph 1 : realtime
        realtimeX.resize(640);
        realtimeY.resize(640);
        for(int i=0; i<640; i++) realtimeX[i] = i;
        ui->soundPlotChart->addGraph();
        ui->soundPlotChart->graph(0)->setData(realtimeX, realtimeY);
        ui->soundPlotChart->xAxis->setLabel("real_time pcm data(0 ~ 255)");
        ui->soundPlotChart->yAxis->setLabel("soundLevel");
        ui->soundPlotChart->xAxis->setRange(0, 640);
        ui->soundPlotChart->yAxis->setRange(0, 255);
        ui->soundPlotChart->replot();
    } // end of set graph 1

    { // set graph 2 : realtime
        ui->soundPlotChart2->addGraph();
        ui->soundPlotChart2->graph(0)->setData(playlistX, playlistY);
        ui->soundPlotChart2->xAxis->setLabel("Frequency (Hz))");
        ui->soundPlotChart2->yAxis->setLabel("soundLevel");
        ui->soundPlotChart2->xAxis->setRange(0, 1000);
        ui->soundPlotChart2->yAxis->setRange(0, 255);
        ui->soundPlotChart2->replot();
    } // end of set graph 2

    {
        ui->comboBoxPlayList->addItem("Do Re Mi Sound");
        ui->comboBoxPlayList->addItem("500 to 20khz");
        ui->comboBoxPlayList->addItem("NO(music)");
        ui->comboBoxPlayList->addItem("1 5 8 12 16");
        ui->comboBoxPlayList->addItem("Nessun Dorrma 48k");
        ui->comboBoxPlayList->addItem("NO(TEST)");
    }

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
        // qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        return;
    }

    audio = new QAudioOutput(format, this);
    qb.open(QIODevice::ReadWrite);
    connect(audio,SIGNAL(stateChanged(QAudio::State)),SLOT(finishedPlaying(QAudio::State)));
    m_audioDevice = audio->start();
    //QString img_path = "/home/ksoo/Downloads/1553473895132.jpg";
    //QImage img(img_path);    
    //QPixmap buf = QPixmap::fromImage(img); 
}
MdiChild::~MdiChild() {
    if( pcmVolumeOutput )
    {
        fclose(pcmVolumeOutput);
        pcmVolumeOutput = nullptr;
    }
}

void MdiChild::pcmPlayRecordStop()
{
    isPlayPcmNow = false;

    if( pcmVolumeOutput )
    {
        CONSOLE_INFO("close pcmRecord.txt file");
        fclose(pcmVolumeOutput);
        pcmVolumeOutput = nullptr;
        ui->closeRecordButton->setText("PCM Textfile Closed. Named \"pcmRecord.txt\"");
    }
    pcm_record_count = 0;
    playlistX.clear();
    playlistY.clear();
}

void MdiChild::on_pushButton_clicked()
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_request_mic_loop_back = 1;


    if( mic_loopback_on ) {
        ui->playPcm->setEnabled(true);
        ui->stopPcm->setEnabled(true);
        mic_loopback_on = false;
        tohi.request_mic_loop_back = mic_loopback_on;
    }
    else {
        on_stopPcm_clicked();
        ui->playPcm->setEnabled(false);
        ui->stopPcm->setEnabled(false);
        mic_loopback_on = true;
        tohi.request_mic_loop_back = mic_loopback_on;
    }

    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
    printf("WOW2\n");
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


void MdiChild::applyResults(const QByteArray& message) {
    if(message.size() >= 1) {
        if(message[0]) {
            ui->chkboxDownloadTest->setChecked(1);
        }
        else {
            ui->chkboxDownloadTest->setChecked(0);
        }
    }
    if(message.size() >= 2) {
        if(message[1]) {
            ui->chkboxUploadTest->setChecked(1);
        }
        else {
            ui->chkboxUploadTest->setChecked(0);
        }
    }
    if(message.size() >= 3) {
        if(message[2]) {
            ui->chkboxSerialTest->setChecked(1);
        }
        else {
            ui->chkboxSerialTest->setChecked(0);
        }
    }

    ui->micSlider->setValue(20);
    // on_micSlider_valueChanged(20);
    ui->speakerSlider->setValue(0);
    // on_speakerSlider_valueChanged(0); 
    on_chkIrCut1_stateChanged(0);
    on_chkIrCut2_stateChanged(0);
}
void MdiChild::showBandwidth(int download, int upload) {
    CONSOLE_INFO("{} / {}-----------------------------", download, upload);
    ui->downloadBandwidth->setText(QString::fromStdString(std::to_string(download/1024.f) + " kb/s"));
    ui->uploadBandwidth->setText(QString::fromStdString(std::to_string(upload/1024.f) + " kb/s"));
}
void MdiChild::mySlotName2(const QByteArray& message) {
    char* buff = (char*)message.data();

    char minBuf = 127;
    char maxBuf = -128;
    int len = message.size();

    for(int i=0; i<len; i++) {
        realtimeY.pop_front();
        realtimeY.push_back(buff[i]+128);
        minBuf = minBuf < buff[i] ? minBuf : buff[i];
        maxBuf = maxBuf > buff[i] ? maxBuf : buff[i];
    }

    ui->soundPlotChart->graph(0)->setData(realtimeX, realtimeY);
    ui->soundPlotChart->replot();

    if( pcmVolumeOutput )
    {
        printf("%d %d\n", maxBuf-minBuf, pcm_record_count);
        playlistX.push_back(pcm_record_count++);
        playlistY.push_back(maxBuf-minBuf);
        ui->soundPlotChart2->graph(0)->setData(playlistX, playlistY);
        ui->soundPlotChart2->xAxis->setRange(0, pcm_record_count);
        ui->soundPlotChart2->replot();
        std::chrono::system_clock::time_point endTimePoint = std::chrono::system_clock::now();
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(endTimePoint - startTimePoint);
        fprintf(pcmVolumeOutput, "%d\t%d\n", ms.count(), maxBuf-minBuf);
    }

    // CONSOLE_INFO("mesize: {}", message.size());
    // m_audioDevice->write(buff, message.size());
    // ssize_t volume = 0;
    // for(int i=0; i<message.size(); i+=2) { 
    //     int16_t* b = (int16_t*)&buff[i];
    //     volume += *b;
    // }
    // volume /= (message.size() / 2);
    // volume *= 100;
    // volume += 30000;

    // volume = sqrtf(volume);
    // ui->progressBar->setValue(volume);

    // // qb = new QBuffer;
    // qb->seek(0);
    // qb->write(m_protocol.mutable_audio()->c_str(), m_protocol.mutable_audio()->size());
    // qb->seek(0);
    // ui->label->setPixmap(pix);
    // ui->label->resize(pix.width(), pix.height()); 
    // if(m_state == 0) {
    //     printf("slot2\n");
    //     audio->start(message);
    // }
    // else {
    //     printf("not slot2\n");
    //     buffers.push(message); 
    // }
}
void MdiChild::finishedPlaying(QAudio::State state) {
    // if(state == QAudio::ActiveState) {
    //     m_state = 1;
    // }
    // else {
    //     m_state = 0;
    //     if(buffers.empty() == false) {
    //         m_state = 1;
    //         QBuffer* qb = buffers.front();
    //         buffers.pop();
    //         qb->seek(0);
    //         printf("self start\n");
    //         audio->start(qb);
    //     }
    //     else {
    //         m_state = 0;
    //     }
    // }
    // if(state == QAudio::StoppedState) {
    // }
}


void MdiChild::on_micSlider_valueChanged(int value)
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_set_mic_vol = 1;
    tohi.set_mic_vol = value;
    auto p = m_session.lock();
    if(p) {
        CONSOLE_INFO("mic slider to pack");
        p->packHiToBuffer(&tohi);
    }

    std::string t = "";
    t += "M.I.C volume(" + std::to_string(value) + ")";
    ui->micVolume->setText(QString(t.c_str()));
    CONSOLE_INFO("mic value: {}", value);

}

void MdiChild::on_speakerSlider_valueChanged(int value)
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_set_speaker_vol = 1;
    tohi.set_speaker_vol = value;
    auto p = m_session.lock();
    if(p) {
        CONSOLE_INFO("spk slider to pack");
        p->packHiToBuffer(&tohi);
    }
    std::string t = "";
    t += "Speaker volume(" + std::to_string(value) + ")";
    ui->speakerVolume->setText(QString(t.c_str()));
    CONSOLE_INFO("spk value: {}", value); 
}

void MdiChild::on_chkHorizonalInvert_stateChanged(int arg1)
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_set_invert_horiz = 1;
    tohi.set_invert_horiz = arg1;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
    CONSOLE_INFO("horiz inv value: {}", arg1);

}

void MdiChild::on_chkVerticalInvert_stateChanged(int arg1)
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_set_invert_vert = 1;
    tohi.set_invert_vert = arg1;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
    CONSOLE_INFO("vert inv value: {}", arg1); 
} 

void MdiChild::on_chkIrCut1_stateChanged(int arg1)
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_set_ir_cut1 = 1;
    tohi.set_ir_cut1 = arg1;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
    CONSOLE_INFO("ir cut1 value: {}", arg1); 
}

void MdiChild::on_chkIrCut2_stateChanged(int arg1)
{
    ToHi tohi = TO_HI__INIT;
    tohi.has_set_ir_cut2 = 1;
    tohi.set_ir_cut2 = arg1;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
    CONSOLE_INFO("ir cut2 value: {}", arg1); 

}

void MdiChild::on_playPcm_clicked()
{
    playlistX.clear();
    playlistY.clear();
    ToHi tohi = TO_HI__INIT;
    tohi.has_play_pcm = 1;
    tohi.play_pcm = ui->comboBoxPlayList->currentIndex() + 1;

    pcm_record_count = 0;

    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
    CONSOLE_INFO("play pcm");

    on_startRecordButton_clicked();
}

void MdiChild::on_stopPcm_clicked()
{
    if( pcmVolumeOutput )
    {
        fclose(pcmVolumeOutput);
        pcmVolumeOutput = nullptr;
    }

    ToHi tohi = TO_HI__INIT;
    tohi.has_play_pcm = 1;
    tohi.play_pcm = 0;
    m_hz_division = 0;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
    CONSOLE_INFO("stop pcm");

}

void MdiChild::onDisconnection() {
    delete this;
}
void MdiChild::showRecvBytes(const QString& message) {
    ui->recvBandwidth->setText(message);
}
void MdiChild::showStatus(const QString& message) {
    ui->statusLabel->setText(message);
}

void MdiChild::on_retryButton_clicked()
{
    ui->chkboxDownloadTest->setChecked(0);
    ui->chkboxUploadTest->setChecked(0);
    ui->chkboxSerialTest->setChecked(0);


    ToHi tohi = TO_HI__INIT;
    tohi.has_qc_retry = 1;
    auto p = m_session.lock();
    if(p) {
        p->packHiToBuffer(&tohi);
    }
    CONSOLE_INFO("retry QC");

}

void MdiChild::on_retryButton_clicked(bool checked)
{

}

void MdiChild::on_retryButton_pressed()
{

}

void MdiChild::on_startRecordButton_clicked()
{
    isPlayPcmNow = true;
    if( !pcmVolumeOutput )
    {
        CONSOLE_INFO("Start PCM Record");
        pcmVolumeOutput = fopen("./pcmRecord.txt", "w");
        startTimePoint = std::chrono::system_clock::now();
        fprintf(pcmVolumeOutput, "ms\tsoundLevel\n");
        pcm_record_count = 0;
    }
    else
    {
        CONSOLE_INFO("PCM Recording... ignore button press");
    }

    ui->startRecordButton->setText("Recoding Now......");
    ui->closeRecordButton->setText("Close Record PCM Data");
}

void MdiChild::on_closeRecordButton_clicked()
{
    if( pcmVolumeOutput )
    {
        CONSOLE_INFO("close pcmRecord.txt file");
        fclose(pcmVolumeOutput);
        pcmVolumeOutput = nullptr;
        ui->closeRecordButton->setText("PCM Textfile Closed. Named \"pcmRecord.txt\"");
    }
    else
    {
        CONSOLE_INFO("PCM Record NOT Start yet. ignore button press");
        ui->closeRecordButton->setText("PCM Record NOT Start yet. ignore button press");
    }
    ui->startRecordButton->setText("Start Record PCM Data");
}
