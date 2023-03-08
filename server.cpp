/*
    PPY server
    2023/03/05
    Papaya Wang
*/

#include"serverext.h"
#include<stdio.h>
#pragma comment(lib, "ws2_32.lib")
#define SPORT 6969
program sys;
const char welcomeMsg[SEND_MAX] = "Welcome To PPY Server!(v0.0.1-Beta)\r\n",
passwdMsg[SEND_MAX] = "Please enter Password: ",
passwdErrMsg[SEND_MAX] = "Wrong password! Please try again.\r\n";
const bool needPasswd = 0;
int TcpInit() { //initialize WSA
    WSADATA wsaData;
    WORD wsVer = MAKEWORD(2, 2);
    int wsChk = WSAStartup(wsVer, &wsaData);
    if(wsChk!=0) {
        return wsChk;
    } else {
        return 0;
    }
}
int TcpMain(SOCKET sock) {
    char passwd_in[RECV_MAX] = "", passwd[SEND_MAX] = "39831";
    bool connectSuccess = 0;
    while(needPasswd) {
        if(strcmp(passwd_in, "\r\n")) {
            send(sock, passwdMsg, SEND_MAX, 0);
        }
        memset(passwd_in, 0, RECV_MAX);
        int passwdLen = recv(sock, passwd_in, RECV_MAX, 0);
        if(!passwdLen) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        if(strcmp(passwd_in, "\r\n")) {
            if(!strcmp(passwd_in, passwd)) {
                connectSuccess = 1;
                send(sock, welcomeMsg, SEND_MAX, 0);
                std::cout << "Client sent a correct password" << std::endl;
                break; 
            } else {
                send(sock, passwdErrMsg, SEND_MAX, 0);
                std::cout << "Client sent a wrong password: " << passwd_in << std::endl;
            }
        } 
    }
    if(!needPasswd) {
        send(sock, welcomeMsg, SEND_MAX, 0);
        connectSuccess = 1;
    }
    sys.sock = sock;
    while(connectSuccess) {
        char recvData[RECV_MAX];
        memset(&recvData, 0, RECV_MAX);
        int recvLen = recv(sock, recvData, RECV_MAX, 0);
        if(recvLen == 0) {
            std::cout << "Client disconnected" << std::endl;
            connectSuccess = 0;
        } else if(recvData[0] == '!') {
            std::stringstream cstream;
            std::string args[5], cmd;
            cstream << recvData + 1;
            cstream >> cmd >> args[0] >> args[1] >> args[2] >> args[3] >> args[4];
            std::cout << "Client sent a command (command: " << cmd <<", arg1: " << args[0] << ", arg2: " << args[1] <<", arg3: " << args[2] << ", arg4: " << args[3] << ", arg5: " << args[4] << ")" << std::endl;
            if(cmd == "exit") {
                sys.SocketShutdown();
                connectSuccess = 0;
            } else if(cmd == "clear") {
                sys.ClearScreen();
            } else if(cmd == "sys") {
                std::string syscmd = "start " + args[0].append(" "+args[1].append(" "+args[2].append(" "+args[3].append(" "+args[4]))));
                system(syscmd.c_str());
            } else if(cmd == "time") {
                char t[SEND_MAX];
                memset(t, 0, sizeof(t));
                strcpy(t, sys.GetTime().c_str());
                send(sock, t, SEND_MAX, 0);
            } else if(cmd == "info"||cmd == "ver") {
                char msg[SEND_MAX];
                memset(msg, 0, sizeof(msg));
                strcpy(msg, sys.GetHostInfo().c_str());
                send(sock, msg, SEND_MAX, 0);
            } else {
                char msg[SEND_MAX] = "\033[31mUnknown command!\033[39m\a\r\n";
                std::cout << "ErrCommand: " << recvData + 1 << std::endl;
                send(sock, msg, SEND_MAX, 0);
            }
        } else {
            if(strcmp(recvData,"\r\n")) {
                recvData[strlen(recvData)] = '\r';
                recvData[strlen(recvData)] = '\n';
                send(sock, recvData, RECV_MAX, 0);
            }
        }
    }
    return WSAGetLastError();
}
int main() {
    if(TcpInit() != 0) {
        std::cout << "process 1: WSA Initializing Failed" << std::endl;
       return -1; 
    }
    SOCKET Lsocket = socket(AF_INET, SOCK_STREAM, 0), Dsocket; //setup listen socket
    if(Lsocket == INVALID_SOCKET) {
        std::cout << "process 2: Listening Socket Creating Failed" << std::endl;
        return -2;
    }
    sockaddr_in addr, CAddr;        
    memset(&addr, 0, sizeof(addr)); 
    addr.sin_family = AF_INET;      
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    addr.sin_port = htons(SPORT);
    if(bind(Lsocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cout << "process 3: Address Binding Failed" << std::endl;
        return -3;
    }
    if(listen(Lsocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "process 4: Socket Listening Failed" << std::endl;
        return -4;
    }
    std::cout << "Waiting for connection..." << std::endl;
    memset(&CAddr, 0, sizeof(CAddr));
    int addrLen = 16;
    Dsocket = accept(Lsocket, (sockaddr*)&CAddr, &addrLen);
    if(Dsocket == INVALID_SOCKET) {
        std::cout << "process 5: Accepting Client Failed" << std::endl;
        return -5;
    }
    std::cout << inet_ntoa(CAddr.sin_addr)  << " connected" << std::endl;
    TcpMain(Dsocket);
    closesocket(Lsocket);
    WSACleanup();
    return 0;
} 