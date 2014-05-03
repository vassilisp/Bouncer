#include "ping.h"

void process_ping(u_char *packet, struct ip *rcv_ip) {
  struct icmp *rcv_icmp; /* The ICMP header */
  printf("%d", 4*rcv_ip->ip_hl);
  rcv_icmp = (struct icmp*)(packet + SIZE_ETHERNET + (rcv_ip->ip_hl)*4);
  printf("\t ICMP: Type: %d %d %d\n", rcv_icmp->icmp_type,
      rcv_icmp->icmp_code, rcv_icmp->icmp_cksum);

  struct ip *send_ip = malloc(sizeof(struct ip));
  struct icmp *send_icmp = malloc(sizeof(struct icmp)); /* The ICMP header */
  memcpy(send_icmp, rcv_icmp, sizeof(struct icmp));
  memcpy(send_ip, rcv_ip, sizeof(struct ip));

  printf("\t ICMP: Type: %d %d %d\n", send_icmp->icmp_type,
      send_icmp->icmp_code, send_icmp->icmp_cksum);

  struct in_addr ip_src, ip_dst;
  ip_src.s_addr = (uint32_t) inet_addr(arg_lip);
  ip_dst.s_addr = (uint32_t) inet_addr(arg_sip);

  //FIX CHECKSUM HERE
  send_icmp->icmp_cksum = 0;
  send_icmp->icmp_cksum = checksum((void *) send_icmp, sizeof(struct icmp));

  size_t size = sizeof(struct ip) + sizeof(struct icmp);
  char *buffer = malloc(size);
  memcpy(buffer, send_ip, sizeof(struct ip));
  memcpy(buffer + sizeof(struct ip), send_icmp, sizeof(struct icmp));

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = ip_dst.s_addr;
  printf("size of buffer: %ld %ld", sizeof(buffer), size);

  int s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
  sendto(s, send_icmp, sizeof(struct icmp), 0, (struct sockaddr *) &sin, sizeof(sin));
}
