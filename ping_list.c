#include "ping_list.h"

struct ping_struct *ping_head = NULL;
struct ping_struct *ping_curr = NULL;

struct ping_struct* ping_create_list(const struct ip ip, const struct icmp icmp) {
  struct ping_struct *ptr = (struct ping_struct*)malloc(sizeof(struct ping_struct));
  if(ptr == NULL) {
    printf("\n Node creation failed \n");
    return NULL;
  }
  ptr->ip = ip;
  ptr->icmp = icmp;
  ptr->next = NULL;

  ping_head = ping_curr = ptr;
  return ptr;
}

struct ping_struct* ping_add_to_list(const struct ip ip, const struct icmp icmp) {

  if(ping_head == NULL) {
    return (ping_create_list(ip, icmp));
  }

  struct ping_struct *ptr = (struct ping_struct*) malloc(sizeof(struct ping_struct));
  if(ptr == NULL) {
    printf("\n Node creation failed \n");
    return NULL;
  }
  ptr->ip = ip;
  ptr->icmp = icmp;
  ptr->next = NULL;

  ping_curr->next = ptr;
  ping_curr = ptr;

  return ptr;
}


struct ping_struct* ping_search_in_list(const struct ip ip, const struct icmp icmp,
    struct ping_struct **prev)
{
  struct ping_struct *ptr = ping_head;
  struct ping_struct *tmp = NULL;
  bool found = false;
  int search_type = 0;

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
/*
    if(icmp.icmp_type == 8) {
      if (icmp.icmp_type == ptr->icmp.icmp_type) {
        condition = true;
      }
    }
    if(condition == true) {
        found = true;
        break;
    }
    else {
      tmp = ptr;
      ptr = ptr->next;
    }
*/
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

void ping_delete_from_list(const struct ip ip, const struct icmp icmp) {
  struct ping_struct *prev = NULL;
  struct ping_struct *del = NULL;

  del = ping_search_in_list(ip, icmp, &prev);
  if(del == NULL) {
    perror("Failed to delete from list element");
    //exit(EXIT_FAILURE);
  }
  else {
    if(prev != NULL) {
      prev->next = del->next;
    }
    if(del == ping_head) {
      ping_head = del->next;
    }
    else if(del == ping_curr) {
      ping_curr = prev;
    }
  }

  free(del);
  del = NULL;
}

void ping_print_list(void) {
  struct ping_struct *ptr = ping_head;

  printf("\n -------Printing list Start------- \n");
  while(ptr != NULL) {
    char *src_ip = inet_ntoa(ptr->ip.ip_src);
    int type = ptr->icmp.icmp_type;

    printf("\n %s with type %d \n",src_ip, type);

    ptr = ptr->next;
  }
  printf("\n -------Printing list End------- \n");

  return;
}
