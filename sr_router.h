/*-----------------------------------------------------------------------------
 * File: sr_router.h
 * Date: ?
 * Authors: Guido Apenzeller, Martin Casado, Virkam V.
 * Contact: casado@stanford.edu
 * 90904102
 *---------------------------------------------------------------------------*/

#ifndef SR_ROUTER_H
#define SR_ROUTER_H

#include <netinet/in.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdbool.h>

#include "sr_protocol.h"
#ifdef VNL
#include "vnlconn.h"
#endif

/* we dont like this debug , but what to do for varargs ? */
#ifdef _DEBUG_
#define Debug(x, args...) printf(x, ## args)
#define DebugMAC(x) \
do { int ivyl; for(ivyl=0; ivyl<5; ivyl++) printf("%02x:", \
(unsigned char)(x[ivyl])); printf("%02x",(unsigned char)(x[5])); } while (0)
#else
#define Debug(x, args...) do{}while(0)
#define DebugMAC(x) do{}while(0)
#endif

#define INIT_TTL 255
#define PACKET_DUMP_SIZE 1024

/* forward declare */
struct sr_if;
struct sr_rt;

/* ----------------------------------------------------------------------------
 * struct sr_instance
 *
 * Encapsulation of the state for a single virtual router.
 *
 * -------------------------------------------------------------------------- */

struct sr_instance
{
    int  sockfd;   /* socket to server */
#ifdef VNL
    struct VnlConn* vc;
#endif
    char user[32]; /* user name */
    char host[32]; /* host name */
    char template[30]; /* template name if any */
    char auth_key_fn[64]; /* auth key filename */
    unsigned short topo_id;
    struct sockaddr_in sr_addr; /* address to server */
    struct sr_if* if_list; /* list of interfaces */
    struct sr_rt* routing_table; /* routing table */
    FILE* logfile;
    volatile uint8_t  hw_init; /* bool : hardware has been initialized */
    
    /* -- pwospf subsystem -- */
    struct pwospf_subsys* ospf_subsys;
};

struct cache
{
    uint32_t ipAddress;
    unsigned char macAddress[ETHER_ADDR_LEN];
    struct cache * next;
};

struct cachePackets
{
    uint8_t * packet;
    int length;
    struct sr_instance* sr;
    char * inter;
    struct cachePackets *next;
}cachePackets;







/* -- sr_main.c -- */
int sr_verify_routing_table(struct sr_instance* sr);

/* -- sr_vns_comm.c -- */
int sr_send_packet(struct sr_instance* , uint8_t* , unsigned int , const char*);
int sr_connect_to_server(struct sr_instance* ,unsigned short , char* );
int sr_read_from_server(struct sr_instance* );

/* -- sr_router.c -- */
void sr_init(struct sr_instance* );
void sr_handlepacket(struct sr_instance* , uint8_t * , unsigned int , char* );
void sr_handleARPpacket(struct sr_instance* sr,uint8_t * packet/* lent */,unsigned int len,struct sr_if* inter,/* lent */struct sr_ethernet_hdr* header,char* interface);
void sr_handleIPpacket(struct sr_instance* sr,uint8_t * packet/* lent */,unsigned int len,char *interface,/* lent */struct sr_ethernet_hdr* header,char *srcInterface);
void addNewCache(uint32_t ip,unsigned char mac[ETHER_ADDR_LEN]);
void printCache();
bool checkCache(uint32_t ip);
struct sr_rt* RoutingTableLookUp(struct sr_instance* sr,uint32_t ipTarget,uint32_t nextHop);
void ForwardPacket(struct sr_instance * sr,struct sr_rt* newInterface,struct sr_ethernet_hdr * header,uint8_t * packet,unsigned int len,struct ip * ipheader, struct cache * node);
struct cache * getNode(uint32_t ip);
void sendARPrequest(struct sr_instance * sr, uint32_t destinationIP, char* interface,struct sr_rt* newInterface,int len);
void cachePacket(uint8_t *packet, struct sr_ethernet_hdr * header, int len,struct sr_instance* sr,char *inter);
bool  isGateway(uint32_t ipAddToMatch, struct sr_instance* sr);
void sendCachedPacket(uint32_t ip_addr,char* tx_interface,char *srcInterface);
void sr_handleICMPpacket(struct sr_instance* sr,uint8_t * packet/* lent */,unsigned int len,struct sr_if* inter,/* lent */struct sr_ethernet_hdr* header,char* interface);
uint32_t setChecksum(uint16_t* tmp, int size);
uint16_t ip_checksum(void* vdata,size_t length);
uint16_t cksum(uint8_t* hdr, int len);
bool doesInterfaceExist(struct sr_instance *sr, char *intf);

///new code
void handlePWOSPFpacket(struct sr_instance* sr,uint8_t * packet/* lent */,unsigned int len,struct sr_if* inter,/* lent */struct sr_ethernet_hdr* header,char* interface);
void cacheMACForIp(struct ip *ipHdr,struct sr_ethernet_hdr *ethHdr);
struct sr_rt* getInterface(uint32_t dest, struct sr_instance * sr,char *srcInterface);
struct sr_if *isInterfaceUp(struct sr_instance *sr,char *interface);
void defualtRoute(uint8_t *packet, struct sr_instance * sr, int len);

/* -- sr_if.c -- */
void sr_add_interface(struct sr_instance* , const char* );
void sr_set_ether_ip(struct sr_instance* , uint32_t );
void sr_set_ether_addr(struct sr_instance* , const unsigned char* );
void sr_print_if_list(struct sr_instance* );
uint16_t packet_checksum(uint16_t* addr, int count);
#endif /* SR_ROUTER_H */




