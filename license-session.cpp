
#include "license-session.h"
#include "packet-buffer.h"
#include "json.hpp"
#include "http-request.h"
#include "ks19937.h"
#include "tcp-packet.h"
#include "chrono-util.h"
using json = nlohmann::json;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include "globalvar.h"

using json = nlohmann::json;

#define PRINT_IF_HAS(OBJ, MEMBER) if(OBJ->has_##MEMBER) { printf(#MEMBER ": %d", (*OBJ).MEMBER); }
#define STRING_IF_HAS(OBJ, MEMBER) (OBJ->has_##MEMBER ? std::string("") + #MEMBER + ": " + std::to_string((*OBJ).MEMBER) : std::string(""))

boost::posix_time::milliseconds interval(0);  // 1 second
void LicenseSession::sendStream() {
    if(m_connected == false) {
        return;
    }
    int sizeToSend = m_sendBuffer.size();
    if(sizeToSend > 0) {
        CONSOLE_INFO("SEND something");
        std::vector<char> vbp(sizeToSend); // bp, bp + totalSize);
        ssize_t redSize;
        redSize = m_sendBuffer.read(&vbp[0], sizeToSend);
        auto buffer = shared_const_buffer(vbp);
        m_inAsyncWrite = true;
        boost::asio::async_write(m_socket, buffer,
                                 [_this = shared_from_this(), this](boost::system::error_code ec, std::size_t length)
                                 {
                                     m_inAsyncWrite = false;

                                     if(ec) {
                                         return;
                                     }
                                     if(m_sendBuffer.size()) {
                                         sendStream();
                                     }
                                         // m_ioService.post(
                                         //     [_this = shared_from_this(), this](){
                                         //     }
                                         //     ); 
                                 });
    }
}
void LicenseSession::packHiToBuffer(const ToHi* d) {
    CONSOLE_INFO("packHiToBuffer");
    int serializedSize = to_hi__get_packed_size(d);
    int requiredSize = sizeof(ModernProtocolWithSTM) + serializedSize;
    ModernProtocolWithSTM* pkt = (ModernProtocolWithSTM*)malloc(requiredSize);
    pkt->sig1 = 0xCA;
    pkt->sig2 = 0xB7;
    pkt->crc = 0xBB;
    pkt->length = serializedSize;
    to_hi__pack(d, pkt->data); 
    m_sendBuffer.writeBytes(pkt, sizeof(ModernProtocolWithSTM));
    m_sendBuffer.writeBytes(pkt->data, pkt->length);
    free(pkt);

    if(m_inAsyncWrite == false) {
        sendStream();
    }
}

int LicenseSession::processBytes() {
    while(m_dataBuffer.size() >= m_requiredSize) {
        switch(m_modernStep) {
            case STEP_CA: 
                {
                    uint8_t one;
                    m_dataBuffer.read(&one, sizeof(one));
                    if(one == 0xCA) {
                        m_modernStep = STEP_B7;
                    }
                    else {
                        printf("wrong packet %x\n", one);
                        m_modernStep = STEP_CA;
                    }
                }
                break;
            case STEP_B7:
                {
                    // uart_printf("[%02x] :: %d\r\n", m_modernStep, __LINE__);
                    uint8_t one;
                    m_dataBuffer.read(&one, sizeof(one));
                    if(one == 0xB7) {
                        m_modernStep = STEP_CRC;
                    }
                    else {
                        printf("wrong packet\n");
                        m_modernStep = STEP_CA;
                    }
                }
                break;
            case STEP_CRC:
                {
                    // uart_printf("[%02x] :: %d\r\n", m_modernStep, __LINE__);
                    uint8_t one;
                    m_dataBuffer.read(&one, sizeof(one));
                    uint8_t crc = one;
                    m_modernStep = STEP_LENGTH;
                    m_requiredSize = 4; 
                }
                break;
            case STEP_LENGTH:
                {
                    uint32_t one;
                    m_dataBuffer.read(&one, sizeof(one));
                    m_requiredSize = one; 
                    m_modernStep = STEP_PROTOBUF;
                    // CONSOLE_INFO("[needed: {}]", m_requiredSize);
                    // uart_printf("[needed: %02d] :: %d\r\n", requiredBytes, __LINE__);
                }
                break;
            case STEP_PROTOBUF:
                {
                    uint8_t* buf = new uint8_t[m_requiredSize];
                    m_dataBuffer.read(buf, m_requiredSize);
                    ToHost* msg2 = to_host__unpack(NULL, m_requiredSize, buf);  
                    processMessage(msg2);
                    delete [] buf;
                    to_host__free_unpacked(msg2, NULL);
                    m_requiredSize = 1;
                    m_modernStep = STEP_CA;
                }
                break;
        }
    } 
    return 0;
}
void LicenseSession::processMessage(ToHost* msg2) {
    if(msg2->has_play_pcm_end) {
        if(msg2->play_pcm_end == 1) {
            m_mdiChild->pcmPlayRecordStop();
        }
    }
    if(msg2->has_audio) {
        //CONSOLE_INFO("has_audio {}", msg2->audio.len);
        QByteArray qa = QByteArray((const char*)msg2->audio.data, (size_t)msg2->audio.len);

        QMetaObject::invokeMethod(m_mdiChild,
                                  "mySlotName2",
                                  Qt::AutoConnection, // Can also use any other except DirectConnection
                                  Q_ARG(QByteArray, qa)); // And some more args if needed






    }
    if(msg2->has_video) {
        QByteArray qa = QByteArray((const char*)msg2->video.data, (size_t)msg2->video.len);
        //const char* myMessage = m_protocol.mutable_video()->c_str(); // QString("TCP event received.");
        //viewMemory((void*)myMessage, 10);
        QMetaObject::invokeMethod(m_mdiChild,
                                  "mySlotName",
                                  Qt::AutoConnection, // Can also use any other except DirectConnection
                                  Q_ARG(QByteArray, qa)); // And some more args if needed
        if(Chrono::tickCount() - m_lastRecvTime > 1000) {
            m_lastRecvTime = Chrono::tickCount();
            m_recvBytes = 0;
        }
        // ToHi tohi = TO_HI__INIT;
        // tohi.has_do_charge = 1;
        // tohi.do_charge = 150;
        // packHiToBuffer(&tohi);
    }

    if(msg2->has_request_start_qc) {
        printf("start_qc!!msg\n"); 
    }
    if(msg2->has_request_bandwidth) {
        printf("start bw-test!!msg\n"); 
        ToHi tohi = TO_HI__INIT; 
        BandwidthTest bt = BANDWIDTH_TEST__INIT;
        std::vector<uint8_t> g(1024*1024*2);
        bt.data.data = &g[0];
        bt.data.len = g.size();
        bt.has_data = 1; 
        bt.has_step = 1;
        bt.step = 1;
        tohi.bandwidth_test = &bt; 
        packHiToBuffer(&tohi);
        bt.step = 3;
        packHiToBuffer(&tohi);
    }
    if(msg2->serial_bandwidth_test) {
        m_mdiChild->ui->chkboxSerialTest->setChecked(1);
        m_mdiChild->ui->serialDownloadBps->setText(QString::fromStdString(std::to_string(msg2->serial_bandwidth_test->bandwidth_by_hi/1024.f) + " kb/s .........."));
    }
    if(msg2->bandwidth_test) {
        QByteArray qa;
        if(msg2->bandwidth_test->step == 1) {
            m_lastRecvTime = Chrono::tickCount();
            CONSOLE_INFO("bandwidth step == 1");
            m_recvBytes = 0;
            qa.push_back((char)1);
            qa.push_back((char)0);
            QMetaObject::invokeMethod(m_mdiChild,
                                      "applyResults",
                                      Qt::AutoConnection, // Can also use any other except DirectConnection
                                      Q_ARG(QByteArray, qa)); // And some more args if needed
        }
        else if(msg2->bandwidth_test->step == 2) {
        }
        else if(msg2->bandwidth_test->step == 3) {
            CONSOLE_INFO("bandwidth step == 3");
            qa.push_back((char)1);
            qa.push_back((char)1);

            CONSOLE_INFO("recv {}", m_recvBps);
            CONSOLE_INFO("bandwidth by hi: {}, recv Bw(kbps): {}", msg2->bandwidth_test->bandwidth_by_hi, m_recvBps / 1024.f);
            m_lastRecvTime = Chrono::tickCount();
            m_mdiChild->ui->downloadBandwidth->setText(QString::fromStdString(std::to_string(msg2->bandwidth_test->bandwidth_by_hi/1024.f) + " kb/s"));
            m_mdiChild->ui->uploadBandwidth->setText(QString::fromStdString(std::to_string(m_recvBps/1024.f) + " kb/s"));
            m_recvBytes = 0;

            QMetaObject::invokeMethod(m_mdiChild,
                                      "applyResults",
                                      Qt::AutoConnection, // Can also use any other except DirectConnection
                                      Q_ARG(QByteArray, qa)); // And some more args if needed
        }
        else {
        }
    }
    if(msg2->tohi_bypass) {
        CONSOLE_INFO("bypass by hi");
        auto tohi = msg2->tohi_bypass;
        std::string logStr = "";
        logStr += STRING_IF_HAS(tohi, tof);
        logStr += std::string(", ") + STRING_IF_HAS(tohi, battery);
        logStr += std::string(", ") + STRING_IF_HAS(tohi, power_button_pressed);
        logStr += std::string(", ") + STRING_IF_HAS(tohi, usb_con);
        QString qa = QString::fromStdString(logStr);
        QMetaObject::invokeMethod(m_mdiChild,
                                  "showStatus",
                                  Qt::AutoConnection, // Can also use any other except DirectConnection
                                  Q_ARG(QString, qa)); // And some more args if needed
    }


}

void LicenseSession::readBytes(const boost::system::error_code& error,
               size_t bytes_transferred) {
    //printf("%s ... %d\n", __FUNCTION__, bytes_transferred);
    if(error) {
        printf("read error! %x\n", this);
        m_connected = false;
        QMetaObject::invokeMethod(m_mdiChild,
                                  "onDisconnection",
                                  Qt::AutoConnection);
        return;
    }
    //MyQObject *myQObject = m_object;

    // TODO: 마지막으로 받은지 1초가 넘었을 때, mdiChild 에 signal 보내기.

    m_recvBytes += bytes_transferred;
    float bps = m_recvBytes / ((Chrono::tickCount() - m_lastRecvTime) / 1000.f);
    // CONSOLE_INFO("{} kbps", bps / 1024.f);
    m_recvBps = bps;
    // m_recvBytes = 0; 
    QString qa = QString::fromStdString(std::to_string(bps / 1024.f) + " kbps");
    QMetaObject::invokeMethod(m_mdiChild,
                              "showRecvBytes",
                              Qt::AutoConnection, // Can also use any other except DirectConnection
                              Q_ARG(QString, qa)); // And some more args if needed
    // if(Chrono::tickCount() - m_lastRecvTime > 1000) {
    // }
    m_dataBuffer.writeBytes(data_, bytes_transferred, "databuffer!!!!!!!!!!!!??"); 
    processBytes(); 
    size_t sizeToRead = sizeof(data_);

    m_socket.async_read_some(boost::asio::buffer(data_, sizeToRead),
                            boost::bind(&LicenseSession::readBytes, shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}
