#pragma once
#include <termio.h>
#define __STDC_FORMAT_MACROS
#define __STDC_CONTANT_MACROS
#include <inttypes.h>
#include <queue>
#include <functional>
#include <cstddef>
#include <memory.h>
#include <cstdlib>

#include <stdio.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h> //  our new library
#include "tcp-packet.h"
#include "packet-buffer.h"
#include <vector>
#include "circular-buffer.h"
#include <memory>
#include "spdlogger.h"


#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include "json.hpp"
using json = nlohmann::json;

namespace asio = boost::asio;
class BLESession  : public std::enable_shared_from_this<BLESession> {
public:
    int m_bleFD = -1;
    char read_msg_[512];
    std::vector<uint8_t> m_remainBuffer;
    CircularBuffer m_sendBuffer;
    uint8_t m_sigMata[10][0xff + 1];
    int m_sigStep = 0;
    uint8_t m_innerSigMata[200][0xff + 1];
    int m_innerSigStep = 0;
    CircularBuffer m_innerBuffer;
    asio::io_service& m_io;
    asio::serial_port m_port;
    std::shared_ptr<PKT> m_pktBody;
    boost::beast::websocket::stream<asio::ip::tcp::socket> m_roomSocket;
    boost::beast::websocket::stream<asio::ip::tcp::socket> m_signalSocket;
    asio::ip::tcp::resolver m_resolver; // {ios};
    asio::deadline_timer m_recoveryTimer;

    int m_remains = 0;
    BLESession(asio::io_service& ios)
        : m_io(ios),
        m_port(ios, "/dev/ttyAMA2"),
        m_sendBuffer(1024),
        m_innerBuffer(256),
        m_bleSendTimer(ios, boost::posix_time::milliseconds(10)),
        m_roomSocket(ios),
        m_signalSocket(ios),
        m_resolver(ios),
        m_recoveryTimer(ios, boost::posix_time::seconds(3)) {
            memset(m_sigMata, 0, sizeof(m_sigMata));
            //for(int i=0; i<256; i++) {
            //for(int j=0; j<10; j++) {
            //m_sigMata[j][i] = 0;
            //}
            //}
            m_sigMata[0][0xab] = 1;
            m_sigMata[1][0xcd] = 2;
            for(int j=2; j<=4; j++) {
                for(int i=0; i<256; i++) {
                    m_sigMata[j][i] = j + 1;
                }
            }
            for(int i=0; i<256; i++) {
                m_sigMata[5][i] = 0;
            }

            memset(m_innerSigMata, 0, sizeof(m_innerSigMata));
            m_innerSigMata[0][0x8a] = 1;
            m_innerSigMata[1][0x8a] = 2;
            m_innerSigMata[2][0x8a] = 3;
            m_innerSigMata[3][0x8a] = 4;

            // option settings...
            m_port.set_option(asio::serial_port_base::baud_rate(921600));
            m_port.set_option(asio::serial_port_base::character_size(8));
            m_port.set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
            m_port.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
            m_port.set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));

            CONSOLE_INFO("open BLE success");
        }
    virtual ~BLESession() {
        CONSOLE_CRITICAL("~BLESession");
    }

    asio::deadline_timer m_bleSendTimer;
    void getOuterPacket(const boost::system::error_code& error, size_t bytes_transferred);
    void innerProtocol(const std::vector<uint8_t>& innerData);
    int sender();
    int start();
    int scanAP();
    int connectAP();
    int connectAuthServer();

    int pushCommand(int msg, const json jsonObj) {
        std::vector<char> pb = makeVectorPKT(
            msg, jsonObj.dump());
        std::vector<uint8_t> outer = makeVectorBLEPKT(pb);
        m_sendBuffer.writeBytes(&outer[0], outer.size());
        return 0;
    }

};
