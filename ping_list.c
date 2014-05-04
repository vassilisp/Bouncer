#include "ping_list.h"

struct ping_struct *head = NULL;
struct ping_struct *curr = NULL;

struct ping_struct* create_list(struct ip ip, struct icmp icmp) {
  struct ping_struct *ptr = (struct ping_struct*)malloc(sizeof(struct ping_struct));
  if(ptr == NULL) {
    printf("\n Node creation failed \n");
    return NULL;
  }
  ptr->ip = ip;
  ptr->icmp = icmp;
  ptr->next = NULL;

  head = curr = ptr;
  return ptr;
}

struct ping_struct* add_to_list(struct ip ip, struct icmp icmp) {

  if(head == NULL) {
    return (create_list(ip, icmp));
  }

  struct ping_struct *ptr = (struct ping_struct*) malloc(sizeof(struct ping_struct));
  if(ptr == NULL) {
    printf("\n Node creation failed \n");
    return NULL;
  }
  ptr->ip = ip;
  ptr->icmp = icmp;
  ptr->next = NULL;

  curr->next = ptr;
  curr = ptr;

  return ptr;
}


struct ping_struct* search_in_list(struct ip ip, struct icmp icmp,
    struct ping_struct **prev)
{
  struct ping_struct *ptr = head;
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

void delete_from_list(struct ip ip, struct icmp icmp) {
  struct ping_struct *prev = NULL;
  struct ping_struct *del = NULL;

  del = search_in_list(ip, icmp, &prev);
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
  struct ping_struct *ptr = head;

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
