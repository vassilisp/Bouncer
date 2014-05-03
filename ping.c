#include "ping.h"

void process_ping(u_char *packet, struct ip *rcv_ip) {
  printf("\t|  * ICMP\n");

  struct icmp *rcv_icmp; /* The ICMP header */
  rcv_icmp = (struct icmp*)(packet + SIZE_ETHERNET + (rcv_ip->ip_hl)*4);
  printf("Type: %d %d %d", rcv_icmp->icmp_type,
      rcv_icmp->icmp_code, rcv_icmp->icmp_cksum);

  struct ip *send_ip;
  struct icmp *send_icmp; /* The ICMP header */
  char buffer[sizeof(*send_ip) + sizeof(*rcv_icmp)];
  send_ip = (struct ip *) buffer;
  send_icmp = (struct icmp *) (buffer + sizeof(struct ip));
  send_icmp = rcv_icmp;

  send_ip = rcv_ip;
  struct in_addr ip_src, ip_dst;
  ip_src.s_addr = (uint32_t) inet_addr(arg_lip);
  
  ip_dst.s_addr = (uint32_t) inet_addr(arg_sip);

  send_ip->ip_src = ip_src;
  send_ip->ip_dst = ip_dst;
  send_ip->ip_sum = 0;

  send_ip->ip_sum = ip_checksum((void *) buffer, sizeof(struct ip));
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = ip_dst.s_addr;

  int s = socket (PF_INET, SOCK_RAW, IPPROTO_ICMP);
  sendto(s, buffer, sizeof(buffer), 0, (struct sockaddr *) &sin, sizeof(sin));
  exit(0);
}
