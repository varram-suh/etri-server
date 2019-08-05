#include "chrono-util.h"

using namespace std;
void ChronoDuration::setBaseTime() {
    m_baseTime = chrono::high_resolution_clock::now();
}
double ChronoDuration::durationWithBaseTime() {
    typedef typename chrono::duration<double, milli> doublemillisec;
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
    doublemillisec doubleMilliSec = chrono::duration_cast<doublemillisec>(end - m_baseTime);
    m_baseTime = chrono::high_resolution_clock::now();
    return doubleMilliSec.count();
}
double ChronoDuration::duration() {
    typedef typename chrono::duration<double, milli> doublemillisec;
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
    doublemillisec doubleMilliSec = chrono::duration_cast<doublemillisec>(end - m_baseTime);
    return doubleMilliSec.count();
}
