#ifndef SERVER_H_
#define SERVER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

int cheetah(const char *port, const char *ip);
void printGetAddrInfoResults(struct addrinfo *results);

#endif