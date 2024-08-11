#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>


#include "../headders/colors.h"
#include "../headders/close_conn.h"


#define S_PORT 5425
#define MAX_CLIENTS 5


using std::cout;
using std::endl;


void reverce(char* line) {
    int end = 0;
    while(line[end] != '\0') {
        end++;
    }

    int start = 0;
    end--; // '\0' symbol

    while(start < end) {
        char s = line[start];
        line[start] = line[end];
        line[end] = s;
        start++;
        end--;
    }
}


void clientConn(int client, int count) {
    char buffer[BUFSIZ];
    strcpy(buffer, "=> Server connected!\n");
    send(client, buffer, BUFSIZ, 0);

    set_color(Yellow);
    cout << "=> Connection with a client #" << count << " successfully created." << endl;
    reset_color();

    while(true) {
        recv(client, buffer, BUFSIZ, 0);
        if(close_conn_sym(buffer)) {
            break;
        }
        reverce(buffer);
        send(client, buffer, BUFSIZ, 0);
    }

    set_color(Yellow);
    cout << "=> Connection with client #" << count << " was closed." << endl;
    reset_color();
    close(client);

}



int main() {
    int server;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0) {
        set_color(Red);
        cout << "SERVER: Socket isn't created." << std::endl;
        reset_color();
        exit(EXIT_FAILURE);
    }

    set_color(Yellow);
    cout << "SERVER: Socket was successfully created." << endl;
    reset_color();

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(S_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(bind(server, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        set_color(Red);
        cout << "SERVER: In bind function was error." << endl;
        set_color(Cyan);
        cout << "| => Chech socket port." << endl;
        reset_color();
        exit(EXIT_FAILURE);
    }

    set_color(Yellow);
    cout << "SERVER: Socket was successfully binded." << endl;
    reset_color();

    listen(server, MAX_CLIENTS);
    socklen_t sAddrLen = sizeof(serverAddress);

    int count = 1;
    while(count <= 5) {
        set_color(Green);
        cout << "SERVER: Listening clients..." << endl;
        reset_color();

        int client = accept(server, reinterpret_cast<sockaddr*>(&serverAddress), &sAddrLen);
        if(client < 0) {
            set_color(Red);
            cout << "SERVER: Can't accept client connection." << endl;
            reset_color();
            continue;
        }
        if(client > 0) {
            std::thread conn(&clientConn, client, count);
            conn.detach();
        }
        count++;
    }
    close(server);
    return 0;
}