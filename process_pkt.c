#include <time.h>
#include "bouncer.h"


void convert_time(const struct timeval *tv1){

  time_t nowtime;
  struct tm *nowtm;
  char tmbuf[64], buf[64];

  nowtime = tv1->tv_sec;
  nowtm = localtime(&nowtime);
  strftime(tmbuf, sizeof tmbuf, "%H:%M:%S", nowtm);
  //snprintf(buf, sizeof buf, "%s::%06ld.", tmbuf, tv1->tv_usec);
  printf("%s::%2d", tmbuf, (int) tv1->tv_usec/1000);
}


void process_pkt(u_char *args, const struct pcap_pkthdr *header,
      const u_char *packet){

  /** DEFINITION IN pcap.h
  struct pcap_pkthdr {
  struct timeval ts; / time stamp /
  bpf_u_int32 caplen; / length of portion present /
  bpf_u_int32 len; / length this packet (off wire) /
  };

  */

  printf("\nPACKET captured - %d ", header->caplen);

  convert_time(&header->ts);

  //const struct sniff_ethernet *ethernet; /* The ethernet header */ - Not used here
  #define SIZE_ETHERNET 14

  const struct ip *rcv_ip; /* The IP header */
  const struct tcphdr *rcv_tcp; /* The TCP header */
  const struct icmp *rcv_icmp; /* The ICMP header */
  const char *payload; /* Packet payload */

  u_int size_ip;
  u_int size_tcp;

/* Magical typecasting */
  //ethernet = (struct sniff_ethernet*)(packet);                    - Not used here
  rcv_ip = (struct ip*)(packet + SIZE_ETHERNET);
  if(rcv_ip->ip_v == 4){
    if (rcv_ip->ip_hl < 20) {
      printf("\t|-  *** Invalid IP header length: %u bytes", rcv_ip->ip_hl);
      return;
    }
    printf("\t\t|   * IP proto: %d",rcv_ip->ip_p);
  }else{
    printf("\t\t|-   ** IP_V6 packet - discarding");
    return;
  }
  size_ip = rcv_ip->ip_hl;
  //TODO replace wit switch for each protocol handler
  if ((rcv_ip->ip_p) == 1){
    printf("\t|  * ICMP");
    rcv_icmp = (struct icmp*)(packet + SIZE_ETHERNET + size_ip);
    printf("Type: %d %d", rcv_icmp->icmp_type, rcv_icmp->icmp_code);
  }else if(rcv_ip->ip_p == 6){
    printf("\t|   * TCP");
  }

  rcv_tcp = (struct tcphdr*)(packet + SIZE_ETHERNET + size_ip);
  size_tcp = (rcv_tcp->th_win)*4;
  if (size_tcp < 20) {
    printf("\t|- *** Invalid TCP header length: %u bytes", size_tcp);
    return;
  }
  payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);

    /* Check IP header*/

    /* Check type of packet and process*/

    /* Check ICMP header*/
    /* Check TCP header*/
    /* Check FTP header*/
    /* Send processed packet */

}

