#include "tcp.h"

void send_tcp(struct in_addr receiver, struct ip send_ip, struct icmp icmp) {

  print_list();

  icmp.icmp_cksum = 0;
  icmp.icmp_cksum = checksum((void *) &icmp, sizeof(struct icmp));

  // The IP header
  send_ip.ip_src = bouncer_ip;
  send_ip.ip_dst = receiver;
  send_ip.ip_sum = 0;
  send_ip.ip_sum = ip_checksum((void *) &send_ip, sizeof(struct ip));

  size_t size = sizeof(struct ip) + sizeof(struct icmp);
  char *buffer = malloc(size);
  memcpy(buffer, &send_ip, sizeof(struct ip));
  memcpy(buffer + sizeof(struct ip), &icmp, sizeof(struct icmp));

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr = receiver;

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

void process_tcp(u_char *packet, struct ip *rcv_ip) {

  bouncer_ip.s_addr = (uint32_t) inet_addr(arg_lip);
  server_ip.s_addr = (uint32_t) inet_addr(arg_sip);

  printf("Received ping packet from %s\n", inet_ntoa(rcv_ip->ip_src));
  client_ip = rcv_ip->ip_src;

  // The ICMP header
  struct tcphdr *tcp;
  size_t offset = SIZE_ETHERNET + (rcv_ip->ip_hl)*4 + sizeof(struct icmp);
  tcp = (struct tcphdr*)(packet + offset);

  printf("\t TCP Options: %d %d %d %d\n", tcp->th_sport, tcp->th_dport,
    tcp->th_seq, tcp->th_ack);


/*
  char *tmp_client_addr = malloc(sizeof(char) * 16);
   memcpy(tmp_client_addr, inet_ntoa(client_ip), sizeof(char)* 16);
  char *tmp_server_addr = malloc(sizeof(char) * 16);
   memcpy(tmp_server_addr, inet_ntoa(server_ip), sizeof(char)* 16);

  if(strcmp(tmp_client_addr, tmp_server_addr) == 0) {
    struct ping_struct *ret = NULL;
    ret = search_in_list(*rcv_ip, *rcv_icmp, NULL);
    if (ret == NULL) {
      perror("could not find client");
      exit(EXIT_FAILURE);
    }
    memcpy(return_ping, ret, sizeof(struct ping_struct));
    delete_from_list(*rcv_ip, *rcv_icmp);
    send_ping(return_ping->ip.ip_src, return_ping->ip, return_ping->icmp);
  }
  else {
    add_to_list(*rcv_ip, *rcv_icmp);
    send_ping(server_ip, *rcv_ip, *rcv_icmp);
  }
  */
}