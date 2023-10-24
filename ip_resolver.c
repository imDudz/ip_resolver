// show IP addresses for a given host.

#include <netdb.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <stdlib.h> 
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int status = 0;
    char ip_string[INET6_ADDRSTRLEN];
    struct addrinfo hints, *result, *result_pointer;

    if (argc != 2) {
        fprintf(stderr, "[F] Usage: %s hostname\n", argv[0]);

        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], NULL, &hints, &result)) != 0) {
        fprintf(stderr, "[F] getaddrinfo: %s\n", gai_strerror(status));

        exit(EXIT_FAILURE);
    }

    printf("\n=========================================\n");
    printf("IP addresses for %s:\n\n", argv[1]);

    for (result_pointer = result; result_pointer != NULL; result_pointer = result_pointer->ai_next) {
        void *address;
        char *ip_version;

        if (result_pointer->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)result_pointer->ai_addr;
            address = &(ipv4->sin_addr);
            ip_version = "IPv4";
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)result_pointer->ai_addr;
            address = &(ipv6->sin6_addr);
            ip_version = "IPv6";
        }

        inet_ntop(result_pointer->ai_family, address, ip_string, sizeof ip_string);
        printf(" %s: %s\n", ip_version, ip_string);
    }

    printf("=========================================\n\n");

    freeaddrinfo(result);

    return EXIT_SUCCESS;
}
