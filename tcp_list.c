#include "tcp_list.h"

struct tcp_struct *head = NULL;
struct tcp_struct *curr = NULL;

struct tcp_struct* create_list(const struct ip ip, const struct tcphdr tcp) {
  struct tcp_struct *ptr = (struct tcp_struct*)malloc(sizeof(struct tcp_struct));
  if(ptr == NULL) {
    printf("\n Node creation failed \n");
    return NULL;
  }
  ptr->ip = ip;
  ptr->tcp = tcp;
  ptr->next = NULL;

  head = curr = ptr;
  return ptr;
}

struct tcp_struct* add_to_list(const struct ip ip, const struct tcphdr tcp) {

  if(head == NULL) {
    return (create_list(ip, tcp));
  }

  struct tcp_struct *ptr = (struct tcp_struct*) malloc(sizeof(struct tcp_struct));
  if(ptr == NULL) {
    printf("\n Node creation failed \n");
    return NULL;
  }
  ptr->ip = ip;
  ptr->tcp = tcp;
  ptr->next = NULL;

  curr->next = ptr;
  curr = ptr;

  return ptr;
}


struct tcp_struct* search_in_list(const struct ip ip, const struct tcphdr tcp,
    struct tcp_struct **prev)
{
  struct tcp_struct *ptr = head;
  struct tcp_struct *tmp = NULL;
  bool found = false;
  int search_type = 0;
/*
  if(icmp.icmp_type == 0) {
    search_type = 8;
  }

  while(ptr != NULL) {
    bool condition = false;
    //unsigned long rcv_ip = ip.ip_src.s_addr;
    //unsigned long src_ip = ptr->ip.ip_src.s_addr;
    if(ptr->icmp.icmp_type == search_type) {
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

void delete_from_list(const struct ip ip, const struct tcphdr tcp) {
  struct tcp_struct *prev = NULL;
  struct tcp_struct *del = NULL;

  del = search_in_list(ip, tcp, &prev);
  if(del == NULL) {
    perror("Failed to delete from list element");
    //exit(EXIT_FAILURE);
  }
  else {
    if(prev != NULL) {
      prev->next = del->next;
    }
    if(del == head) {
      head = del->next;
    }
    else if(del == curr) {
      curr = prev;
    }
  }

  free(del);
  del = NULL;
}

void print_list(void) {
  struct tcp_struct *ptr = head;
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
