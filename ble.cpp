#include "ble.h"
#include <cstring>
#include "spdlogger.h"
#include "sockethelper.h"
#include "tcp-packet.h"
#include "shell.h"
#include "save-data.h"
#include "json.hpp"
#include <boost/format.hpp>
#include "diffie-hellman.h"
#include "http-request.h"
#include "pinger.h"
#include "chrono-util.h"
#include "globalvar.h"

#include "signal-session.h"
#include "room-session.h"
#include "jwt-util.h"

using json = nlohmann::json;
using namespace save_data;

char sendBuffer[100];

namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>
static void writeToWebSocket(websocket::stream<tcp::socket>& ws, const std::string& jsonString,
                      std::function<void(std::string rsp)> ff) {
    shared_const_buffer t4 = shared_const_buffer(std::vector<char>(jsonString.begin(), jsonString.end()));
    ws.write(t4);
    std::string s;

    boost::beast::multi_buffer buffer;
    if(ff != nullptr) {
        ws.read(buffer);

        ff(boost::beast::buffers_to_string(buffer.data()));
    }
    // Read a message into our buffer
}
int BLESession::start() {
    CONSOLE_INFO("BLE START");
    m_remains = 1;
    m_port.async_read_some(asio::buffer(read_msg_, m_remains),
                           boost::bind(&BLESession::getOuterPacket,
                                       shared_from_this(),
                                       asio::placeholders::error,
                                       asio::placeholders::bytes_transferred) );

    return 0;
}


int BLESession::sender() {
    if(m_sendBuffer.size() > 0) {
        char buf[30];
        int readBytes = std::min({(size_t)19, m_sendBuffer.size()});
        m_sendBuffer.read(buf, readBytes);
        std::vector<char> vbp(buf, &buf[readBytes]);
        auto buffer = shared_const_buffer(vbp);
        asio::async_write(m_port, buffer, [this, self = shared_from_this()](boost::system::error_code ec, std::size_t length) {
            if(ec) {
                printf("error\n");
                return;
            }
            printf("write length = %d\n", length);
        });
    }

    m_io.post([this, self = shared_from_this()]() {
        m_bleSendTimer.expires_at(m_bleSendTimer.expires_at() + boost::posix_time::milliseconds(10));
        m_bleSendTimer.async_wait(boost::bind(&BLESession::sender, shared_from_this()));
    });

    return 0;
}
void BLESession::getOuterPacket(const boost::system::error_code& error, size_t bytes_transferred)
{
    if(error) {
        printf("ble getOuterPacket error\n");
        std::cout << error.message() << std::endl;
        return;
    }
    if(bytes_transferred != 0) {
        //viewMemory(read_msg_, bytes_transferred, "..");
    }

    m_remains -= bytes_transferred;
    for(int i=0; i<bytes_transferred; i++) {
        m_remainBuffer.push_back((uint8_t)read_msg_[i]);
    }
    if(m_remains == 0) {
        int before = m_sigStep;
        m_sigStep = m_sigMata[m_sigStep][read_msg_[0] & 0xFF];
        int after = m_sigStep;
        //printf("%d -> %d\n", before, after);
        const int H1 = 0;
        const int H2 = 1;
        const int GET_LENGTH = 2;
        const int INNER_DATA = 3;
        const int CRC = 4;
        const int TAIL = 5;
        if(before == H1) {
            m_remains = 1;
        }
        else if(before == H2) {
            m_remains = 1;
        }
        else if(before == GET_LENGTH) {
            int lengthOfLength = read_msg_[0] & 0xFF;
            m_remains = lengthOfLength;
        }
        else if(before == INNER_DATA) {
            innerProtocol(m_remainBuffer);
            m_remains = 1;
        }
        else if(before == CRC) {
            printf("CRC = %x\n", read_msg_[0]);
            m_remains = 1;
        }
        else if(before == TAIL) {
            printf("TAIL = %x\n", read_msg_[0]);
            m_remains = 1;
        }
        m_remainBuffer.clear();
    }
    memset(read_msg_, 0, sizeof(read_msg_));
    m_port.async_read_some(asio::buffer(read_msg_, m_remains),
                           boost::bind(&BLESession::getOuterPacket,
                                       shared_from_this(),
                                       asio::placeholders::error,
                                       asio::placeholders::bytes_transferred) );
}
void BLESession::innerProtocol(const std::vector<uint8_t>& innerData) {
    m_innerBuffer.writeBytes(&innerData[0], innerData.size());

    while(1) {
        if(m_innerSigStep <= 3) {

            if(m_innerBuffer.size() >= 1) {
                CONSOLE_INFO("sig header");
                uint8_t buff;
                m_innerBuffer.readBytes(&buff, 1);
                int before = m_innerSigStep;
                m_innerSigStep = m_innerSigMata[m_innerSigStep][buff];
                int after = m_innerSigStep;
                CONSOLE_INFO("{} -> {} -> {}", before, buff, after);
            }
            else {
                break;
            }
        }
        // cmd 2, length 4
        else if(m_innerSigStep == 4) {
            if(m_innerBuffer.size() >= 6) {
                CONSOLE_INFO("cmd, length");
                uint8_t buff[6];
                m_innerBuffer.readBytes(buff, 6);
                PKT* tempPacket = (PKT*)buff;
                uint16_t cmd = tempPacket->cmdType;
                int length = tempPacket->dataLen;
                m_pktBody = std::shared_ptr<PKT>((PKT*)malloc(sizeof(PKT) + length), [](PKT* p) { free(p); });
                m_pktBody->cmdType = cmd;
                m_pktBody->dataLen = length;
                m_sigStep = 5;
                std::cout << m_pktBody->cmdType << ", " << m_pktBody->dataLen;
                int ss = m_pktBody->dataLen;
                CONSOLE_INFO("body: {}, {}", m_pktBody->cmdType, ss);
                m_innerSigStep = 5;
            }
            else {
                break;
            }
        }
        else if(m_innerSigStep == 5) {
            if(m_innerBuffer.size() >= m_pktBody->dataLen) {
                CONSOLE_INFO("reading body");
                m_innerBuffer.readBytes(m_pktBody->data, m_pktBody->dataLen);
                viewMemory(m_pktBody.get(), sizeof(PKT) + m_pktBody->dataLen);
                m_innerSigStep = 0;

                if(m_pktBody->cmdType == REQ_SCAN_WIFI) {
                    CONSOLE_INFO("call scanAP ");
                    scanAP();
                }
                else if(m_pktBody->cmdType == REQ_CONNECT_AP) {
                    CONSOLE_INFO("call connectAP ");
                    GVar::get()->isBLESetting = true;
                    GVar::get()->setupFailCount = 0;
                    connectAP();
                }
                else if(m_pktBody->cmdType == REQ_CONNECT_SERVER) {
                    CONSOLE_INFO("call connect to server ");
                    connectAuthServer();
                }
                else if(m_pktBody->cmdType == REQ_USERKEY) {
                    CONSOLE_INFO("call REQ_USERKEY ");
                    int64_t phonePublicKey = -1;
                    try{
                        std::string s = std::string(m_pktBody->data, &m_pktBody->data[m_pktBody->dataLen]);
                        json root = json::parse(s); // contains "access_token", "refresh_token"
                        phonePublicKey = root["public_key"].get<int64_t>();
                    } catch(json::parse_error pe) {
                        std::cout << __FILE__ << "::" << __LINE__ << ": " << pe.what() << std::endl;
                    }
                    MacAddr macAddr = getMacAddress("wlan0");
                    int64_t secretKey = 12341234L;
                    //(macAddr.bytes[0] << 40) |
                    //(macAddr.bytes[1] << 32) |
                    //(macAddr.bytes[2] << 24) |
                    //(macAddr.bytes[3] << 16) |
                    //(macAddr.bytes[4] << 8) |
                    //(macAddr.bytes[5] << 0);
                    const int64_t P = 36854777731L;
                    const int64_t G = 5L;
                    std::cout << "secret Key: " << secretKey << std::endl;
                    int64_t publicKey = powmodp(G, secretKey, P);
                    std::cout << "secret Key: " <<  secretKey << ", publicKey: " << publicKey << std::endl;
                    int64_t sharedKey = powmodp(phonePublicKey, secretKey, P);
                    std::cout << "calculated sharedKey: " << sharedKey << std::endl;
                    //assert(secret1 == secret2);
                    //std::cout << "a=" << a << ", b=" << b << ", s1=" << secret1 << ", s2=" << secret2 << std::endl;
                    json makeRsp;
                    makeRsp["public_key"] = publicKey;
                    makeRsp["model_name"] = "fitcam";
                    std::vector<char> pb = makeVectorPKT(RSP_USERKEY, makeRsp.dump());
                    std::vector<uint8_t> outer = makeVectorBLEPKT(pb);
                    m_sendBuffer.writeBytes(&outer[0], outer.size());
                }
            }
            else {
                break;
            }
        }
        else {
            break;
        }
    }
}

int BLESession::connectAuthServer() {
    MacAddr macAddrNums = getMacAddress("wlan0");
    char macAddrStr[256];
    sprintf(macAddrStr, "%02X:%02X:%02X:%02X:%02X:%02X",
            macAddrNums.bytes[0],
            macAddrNums.bytes[1],
            macAddrNums.bytes[2],
            macAddrNums.bytes[3],
            macAddrNums.bytes[4],
            macAddrNums.bytes[5]);
    CONSOLE_CRITICAL("{}", __FUNCTION__);
    int gmtDif = 0;
    try{
        std::string s = std::string(m_pktBody->data, &m_pktBody->data[m_pktBody->dataLen]);
        json root = json::parse(s); // contains "access_token", "refresh_token"

        CONSOLE_INFO("user_id, gmt_dif = {}", root.dump());
        gmtDif = root["gmt_dif"].get<int>();
        SD::get()->saveValue("user_id", root["user_id"].get<std::string>());
    }catch(json::parse_error pe) {
        std::cout << __FILE__ << "::" << __LINE__ << ": " << pe.what() << std::endl;
    }

    json sendJson;
    sendJson["user_id"] = SD::get()->getValue<std::string>("user_id");
    sendJson["skey"] = QD::get()->getValue<std::string>("skey");
    sendJson["mac_addr"] = macAddrStr;
    sendJson["gmt_dif"] = gmtDif;


    CONSOLE_INFO("sendJson: {}", sendJson.dump());
    // blocking
    HTTPSession::request(
        m_io,
        IP::authServer, IP::authServerPort, "/setup/dev", sendJson.dump(),
        [this, self = shared_from_this()](std::string s, boost::system::error_code ec) {
            // {"type":"success","data":{"skey":"XykaTsyR83RnUK4dgTyFJqKmrEifotJlw0E/ZEn2IiA="}}
            if(ec) {
                CONSOLE_CRITICAL("It is failed to request AUTH Server: {}", ec.message());
                CONSOLE_CRITICAL("    {} /setup/dev", s);
                GVar::get()->setupFailCount++;
                if(GVar::get()->setupFailCount >= 5) {
                    json jsonObj = {
                        {"result", "au_conn_err"}
                    };
                    GVar::get()->bleSession->pushCommand(RSP_CONNECT_SERVER, jsonObj);
                }
                else {
                    m_recoveryTimer.expires_at(m_recoveryTimer.expires_at() + boost::posix_time::seconds(3));
                    m_recoveryTimer.async_wait([this, self = shared_from_this()](const boost::system::error_code& ec) {
                        connectAuthServer();
                    });
                }
                return;
            }
            json jsonObj = {
                {"result", "au_conn_ok"}
            };
            GVar::get()->bleSession->pushCommand(RSP_CONNECT_SERVER, jsonObj);
            CONSOLE_INFO("/setup/dev result: {}", s);
            // iat, exp, user_id
            json root;
            try{
                root = json::parse(s); // contains "access_token", "refresh_token"
                json data = root["data"];
                updateTokens(data);
                printf("setup complete\n");

                auto const host = IP::signalServer;
                auto const port = IP::signalServerPort;
#if 0
                if(GVar::get()->signalSession->m_connectionStatus == SignalSession::CONNECTED) {
                    CONSOLE_INFO("try to connect signal server");
                    GVar::get()->signalSession->requestSetup();
                }
#endif
                GVar::get()->signalSession->connectSignalServer(host, port, "hello", [](int b) {
                                                                                         CONSOLE_CRITICAL("signal server return value: {}, {}::{}", b, __FILE__, __LINE__);
                    GVar::get()->signalSession->requestSetup();
                });

                //signalServerConnect();
            } catch(json::parse_error pe) {
                CONSOLE_INFO("{}::{}: {}", __FILE__, __LINE__, pe.what());
            }
        });
    return 0;
}

int BLESession::scanAP() {
    CONSOLE_CRITICAL("{}", __FUNCTION__);
    std::string status1 = resultFromCmd("ifconfig wlan0 up");
    std::string status = resultFromCmd("cd /root && ./iwlist wlan0 scan");
    //printf("<<<\n%s\n>>>\n", status.c_str());
    std::vector<AP> aps;
    parseAP(&aps, status);
    json root;
    root["skey"] = QD::get()->getValue<std::string>("skey");
    root["wifi_list"] = {};
    for(int i=0; i<aps.size(); i++) {
        printf("signal level %d\n", aps[i].signalLevel);
        printf("qual %d\n", aps[i].quality);
        json wifiList;
        wifiList["ssid"] = aps[i].apName;
        wifiList["sec"] = "PSK";
        wifiList["stren"] = aps[i].signalLevel;
        root["wifi_list"].push_back(wifiList);
    }
    std::vector<char> pb = makeVectorPKT(
        RSP_SCAN_WIFI, root.dump());

    std::vector<uint8_t> outer = makeVectorBLEPKT(pb);
    viewMemory(&outer[0], outer.size());
    std::string ttt = root.dump();
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << ttt << std::endl;
    //m_sendBuffer.writeBytes(&ttt[0], ttt.size());
    m_sendBuffer.writeBytes(&outer[0], outer.size());
    return 0;
}

int BLESession::connectAP() {
    CONSOLE_CRITICAL("{}", __FUNCTION__);
    json root;
    std::string s = std::string(m_pktBody->data, &m_pktBody->data[m_pktBody->dataLen]);
    try {
        root = json::parse(s); // contains "access_token", "refresh_token"
        CONSOLE_INFO("ssid ids = {}", root.dump());
    }catch(json::parse_error pe) {
        std::cout << __FILE__ << "::" << __LINE__ << ": " << pe.what() << std::endl;
        std::cout << s << std::endl;
    }

    uint32_t result = connectToWifi(root["ssid"], root["password"], WIFI_CONN_TYPE::PSK);
    //uint32_t result = WIFI_RESULT::CONNECTED;
    if(result == WIFI_RESULT::CONNECTED) {
        drillPort();
        json jsonObj = {
            {"result", "success"}
        };
        GVar::get()->bleSession->pushCommand(RSP_CONNECT_AP, jsonObj);

        KLOG("wifi is connected", %s);
        SD::get()->saveValue("ssid", root["ssid"].get<std::string>());
        SD::get()->saveValue("wifi_password", root["password"].get<std::string>());
        SD::get()->saveValue("wifi_enc_type", "psk");
        //GVar::get()->eMode = INTERNET;
        printf("go internet\n");
        struct IpPing {
            std::string ip;
            int ping;
            IpPing(const std::string& _ip, int _ping) {
                ip = _ip;
                ping = _ping;
            }
            bool operator<(const IpPing& _self) {
                return ping < _self.ping;
            }
        };
        std::vector<IpPing> pings;
        std::vector<std::string> servers = {"google.com", "youtube.com", "kldp.org"};
        for(auto addr : servers) {
            pings.push_back(IpPing(addr, pinger::ping(addr, 3)));
        }
        std::sort(pings.begin(), pings.end());
        for(auto const& ips : pings) {
            CONSOLE_INFO("{}: {}", ips.ip, ips.ping);
        }
        if(pings[0].ping > 10000) {
            json jsonObj = {
                            {"result", "InvalidInternet"}
            };
            GVar::get()->bleSession->pushCommand(RSP_CONNECT_AP, jsonObj);
        }
        else {
        }
    }
    else {
        json jsonObj = {
            {"result", "InvalidAPPasswordError"}
        };
        GVar::get()->bleSession->pushCommand(RSP_CONNECT_AP, jsonObj);
        CONSOLE_CRITICAL("wifi is failed ");
        //led2Slave(1, 0, 0, 0xff);
    }
    return 0;
}
