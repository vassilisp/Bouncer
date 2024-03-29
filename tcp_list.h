#ifndef TCP_LIST_H
#define TCP_LIST_H

#include "globals.h"

struct tcp_struct {
  struct ip ip;
  struct tcphdr tcp;
  int bouncing_port;
  struct tcp_struct *next;
};

struct tcp_struct* create_list(const struct ip ip, const struct tcphdr tcp, int port);
struct tcp_struct* add_to_list(const struct ip ip, const struct tcphdr tcp, int port);
struct tcp_struct* search_in_list(const struct ip ip, const struct tcphdr tcp, int port,
    struct tcp_struct **prev);
struct tcp_struct* search_in_list_by_ip(const struct ip ip, const struct tcphdr tcp,
    struct in_addr ip_src, struct tcp_struct **prev);
void delete_from_list(const struct ip ip, const struct tcphdr tcp, int port);
void print_list(void);

#endif
