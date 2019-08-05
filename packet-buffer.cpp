#include "packet-buffer.h"

//#include "shell.h"
//#include "sockethelper.h"
shared_const_buffer makePKT(short cmdType, uint8_t* data, size_t len) {
    //std::string jsonString = "{\"user_key\":\"AA:BB:CC:DD\", \"model_name\":\"lile\"}";
    int totalSize = sizeof(PKT_SIG) + len;
    PKT_SIG* pkt = (PKT_SIG*)malloc(totalSize);
    pkt->s1 = 0x8a;
    pkt->s2 = 0x8a;
    pkt->s3 = 0x8a;
    pkt->s4 = 0x8a;
    pkt->cmdType = cmdType;
    pkt->dataLen = len;
    uint8_t* bp = (uint8_t*)pkt;

    memcpy(pkt->data, data, len);
    std::vector<char> vbp(bp, bp + totalSize);
    free(pkt);
    //for(int i=0; i<vbp.size(); i++) {
        //printf("%d%c ", vbp[i], vbp[i]);
    //}
    //printf("\n");
    return shared_const_buffer(vbp);

}
shared_const_buffer makePKT(short cmdType, std::string jsonString) {
    //std::string jsonString = "{\"user_key\":\"AA:BB:CC:DD\", \"model_name\":\"lile\"}";
    int totalSize = sizeof(PKT_SIG) + jsonString.size();
    PKT_SIG* pkt = (PKT_SIG*)malloc(totalSize);
    pkt->s1 = 0x8a;
    pkt->s2 = 0x8a;
    pkt->s3 = 0x8a;
    pkt->s4 = 0x8a;
    pkt->cmdType = RSP_USERKEY;
    pkt->dataLen = jsonString.size();
    uint8_t* bp = (uint8_t*)pkt;

    memcpy(pkt->data, jsonString.c_str(), jsonString.size()); 
    std::vector<char> vbp(bp, bp + totalSize);
    free(pkt);
    //for(int i=0; i<vbp.size(); i++) {
        //printf("%d%c ", vbp[i], vbp[i]);
    //}
    //printf("\n");
    return shared_const_buffer(vbp);

}
std::vector<uint8_t> makeVectorBLEPKT(const std::vector<char>& inner) {
    // tt = 36;
    // min(tt, 20)
    int totalSize = inner.size();
    int offset = 0;
    std::vector<uint8_t> vbp;
    while(offset < inner.size()) {
        uint8_t segmentSize = std::min({20, totalSize - offset}); 
        vbp.push_back(0xAB);
        vbp.push_back(0xCD);
        vbp.push_back(segmentSize);
        vbp.insert(vbp.end(), &inner[offset], &inner[offset + segmentSize]);
        uint32_t crc = 0xAB + 0xCD + segmentSize;
        for(int i=offset; i<offset + segmentSize; i++) {
            crc += inner[i];
            crc = crc & 0xFF;
        }
        offset += segmentSize; 
        vbp.push_back(crc & 0xFF);
        vbp.push_back(0xFE);
    }
    return vbp;
}
std::vector<char> makeVectorPKT(short cmdType, std::string jsonString) {
    //std::string jsonString = "{\"user_key\":\"AA:BB:CC:DD\", \"model_name\":\"lile\"}";
    int totalSize = sizeof(PKT_SIG) + jsonString.size();
    PKT_SIG* pkt = (PKT_SIG*)malloc(totalSize);
    pkt->s1 = 0x8a;
    pkt->s2 = 0x8a;
    pkt->s3 = 0x8a;
    pkt->s4 = 0x8a;
    pkt->cmdType = cmdType;
    pkt->dataLen = jsonString.size();
    uint8_t* bp = (uint8_t*)pkt;

    memcpy(pkt->data, jsonString.c_str(), jsonString.size()); 
    std::vector<char> vbp(bp, bp + totalSize);
    free(pkt);
    return vbp;

}
