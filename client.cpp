#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


#define SERVER_CLOSE_CON_SYMBOL '#'
#define DEFAULT_PORT 1650
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"


enum Colors {
    Red = 31,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan
};


void set_color(int color) {
    std::cout << "\033[" << color << "m";
}


void reset_color() { std::cout << "\033[0m"; }


bool close_conn(const char* msg) {
    for(int i = 0; i < strlen(msg); i++) {
        if(msg[i] == SERVER_CLOSE_CON_SYMBOL) {
            return true;
        }
    }
    return false;
}


int main() {
    int client;
    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0) {
        set_color(Red);
        std::cout << "CLIENT ERROR: establishing socket error." << std::endl;
        reset_color();
        exit(EXIT_FAILURE);
    }


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

    char buff[BUFFER_SIZE];
    set_color(Cyan);
    std::cout << "Waiting for server confirmation..." << std::endl;
    recv(client, buff, BUFFER_SIZE, 0);
    set_color(Magenta);
    std::cout << "=> Connection established." << std::endl;
    set_color(Green);
    std::cout << "Enter '" << SERVER_CLOSE_CON_SYMBOL << "' for close connection." << std::endl;
    reset_color();


    while(true) {
        set_color(Cyan);
        std::cout << "CLIENT: ";
        reset_color();
        std::cin.getline(buff, BUFFER_SIZE);
        send(client, buff, BUFFER_SIZE, 0);
        if(close_conn(buff)) {
            break;
        }

        set_color(Yellow);
        std::cout << "SERVER: ";
        recv(client, buff, BUFFER_SIZE, 0);
        std::cout << buff << std::endl;
        reset_color();

        if(close_conn(buff)) {
            break;
        }
        std::cout << std::endl;
    }

    close(client);
    set_color(Yellow);
    std::cout << "\n Closed..." << std::endl;
    reset_color();

    return 0;
}