
/* $Id: ec_packet.h,v 1.17 2003/09/18 22:15:01 alor Exp $ */

#if !defined(EC_PACKET_H)
#define EC_PACKET_H

#include <ec_proto.h>
#include <ec_profiles.h>
#include <ec_fingerprint.h>
#include <ec_inet.h>

#include <sys/time.h>

struct packet_object {
 
   /* timestamp of the packet */
   struct timeval ts;
   
   struct L2 {
      u_int16 proto;
      u_char * header;
      size_t len;
      u_int8 src[ETH_ADDR_LEN];
      u_int8 dst[ETH_ADDR_LEN];
   } L2;
   
   struct L3 {
      u_int16 proto;
      u_char * header;
      u_char * options;
      size_t len;
      size_t payload_len;
      size_t optlen;
      struct ip_addr src;
      struct ip_addr dst;
      u_int8 ttl;
   } L3;
   
   struct L4 {
      u_int8 proto;
      u_int8 flags;
      u_char * header;
      u_char * options;
      size_t len;
      size_t optlen;
      u_int16 src;
      u_int16 dst;
   } L4;
   
   struct data {
      u_char * data;
      size_t len;
      /* 
       * buffer containing the data to be displayed.
       * some dissector decripts the traffic, but the packet must be forwarded as
       * is, so the decripted data must be placed in a different buffer. 
       * this is that bufffer and it is malloced by tcp or udp dissector.
       */
      size_t disp_len;
      u_char * disp_data;
      /* for modified packet this is the delta for the lenght */
      int delta;  
   } DATA;

   size_t fwd_len;         /* lenght of the packet to be forwarded */
   u_char * fwd_packet;    /* the pointer to the buffer to be forwarded */
   
   size_t len;             /* total lenght of the packet */
   u_char * packet;        /* the buffer containing the real packet */

   /* 
    * chain of packets created by the filter engine 
    * when the filtered data exceeded the mtu.
    * we have to inject some packet after the replaced one
    */
   struct packet_object *inject; 
   
   u_int8 flags;                       /* flags relative to the packet */
      #define PO_IGNORE       ((u_int8)(1))        /* this packet should not be processed (e.g. sniffing filter didn't match it) */
      #define PO_FORWARDABLE  ((u_int8)(1<<1))     /* the packet has our MAC address, by the IP is not ours */
      
      #define PO_FROMIFACE    ((u_int8)(1<<2))     /* this packet comes from the primary interface */
      #define PO_FROMBRIDGE   ((u_int8)(1<<3))     /* this packet comes form the bridged interface */
      
      #define PO_MODIFIED     ((u_int8)(1<<4))     /* it needs checksum recalculation before forwarding */
      #define PO_DROPPED      ((u_int8)(1<<5))     /* the packet has to be dropped */
  
      #define PO_DUP          ((u_int8)(1<<6))     /* the packet is a duplicate we have to free the buffer on destroy */
      
      #define PO_EOF          ((u_int8)(1<<7))     /* we are reading from a file and this is the last packet */
   
   /* 
    * here are stored the user and pass collected by dissectors 
    * the "char *" are malloc(ed) by dissectors
    */
   struct dissector_info DISSECTOR;
  
   /* the struct for passive identification */
   struct passive_info PASSIVE;
   
};

extern inline int packet_create_object(struct packet_object *po, u_char * buf, size_t len);
extern inline int packet_destroy_object(struct packet_object *po);
extern int packet_disp_data(struct packet_object *po, u_char *buf, size_t len);
extern struct packet_object * packet_dup(struct packet_object *po);

extern void packet_print(struct packet_object *po);

#endif

/* EOF */

// vim:ts=3:expandtab

