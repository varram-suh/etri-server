#pragma once
#include <algorithm> // for std::min
#include <vector>
#include <inttypes.h>

class CircularBuffer
{
public:
    CircularBuffer(size_t capacity);
    ~CircularBuffer();

    size_t size() const { return size_; }
    size_t capacity() const { return data_.size(); }
    // Return number of bytes written.
    ssize_t writeBytes(const void *data, size_t bytes);
    ssize_t writeBytes(const void * data, size_t bytes, const char* msg);
    // Return number of bytes read.
    size_t read(void *data, size_t bytes);
    size_t readBytes(void *data, size_t bytes);
    size_t flushBuffer(size_t bytes);
    size_t readOnlyRead(void* data, size_t bytes) const;
    size_t remainSize() { return data_.size() - size_; };
    void clear();
    void showStatus(const char* msg);
public:
    size_t beg_index_, end_index_, size_;
    std::vector<uint8_t> data_; 
    // char *data_;
    // char *data_;
};
