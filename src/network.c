#include "network.h"
#include <stdio.h>


int network_usage(const char *path,int max_interfaces,NetworkIO *info_N){
    FILE *net = fopen(path,"r");
    if(!net){
        printf("Error reading path\n");
        return -1;
    }

    unsigned long long ttrx[4];            //Temp for "t"

    char store_net[128];
    char interface1[16];

    int count = 0;

    while(fgets(store_net,sizeof(store_net),net)){

        if(sscanf(store_net,
                  "%15[^:]: %llu %llu %*u %*u %*u %*u %*u %*u %llu %llu",interface1,&ttrx[0],&ttrx[1],&ttrx[2],&ttrx[3]) == 5){


            if(count >= max_interfaces){
                break;
            }

            snprintf(info_N[count].interface0,
                     sizeof(info_N[count].interface0),
                     "%s",
                     interface1);

            info_N[count].rx_bytes = ttrx[0];
            info_N[count].rx_packets = ttrx[1];
            info_N[count].tx_bytes = ttrx[2];
            info_N[count].tx_packets = ttrx[3];

            count++;
        }
    }

    fclose(net);
    return count;
}
