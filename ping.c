#include "ping.h"

void process_ping(u_char *packet, struct ip *rcv_ip) {
  printf("\t|  * ICMP");
  struct icmp *rcv_icmp; /* The ICMP header */
  rcv_icmp = (struct icmp*)(packet + SIZE_ETHERNET + (rcv_ip->ip_hl)*4);
  printf("Type: %d %d", rcv_icmp->icmp_type, rcv_icmp->icmp_code);

  struct ip *send_ip;
  struct icmp *send_icmp; /* The ICMP header */
  char buffer[sizeof(*send_ip) + sizeof(*rcv_icmp)];
  send_ip = (struct ip *) buffer;
  send_icmp = (struct icmp *) (buffer + sizeof(struct ip));
  memcpy(send_icmp,rcv_icmp, sizeof(struct icmp));
  //send_icmp = rcv_icmp;

  //send_ip = rcv_ip;
  memcpy(send_ip, rcv_ip, sizeof(struct ip));
  
  struct in_addr ip_src, ip_dst;

  ip_src.s_addr = inet_addr(arg_lip);
  ip_dst.s_addr = inet_addr(arg_sip);

  send_ip->ip_src = ip_src;
  send_ip->ip_dst = ip_dst;
  send_ip->ip_sum = 0;

  send_ip->ip_sum = ip_checksum((void *) buffer, sizeof(struct ip));
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = ip_dst.s_addr;

  //int s = socket (PF_INET, SOCK_RAW, IPPROTO_ICMP);
  //sendto(s, buffer, sizeof(buffer), 0, (struct sockaddr *) &sin, sizeof(sin));
  
  
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
  if (sendto (sd, buffer, sizeof(buffer), 0, (struct sockaddr *) &sin, sizeof (sin)) < 0)  {
    perror ("sendto() failed ");
    exit (EXIT_FAILURE);
  }

  // Close socket descriptor.
  close (sd);
}
