#include "lib.h"

char *getIPonDNS(string domain);

void getTargetIP (char *buf, string &ip, string &port, bool isLocalNet);
queue<string> msgQueue;
string inputKeyBoard();

int main() {
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    string server;
    cout << "Input server IP or domain: "; cin >> server;

    char *ip = getIPonDNS(server);



    SOCKET clisock = socket(AF_INET, SOCK_DGRAM, 0);
    if (clisock == INVALID_SOCKET) {
        cout << "socket() error" << endl;
        return 0;
    }

    SOCKADDR_IN servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ip);
    servAddr.sin_port = htons(3478);

    if (connect(clisock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        cout << "connect() error" << endl;
        closesocket(clisock);
        return 0;
    }



    SOCKADDR_IN localAddr;
    int addrSize = sizeof(localAddr);
    if (getsockname(clisock, (SOCKADDR*)&localAddr, &addrSize) == SOCKET_ERROR) {
        cout << "getsockname() error" << endl;
        closesocket(clisock);
        return 0;
    }

    char buf[1024];
    sprintf(buf, "%s:%d", inet_ntoa(localAddr.sin_addr), ntohs(localAddr.sin_port));



    int sendlen = sendto(clisock, buf, strlen(buf) + 1, 0, (SOCKADDR *)&servAddr, sizeof(servAddr));
    if (sendlen == SOCKET_ERROR) {
        cout << "sendto() error" << endl;
        return 0;
    }

    int recvlen = recvfrom(clisock, buf, sizeof(buf), 0, NULL, NULL);
    if (recvlen == SOCKET_ERROR) {
        cout << "recvfrom() error" << endl;
        return 0;
    }

    cout << "STUN: " << buf << endl;



    bool isLocalNet = true; // 로컬 넷(같은 NAT)에서 홀펀칭을 하려면 true로 변경
    string destIP, destPort;
    getTargetIP(buf, destIP, destPort, isLocalNet);

    cout << "Destination IP: " << destIP << ":" << destPort << endl;

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(destIP.c_str());
    servAddr.sin_port = htons(atoi(destPort.c_str()));

    if (connect(clisock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        cout << "connect() error" << endl;
        closesocket(clisock);
        return 0;
    }



    while (true) {
        string message = "Heart Beat";
        message = inputKeyBoard();

        int sendlen = sendto(clisock, message.c_str(), message.length() + 1, 0, (sockaddr*)&servAddr, sizeof(servAddr));
        if (sendlen == SOCKET_ERROR) {
            cout << "sendto() error" << endl;
            return 0;
        }

        int recvlen = recvfrom(clisock, buf, sizeof(buf), 0, NULL, NULL);
        if (recvlen == SOCKET_ERROR) {
            cout << "recvfrom() error" << endl;
            return 0;
        }

        cout << "상대방 : " << buf << endl;

        this_thread::sleep_for(100ms);
    }



    closesocket(clisock);
    WSACleanup();
    return 0;
}

char *getIPonDNS(string domain) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    struct hostent *host;
    char *ip = new char[16];
    host = gethostbyname(domain.c_str());
    if (!host) {
        cout << "gethostbyname() error" << endl;
        return nullptr;
    }
    strcpy(ip, inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));
    WSACleanup();
    return ip;
}

void getTargetIP(char *buf, string &ip, string &port, bool isLocalNet) {
    istringstream iss(buf);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    // [0]: 자신의 pub IP:Port, [1]: 상대의 pub IP:Port,
    // [2]: 자신의 priv IP:Port, [3]: 상대의 priv IP:Port

    if (isLocalNet) { // 내부망(같은 NAT)일 경우
        ip = tokens[3].substr(0, tokens[3].find(':'));
        port = tokens[3].substr(tokens[3].find(':') + 1);
        return;
    }

    ip = tokens[1].substr(0, tokens[1].find(':'));
    port = tokens[1].substr(tokens[1].find(':') + 1);
}

string inputKeyBoard() {
    string returnData = "";
    if(GetAsyncKeyState(0x41) & 0x8000) {
            msgQueue.push("a");
        } else if(GetAsyncKeyState(0x42) & 0x8000) {
            msgQueue.push("b");
        } else if(GetAsyncKeyState(0x43) & 0x8000) {
            msgQueue.push("c");
        } else if(GetAsyncKeyState(0x44) & 0x8000) {
            msgQueue.push("d");
        } else if(GetAsyncKeyState(0x45) & 0x8000) {
            msgQueue.push("e");
        } else if(GetAsyncKeyState(0x46) & 0x8000) {
            msgQueue.push("f");
        } else if(GetAsyncKeyState(0x47) & 0x8000) {
            msgQueue.push("g");
        } else if(GetAsyncKeyState(0x48) & 0x8000) {
            msgQueue.push("h");
        } else if(GetAsyncKeyState(0x49) & 0x8000) {
            msgQueue.push("i");
        } else if(GetAsyncKeyState(0x4A) & 0x8000) {
            msgQueue.push("j");
        } else if(GetAsyncKeyState(0x4B) & 0x8000) {
            msgQueue.push("k");
        } else if(GetAsyncKeyState(0x4C) & 0x8000) {
            msgQueue.push("l");
        } else if(GetAsyncKeyState(0x4D) & 0x8000) {
            msgQueue.push("m");
        } else if(GetAsyncKeyState(0x4E) & 0x8000) {
            msgQueue.push("n");
        } else if(GetAsyncKeyState(0x4F) & 0x8000) {
            msgQueue.push("o");
        } else if(GetAsyncKeyState(0x50) & 0x8000) {
            msgQueue.push("p");
        } else if(GetAsyncKeyState(0x51) & 0x8000) {
            msgQueue.push("q");
        } else if(GetAsyncKeyState(0x52) & 0x8000) {
            msgQueue.push("r");
        } else if(GetAsyncKeyState(0x53) & 0x8000) {
            msgQueue.push("s");
        } else if(GetAsyncKeyState(0x54) & 0x8000) {
            msgQueue.push("t");
        } else if(GetAsyncKeyState(0x55) & 0x8000) {
            msgQueue.push("u");
        } else if(GetAsyncKeyState(0x56) & 0x8000) {
            msgQueue.push("v");
        } else if(GetAsyncKeyState(0x57) & 0x8000) {
            msgQueue.push("w");
        } else if(GetAsyncKeyState(0x58) & 0x8000) {
            msgQueue.push("x");
        } else if(GetAsyncKeyState(0x59) & 0x8000) {
            msgQueue.push("y");
        } else if(GetAsyncKeyState(0x5A) & 0x8000) {
            msgQueue.push("z");
        } else if(GetAsyncKeyState(0x20) & 0x8000) {
            msgQueue.push(" ");
        } else if(GetAsyncKeyState(VK_RETURN) & 0x8000) {
            while(!msgQueue.empty()) {
                returnData += msgQueue.front();
                msgQueue.pop();
            }
        }

        return returnData;
}