/* Port Bouncer
* To be called as nbouncer local_ip local_port remote_ip remote_port
*/

#include "bouncer.h"

#define lookupDevice "eth0"
#define packet_cnt 0

void process_pkt(u_char *args, const struct pcap_pkthdr *header,
	    const u_char *packet);

int main(int argc, char *argv[])
{

  /* TODO simple arguments checking */
  if(!(argc==4)){
	printf("Wrong number of arguments given \n -usage of bouncer: bouncer <listen_ip> <listen_port> <server_ip> <server_port>\n");
	//exit(1);
  }

  /* Parsing arguments */
  char *arg_dev		= argv[1];
  char *arg_lip 	= argv[2];
  char *arg_lport 	= argv[3];
  char *arg_sip 	= argv[4];
  char *arg_sport 	= argv[5];

  /* Include here your code to initialize the PCAP capturing process */
  char *dev; 
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t* descr;
  struct bpf_program fp;      /* hold compiled program     */
  bpf_u_int32 maskp;          /* subnet mask               */
  bpf_u_int32 netp;           /* ip                        */
  //u_char* args = NULL;

  dev = arg_dev;
  printf("Trying to use device %s\n",dev);
  /* ask pcap for the network address and mask of the device */
  if (pcap_lookupnet(dev, &netp, &maskp, errbuf) == -1) {
	fprintf(stderr, "Can't get netmask for device %s\n", dev);
	netp = 0;
	maskp = 0;
  }else{
  struct in_addr net_add;
  net_add.s_addr = netp;
  printf("Preparing to listen %s ", inet_ntoa(net_add));
  
  net_add.s_addr = maskp;
  printf("  --NETMASK %s \n", inet_ntoa(net_add));
  }

  /* open device for reading. NOTE: defaulting to promiscuous mode*/
  descr = pcap_open_live(dev,BUFSIZ,1,-1,errbuf);
  if(descr == NULL){
	fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
	exit(1);
  }

  /* Define a filter expression and try and compile the program.. non-optimized */
  char filter_exp[] = "";
  if(!(filter_exp== "")){
	if(pcap_compile(descr,&fp,filter_exp,0,netp) == -1){
	  fprintf(stderr,"Error calling pcap_compile\n");
	  exit(1);
	}

	/* set the compiled program as the filter */
	if(pcap_setfilter(descr,&fp) == -1){
	  fprintf(stderr,"Error setting filter\n");
	  exit(1);

	}
  }


  /* ... and loop */ 
  pcap_loop(descr, packet_cnt, process_pkt, NULL);

  fprintf(stdout,"\nfinished\n");
 
  /* Initialize raw socket */
	
	return 0;
}//End of the bouncer