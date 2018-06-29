/*-----------------------------------------------------------------------------
 * file:  sr_pwospf.h
 * date:  Tue Nov 23 23:21:22 PST 2004
 * Author: Martin Casado
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifndef SR_PWOSPF_H
#define SR_PWOSPF_H

#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include "sr_rt.h"
#include<stdbool.h>
/* forward declare */
struct sr_instance;

struct pwospf_subsys
{
    /* -- pwospf subsystem state variables here -- */
    
    
    /* -- thread and single lock for pwospf subsystem -- */
    pthread_t thread;
    pthread_mutex_t lock;
};

int pwospf_init(struct sr_instance* sr);

//void handle_ospf_packet(struct sr_instance *sr, uint8_t *packet, struct sr_if *iface);

struct neighbours
{
    char *interFace;
    uint32_t ip;
    uint32_t routerId;
    uint32_t mask;
    time_t lastUpdate;
    struct neighbours *next;
};

struct vertex
{
    uint32_t  subnet;
    uint32_t  routerId;
    uint32_t  id;
    uint32_t  mask;
    uint32_t  nextHop;
    int sequence;
    struct vertex * next;
};


void* sendLSU(void* arg);
void* sendHello(void* arg);
void* monitor(void *arg);
void handleLsuPackets(struct sr_instance *sr, uint8_t *packet, struct sr_if *iface);
void handleHelloPackets(struct sr_instance *sr, uint8_t *packet, struct sr_if *iface);

void deleteNeighbour(struct sr_instance*,struct neighbours *target);
void addVertex(uint32_t routerId, uint32_t subnet, uint32_t mask, uint32_t id, uint32_t nextHop, uint16_t sequence);
struct vertex * findVertex(uint32_t routerId ,uint32_t subnet);
void updateRTable(struct sr_instance* sr);
void populateRoutingTable(struct sr_instance *sr,uint32_t destination ,uint32_t nextHop,uint32_t mask,char *interface);
char* longestMatch(struct sr_instance *sr,uint32_t destination);
void addDefaultEntry(struct sr_instance *sr);
struct sr_if* matchInterface(struct sr_instance *sr,char * target);
struct neighbours * findIP(uint32_t ip);
struct neighbours * findRouterID(uint32_t routerId);

#endif /* SR_PWOSPF_H */

