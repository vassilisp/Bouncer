#ifndef PING_LIST_H
#define PING_LIST_H

#include <stdbool.h>
#include "globals.h"
#include "bouncer.h"

struct ping_struct {
  struct ip ip;
  struct icmp icmp;
  struct ping_struct *next;
};

struct ping_struct* create_list(struct ip ip, struct icmp icmp);
struct ping_struct* add_to_list(struct ip ip, struct icmp icmp);
struct ping_struct* search_in_list(struct ip ip, struct icmp icmp,
    struct ping_struct **prev);
void delete_from_list(struct ip ip, struct icmp icmp);
void print_list(void);

#endif
