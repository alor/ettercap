/*
    ettercap -- ETH decoder module

    Copyright (C) ALoR & NaGA

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

    $Id: ec_eth.c,v 1.5 2003/09/18 22:15:04 alor Exp $
*/

#include <ec.h>
#include <ec_decode.h>

/* globals */

struct eth_header
{
   u_int8   dha[ETH_ADDR_LEN];       /* destination eth addr */
   u_int8   sha[ETH_ADDR_LEN];       /* source ether addr */
   u_int16  proto;                   /* packet type ID field */
};


/* protos */

FUNC_DECODER(decode_eth);
void eth_init(void);

/*******************************************/

/*
 * this function is the initializer.
 * it adds the entry in the table of registered decoder
 */

void __init eth_init(void)
{
   add_decoder(LINK_LAYER, IL_TYPE_ETH, decode_eth);
   add_iface_mtu(IL_TYPE_ETH, 1500);
}


FUNC_DECODER(decode_eth)
{
   FUNC_DECODER_PTR(next_decoder);
   struct eth_header *eth;

   DECODED_LEN = sizeof(struct eth_header);
   
   eth = (struct eth_header *)DECODE_DATA;

   /* fill the packet object with sensitive data */
   PACKET->L2.header = (u_char *)DECODE_DATA;
   PACKET->L2.proto = htons(IL_TYPE_ETH);
   PACKET->L2.len = DECODED_LEN;
   
   memcpy(PACKET->L2.src, eth->sha, ETH_ADDR_LEN);
   memcpy(PACKET->L2.dst, eth->dha, ETH_ADDR_LEN);

   /* HOOK POINT : PACKET_ETH */
   hook_point(PACKET_ETH, po);
   
   /* leave the control to the next decoder */   
   next_decoder = get_decoder(NET_LAYER, ntohs(eth->proto));

   EXECUTE_DECODER(next_decoder);
      
   /* eth header don't care about modification of upper layer */
   
   return NULL;
}

/* EOF */

// vim:ts=3:expandtab

