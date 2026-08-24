#include "network_object_gtk.h"
#include "glib-object.h"
#include "glib.h"
#include "glibconfig.h"
#include <stdio.h>

struct _NetworkObject {
    GObject parent_instance;
    NetworkIO network;
    unsigned long long previous_rx_bytes;
    unsigned long long previous_tx_bytes;
    unsigned long long rx_rate;
    unsigned long long tx_rate;
    gint64 previous_time;

};

enum {
    UPDATED,
    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

G_DEFINE_FINAL_TYPE(NetworkObject, network_object, G_TYPE_OBJECT)

static void network_object_class_init(NetworkObjectClass *class){
    signals[UPDATED] = g_signal_new("updated", G_TYPE_FROM_CLASS(class),G_SIGNAL_RUN_LAST,0,NULL,NULL,NULL,G_TYPE_NONE,0);
}

static void network_object_init(NetworkObject *self){

    self->previous_rx_bytes = 0;
    self->previous_tx_bytes = 0;
    self->rx_rate=0;
    self->tx_rate=0;
    self->previous_time=0;

}

NetworkObject *network_object_new(void){
    return g_object_new(NETWORK_TYPE_OBJECT,NULL);
}

void network_object_set_network(NetworkObject *self, NetworkIO *network){

    gint64 current_time = g_get_monotonic_time();

    if(self->previous_time != 0){

        gint64 elapsed_time =
            current_time - self->previous_time;

        if(elapsed_time >= 500000){

            self->rx_rate =
                (network->rx_bytes - self->previous_rx_bytes)
                * 1000000ULL
                / elapsed_time;

            self->tx_rate =
                (network->tx_bytes - self->previous_tx_bytes)
                * 1000000ULL
                / elapsed_time;

            self->previous_rx_bytes = network->rx_bytes;
            self->previous_tx_bytes = network->tx_bytes;
            self->previous_time = current_time;
        }

    } else {

        self->previous_rx_bytes = network->rx_bytes;
        self->previous_tx_bytes = network->tx_bytes;
        self->previous_time = current_time;
    }

    self->network = *network;

    g_signal_emit(self,signals[UPDATED],0);
}

const NetworkIO *network_object_get_network(NetworkObject *self){

    return &self->network;

}

unsigned long long network_object_get_rx_rate(NetworkObject *self){
    
    return self->rx_rate;

}
unsigned long long network_object_get_tx_rate(NetworkObject *self){
    
    return self->tx_rate;

}