#include "ftp_list.h"

struct ftp_struct *ftp_head = NULL;
struct ftp_struct *ftp_curr = NULL;

struct ftp_struct* ftp_create_list(const struct ip ip, const struct tcphdr tcp,
    int bouncing_port, int ftp_data_port) {
  struct ftp_struct *ptr = (struct ftp_struct*)malloc(sizeof(struct ftp_struct));
  if(ptr == NULL) {
    printf("\n Node creation failed \n");
    return NULL;
  }
  ptr->ip = ip;
  ptr->tcp = tcp;
  ptr->bouncing_port = bouncing_port;
  ptr->ftp_data_port = ftp_data_port;
  ptr->next = NULL;

  ftp_head = ftp_curr = ptr;
  return ptr;
}

struct ftp_struct* ftp_add_to_list(const struct ip ip, const struct tcphdr tcp,
    int bouncing_port, int ftp_data_port) {

  if(ftp_head == NULL) {
    return (create_list(ip, tcp, bouncing_port, ftp_data_port));
  }

  struct ftp_struct *ptr = (struct ftp_struct*) malloc(sizeof(struct ftp_struct));
  if(ptr == NULL) {
    printf("\n Node creation failed \n");
    return NULL;
  }
  ptr->ip = ip;
  ptr->tcp = tcp;
  ptr->bouncing_port = bouncing_port;
  ptr->ftp_data_port = ftp_data_port;
  ptr->next = NULL;

  ftp_curr->next = ptr;
  ftp_curr = ptr;

  return ptr;
}


struct ftp_struct* ftp_search_in_list(const struct ip ip, const struct tcphdr tcp,
    int bouncing_port, int ftp_data_port, struct ftp_struct **prev)
{
  struct ftp_struct *ptr = ftp_head;
  struct ftp_struct *tmp = NULL;
  bool found = false;
/*
  while(ptr != NULL) {
    bool condition = false;
    //unsigned long rcv_ip = ip.ip_src.s_addr;
    //unsigned long src_ip = ptr->ip.ip_src.s_addr;
    printf("by port: Equal? %d  %d\n", ntohs(ptr->bouncing_port), port);
    if(ntohs(ptr->bouncing_port) == port) {
      found = true;
      break;
    }
    else {
      tmp = ptr;
      ptr = ptr->next;
    }
  }
*/
  if(found == true) {
    if(prev) {
      *prev = tmp;
    }
    return ptr;
  }
  else {
    return NULL;
  }
}
/*
struct ftp_struct* search_in_list_by_ip(const struct ip ip, const struct tcphdr tcp,
    struct in_addr ip_src, struct ftp_struct **prev)
{
  struct ftp_struct *ptr = ftp_head;
  struct ftp_struct *tmp = NULL;
  bool found = false;
  bool condition = false;

  while(ptr != NULL) {
    bool condition = false;
    //unsigned long rcv_ip = ip.ip_src.s_addr;
    //unsigned long src_ip = ptr->ip.ip_src.s_addr;
    printf("by ip: Equal? %s  %s\n", inet_ntoa(ptr->ip.ip_src), inet_ntoa(ip_src));
    if(inet_ntoa(ptr->ip.ip_src) == inet_ntoa(ip_src)) {
      if(ntohs(ptr->tcp.th_sport) == ntohs(tcp.th_sport)) {
        condition = true;
      }
    }

    if (condition) {
      found = true;
      break;
    }
    else {
      tmp = ptr;
      ptr = ptr->next;
    }
  }

  if(found == true) {
    if(prev) {
      *prev = tmp;
    }
    return ptr;
  }
  else {
    return NULL;
  }
}
*/

void ftp_delete_from_list(const struct ip ip, const struct tcphdr tcp,
    int bouncing_port, int ftp_data_port) {
  struct ftp_struct *prev = NULL;
  struct ftp_struct *del = NULL;

  del = search_in_list(ip, tcp, bouncing_port, ftp_data_port, &prev);
  if(del == NULL) {
    perror("Failed to delete from list element");
    //exit(EXIT_FAILURE);
  }
  else {
    if(prev != NULL) {
      prev->next = del->next;
    }
    if(del == ftp_head) {
      ftp_head = del->next;
    }
    else if(del == ftp_curr) {
      ftp_curr = prev;
    }
  }

  free(del);
  del = NULL;
}

void ftp_print_list(void) {
  struct ftp_struct *ptr = ftp_head;
/*
  printf("\n -------Printing list Start------- \n");
  while(ptr != NULL) {
    char *src_ip = inet_ntoa(ptr->ip.ip_src);
    int type = ptr->icmp.icmp_type;

    printf("\n %s with type %d \n",src_ip, type);

    ptr = ptr->next;
  }
  printf("\n -------Printing list End------- \n");
*/
  return;
}
