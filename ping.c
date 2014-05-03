#include "ping.h"

void process_ping(u_char *packet, struct ip *rcv_ip) {
  int on = 1;

  printf("Received ping packet from %s\n", inet_ntoa(rcv_ip->ip_src));

  struct in_addr bouncer_ip, server_ip, client_ip;
  bouncer_ip.s_addr = (uint32_t) inet_addr(arg_lip);
  server_ip.s_addr = (uint32_t) inet_addr(arg_sip);
  client_ip = rcv_ip->ip_src;

  // The ICMP header
  struct icmp *rcv_icmp;
  rcv_icmp = (struct icmp*)(packet + SIZE_ETHERNET + (rcv_ip->ip_hl)*4);
  printf("\t ICMP: Type: %d %d %d\n", rcv_icmp->icmp_type,
    rcv_icmp->icmp_code, rcv_icmp->icmp_cksum);

  struct icmp *send_icmp = malloc(sizeof(struct icmp));
  memcpy(send_icmp, rcv_icmp, sizeof(struct icmp));

  //calculate checksum
  send_icmp->icmp_cksum = 0;
  send_icmp->icmp_cksum = checksum((void *) send_icmp, sizeof(struct icmp));

  // The IP header
  struct ip *send_ip = malloc(sizeof(struct ip));
  memcpy(send_ip, rcv_ip, sizeof(struct ip));

  send_ip->ip_src = bouncer_ip;
  send_ip->ip_dst = server_ip;
  send_ip->ip_sum = 0;
  send_ip->ip_sum = ip_checksum((void *) send_ip, sizeof(struct ip));

  size_t size = sizeof(struct ip) + sizeof(struct icmp);
  char *buffer = malloc(size);
  memcpy(buffer, send_ip, sizeof(struct ip));
  memcpy(buffer + sizeof(struct ip), send_icmp, sizeof(struct icmp));

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr = server_ip;

  int s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0 ) {
    perror("setsocopt() failed to set IP_HDRINCL");
    exit(EXIT_FAILURE);
  }

  if (sendto(s, buffer, size, 0, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
    perror("sendto() failed");
    exit(EXIT_FAILURE);
  }
}



  /*
  if(inet_ntoa(client_ip) == "192.168.185.131") {
    send_ip->ip_dst = client_ip
  }
  else {
    send_ip->ip_dst = server_ip
  }
  */


  /*
  if(inet_ntoa(client_ip) == "192.168.185.131") {
    printf("-----------------> %s\n\n", inet_ntoa(client_ip));
    server_ip.s_addr = (uint32_t) inet_addr("192.168.185.129");
    sin.sin_addr = server_ip;
  }
  else {
    printf("-----------------> %s\n\n", inet_ntoa(client_ip));
    sin.sin_addr = server_ip;
  }*/
