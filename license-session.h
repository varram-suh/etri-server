

#pragma once
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <exception>
#include "tcp-packet.h"
//#include "direct-mode.h"
#include <map>
#include "debugger.h"
#include "json.hpp"
#include "spdlogger.h"
#include "circular-buffer.h"
#include "debugger.h"
#include <QString>
#include <QBuffer>
#include "mdi-child.h"

#include "simple.pb-c.h"
#include "protocol-struct.h"
#include "chrono-util.h"

using namespace boost::asio;
using namespace boost::asio::ip;
// using boost::asio::ip::tcp;
// using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
//namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

#include "mdi-child.h"

class LicenseSession : public boost::enable_shared_from_this<LicenseSession>
{
    public:

        tcp::socket m_socket;
        MdiChild* m_mdiChild = nullptr;
        boost::asio::io_service& m_ioService;
        typedef boost::shared_ptr<LicenseSession> pointer;
        enum E_STEP {
            STEP_CB = 0, STEP_B7, STEP_LENGTH, STEP_PROTOBUF,
            STEP_CE, STEP_ELSEN_LENGTH, STEP_ELSEN_PACKET
        } ;
        uint8_t data_[1024*1024];///< [수신]
        E_STEP m_modernStep = STEP_CB;///< [수신]
        uint32_t m_requiredSize = 1;///< [수신]
        CircularBuffer m_dataBuffer = CircularBuffer(205344); ///< [수신]
        CircularBuffer m_sendBuffer = CircularBuffer(1024); ///< [송신]
        bool m_connected = true;
        bool m_inAsyncWrite = false;
        boost::asio::deadline_timer m_sendTimer = boost::asio::deadline_timer(m_ioService, boost::posix_time::milliseconds(0)); 

        uint64_t m_lastRecvTime = 0;
        uint32_t m_recvBytes = 0;
        uint32_t m_recvBps = 0;
    public:
        static pointer create(boost::asio::io_service& io_service)
        {
            return pointer(new LicenseSession(io_service));
        }

        LicenseSession(boost::asio::io_service& io_service)
            : m_socket(io_service),
              m_ioService(io_service) {
            //start();
        }
        virtual ~LicenseSession() {
            CONSOLE_INFO("dest ~LicenseSession");
        }

        tcp::socket& socket()
        {
            return m_socket;
        }

        void start()
        {
            m_socket.async_read_some(boost::asio::buffer(data_, 1),
                                    boost::bind(&LicenseSession::readBytes, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
            sendStream();
        }
        //void LicenseSession::requestHTTP( char const* host, const std::string& port, char const* target, const std::string& body, std::function<void(std::string)> onRead);
    public:
        int processBytes() ;
        void processMessage(ToHost* msg2) ;

        void readBytes(const boost::system::error_code& error,
                         size_t bytes_transferred) ;
        void sendStream();
        void packHiToBuffer(const ToHi* d);
};
