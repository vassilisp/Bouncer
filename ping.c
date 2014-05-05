#include "ping.h"

void send_ping(struct in_addr receiver, struct ip send_ip, struct icmp icmp,
    char *data, int len) {

//  ping_print_list();
  size_t rest_len = len - SIZE_ETHERNET - sizeof(struct ip) - sizeof(struct icmp);

  // The IP header
  send_ip.ip_src = bouncer_ip;
  send_ip.ip_dst = receiver;
  send_ip.ip_sum = 0;
  send_ip.ip_sum = ip_checksum((void *) &send_ip, sizeof(struct ip));

  size_t size = sizeof(struct ip) + sizeof(struct icmp) + rest_len;
  char *buffer = malloc(size);
  memcpy(buffer, &send_ip, sizeof(struct ip));
  memcpy(buffer + sizeof(struct ip), &icmp, sizeof(struct icmp));
  memcpy(buffer + sizeof(struct ip) + sizeof(struct icmp), data, rest_len);

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr = receiver;

  printf("icmp type: %d\n", icmp.icmp_type);

  // Submit request for a raw socket descriptor.
  int sd;
  if ((sd = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
    perror ("socket() failed ");
    exit (EXIT_FAILURE);
  }
/*
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
*/
  // Send packet.
  if (sendto (sd, buffer, size, 0, (struct sockaddr *) &sin, sizeof (sin)) < 0)  {
    perror ("sendto() failed ");
    exit (EXIT_FAILURE);
  }

  // Close socket descriptor.
  close (sd);

}

void process_ping(u_char *packet, struct ip *rcv_ip, int len) {

  bouncer_ip.s_addr = (uint32_t) inet_addr(arg_lip);
  server_ip.s_addr = (uint32_t) inet_addr(arg_sip);

  printf("Received ping packet from %s\n", inet_ntoa(rcv_ip->ip_src));
  client_ip = rcv_ip->ip_src;

  // The ICMP header
  struct icmp *rcv_icmp;
  rcv_icmp = (struct icmp*)(packet + SIZE_ETHERNET + (rcv_ip->ip_hl)*4);

  //len ??
  char *rest_data = malloc(sizeof(len));

  rest_data = packet + SIZE_ETHERNET + (rcv_ip->ip_hl)*4 + sizeof(struct icmp);

  printf("\t ICMP: Type: %d %d %04x\n", rcv_icmp->icmp_type,
    rcv_icmp->icmp_code, rcv_icmp->icmp_cksum);

  struct ping_struct *return_ping = malloc(sizeof(struct ping_struct));

  char *tmp_client_addr = malloc(sizeof(char) * 16);
   memcpy(tmp_client_addr, inet_ntoa(client_ip), sizeof(char)* 16);
  char *tmp_server_addr = malloc(sizeof(char) * 16);
   memcpy(tmp_server_addr, inet_ntoa(server_ip), sizeof(char)* 16);

   printf("%s    %s\n\n\n", tmp_client_addr, tmp_server_addr);

  if(strcmp(tmp_client_addr, tmp_server_addr) == 0) {
    struct ping_struct *ret = NULL;
    ret = ping_search_in_list(*rcv_ip, *rcv_icmp, NULL);
    if (ret == NULL) {
      perror("could not find client");
      exit(EXIT_FAILURE);
    }
    memcpy(return_ping, ret, sizeof(struct ping_struct));
    ping_delete_from_list(*rcv_ip, *rcv_icmp);
    return_ping->icmp.icmp_type = 0;
    send_ping(return_ping->ip.ip_src, return_ping->ip, *rcv_icmp, rest_data, len);
  }
  else {
    ping_add_to_list(*rcv_ip, *rcv_icmp);
    send_ping(server_ip, *rcv_ip, *rcv_icmp, rest_data, len);
  }
}



