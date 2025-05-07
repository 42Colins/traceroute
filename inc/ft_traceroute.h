#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

# include "libft.h"
# include <errno.h>
# include <stdbool.h>
# include <stdio.h>
# include <netdb.h>
# include <sys/time.h>
# include <sys/ioctl.h>
# include <sys/param.h>
# include <net/if.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <getopt.h>
# include <sys/socket.h>

typedef struct s_root
{
	unsigned int ttl;
	unsigned int starting_port;
	char *dest;
	char *dest_ip;
	char *end_ip;
	bool destReached;
	unsigned int hops_max;
	unsigned int packet_size;
	unsigned int probes;
	int recvsocketFd;
	int sendsocketFd;
	char *current_ip;
	struct sockaddr_in recv_addr;
	struct sockaddr_in send_addr;
	double *timeArray;
	char *hostname;
}	t_root;


// Main.c
void	doHop(t_root *data);

// Print.c
bool	isTimeout(t_root *data);
void	printHop(t_root *data);

// Init.c
t_root	*initTraceroute(int argc, char **argv);

// Utils.c
bool	isRoot(void);
void	sendPacket(t_root *data);
bool	receivePacket(t_root *data);
void 	initRecvSocket(t_root *data);
void 	initSendSocket(t_root *data);
void 	setSocketOptions(int socketFd, unsigned int ttl);
char 	*getHostnameFromIp(const char *ip_addr);

// Udp.c
void	initUdpPacket(t_root *data);

#endif