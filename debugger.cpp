#include "debugger.h"
#include <cstdio>
#include <algorithm>

void viewMemory(void* address, size_t len) {
    unsigned char* pp = (unsigned char*)address;
    size_t cols = std::min({(size_t)8, len});
    for(size_t i=0; i<len; ) {
        printf("\n");
        printf("%X : ", (uint32_t*)&pp[i]);
        for(size_t j=i; j<i + cols; j++) {
            printf("%02X ", (unsigned char)(pp[j]));
        }
        printf(" | ");
        for(size_t j=i; j<i + cols; j++) {
            if(0x20 <= pp[j] && pp[j] <= 0x7E) {
                printf("%c", (pp[j]));
            }
            else {
                printf(".");
            } 
        } 
        i += 8;
    }
    printf("\n");
}
