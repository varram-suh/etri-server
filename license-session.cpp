
#include "license-session.h"
#include "packet-buffer.h"
#include "http-request.h"
#include "chrono-util.h"


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
    int requiredSize = sizeof(EtriProtocol) + serializedSize;
    EtriProtocol* pkt = (EtriProtocol*)malloc(requiredSize);
    pkt->sig1 = 0xAB;
    pkt->sig2 = 0xCD;
    pkt->length = serializedSize;
    to_hi__pack(d, pkt->data); 
    m_sendBuffer.writeBytes(pkt, sizeof(EtriProtocol));
    m_sendBuffer.writeBytes(pkt->data, pkt->length);
    free(pkt);

    if(m_inAsyncWrite == false) {
        sendStream();
    }
}

int LicenseSession::processBytes() {
    while(m_dataBuffer.size() >= m_requiredSize) {
        switch(m_modernStep) {
            case STEP_CB: 
                {
                    uint8_t one;
                    m_dataBuffer.read(&one, sizeof(one));
                    if(one == 0xAB) {
                        m_modernStep = STEP_B7;
                    }
                    else if( one == 0xEA ) {
                        m_modernStep = STEP_CE;
                    }
                    else {
                        printf("wrong packet %02x\n", one);
                        m_modernStep = STEP_CB;
                    }
                }
                break;
            case STEP_B7:
                {
                    // uart_printf("[%02x] :: %d\r\n", m_modernStep, __LINE__);
                    uint8_t one;
                    m_dataBuffer.read(&one, sizeof(one));
                    if(one == 0xCD) {
                        m_modernStep = STEP_LENGTH;
                    }
                    else {
                        printf("wrong packet\n");
                        m_modernStep = STEP_CB;
                    }
                }
                break;
            case STEP_CE:
                {
                    // uart_printf("[%02x] :: %d\r\n", m_modernStep, __LINE__);
                    uint8_t one;
                    m_dataBuffer.read(&one, sizeof(one));
                    if(one == 0xCE) {
                        m_modernStep = STEP_ELSEN_LENGTH;
                        m_requiredSize = 4;
                    }
                    else {
                        printf("wrong packet\n");
                        m_modernStep = STEP_CB;
                    }
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
            case STEP_ELSEN_LENGTH:
                {
                    uint32_t one;
                    m_dataBuffer.read(&one, sizeof(one));
                    m_requiredSize = one; 
                    m_modernStep = STEP_ELSEN_PACKET;
                    printf("LEN : %u\n", one);
                }
                break;
            case STEP_ELSEN_PACKET:
                {
                    uint8_t* buf = new uint8_t[m_requiredSize];
                    m_dataBuffer.read(buf, m_requiredSize);
                    CONSOLE_INFO("ELSEN PACKET");
                    printf("0xEA 0xCE ");
                    printf("0x%02X ", (m_requiredSize << 0 ) & 0xFF);
                    printf("0x%02X ", (m_requiredSize << 8 ) & 0xFF);
                    printf("0x%02X ", (m_requiredSize << 16) & 0xFF);
                    printf("0x%02X ", (m_requiredSize << 24) & 0xFF);

                    for(size_t i=0; i<m_requiredSize; i++) {
                        printf("0x%02X ", buf[i]);
                    }
                    printf("\n");
                    delete [] buf;
                    m_requiredSize = 1;
                    m_modernStep = STEP_CB;
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
                    m_modernStep = STEP_CB;
                }
                break;
        }
    } 
    return 0;
}

void LicenseSession::processMessage(ToHost* msg2) {
    if(msg2->has_audio) {
        //CONSOLE_INFO("has_audio {}", msg2->audio.len);
        QByteArray qa = QByteArray((const char*)msg2->audio.data, (size_t)msg2->audio.len);

        QMetaObject::invokeMethod(m_mdiChild,
                                  "mySlotName2",
                                  Qt::AutoConnection,
                                  Q_ARG(QByteArray, qa));
    }

    if(msg2->has_video) {
        QByteArray qa = QByteArray((const char*)msg2->video.data, (size_t)msg2->video.len);
        QMetaObject::invokeMethod(m_mdiChild,
                                  "mySlotName",
                                  Qt::AutoConnection,
                                  Q_ARG(QByteArray, qa));
        if(Chrono::tickCount() - m_lastRecvTime > 1000) {
            m_lastRecvTime = Chrono::tickCount();
            m_recvBytes = 0;
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
