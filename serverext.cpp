#include"serverext.h"
void program::GetTime() {
    time_t rtime;
    tm *tt;
    time(&rtime);
    tt = localtime(&rtime);
    char msg[SEND_MAX];
    memset(msg, 0, SEND_MAX);
    strftime(msg, sizeof(msg), "%Y %B %d %H:%M:%S (UTC+8)\r\n", tt);
    std::string s(msg);
    send(sock, msg, SEND_MAX, 0);
};
void program::SocketShutdown() {
    char msg[SEND_MAX] = "Server disconnected\r\n";
    send(sock, msg, sizeof(msg), 0);
    std::cout << "Client exited manually" << std::endl;
    SleepEx(500, 1);
    shutdown(sock, SD_BOTH);
    
};
void program::GetHostInfo() {
    char msg[SEND_MAX];
    memset(msg, 0, sizeof(msg));
    std::string info = "Host Version:\t_VER\r\nHost Name:\t_NAME\r\nHost ip:\t_IP\r\n";
    hostent *hostInfo;
    char hostName[256];
    in_addr ipBuf;
    memset(hostName, 0, sizeof(hostName));
    gethostname(hostName, sizeof(hostName));
    hostInfo = gethostbyname(hostName);
    memcpy(&ipBuf, hostInfo->h_addr_list[0], sizeof(ipBuf));
    info.replace(info.find("_VER"), 4, HOST_VER);
    info.replace(info.find("_NAME"), 5, hostName);
    info.replace(info.find("_IP"), 3, inet_ntoa(ipBuf));
    strcpy(msg, info.c_str());
    send(sock, msg, SEND_MAX, 0);
};
void program::ClearScreen() {
    send(sock, "\033[2J\033[H", 8, 0);
}