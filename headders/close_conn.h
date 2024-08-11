#ifndef CLOSE_CONN_H
#define CLOSE_CONN_H

#include <iostream>
#include <unistd.h>

#define CLIENT_CLOSE_CON_SYMBOL '#'

inline bool close_conn_sym(const char* msg) {
    for(int i = 0; i < strlen(msg); i++) {
        if(msg[i] == CLIENT_CLOSE_CON_SYMBOL) {
            return true;
        }
    }
    return false;
}


inline void close_conn(const int* client) {
    set_color(Yellow);
    std::cout << "\n Closed..." << std::endl;
    reset_color();
    close(*client);
}

#endif //CLOSE_CONN_H
