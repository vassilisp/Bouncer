#ifndef GLOBALS_H
#define GLOBALS_H

#define _BSD_SOURCE 1
#define __FAVOR_BSD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* PCAP declarations */
#include <pcap.h>

/* Standard networking declaration */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
 * The following system include files should provide you with the 
 * necessary declarations for Ethernet, IP, and TCP headers
 */

#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"


char *arg_dev;
char *arg_lip;
char *arg_lport;
char *arg_sip;
char *arg_sport;

int BOUNCING_PORT;

#define SIZE_ETHERNET 14

/* Add any other declarations you may need here... */

struct in_addr bouncer_ip, server_ip, client_ip;

const int on;
#endif
