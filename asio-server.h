#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp> 
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp> 
#include <boost/shared_ptr.hpp> 
template <typename T>
class ASIOServer
{
    using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
    public:
        ASIOServer(boost::asio::io_service& io_service, short port)
            : io_service_(io_service),
            acceptor_(io_service) // , tcp::v4()) //, port))
    {

        acceptor_.open(tcp::v4());

        int one = 1;
        setsockopt(acceptor_.native_handle(), SOL_SOCKET, SO_REUSEADDR/* | SO_REUSEPORT*/, &one, sizeof(one)); 
        acceptor_.bind(tcp::endpoint(tcp::v4(), port));
        acceptor_.listen();
         // resolving
        start_accept();
    }
    private:
        void start_accept()
        {
            //printf("hmm2?\n");
            auto new_session = T::create(acceptor_.get_io_service());
            printf("try accept\n");
            acceptor_.async_accept(new_session->socket(),
                                   boost::bind(&ASIOServer::handle_accept, this, new_session,
                                               boost::asio::placeholders::error));
            //acceptor_.async_accept(new_session->socket(),
                                   //boost::bind(&server::handle_accept, this, new_session,
                                               //boost::asio::placeholders::error));
        }

        void handle_accept(typename T::pointer new_session,
                           const boost::system::error_code& error)
        {
            printf("SetupSession is connected\n");
            if (!error)
            {
                new_session->start();
            }
            else
            {
                printf("delete %x\n", new_session.get());
                std::cout << error.message() << std::endl;
            }

            start_accept();
        }

        boost::asio::io_service& io_service_;
        tcp::acceptor acceptor_;
};
