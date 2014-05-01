/* Global definitions for the port bouncer
 * Packet headers and so on
 */

#ifndef BOUNDER_H
#define BOUNDER_H

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


#define SIZE_ETHERNET 14

/* Add any other declarations you may need here... */

static char *arg_dev;
static char *arg_lip;
static char *arg_lport;
static char *arg_sip;
static char *arg_sport;

#endif
