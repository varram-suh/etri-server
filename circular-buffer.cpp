
#include "circular-buffer.h"
#include <memory.h>
#include <cstdio>

CircularBuffer::CircularBuffer(size_t capacity)
    : beg_index_(0), end_index_(0), size_(0), data_(capacity) {
    printf("capa: %d\n", capacity);
    // data_ = new char[capacity];
    // if(data_ == nullptr) {
    //     printf("circular buffer alloc failed %d\n", capacity);
    // }
}

CircularBuffer::~CircularBuffer(){
    // delete [] data_;
}

ssize_t CircularBuffer::writeBytes(const void *data, size_t bytes) {
    return writeBytes(data, bytes, "");
}

// Return number of bytes written.
ssize_t CircularBuffer::writeBytes(const void * dataParam, size_t bytes, const char* msg) {
    uint8_t* data = (uint8_t*)dataParam;
    if (bytes == 0) return 0;

    size_t capacity = data_.size();
    //size_t bytes_to_write = std::min(bytes, capacity - size_);
    int overwriteBytes = bytes - (capacity - size_);
    if(overwriteBytes > 0) {
        // printf("[%s], warning!! queue over-written, cap = %d, size_ : %d, to write Size = %zu\n", msg, capacity, size_, bytes);
        size_t before = data_.size();
        if(beg_index_ < end_index_) {
            printf("if(beg_index_ < end_index_ %s) {\n", msg);
            size_t reallocSize = std::max(data_.size() + overwriteBytes, data_.size() * 2);
            data_.resize(reallocSize);
        }
        else {
            printf("else { %s\n", msg);
            size_t reallocSize = std::max(data_.size() + overwriteBytes, data_.size() * 2);
            size_t lastIndex = data_.size();
            data_.resize(reallocSize);
            if(size_ != 0) {
                memcpy(&data_[lastIndex], &data_[0], end_index_);
                end_index_ = lastIndex + end_index_;
            }
        }
        printf("size: %zu -> %zu\n", before, data_.size());
        // return -1;
    }
    size_t bytes_to_write = bytes;

    capacity = data_.size();
    // Write in a single step
    if (bytes_to_write <= capacity - end_index_)
    {
        // std::copy(&data[0], &data[bytes_to_write], data_.begin() + end_index_);
        memcpy(&data_[end_index_], data, bytes_to_write);
        end_index_ += bytes_to_write;
        end_index_ = (end_index_ % capacity);
    }
    // Write in two steps
    else
    {
        size_t size_1 = capacity - end_index_;
        memcpy(&data_[0] + end_index_, data, size_1);
        size_t size_2 = bytes_to_write - size_1;
        memcpy(&data_[0], data + size_1, size_2);
        end_index_ = size_2;
    }


    size_ += bytes_to_write;
    return bytes_to_write;
}

size_t CircularBuffer::readBytes(void * data, size_t bytes) {
    return this->read(data, bytes);
}
// Return number of bytes read.
size_t CircularBuffer::read(void * _data, size_t bytes) {
    if (bytes == 0) return 0;

    size_t capacity = data_.size();
    size_t bytes_to_read = std::min(bytes, size_);

    char* data = (char*)_data;
    // Read in a single step
    // [0 0 0 0 0 0 1 1 1 1]
    // 10 - 6 = 4
    if (bytes_to_read <= capacity - beg_index_)
    {
        memcpy(data, &data_[beg_index_], bytes_to_read);
        beg_index_ += bytes_to_read;
        beg_index_ = (beg_index_ % capacity);
    }
    // Read in two steps
    else
    {
        size_t size_1 = capacity - beg_index_;
        memcpy(data, &data_[beg_index_], size_1);
        size_t size_2 = bytes_to_read - size_1;
        memcpy(data + size_1, &data_[0], size_2);

        beg_index_ = size_2;
    }
    //std::cout << "before size_ : " << size_ << std::endl;
    size_ -= bytes_to_read;
    //std::cout << "after size_ : " << size_ << std::endl;
    return bytes_to_read;
}

size_t CircularBuffer::flushBuffer(size_t bytes) {
    if (bytes == 0) return 0;

    size_t capacity = data_.size();
    size_t bytes_to_read = std::min(bytes, size_);

    // Read in a single step
    if (bytes_to_read <= capacity - beg_index_) {
        beg_index_ += bytes_to_read;
        if (beg_index_ == capacity) beg_index_ = 0;
    }
    // Read in two steps
    else
    {
        size_t size_1 = capacity - beg_index_;
        size_t size_2 = bytes_to_read - size_1;
        beg_index_ = size_2;
    }
    //std::cout << "before size_  " << size_ << std::endl;
    size_ -= bytes_to_read;
    //std::cout << "after size_  " << size_ << std::endl;
    return bytes_to_read;
}

size_t CircularBuffer::readOnlyRead(void * _data, size_t bytes) const {
    char* data = (char*)_data;

    if (bytes == 0) return 0;

    size_t capacity = data_.size();
    size_t bytes_to_read = std::min(bytes, size_);

    // Read in a single step
    if (bytes_to_read <= capacity - beg_index_)
    {
        memcpy(data, &data_[beg_index_], bytes_to_read);
    }
    // Read in two steps
    else
    {
        size_t size_1 = capacity - beg_index_;
        memcpy(data, &data_[beg_index_], size_1);
        size_t size_2 = bytes_to_read - size_1;
        memcpy(data + size_1, &data_[0], size_2);
    }

    return bytes_to_read;
}

void CircularBuffer::clear() {

    beg_index_ = 0;
    end_index_ = 0;
    size_ = 0;
}

void CircularBuffer::showStatus(const char* msg) {
    float percent = (float)size_ / (float)remainSize();
    if(percent >= 0.3f) {
        printf("[%s] %d : [%f %%]\n", msg, data_.size(), percent * 100.f);
    }
}
