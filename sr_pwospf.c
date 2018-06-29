/*-----------------------------------------------------------------------------
 * file: sr_pwospf.c
 * date: Tue Nov 23 23:24:18 PST 2004
 * Author: Martin Casado
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

#include "sr_pwospf.h"
#include "sr_router.h"
#include "sr_if.h"
#include "sr_rt.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sr_protocol.h"
#include "pwospf_protocol.h"

/* -- declaration of main thread function for pwospf subsystem --- */
static int len = sizeof(struct sr_ethernet_hdr) + sizeof(struct ip) + sizeof(struct ospfv2_hdr) + sizeof(struct ospfv2_hello_hdr);
static int lenPacketLSU=(sizeof(struct sr_ethernet_hdr) + sizeof(struct ip) + sizeof(struct ospfv2_hdr) + sizeof(struct ospfv2_lsu_hdr) + (sizeof(struct ospfv2_lsu) * 3));

static int seqnum = 0;
struct neighbours *neighbourBoss = NULL;
struct vertex *vertexList = NULL;
static void* pwospf_run_thread(void* arg);

pthread_t HELLOThread;
pthread_t LSUThread;
pthread_t CHECKThread;

/*---------------------------------------------------------------------
 * Method: pwospf_init(..)
 *
 * Sets up the internal data structures for the pwospf subsystem
 *
 * You may assume that the interfaces have been created and initialized
 * by this point.
 *---------------------------------------------------------------------*/
struct sr_if* matchInterface(struct sr_instance *sr,char * target){
    
    struct sr_if* inter = sr->if_list;
    while (inter) {
        
        if (strcmp(inter->name,target) == 0) {
            return inter;
        }
        inter = inter->next;
    }
    return NULL;
    
}



int pwospf_init(struct sr_instance* sr)
{
    assert(sr);
    HELLOThread=(pthread_t*)malloc(sizeof(pthread_t));
    LSUThread=(pthread_t*)malloc(sizeof(pthread_t));
    CHECKThread=(pthread_t*)malloc(sizeof(pthread_t));
    
    sr->ospf_subsys = (struct pwospf_subsys*)malloc(sizeof(struct pwospf_subsys));
    
    assert(sr->ospf_subsys);
    pthread_mutex_init(&(sr->ospf_subsys->lock), 0);
    
    /* -- start thread subsystem -- */
    if ( pthread_create(&sr->ospf_subsys->thread, 0, pwospf_run_thread, sr)) {
        perror("pthread_create");
        assert(0);
    }
    
    pthread_create(&HELLOThread, NULL, &sendHello, sr);
    pthread_create(&CHECKThread, NULL, &monitor, sr);
    pthread_create(&LSUThread, NULL, &sendLSU, sr);
    return 0; /* success */
} /* -- pwospf_init -- */


void * monitor(void *arg)
{
    struct sr_instance *sr = (struct sr_instance *)arg;
    while (1)
    {
        struct neighbours *temp = neighbourBoss;
        time_t currTime = time(NULL);
        while (temp)
        {
            time_t lastUpdate = temp->lastUpdate;
            double diff = difftime(currTime, lastUpdate);
            
            if (diff > OSPF_NEIGHBOR_TIMEOUT && temp->routerId != 0)
            {
                printf("SOMETHING HAS CHANGED!!!!!!!!!remove neighbour\n");
                deleteNeighbour(sr,temp);
            }
            temp = temp->next;
            
        }
        sleep(OSPF_DEFAULT_LSUINT);
    }
    return NULL;
}
/*---------------------------------------------------------------------
 * Method: pwospf_lock
 *
 * Lock mutex associated with pwospf_subsys
 *
 *---------------------------------------------------------------------*/

void pwospf_lock(struct pwospf_subsys* subsys)
{
    if ( pthread_mutex_lock(&subsys->lock) )
    { assert(0); }
} /* -- pwospf_subsys -- */

/*---------------------------------------------------------------------
 * Method: pwospf_unlock
 *
 * Unlock mutex associated with pwospf subsystem
 *
 *---------------------------------------------------------------------*/

void pwospf_unlock(struct pwospf_subsys* subsys)
{
    if ( pthread_mutex_unlock(&subsys->lock) )
    { assert(0); }
} /* -- pwospf_subsys -- */

/*---------------------------------------------------------------------
 * Method: pwospf_run_thread
 *
 * Main thread of pwospf subsystem.
 *
 *---------------------------------------------------------------------*/

static void* pwospf_run_thread(void* arg)
{
    return NULL;
} /* -- run_ospf_thread -- */


void setUpPacket(uint8_t* packet,struct sr_if * list, int length, int type){
    
    struct sr_ethernet_hdr* header=(struct sr_ethernet_hdr *) packet ;
    struct ip* ip=((struct ip*)(packet + sizeof(struct sr_ethernet_hdr)));
    struct ospfv2_hdr* ospf=(struct ospfv2_hdr*)(packet+ sizeof(struct ip)+sizeof(struct sr_ethernet_hdr));
    
    for (int i = 0; i < ETHER_ADDR_LEN; i++)
    {
        header->ether_dhost[i] =  htons(0xff);
    }
    for (int i = 0; i < ETHER_ADDR_LEN; i++)
    {
        header->ether_shost[i] = list->addr[i];
    }
    header->ether_type = htons(ETHERTYPE_IP);
    ip->ip_v = 4;
    ip->ip_hl = 5;
    ip->ip_tos = 0;
    ip->ip_id = htons(rand());
    ip->ip_off = htons(IP_DF);
    
    if (type==OSPF_TYPE_HELLO) {
        ip->ip_len = htons(sizeof(struct ip) + sizeof(struct ospfv2_hdr) + (sizeof(struct ospfv2_hello_hdr)));
    }
    else{
        ip->ip_len = htons(sizeof(struct ip) + sizeof(struct ospfv2_hdr) + sizeof(struct ospfv2_lsu_hdr) + (sizeof(struct ospfv2_lsu) * 3));
    }
    
    ip->ip_ttl = 64;
    ip->ip_sum=0;
    ip->ip_sum=cksum(((uint8_t*)(ip)),sizeof(struct ip));
    ip->ip_src.s_addr = list->ip;
    ip->ip_dst.s_addr = htonl(OSPF_AllSPFRouters);
    ip->ip_p = 89;
    ospf->version = OSPF_V2;
    ospf->type = type;
    if (type==OSPF_TYPE_HELLO) {
        ospf->len = htons(length - sizeof(struct sr_ethernet_hdr) - sizeof(struct ip));
    }
    else{
        ospf->len = htons(sizeof(struct ospfv2_hdr) + sizeof(struct ospfv2_lsu_hdr) + (sizeof(struct ospfv2_lsu) * 3));
    }
    ospf->aid=htonl(0);
    ospf->csum=0;
    ospf->csum=cksum(((uint8_t*)(ospf)),sizeof(struct ospfv2_hdr));
    ospf->autype = 0;
    ospf->audata = 0;
    if (strcmp(list->name, "eth0") == 0) {
        ospf->rid = list->ip;
    }
    
}


void * sendHello(void* arg)
{
    while (1)
    {
        struct sr_instance* sr = (struct sr_instance *)arg;
        pthread_mutex_lock(&(sr->ospf_subsys->lock));
        struct sr_if *ifaceList = sr->if_list;
        while(ifaceList){
            uint8_t* packet=((uint8_t*)(malloc(len)));
            setUpPacket(packet,ifaceList,len,OSPF_TYPE_HELLO);
            struct ospfv2_hello_hdr* hello=(struct ospfv2_hello_hdr*)(packet + sizeof(struct sr_ethernet_hdr)+sizeof(struct ip)+ sizeof(struct ospfv2_hdr));
            hello->helloint = htons(OSPF_DEFAULT_HELLOINT);
            hello->padding = 0;
            sr_send_packet(sr,packet, len,ifaceList->name);
            free(packet);
            ifaceList=ifaceList->next;
        }
        pthread_mutex_unlock(&(sr->ospf_subsys->lock));
        sleep(OSPF_DEFAULT_HELLOINT);
    }
    return NULL;
}


struct neighbours * findIP(uint32_t ipAddr)
{
    struct neighbours *temp = neighbourBoss;
    while (temp) {
        if (temp->ip == ipAddr)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

struct neighbours * findRouterID(uint32_t ipAddr)
{
    struct neighbours *temp = neighbourBoss;
    while (temp) {
        if (temp->routerId == ipAddr)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void handleLsuPackets(struct sr_instance *sr, uint8_t *packet, struct sr_if *iface)
{
    printf("\n LSU Packet Received==============================");
    
    struct ip *ip = ( struct ip *)(packet + sizeof(struct sr_ethernet_hdr));
    struct ospfv2_hdr *ospf = (struct ospfv2_hdr *)(packet + sizeof(struct sr_ethernet_hdr) + sizeof( struct ip));
    struct ospfv2_lsu_hdr *lsu = (struct ospfv2_lsu_hdr *)(packet + sizeof(struct sr_ethernet_hdr) + sizeof( struct ip) + sizeof(struct ospfv2_hdr));
    struct ospfv2_lsu *array = (struct ospfv2_lsu *)(packet + sizeof(struct sr_ethernet_hdr) + sizeof( struct ip) + sizeof(struct ospfv2_hdr) + sizeof(struct ospfv2_lsu_hdr));
    
    for (int i = 0; i < 3; i++)
    {
        if (findVertex(ospf->rid, array[i].subnet)==NULL)
        {
            addVertex(ospf->rid, array[i].subnet, array[i].mask, array[i].rid, ip->ip_src.s_addr, lsu->seq);
        }
    }
    
    updateRTable(sr);
    
}


struct neighbours * findSpot(){
    
    if (neighbourBoss==NULL) {
        return neighbourBoss;
    }
    else{
        struct neighbours *temp=neighbourBoss;
        while (temp->next!=NULL) {
            temp=temp->next;
        }
        return temp->next;
    }
    
}

void addNewNeighbor(struct sr_instance *sr, uint8_t *packet, struct sr_if *iface){
    
    struct ip * ip = ((struct ip*)(packet + sizeof(struct sr_ethernet_hdr)));
    struct ospfv2_hdr *ospf = (struct ospfv2_hdr*)(packet + sizeof(struct sr_ethernet_hdr) + sizeof( struct ip));
    struct ospfv2_hello_hdr  *hello = (struct ospfv2_hello_hdr *)(packet + sizeof(struct sr_ethernet_hdr) + sizeof( struct ip) + sizeof(struct ospfv2_hdr));
    time_t curr_time = time(NULL);
    
    struct neighbours *temp=findRouterID(ospf->rid);
    if (neighbourBoss==NULL) {
        printf("\n   to%s",inet_ntoa(*(struct in_addr*)(&ip->ip_src.s_addr)));
        printf("\n   to%s",iface->name);
        printf("\n   to%s",inet_ntoa(*(struct in_addr*)(&ospf->rid)));
        neighbourBoss = (struct neighbours*)malloc(sizeof(struct neighbours));
        neighbourBoss->ip = ip->ip_src.s_addr;
        neighbourBoss->mask = hello->nmask;
        neighbourBoss->lastUpdate = curr_time;
        neighbourBoss->interFace = iface->name;
        neighbourBoss->routerId = ospf->rid;
        neighbourBoss->next = NULL;
        struct sr_if *inter=matchInterface(sr,iface->name);
        if (inter) {
            inter->neighborId = ospf->rid;
            inter->neighborIp = ip->ip_src.s_addr;
        }
        //setUpInterface(sr,iface,ip,ospf);
    }
    else if (temp==NULL && neighbourBoss!=NULL){
        printf("\n  2 to%s",inet_ntoa(*(struct in_addr*)(&ip->ip_src.s_addr)));
        printf("\n  2 to%s",iface->name);
        printf("\n  2 to%s",inet_ntoa(*(struct in_addr*)(&ospf->rid)));
        temp = neighbourBoss;
        while (temp->next)
            temp = temp->next;
        temp->next = (struct neighbours *)malloc(sizeof(struct neighbours));
        
        temp->next->ip = ip->ip_src.s_addr;
        temp->next->mask = hello->nmask;
        temp->next->lastUpdate = curr_time;
        temp->next->interFace = iface->name;
        temp->next->routerId = ospf->rid;
        temp->next->next = NULL;
        struct sr_if *inter=matchInterface(sr,iface->name);
        if (inter) {
            inter->neighborId = ospf->rid;
            inter->neighborIp = ip->ip_src.s_addr;
        }
        //setUpInterface(sr,iface,ip,ospf);
    }
    else{
        
        temp=findIP(ip->ip_src.s_addr);
        if (temp!=NULL) {
            temp->lastUpdate = curr_time;
        }
    }
    
    
    
}
void handleHelloPackets(struct sr_instance *sr, uint8_t *packet, struct sr_if *iface)
{
    printf("\n Hello Packet Received==============================");
    addNewNeighbor(sr,packet,iface);
}


void * sendLSU(void* arg)
{
    while (1)
    {
        struct sr_instance* sr = (struct sr_instance *)arg;
        pthread_mutex_lock(&(sr->ospf_subsys->lock));
        struct sr_if *ifaceList = sr->if_list;
        //if(neighbourBoss){
        while(ifaceList){
            
            uint8_t* packet = (uint8_t*)malloc(lenPacketLSU);
            setUpPacket(packet,ifaceList,len,OSPF_TYPE_LSU);
            
            struct ospfv2_lsu_hdr* lsu = (struct ospfv2_lsu*) (packet + sizeof(struct sr_ethernet_hdr)+ sizeof(struct ip) + sizeof(struct ospfv2_hdr)+ (sizeof(struct ospfv2_lsu_hdr)*3));//*3?
            
            lsu->unused = 0;
            lsu->ttl = 64;
            seqnum++;
            lsu->seq = seqnum;
            if(ifaceList->neighborId!=1)
            {
                struct sr_if* inter = sr->if_list;
                struct ospfv2_lsu* lsuArray = ((struct ospfv2_lsu*)(malloc(3 * sizeof(struct ospfv2_lsu))));
                int j = 0;
                while (inter) {
                    uint32_t tempmask =inter->ip & inter->mask;
                    lsuArray[j].subnet =tempmask ;
                    lsuArray[j].mask = inter->mask;
                    lsuArray[j].rid = inter->neighborId;
                    j++;
                    inter = inter->next;
                }

                memcpy(packet + sizeof(struct sr_ethernet_hdr) + sizeof(struct ip) + sizeof(struct ospfv2_hdr) + sizeof(struct ospfv2_lsu_hdr),lsuArray, 3 * sizeof(struct ospfv2_lsu));
                sr_send_packet(sr, packet,lenPacketLSU, ifaceList->name);
                
            }
            
            free(packet);
            ifaceList=ifaceList->next;
        }
        //}
        pthread_mutex_unlock(&(sr->ospf_subsys->lock));
        sleep(OSPF_DEFAULT_LSUINT);
    }
    return NULL;
    
    
}


void deleteNeighbour(struct sr_instance *sr,struct neighbours * target)
{
    
    struct neighbours *temp = neighbourBoss;
    struct neighbours *previous=NULL;
    struct sr_if *inter = matchInterface(sr,target->interFace);
    if (inter) {
        inter->neighborId=1;
    }
    
    if(target==neighbourBoss)
    {
        neighbourBoss=neighbourBoss->next;
        free(target);
        return;
    }
    else{
        
        while(temp)
        {
            if(temp->ip==target->ip)
            {
                previous->next= target->next;
                free(target);
                return;
            }
            previous=temp;
            temp=temp->next;
        }
    }
}


void addVertex(uint32_t routerId, uint32_t subnet, uint32_t mask, uint32_t id, uint32_t nextHop, uint16_t sequence)
{
    
    if (vertexList == NULL)
    {
        vertexList = malloc(sizeof(struct vertex));
        vertexList->routerId = routerId;
        vertexList->subnet = subnet;
        vertexList->mask = mask;
        vertexList->id = id;
        vertexList->nextHop = nextHop;
        vertexList->sequence = sequence;
        
        vertexList->next = NULL;
    }
    else
    {
        struct vertex *temp = vertexList;
        while (temp->next)
            temp = temp->next;
        temp->next = (malloc)(sizeof(struct vertex));
        temp->next->routerId = routerId;
        temp->next->subnet = subnet;
        temp->next->mask = mask;
        temp->next->id = id;
        temp->next->nextHop = nextHop;
        temp->next->sequence = sequence;
        
        temp->next->next = NULL;
    }
    
}


struct vertex *  findVertex(uint32_t rid, uint32_t subnet)
{
    struct vertex *temp = vertexList;
    while (temp)
    {
        if (temp->routerId== rid && temp->subnet== subnet)
            return temp;
        temp = temp->next;
    }
    return NULL;
}



void updateRTable(struct sr_instance *sr)
{
    printf("Updating!!!!!!\n");
    struct vertex *temp = vertexList;
    
    while (temp)
    {
        char *interface = longestMatch(sr, temp->nextHop);
        
        if (interface && RoutingTableLookUp(sr, temp->subnet, temp->nextHop)==NULL)
        {
            printf("ADDING ENTRY2!!!!!!\n");
            populateRoutingTable(sr, temp->subnet, temp->nextHop, temp->mask, interface);
            sr_print_routing_table(sr);
        }
        
        temp = temp->next;
    }
    struct sr_if *inter = sr->if_list;
    while (inter)
    {
        bool found = doesInterfaceExist(sr, inter->name);
        if (!found)
            break;
        inter = inter->next;
    }
    if (inter){
        printf("ADDING ENTRY!!!!!!\n");
        populateRoutingTable(sr, (inter->ip & inter->mask), 0, inter->mask, inter->name);
        //sr_print_routing_table(sr);
    }
    addDefaultEntry(sr);
}

void addDefaultEntry(struct sr_instance *sr) {
    
    struct sr_rt* rtable = sr->routing_table;
    
    while (rtable) {
        if (rtable->dest.s_addr == 0) {
            return;
        }
        rtable = rtable->next;
    }
    struct sr_if* inter = matchInterface(sr,"eth0");
    
    populateRoutingTable(sr, 0, inter->neighborIp, 0, inter->name);
}

char* longestMatch(struct sr_instance *sr, uint32_t destination)
{
    if (destination == 0)
        return NULL;
    struct sr_if *temp = sr->if_list;
    uint32_t max = 0;
    char * string;
    while (temp)
    {
        uint32_t value = temp->ip & destination;
        if (value > max) {
            max = value;
            string = temp->name;
        }
        temp = temp->next;
    }
    
    return string;
    
    
}

void populateRoutingTable(struct sr_instance *sr, uint32_t destination, uint32_t nextHop, uint32_t mask, char *interface)
{
    
    struct in_addr dest_addr;
    struct in_addr gw_addr;
    struct in_addr mask_addr;
    
    dest_addr.s_addr = destination;
    gw_addr.s_addr = nextHop;
    mask_addr.s_addr = mask;
    
    sr_add_rt_entry(sr,dest_addr,gw_addr,mask_addr,interface);
}



