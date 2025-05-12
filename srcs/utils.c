#include "ft_traceroute.h"
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>

bool isRoot(void)
{
    if (geteuid() == 0)
        return true;
    return false;
}

void sendPacket(t_root *data)
{
    data->send_addr.sin_port = htons(data->starting_port);
    
    char packet[data->packet_size];
    ft_memset(packet, 0x42, data->packet_size);
    
    setSocketOptions(data->sendsocketFd, data->ttl);
    
    if (sendto(data->sendsocketFd, packet, data->packet_size, 0, (struct sockaddr *)&data->send_addr, sizeof(data->send_addr)) < 0)
    {
        perror("Error while sending packet");
        exit(1);
    }
}

bool receivePacket(t_root *data)
{
    struct sockaddr_in recv_addr;
    ft_memset(&recv_addr, 0, sizeof(recv_addr)); 
    char buffer[1024];
    socklen_t addr_len = sizeof(recv_addr);

    int recv_len = recvfrom(data->recvsocketFd, buffer, sizeof(buffer), 0, (struct sockaddr *)&recv_addr, &addr_len);
    if (recv_len < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        }
        perror("Error while receiving packet");
        exit(1);
    }
    if (recv_len > 0) {
        struct ip *ip_header = (struct ip *)buffer;
        int ip_header_len = ip_header->ip_hl * 4;
        struct icmp *icmp_header = (struct icmp *)(buffer + ip_header_len);
        char *router_ip = NULL;
        
        if (icmp_header->icmp_type == ICMP_TIMXCEED || 
            icmp_header->icmp_type == ICMP_DEST_UNREACH) {
            router_ip = ft_strdup(inet_ntoa(ip_header->ip_src));
        }
        if (router_ip) {
            if (data->current_ip[data->runner])
            {
                free(data->current_ip[data->runner]);
            }
            data->current_ip[data->runner] = router_ip;
        }
    }
    if (!data->current_ip[data->runner]) {
        perror("Failed to get router IP");
        exit(1);
    }
    if (ft_strcmp(data->current_ip[data->runner], data->dest_ip) == 0) {
        data->destReached = true;
    }
    if (data->hostname)
        free(data->hostname);
    data->hostname = getHostnameFromIp(data->current_ip[data->runner]);
    return true;
}

void setSocketOptions(int socketFd, unsigned int ttl)
{
    if (setsockopt(socketFd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("Error setting TTL");
        exit(1);
    }
}

char *getHostnameFromIp(const char *ip_addr)
{
    struct sockaddr_in sa;
    char *hostname;
    
    hostname = malloc(NI_MAXHOST);
    if (!hostname) {
        perror("Memory allocation failed");
        return NULL;
    }
    ft_memset(hostname, 0, NI_MAXHOST);
    ft_memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip_addr, &(sa.sin_addr)) <= 0) {
        perror("Invalid IP address");
        free(hostname);
        return NULL;
    }
    int flags = 0;
    
    if (getnameinfo((struct sockaddr*)&sa, sizeof(sa), hostname, NI_MAXHOST - 1, NULL, 0, flags) == 0) {
        return hostname;
    } else {
        ft_strlcpy(hostname, ip_addr, NI_MAXHOST - 1);
        hostname[NI_MAXHOST - 1] = '\0';
        return hostname;
    }
}