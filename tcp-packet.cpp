#include "tcp-packet.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <memory.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// retry = true 시 보낸 바이트가 리턴됨
// retry = false 시 retry 가 발생하기 전까지의 보낸 바이트가 리턴됨
int writeWithSegment(int fds, uint8_t* buf, int bufSize, int segmentSize, bool retry) {
    int written = 0;
    while(written != bufSize) {
        int sendingSize = std::min(segmentSize, bufSize - written);
        int sentSize = ::write(fds, &buf[written], sendingSize);
        if(sentSize == -1) {
            if(errno == EINTR || errno == EAGAIN) {
                if(retry == true) {
                    //printf("retry\n");
                    continue;
                }
                else  {
                    //printf("retry break\n");
                    return written;
                }
            }
            else if(errno == ECONNRESET ) /* TCP CONN RESET */
            {
                printf("peer connect reset\n");
                return -ECONNRESET;
            }
            else{
                printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));
                //close(fds);
                return -ECONNRESET;
            }
        }
        else if(sentSize == 0) {
            printf("may disconnect writeWithSegment\n");
            return -ECONNRESET;
        }
        else {
            written += sentSize;
        }
        //KLOG(gg, %d);
        //KLOG(33333333, %d);
    }
    return written;
}

int readWithSegment(int fds, uint8_t* buf, int bufSize, int segmentSize, bool retry) {
    int written = 0;
    while(written != bufSize) {
        int sendingSize = std::min(segmentSize, bufSize - written);
        int sentSize = ::read(fds, &buf[written], sendingSize);
        if(sentSize == -1) {
            if(errno == EINTR || errno == EAGAIN) {
                if(retry == true) {
                    //printf("retry\n");
                    continue;
                }
                else  {
                    //printf("retry break\n");
                    return written;
                    //return -EAGAIN;
                }
            }
            else if(errno == ECONNRESET ) /* TCP CONN RESET */
            {
                printf("peer connect reset\n");
                return -ECONNRESET;
            }
            else{
                printf("read error\n");
                return -1;
            }
        }
        else if(sentSize == 0) {
            printf("may disconnect\n");
            return -ECONNRESET;
        }
        else {
            written += sentSize;
        }
        //KLOG(gg, %d);
        //KLOG(33333333, %d);
    }
    return written;
}

int makeHeader(int cmdType, uint8_t* pkt, int bugSize) {
    return 0;
}

int writeWithPKT(int fds, int cmdType, uint8_t* buf, int bufSize, bool retry)
{
    PKT* pkt = (PKT*)malloc(sizeof(PKT) + bufSize);
    pkt->cmdType = cmdType;
    pkt->dataLen = bufSize;
    memcpy(pkt->data, buf, bufSize);
    //pkt->data = buf;
    auto ret = writeWithSegment(fds, (uint8_t*)pkt, sizeof(PKT) + bufSize, 1024, true);
    free(pkt);
    return ret;
}
