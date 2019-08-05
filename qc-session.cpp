#include "qc-session.h"
#include "packet-buffer.h"
#include "json.hpp"
#include "shell.h"
#include "http-request.h"
#include "ks19937.h"
using json = nlohmann::json;

//#include "globalvar.h" 

void QcSession::readPacketBody(const boost::system::error_code& error,
                    size_t bytes_transferred) {
    if(error) {
        printf("delete %x\n", this);
        return;
    }

    PKT* pktSig = m_pktBody.get();
    char* tt = (char*)m_pktBody->data;
    viewMemory(pktSig->data, bytes_transferred);
    std::string jsonString = tt;
    std::cout << jsonString << std::endl;
    std::map<int, std::function<int (PKT*)>> requester;

    //requestProcessor[TYPE_GET_REALTIME_IMG] = std::bind(&DirectMode::requestImg, this, std::placeholders::_1, std::placeholders::_2);
    //requestProcessor[TYPE_D_CERTIFICATION] = std::bind(&DirectMode::requestCert, this, std::placeholders::_1, std::placeholders::_2);
    //requester[REQ] = std::bind(&QcSession::requestUserKey, this, std::placeholders::_1);
    printf("cmdType = %d\n", pktSig->cmdType);
    requester[pktSig->cmdType](nullptr);
    this->start();
}

