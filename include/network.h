#ifndef NETWORK_H
#define NETWORK_H

typedef struct {
    char interface0[16];
    unsigned long long rx_bytes;
    unsigned long long rx_packets;
    unsigned long long tx_bytes;
    unsigned long long tx_packets;

} NetworkIO;

int network_usage(const char *path,int max_interfaces, NetworkIO *info_N);



#endif