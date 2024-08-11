#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../headders/colors.h"
#include "../headders/close_conn.h"



#define SERVER_CLOSE_CON_SYMBOL '#'
#define DEFAULT_PORT 5425
#define SERVER_IP "127.0.0.1"


int main() {
    int client = socket(AF_INET, SOCK_STREAM, 0);

    if(client < 0) {
        set_color(Red);
        std::cout << "CLIENT ERROR: establishing socket error." << std::endl;
        reset_color();
        exit(EXIT_FAILURE);
    }

    sockaddr_in server_address{};

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    set_color(Magenta);
    std::cout << "=> Client socket created." << std::endl;
    reset_color();

    if(connect(client, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == 0) {
        set_color(Magenta);
        std::cout << "=> Connection to the server " << inet_ntoa(server_address.sin_addr) << " with port number: " << DEFAULT_PORT << std::endl;
        reset_color();
    }

    char buff[BUFSIZ];
    set_color(Cyan);
    std::cout << "Waiting for server confirmation..." << std::endl;
    recv(client, buff, BUFSIZ, 0);
    set_color(Magenta);
    std::cout << "=> Connection established." << std::endl;
    set_color(Green);
    std::cout << "Enter '" << SERVER_CLOSE_CON_SYMBOL << "' for close connection." << std::endl;
    reset_color();


    bool isEnd = false;
    while(!isEnd) {
        set_color(Cyan);
        std::cout << "CLIENT: ";
        reset_color();
        std::cin.getline(buff, BUFSIZ);
        send(client, buff, BUFSIZ, 0);
        if(close_conn_sym(buff)) {
            isEnd = true;
        }

        set_color(Yellow);
        std::cout << "SERVER: ";
        recv(client, buff, BUFSIZ, 0);
        std::cout << buff << std::endl;
        reset_color();

        if(close_conn_sym(buff)) {
            isEnd = true;
        }
        std::cout << std::endl;
    }

    close_conn(&client);

    return 0;
}