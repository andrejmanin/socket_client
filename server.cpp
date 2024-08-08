#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


#define DEFAULT_PORT 1650
#define BUFFER_SIZE 1024
#define CLIENT_CLOSE_CON_SYMBOL '#'


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
        if(msg[i] == CLIENT_CLOSE_CON_SYMBOL) {
            return true;
        }
    }
    return false;
}


int main() {
    int client, server;
    int opt = 1;

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if(client < 0) {
        set_color(Red);
        std::cout << "SERVER ERROR: socket creation error." << std::endl;
        set_color(Magenta);
        std::cout << "=> Check port number." << std::endl;
        reset_color();
        exit(EXIT_FAILURE);
    }

    if (setsockopt(client, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    set_color(Yellow);
    std::cout << "SERVER: Socket for server was successfully created." << std::endl;
    reset_color();

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    if(bind(client, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0) {
        set_color(Red);
        std::cout << "SERVER ERROR: bind failed." << std::endl;
        reset_color();
        exit(EXIT_FAILURE);
    }

    socklen_t size = sizeof(server_address);
    set_color(Yellow);
    std::cout << "SERVER: Listening clients..." << std::endl;
    reset_color();

    listen(client, 1);
    server = accept(client, reinterpret_cast<sockaddr*>(&server_address), &size);

    if(server < 0) {
        set_color(Red);
        std::cout << "SERVER: Can't accepting client." << std::endl;
        reset_color();
    }

    char buffer[BUFFER_SIZE];
    bool isExit = false;

    while (server > 0) {
        strcpy(buffer, "=> Server connected!\n");
        send(server, buffer, BUFFER_SIZE, 0);

        set_color(Yellow);
        std::cout << "=> Connected to the client #1!" << std::endl;
        reset_color();
        set_color(Green);
        std::cout << "=> Enter '" << CLIENT_CLOSE_CON_SYMBOL << "' to close connection. \n" << std::endl;
        reset_color();

        set_color(Blue);
        std::cout << "Client: ";
        recv(server, buffer, BUFFER_SIZE, 0);
        std::cout << buffer << std::endl;
        reset_color();


        if(close_conn(buffer)) {
            isExit = true;
        }

        while(!isExit) {
            set_color(Yellow);
            std::cout << "SERVER: ";
            reset_color();
            std::cin.getline(buffer, BUFFER_SIZE);
            send(server, buffer, BUFFER_SIZE, 0);

            if(close_conn(buffer)) {
                break;
            }

            set_color(Blue);
            std::cout << "\nClient: ";
            recv(server, buffer, BUFFER_SIZE, 0);
            std::cout << buffer << std::endl;
            reset_color();

            if(close_conn(buffer)) {
                break;
            }
        }

        set_color(Yellow);
        std::cout << "\n Closed..." << std::endl;
        reset_color();
        isExit = false;
        close(client);
        exit(0);
    }
}