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
#include <map>


class shared_const_buffer
{
    public:
        // Construct from a std::string.
        explicit shared_const_buffer(const std::vector<char>& data)
            : data_(new std::vector<char>(data.begin(), data.end())),
            buffer_(boost::asio::buffer(*data_))
    {
    }

        explicit shared_const_buffer(const char* data, size_t len)
            : data_(new std::vector<char>(data, data + len)),
            buffer_(boost::asio::buffer(*data_))
    {
    }
        typedef boost::asio::const_buffer value_type;
        typedef const boost::asio::const_buffer* const_iterator;
        const boost::asio::const_buffer* begin() const { return &buffer_; }
        const boost::asio::const_buffer* end() const { return &buffer_ + 1; }

    private:
        boost::shared_ptr<std::vector<char> > data_;
        boost::asio::const_buffer buffer_;
};
