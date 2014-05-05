#include "tcp.h"

void send_tcp(struct in_addr receiver, struct ip send_ip, struct tcphdr tcp,
    char *rest_data, int rest_data_len, int port) {

  //print_list();
  // The IP header
  send_ip.ip_src = bouncer_ip;
  send_ip.ip_dst = receiver;
  send_ip.ip_sum = 0;
  send_ip.ip_sum = ip_checksum((void *) &send_ip, sizeof(struct ip));

  //the TCP header
  tcp.th_sport = htons((u_short) port);
  printf("port ----> %d\n", tcp.th_sport);
  tcp.th_sum = 0;

  //pseudoheader
  struct tcp_pseudo pseudo;

  pseudo.src_addr = bouncer_ip.s_addr;
  pseudo.dst_addr = receiver.s_addr;
  pseudo.zero = 0;
  pseudo.proto = IPPROTO_TCP;
  int pseudo_length = sizeof(struct tcphdr) + rest_data_len;
  pseudo.length = htons(pseudo_length);

  size_t size = sizeof(struct tcp_pseudo) + pseudo_length;
  char *buffer = malloc(size);
  memcpy(buffer, &pseudo, sizeof(struct tcp_pseudo));
  memcpy(buffer + sizeof(struct tcp_pseudo), &tcp, sizeof(struct tcphdr));
  memcpy(buffer + sizeof(struct tcp_pseudo) + sizeof(struct tcphdr), rest_data, rest_data_len);


  tcp.th_sum = checksum(buffer, size);

  printf("Hello world\n\n\n");
  size_t final_size = sizeof(struct ip) + sizeof(struct tcphdr) + rest_data_len;
  char *final_buffer = malloc(final_size);
  memcpy(final_buffer, &send_ip, sizeof(struct ip));
  memcpy(final_buffer + sizeof(struct ip), &tcp, sizeof(struct tcphdr));
  memcpy(final_buffer + sizeof(struct tcphdr) + sizeof(struct ip), rest_data, rest_data_len);

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr = receiver;

  // Submit request for a raw socket descriptor.
  int sd;
  if ((sd = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
    perror ("socket() failed ");
    exit (EXIT_FAILURE);
  }

  // Set flag so socket expects us to provide IPv4 header.
  int on = 1;
  if (setsockopt (sd, IPPROTO_IP, IP_HDRINCL, &on, sizeof (on)) < 0) {
    perror ("setsockopt() failed to set IP_HDRINCL ");
    exit (EXIT_FAILURE);
  }

  // Bind socket to interface index.
  struct ifreq ifr;
  char iname[16] = "eth0";

  memcpy(&ifr, &iname , sizeof(iname));
  if (setsockopt (sd, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof (ifr)) < 0) {
    perror ("setsockopt() failed to bind to interface ");
    exit (EXIT_FAILURE);
  }

  // Send packet.
  if (sendto (sd, final_buffer, final_size, 0, (struct sockaddr *) &sin, sizeof (sin)) < 0)  {
    perror ("sendto() failed ");
    exit (EXIT_FAILURE);
  }

  // Close socket descriptor.
  close (sd);

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

  size_t rest_data_len = len - SIZE_ETHERNET - sizeof(struct ip) - sizeof(struct tcphdr);
  char *rest_data = malloc(sizeof(rest_data_len));
  rest_data = packet + SIZE_ETHERNET + (rcv_ip->ip_hl)*4 + sizeof(struct tcphdr);

  printf("\t TCP Options: %d %d %d %d\n", ntohs(tcp->th_sport), ntohs(tcp->th_dport),
    tcp->th_seq, tcp->th_ack);

  char flags = tcp->th_flags;
  char syn_on = flags & TH_SYN;
  char ack_on = flags & TH_ACK;
  char synack_on = flags & (TH_SYN | TH_ACK);
  printf("Flag SYN: %04x", syn_on);
  printf("Flag SYN: %04x", ack_on);

  if(ack_on) {
      printf("STATE: ACK ON\n");

  }
  else {
    printf("STATE: ACK OFF\n");
    if(syn_on) {
      printf("STATE: ACK OFF, SYN ON\n");
      if (STARTING_PORT > 50000) {
       // exit(0);
      }
      add_to_list(rcv_ip, tcp, STARTING_PORT);
      send_tcp(server_ip, *rcv_ip, *tcp, rest_data, rest_data_len, STARTING_PORT);
      STARTING_PORT++;


    }
    else {
      printf("STATE: ACK OFF, SYN OFF\n");
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
