#ifndef PING_LIST_H
#define PING_LIST_H

#include "globals.h"

struct ping_struct {
  struct ip ip;
  struct icmp icmp;
  struct ping_struct *next;
};

struct ping_struct* ping_create_list(const struct ip ip, const struct icmp icmp);
struct ping_struct* ping_add_to_list(const struct ip ip, const struct icmp icmp);
struct ping_struct* ping_search_in_list(const struct ip ip, const struct icmp icmp,
    struct ping_struct **prev);
void ping_delete_from_list(const struct ip ip, const struct icmp icmp);
void ping_print_list(void);

#endif
