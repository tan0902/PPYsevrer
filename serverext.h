#include<iostream>
#include<sstream>
#include<vector>
#include<string>
#include<string.h>
#include<WS2tcpip.h>
#include<time.h>
#include<windows.h>
#define RECV_MAX 2048
#define SEND_MAX 1024
#define HOST_VER "PPY SERVER v0.0.1-Beta"
class program {
    public:
        SOCKET sock;
        void GetTime();
        void GetHostInfo();
        void SocketShutdown();
        void ClearScreen();
};