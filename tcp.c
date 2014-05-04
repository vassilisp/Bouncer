#include "tcp.h"

void send_tcp(struct in_addr receiver, struct ip send_ip, struct tcphdr tcp,
    char *data, int len, int port) {

  //print_list();

  // The IP header
  send_ip.ip_src = bouncer_ip;
  send_ip.ip_dst = receiver;
  send_ip.ip_sum = 0;
  send_ip.ip_sum = ip_checksum((void *) &send_ip, sizeof(struct ip));

  //the TCP header
  tcp.th_sport = port;
  tcp.th_sum = 0;

  //pseudoheader
  struct tcp_pseudo pseudo;

  pseudo.src_addr = bouncer_ip.s_addr;
  pseudo.dst_addr = receiver.s_addr;
  pseudo.zero = 0;
  pseudo.proto = IPPROTO_TCP;
  pseudo.length = sizeof(tcp) + len;

  size_t size = sizeof(struct tcp_pseudo) + pseudo.length;
  char *buffer = malloc(size);
  memcpy(buffer, &pseudo, sizeof(pseudo));
  memcpy(buffer + sizeof(pseudo), &tcp, sizeof(tcp));
  memcpy(buffer + sizeof(pseudo) + sizeof(tcp), data, len);

  tcp.th_sum = checksum(data, size);

  size_t final_size = sizeof(tcp) + len;
  char *final_buffer = malloc(final_size);
  memcpy(final_buffer, &tcp, sizeof(tcp));
  memcpy(final_buffer + sizeof(tcp), data, len);

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr = receiver;

  int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0 ) {
    perror("setsocopt() failed to set IP_HDRINCL");
    exit(EXIT_FAILURE);
  }

  if (sendto(s, final_buffer, final_size, 0, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
    perror("sendto() failed");
    exit(EXIT_FAILURE);
  }

}

void process_tcp(u_char *packet, struct ip *rcv_ip, int len) {

  bouncer_ip.s_addr = (uint32_t) inet_addr(arg_lip);
  server_ip.s_addr = (uint32_t) inet_addr(arg_sip);

  printf("Received ping packet from %s\n", inet_ntoa(rcv_ip->ip_src));
  client_ip = rcv_ip->ip_src;

  // The tcp header
  struct tcphdr *tcp;
  size_t offset = SIZE_ETHERNET + (rcv_ip->ip_hl)*4;
  tcp = (struct tcphdr*)(packet + offset);

  size_t rest_len = len - SIZE_ETHERNET - sizeof(struct ip) - sizeof(struct tcphdr);
  char *rest_data = malloc(sizeof(rest_len));
  rest_data = packet + SIZE_ETHERNET + (rcv_ip->ip_hl)*4 + sizeof(struct tcphdr);

  printf("\t TCP Options: %d %d %d %d\n", ntohs(tcp->th_sport), ntohs(tcp->th_dport),
    tcp->th_seq, tcp->th_ack);

  char flags = tcp->th_flags;
  char syn_on = flags && TH_SYN;
  char ack_on = flags && TH_ACK;
  char synack_on = flags && (TH_SYN || TH_ACK);
  printf("Flag SYN: %04x", syn_on);

  if(ack_on) {



  }
  else {
    if(syn_on) {
      add_to_list(rcv_ip, tcp, ++STARTING_PORT);
      send_tcp(server_ip, *rcv_ip, *tcp, rest_data, rest_len, STARTING_PORT);


    }

  }


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
