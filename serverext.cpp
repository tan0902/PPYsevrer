#include"serverext.h"
std::string program::GetTime() {
    time_t rtime;
    tm *tt;
    time(&rtime);
    tt = localtime(&rtime);
    char msg[SEND_MAX];
    memset(msg, 0, SEND_MAX);
    strftime(msg, sizeof(msg), "%Y %B %d %H:%M:%S (UTC+8)\r\n", tt);
    std::string s(msg);
    return s;
};
void program::SocketShutdown() {
    char msg[SEND_MAX] = "Server disconnected\r\n";
    send(sock, msg, sizeof(msg), 0);
    std::cout << "Client exited manually" << std::endl;
    SleepEx(500, 1);
    shutdown(sock, SD_BOTH);
    
};
std::string program::GetHostInfo() {
    std::string info;
    hostent *hostInfo;
    char hostName[256];
    in_addr ipBuf;
    memset(hostName, 0, sizeof(hostName));
    gethostname(hostName, sizeof(hostName));
    hostInfo = gethostbyname(hostName);
    memcpy(&ipBuf, hostInfo->h_addr_list[0], sizeof(ipBuf));
    info.append("Host version:\t");
    info.append(HOST_VER);
    info.append("\r\n");
    info.append("Host name:\t");
    info.append(hostInfo->h_name);
    info.append("\r\n");
    info.append("Host ip:\t");
    info.append(inet_ntoa(ipBuf));
    info.append("\r\n");
    return info;
}