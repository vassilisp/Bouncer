#ifndef FTP_LIST_H
#define FTP_LIST_H

#include "globals.h"

struct ftp_struct {
  struct ip ip;
  struct tcphdr tcp;
  int bouncing_port;
  int ftp_data_port;
  struct ftp_struct *next;
};

struct ftp_struct* ftp_create_list(const struct ip ip, const struct tcphdr tcp,
    int bouncing_port, int ftp_data_port);
struct ftp_struct* ftp_add_to_list(const struct ip ip, const struct tcphdr tcp,
    int bouncing_port, int ftp_data_port);
struct ftp_struct* ftp_search_in_list(const struct ip ip, const struct tcphdr tcp,
    int bouncing_port, int ftp_data_port, struct ftp_struct **prev);
struct ftp_struct* ftp_search_in_list_by_ip(const struct ip ip, const struct tcphdr tcp,
int bouncing_port, int ftp_data_port, struct in_addr ip_src, struct ftp_struct **prev);
void ftp_delete_from_list(const struct ip ip, const struct tcphdr tcp,
    int bouncing_port, int ftp_data_port);
void ftp_print_list(void);

#endif
