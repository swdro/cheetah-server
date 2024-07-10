#include "server.h"


int cheetah(const char *port, const char *ip) {
    // 
    int addr_info_status;
    struct addrinfo hints;
    struct addrinfo *get_addr_info_results;
    int socket_fd;

    memset(&hints, 0, sizeof(hints)); // make sure struct is empty
    hints.ai_family = AF_UNSPEC; // accept IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in IP as localhost for me

    if ((addr_info_status = getaddrinfo(ip, port, &hints, &get_addr_info_results)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(addr_info_status));
        exit(1);
    }

    printGetAddrInfoResults(get_addr_info_results);

    // get socket file descriptor
    socket_fd = socket(get_addr_info_results->ai_family, get_addr_info_results->ai_socktype, get_addr_info_results->ai_protocol);
    if (socket_fd == -1) {
        fprintf(stderr, "socket assignment error: %s\n", strerror(errno));
        exit(1);
    }

    // bind socket to port
    int bind_status = 0;
    bind_status = bind(socket_fd, get_addr_info_results->ai_addr, get_addr_info_results->ai_addrlen);
    if (bind_status == -1) {
        fprintf(stderr, "error on binding socket to port: %s\n", strerror(errno));
        exit(1);
    }

    // listen for incoming connections on socket
    int listen_status = 0;
    int BACKLOG = 10; // adjust backlog parameter (number of connections allowed on incoming queue)
    listen_status = listen(socket_fd, BACKLOG);
    if (listen_status != 0) {
        fprintf(stderr, "error occurred while listening for incoming connections: %s\n", strerror(errno));
        exit(1);
    }
    printf("Cheetah server listening on port %s\n", port);

    // accept incoming connection
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(struct sockaddr_storage);
    int client_fd = 0;
    client_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &client_addr_size);
    if (client_fd <= -1) {
        fprintf(stderr, "error occurred while accepting incoming connection: %s\n", strerror(errno));
        exit(1);
    }

    // send response
    /*
    send() returns the number of bytes actually sent out—this might 
    be less than the number you told it to send! See, sometimes you 
    tell it to send a whole gob of data and it just can’t handle it. 
    It’ll fire off as much of the data as it can, and trust you to 
    send the rest later. Remember, if the value returned by send() 
    doesn’t match the value in len, it’s up to you to send the rest 
    of the string. The good news is this: if the packet is small 
    (less than 1K or so) it will probably manage to send the whole 
    thing all in one go. Again, -1 is returned on error, and errno 
    is set to the error number.
    */
    int bytes_sent = 0;
    bytes_sent = send(client_fd, "Hello, World!\n", 14, 0); // look into flags for send()
    if (bytes_sent == -1) {
        fprintf(stderr, "error occurred while sending response: %s\n", strerror(errno));
        exit(1);
    }

    // receive response 
    size_t resposne_msg_length = sizeof(char) * 128; // adjust to allow actual responses
    char *response_msg = (char *) malloc(resposne_msg_length);
    int response_bytes_read;
    /*
    recv() can return 0. This can mean only one thing: the remote 
    side has closed the connection on you! A return value of 0 is 
    recv()’s way of letting you know this has occurred.
    */
    response_bytes_read = recv(client_fd, (char *) response_msg, resposne_msg_length, 0);
    if (response_bytes_read == -1) {
        fprintf(stderr, "error occurred while receiving request: %s\n", strerror(errno));
        exit(1);
    } else if (response_bytes_read == 0) {
        printf("connection closed\n");
        fprintf(stderr, "connection closed\n");
        exit(1);
    } else {
        printf("received: \n\n%s\n\n", response_msg);
    }

    freeaddrinfo(get_addr_info_results); // free server info linked list
    return 0;
}

void printGetAddrInfoResults(struct addrinfo *results) {
    char ipstr[INET6_ADDRSTRLEN];
    struct addrinfo *res; 

    for(res = results; res != NULL; res = res->ai_next) {
        void *addr;
        char *ipver;
        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (res->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr; // sockaddr_in is a structure that has stores the address and port information of the socket
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)res->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }
        // convert the IP to a string and print it:
        // printf("IP address as stored in sockaddr_in structure w/o using inet_ntop: %d\n", *(int *)addr);
        inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }
}