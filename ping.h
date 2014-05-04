#ifndef PING_H
#define PING_H

#include "bouncer.h"
#include "ping_list.h"

static struct client_id{
  struct in_addr src_ip;
  struct id {
    u_int16_t id;
    u_int16_t sequence;
  }s_id;
}saved_clients[10];

void process_ping(u_char *packet, struct ip *rcv_ip);

const int on = 1;

struct PingClients{
  struct icmp icmp;
  struct ip ip;
};
struct PingClients *p_clients;
int p_clients_num = 0;
#endif
