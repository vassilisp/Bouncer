#include "tcp.h"


void send_tcp(struct in_addr receiver, struct ip send_ip, struct tcphdr tcp,
    char *rest_data, int rest_data_len, int sport, int dport) {

  //print_list();
  // The IP header
  send_ip.ip_src = bouncer_ip;
  send_ip.ip_dst = receiver;
  send_ip.ip_sum = 0;
  send_ip.ip_sum = ip_checksum((void *) &send_ip, sizeof(struct ip));

  //the TCP header
  tcp.th_sport = htons((u_short) sport);
  tcp.th_dport = htons((u_short) dport);
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
/*
  // Bind socket to interface index.
  struct ifreq ifr;
  char iname[16] = "eth0";

  memcpy(&ifr, &iname , sizeof(iname));
  if (setsockopt (sd, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof (ifr)) < 0) {
    perror ("setsockopt() failed to bind to interface ");
    exit (EXIT_FAILURE);
  }
*/
  printf("Buffer size to sent: %zu\n", final_size);
  // Send packet.
  if (sendto (sd, final_buffer, final_size, 0,
        (struct sockaddr *) &sin, sizeof (sin)) < 0)  {
    perror ("sendto() failed ");
    exit (EXIT_FAILURE);
  }

  // Close socket descriptor.
  close (sd);

}

void process_tcp(u_char *packet, struct ip *rcv_ip, int len) {
  bool is_server = false;
  bool ftp_on = false;
  bouncer_ip.s_addr = (uint32_t) inet_addr(arg_lip);
  server_ip.s_addr = (uint32_t) inet_addr(arg_sip);

  printf("Received TCP packet from %s\n", inet_ntoa(rcv_ip->ip_src));
  client_ip = rcv_ip->ip_src;

  // The tcp header
  struct tcphdr *tcp;
  size_t offset = SIZE_ETHERNET + (rcv_ip->ip_hl)*4;
  tcp = (struct tcphdr*)(packet + offset);

  size_t rest_data_len = len - SIZE_ETHERNET - sizeof(struct ip) - sizeof(struct tcphdr);
  char *rest_data = malloc(sizeof(rest_data_len));
  rest_data = packet + SIZE_ETHERNET + (rcv_ip->ip_hl)*4 + sizeof(struct tcphdr);

  if (tcp->th_off*4 < 20){
    printf("Bad tcp minimum length -- Discarding XXXX)");
    return;
  }

  u_short checksum;
  checksum = tcp->th_sum;
  tcp->th_sum = 0;
  // TODO test checksum

  printf("\t TCP Options: %d %d %d %d\n", ntohs(tcp->th_sport), ntohs(tcp->th_dport),
    tcp->th_seq, tcp->th_ack);

  char flags = tcp->th_flags;
  char syn_on = flags & TH_SYN;
  char ack_on = flags & TH_ACK;
  char synack_on = flags & (TH_SYN | TH_ACK);
  printf("Flag SYN IS: %04x\t", syn_on);
  printf("Flag ACK IS: %04x\n", ack_on);

  char *tmp_client_addr = malloc(sizeof(char) * 16);
   memcpy(tmp_client_addr, inet_ntoa(client_ip), sizeof(char)* 16);
  char *tmp_server_addr = malloc(sizeof(char) * 16);
   memcpy(tmp_server_addr, inet_ntoa(server_ip), sizeof(char)* 16);

  if(strcmp(tmp_client_addr, tmp_server_addr) == 0) {
    is_server = true;
  }
  else {
    int test_dport;
    test_dport = ntohs(tcp->th_dport);
    int test_dport2 = atoi(arg_lport);
    if (test_dport != test_dport2){
      printf("Packet with WRONG destination port -- Discarding XXXX");
      return;
    }
  }

  char *ftp_port = strstr(rest_data, "PORT");
  if (ftp_port != NULL) {
    ftp_on = true;
    printf("FTP:::: Found FTP packet !!!\n");

    struct ftp_struct *ftp_ret;
    ftp_ret = ftp_search_in_list_by_ip(*rcv_ip, *tcp, 0, 0, client_ip, NULL);
    if (ftp_ret == NULL) {
      printf("FTP::::: Searching in list failed \n");
      //if is null, change IP in payload
      //add to list
      //send
      char *last_char =  strstr(ftp_port, "\r\n");
      char *ftp_data = malloc(sizeof(char)*(last_char - ftp_port));
      memcpy(ftp_data, ftp_port, last_char - ftp_port);

      printf("\n\n\n%s \n\n\n", ftp_data);

      struct ftp_ip_port tmp = extract_ip_port(ftp_data);
    }
    else {
      printf("FTP:::::: Search in list succeded !!!\n");
    }
  }

  if(ack_on) {
      printf("STATE: ACK ON\n");
      struct tcp_struct *ret = NULL;
      int sport, dport;
      struct in_addr receiver;
      if (is_server) {
        printf("Packet received from server\n");
        ret = search_in_list(*rcv_ip, *tcp, tcp->th_dport, NULL);
        if (ret != NULL) {
          sport = ntohs(ret->tcp.th_dport);
          dport = ntohs(ret->tcp.th_sport);
          receiver = ret->ip.ip_src;
        }
        else {
          printf("Not found in list\n");
          return;
        }
      }
      else {
        printf("Packet received from client\n");

        ret = search_in_list_by_ip(*rcv_ip, *tcp, client_ip, NULL);
        if (ret != NULL) {
          sport = ret->bouncing_port;
          //this needs to be fixed to argument lip
          //dport = ntohs(ret->tcp.th_dport);
          dport = atoi(arg_sport);
          receiver = server_ip;
        }
        else {
          printf("Not found in list\n");
          return;
        }
      }
      send_tcp(receiver, *rcv_ip, *tcp, rest_data, rest_data_len, sport, dport);
      printf("Packet from %s to %s!!\n", inet_ntoa(rcv_ip->ip_dst),
          inet_ntoa(ret->ip.ip_src));
  }
  else {
    printf("STATE: ACK OFF\n");
    if(syn_on) {
      printf("STATE: ACK OFF, SYN ON\n");
      add_to_list(*rcv_ip, *tcp, BOUNCING_PORT);
      send_tcp(server_ip, *rcv_ip, *tcp, rest_data, rest_data_len,
          BOUNCING_PORT, atoi(arg_sport));
      BOUNCING_PORT++;
    }
    else {
      printf("STATE: ACK OFF, SYN OFF\n");
      printf("packet dropped \n");
      return;
    }
  }
}

struct ftp_ip_port extract_ip_port(char *data, int len) {
  struct ftp_ip_port ret;

  data += 5;
  char *comma = strstr(data, ",");
  char *ip_1 = malloc(sizeof(char)*(comma - data));
  strncpy(ip_1, data, (comma - data));


  data = comma + 1;
  comma = strstr(data, ",");
  char *ip_2 = malloc(sizeof(char)*(comma - data));
  strncpy(ip_2, data, (comma - data));

  data = comma + 1;
  comma = strstr(data, ",");
  char *ip_3 = malloc(sizeof(char)*(comma - data));
  strncpy(ip_3, data, (comma - data));

  data = comma + 1;
  comma = strstr(data, ",");
  char *ip_4 = malloc(sizeof(char)*(comma - data));
  strncpy(ip_4, data, (comma - data));

  data = comma + 1;
  comma = strstr(data, ",");
  char *ip_5 = malloc(sizeof(char)*(comma - data));
  strncpy(ip_5, data, (comma - data));

  data = comma + 1;
  comma = strstr(data, "\r\n");
  char *ip_6 = malloc(sizeof(char)*(comma - data));
  strncpy(ip_6, data, (comma - data));

  printf("%s %s %s %s %s %s\n\n\n\n", ip_1, ip_2, ip_3, ip_4, ip_5, ip_6);

  return ret;
}

