#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>


enum HTTPErrors
{
  /// Permission denied.
    notResolved = 1000,
    notConnected = 1001,
    _404 = 404,
};

namespace boost {
    namespace system { 
        template<> struct is_error_code_enum<HTTPErrors>
        {
            static const bool value = true;
        };
    }
}

namespace {
struct HTTPErrorCategory : boost::system::error_category
{
    const char* name() const noexcept override;
    std::string message(int ev) const override;
};

const char* HTTPErrorCategory::name() const noexcept
{
    return "HTTPError";
}

std::string HTTPErrorCategory::message(int ev) const
{
    if(ev == HTTPErrors::notResolved) {
        return std::string("http is not resolved");
    }
    else if(ev == HTTPErrors::notConnected) {
        return std::string("http is not connected");
    }
    return std::string("http error code: ") + std::to_string(ev);
}
const HTTPErrorCategory theHTTPCategory {};
}


//boost::system::error_code make_error_code(HTTPErrors fe) {
    //return {static_cast<int>(e), theFlightsErrCategory};
//}

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

//------------------------------------------------------------------------------

// Report a failure

// custom error conditions enum type:

class HTTPSession : public std::enable_shared_from_this<HTTPSession>
{
    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::beast::flat_buffer buffer_; // (Must persist between reads)
    http::request<http::string_body> req_;
    http::response<http::string_body> res_; 
public:
    void fail(boost::system::error_code ec, char const* what)
    {
        CONSOLE_CRITICAL("{} {}", ec.message(), ec.value());
        //std::cerr << what << ": " << ec.message() << "\n";
    }
    // Resolver and socket require an io_service
    explicit
    HTTPSession(boost::asio::io_service& ios)
        : resolver_(ios)
        , socket_(ios)
    {
    }

    static void request( 
        boost::asio::io_service& ios,
        char const* host,
        const std::string& port,
        char const* target,
        const std::string& body, 
        std::function<void(std::string, boost::system::error_code)> onRead) { 
        printf("request\n");

        // Launch the asynchronous operation
        std::make_shared<HTTPSession>(ios)->run(host, port, target, body, onRead); 
        // Run the I/O service. The call will return when
        // the get operation is complete.
        //ios.run();
    }
    // Start the asynchronous operation
    void
    run(
        char const* host,
        const std::string& port,
        char const* target,
        const std::string& body, 
        std::function<void(std::string, boost::system::error_code)> onRead
        )
    {
        printf("line : %d\n", __LINE__);
        // Set up an HTTP GET request message
        req_.version(11);
        req_.method(http::verb::post);
        req_.target(target);
        req_.set(http::field::host, host);
        req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req_.set(http::field::content_type, "application/json");
        req_.body() = body;
        req_.prepare_payload();
        // Look up the domain name
        auto self = shared_from_this();
        resolver_.async_resolve(
            {host, port}, 
            [this, onRead, self](
                const boost::system::error_code& ec,
                const tcp::resolver::iterator& result) {
                if(ec) {
                    onRead("not resolved", ec);
                    return fail(ec, "resolve!!!"); 
                }

                //boost::asio::async_connect(socket_, result,
                                           //std::bind(
                                               //&session::on_connect,
                                               //shared_from_this(),
                                               //std::placeholders::_1));
                boost::asio::async_connect(
                    socket_, result,
                    [this, onRead, self](boost::system::error_code ec, tcp::resolver::iterator)
                    {
                        if(ec)
                        {
                            onRead("not connected", ec);
                            return fail(ec, "connect!!!");
                        }

                        printf("on connect\n");
                        // Send the HTTP request to the remote host
                        http::async_write(
                            socket_, req_,
                            [this, self, onRead](
                                boost::system::error_code ec,
                                std::size_t bytes_transferred)
                            {
                                boost::ignore_unused(bytes_transferred);

                                if(ec)
                                {
                                    onRead("failed to write", ec);
                                    return fail(ec, "failed to write");
                                }

                                // Receive the HTTP response
                                http::async_read(
                                    socket_, buffer_, res_,
                                    [this, onRead, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                                        boost::ignore_unused(bytes_transferred);
                                        bool successStatus = 200 <= res_.result_int() && res_.result_int() < 300;
                                        std::ostringstream oss;
                                        oss << res_.body(); 
                                        socket_.shutdown(tcp::socket::shutdown_both, ec); 

                                        if(ec) {
                                            onRead("", ec);
                                        }
                                        else if(successStatus == false) {
                                            auto makeEc = boost::system::error_code(static_cast<int>(res_.result_int()), 
                                                                                    theHTTPCategory); 
                                                                                    //boost::system::generic_category());
                                            onRead(oss.str(), makeEc); 
                                        } 
                                        else {
                                            onRead(oss.str(), ec);
                                        }
                                    });
                            });
                    });
            }); 
    }
};

