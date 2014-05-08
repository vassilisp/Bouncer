#ifndef PING_H
#define PING_H

#include "globals.h"
#include "tcp_list.h"
#include "ftp_list.h"

/* TCP header */
struct tcp_pseudo /*the tcp pseudo header*/
{
  unsigned int src_addr;
  unsigned int dst_addr;
  unsigned char zero;
  unsigned char proto;
  unsigned short int length;
};
#endif
