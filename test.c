#include "globals.h"
#include <string.h>
/*
struct ftp_ip_port {
  struct in_addr src_ip;
  short port;
};
*/
struct ftp_man_port {
  u_short port;
  char *new_msg;
  int msglen;
};


struct ftp_man_port extract_ip_port(char *data, struct in_addr ip, int len) {

  char *bouncer_ip_str = inet_ntoa(ip);
  size_t s_size = strlen(bouncer_ip_str);

  printf("%s --- %zd\n", bouncer_ip_str, s_size);

  char *gsub;
  gsub = strstr(bouncer_ip_str, ".");
  memcpy(gsub,",", 1);

  gsub += 1;
  gsub = strstr(bouncer_ip_str, ".");
  memcpy(gsub,",", 1);

  gsub += 1;
  gsub = strstr(bouncer_ip_str, ".");
  memcpy(gsub,",", 1);


  printf("%s --- %zd\n", bouncer_ip_str, s_size);

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

  char *buffer = malloc(5 + s_size + strlen(ip_5) + strlen(ip_6) + 2 +3);


  memcpy(buffer, "PORT ", 5);
  memcpy(buffer + 5, bouncer_ip_str , s_size);
  memcpy(buffer + 5 + s_size, "," , 1);
  memcpy(buffer + 5 + 1 + s_size, ip_5 , strlen(ip_5));
  memcpy(buffer + 5 + 1 + s_size + strlen(ip_5), "," , 1);
  memcpy(buffer + 5 + 1 + s_size + strlen(ip_5) + 1, ip_6 , strlen(ip_6));
  memcpy(buffer + 5 + 1 + s_size + strlen(ip_5) + 1 + strlen(ip_6), " \r\n" , 3);

  printf("%s\n", buffer);
  char *ip_5_buf = malloc(sizeof(char)*2);
  char *ip_6_buf =  malloc(sizeof(char)*2);
  char *ip_final_buf =  malloc(sizeof(char)*4);

//  int len_ip_5 = uintToHexStr(atoi(ip_5), ip_5_buf);
 // int len_ip_6 = uintToHexStr(atoi(ip_6), ip_6_buf);
  
  sprintf(ip_5_buf, "%x", atoi(ip_5));
  sprintf(ip_6_buf, "%x", atoi(ip_6));
  strncat(ip_final_buf, ip_5_buf, 2);
  strncat(ip_final_buf, ip_6_buf, 2);

  printf("%s\n", ip_final_buf);
  
  u_int x;
  sscanf(ip_final_buf, "%x", &x);

  struct ftp_man_port ret;
  ret.port = x;
  
  ret.new_msg = buffer;
  ret.msglen = sizeof(buffer)*4;
  return ret;
}


void main(void) {

  char data[] = "PORT 192,168,3,119,210,215\r";

  struct in_addr *test = malloc(sizeof(struct in_addr));
  inet_aton("192.168.135.132", test);

  struct ftp_man_port tmp;
  tmp = extract_ip_port(data, *test, 28);
  printf("ftp-port %d,   %.*s\n",tmp.port, tmp.msglen ,tmp.new_msg);


}
