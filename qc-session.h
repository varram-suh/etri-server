

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
//#include "wifi.h"
//#include "shell.h"
//#include "sockethelper.h"
//#include "json/json.h"
#include "debugger.h"
#include "gui.h"


using boost::asio::ip::tcp;
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
//namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

class QcSession : public boost::enable_shared_from_this<QcSession>
{
    public:
        typedef boost::shared_ptr<QcSession> pointer;

        static pointer create(boost::asio::io_service& io_service)
        {
            return pointer(new QcSession(io_service));
        }

        QcSession(boost::asio::io_service& io_service)
            : socket_(io_service), m_ioService(io_service)
        {
            m_sigStep = 0;
            memset(m_mataSignature, 0, sizeof(m_mataSignature));
            m_mataSignature[0][0x8a] = 1;
            m_mataSignature[1][0x8a] = 2;
            m_mataSignature[2][0x8a] = 3;
            m_mataSignature[3][0x8a] = 4;

        }

        tcp::socket& socket()
        {
            return socket_;
        }

        void start()
        {
            printf("try readSig\n");
            //socket_.async_read_some(boost::asio::buffer(data_, 1),
                                    //boost::bind(&session::readData, this,
                                                //boost::asio::placeholders::error,
                                                //boost::asio::placeholders::bytes_transferred));

            m_sigStep = 0;
            socket_.async_read_some(boost::asio::buffer(data_, 1),
                                    boost::bind(&QcSession::readSignature, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }

    private:
        void readSignature(const boost::system::error_code& error,
                         size_t bytes_transferred) {
            if(error) {
                printf("delete %x\n", this);
                return;
            }
            printf("sig %x, %d\n", (char)(data_[0] & 0xff), bytes_transferred);
            m_sigStep = m_mataSignature[m_sigStep][data_[0] & 0xff];
            printf("sigStep = %d, %d\n", m_sigStep, data_[0] & 0xff);
            if(m_sigStep < 4) {
                socket_.async_read_some(boost::asio::buffer(data_, 1),
                                        boost::bind(&QcSession::readSignature, shared_from_this(),
                                                    boost::asio::placeholders::error,
                                                    boost::asio::placeholders::bytes_transferred));
            }
            else {
                printf("try read PKTHeader\n");
                socket_.async_read_some(boost::asio::buffer(&m_pktHeader, sizeof(m_pktHeader)),
                                        boost::bind(&QcSession::readPacketHeader, shared_from_this(),
                                                    boost::asio::placeholders::error,
                                                    boost::asio::placeholders::bytes_transferred));

            } 
        }
        void readPacketHeader(const boost::system::error_code& error,
                         size_t bytes_transferred) {

            viewMemory(&m_pktHeader, bytes_transferred);
            if(error) {
                printf("delete %x\n", this);
                return;
            }
            printf("pkt HeaderLen %x , %d\n", m_pktHeader.cmdType, m_pktHeader.dataLen);

            m_pktBody.reset((PKT*)new uint8_t[sizeof(PKT) + m_pktHeader.dataLen], [](PKT* p) { 
                delete [] p;
            });

            memcpy(m_pktBody.get(), &m_pktHeader, sizeof(PKT)); 
            socket_.async_read_some(boost::asio::buffer((m_pktBody.get())->data, m_pktHeader.dataLen),
                                    boost::bind(&QcSession::readPacketBody, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }
        void readPacketBody(const boost::system::error_code& error,
                         size_t bytes_transferred); 
        tcp::socket socket_;
        enum { max_length = 1024 };
        char data_[max_length];
        uint8_t m_mataSignature[10][0xff + 1];
        boost::asio::io_service& m_ioService;

        PKT m_pktHeader;
        std::shared_ptr<PKT> m_pktBody;
        int m_sigStep;
};



