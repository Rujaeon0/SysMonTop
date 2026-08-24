#ifndef NETWORK_OBJECT_GTK_H
#define NETWORK_OBJECT_GTK_H

#include <glib-object.h>
#include "network.h"

G_BEGIN_DECLS

#define NETWORK_TYPE_OBJECT (network_object_get_type())

G_DECLARE_FINAL_TYPE(NetworkObject,network_object,NETWORK,OBJECT,GObject)

const NetworkIO *network_object_get_network(NetworkObject *self);

NetworkObject *network_object_new(void);
void network_object_set_network(NetworkObject *self, NetworkIO *network);

unsigned long long network_object_get_rx_rate(NetworkObject *self);
unsigned long long network_object_get_tx_rate(NetworkObject *self);



G_END_DECLS

#endif