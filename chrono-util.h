#pragma once

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

class ChronoDuration {
public:
    std::chrono::high_resolution_clock::time_point m_baseTime;
    void setBaseTime();
    double durationWithBaseTime();
    double duration();
};

class Chrono{
    public:
        static uint64_t tickCount() {
            std::chrono::time_point<std::chrono::system_clock> p1, p2, p3; 
            p2 = std::chrono::system_clock::now();
            auto t = std::chrono::duration_cast<std::chrono::milliseconds>(
                    p2.time_since_epoch()).count();
            return t;
        }
};

