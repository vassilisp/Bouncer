#ifndef PING_H
#define PING_H

#include "bouncer.h"

static struct client_id{
  struct in_addr src_ip;
  struct id {
    u_int16_t id;
    u_int16_t sequence;
  }s_id;
}saved_clients[10];

void process_ping(u_char *packet, struct ip *rcv_ip);

#endif
