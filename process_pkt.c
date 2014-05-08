#include <time.h>
#include "globals.h"
#include "utils.h"

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

  printf("PACKET captured - %d with length: %d  ", header->caplen, header->len);

  convert_time(&header->ts);

  //const struct sniff_ethernet *ethernet; /* The ethernet header */ - Not used here
  printf("-------------> %d\n", header->len);

  struct ip *rcv_ip; /* The IP header */
  const struct tcphdr *rcv_tcp; /* The TCP header */

  const char *payload; /* Packet payload */

  u_int size_ip;
  u_int size_tcp;

  /* Magical typecasting */
  //ethernet = (struct sniff_ethernet*)(packet);                    - Not used here
  rcv_ip = (struct ip*)(packet + SIZE_ETHERNET);
  printf("\nRECEIVED PACKET FROM %s \n", inet_ntoa(rcv_ip->ip_src));
  size_ip = (rcv_ip->ip_hl)*4;
  if(rcv_ip->ip_v == 4){
    if ((size_ip < 20) || (size_ip > 60)) {
      printf("\t| *** Invalid IP header length: %u bytes\n", rcv_ip->ip_hl);
      return;
    }
    printf("\t| *** Version: IPv4");
  }
  else {
    printf("\t| *** IP_V6 packet - discarding\n");
    return;
  }

  u_short test_checksum;
  test_checksum = rcv_ip->ip_sum;
  rcv_ip->ip_sum = 0;
  rcv_ip->ip_sum = ip_checksum(rcv_ip, size_ip);
  if(test_checksum != rcv_ip->ip_sum){
    printf("Bad IP checksum -- Discarding XXXX");
    return;
  }

  if (rcv_ip->ip_ttl == 0){
    printf("Bad TTL = 0 -- Discarding XXXX");
  }

  if (ntohs(rcv_ip->ip_off) == IP_RF){
    printf("EVIL BIT SET -- Discarding XXXX");
    return;
  }

  if ((rcv_ip->ip_p) == 1){
    printf("\t| *** Protocol: ICMP\n");
    process_ping(packet, rcv_ip, header->len);
    return;
  }else if(rcv_ip->ip_p == 6){
    process_tcp(packet, rcv_ip, header->len);
    printf("\t| *** Protocol: TCP\n");
  }

  rcv_tcp = (struct tcphdr*)(packet + SIZE_ETHERNET + size_ip);
  size_tcp = (rcv_tcp->th_off)*4;
  if (size_tcp < 20) {
    printf("\t| *** Invalid TCP header length: %u bytes\n", size_tcp);
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

