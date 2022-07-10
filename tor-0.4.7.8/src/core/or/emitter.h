#ifndef EMITTER_H_
#define EMITTER_H_

#include "core/or/cell_st.h"
#include "core/or/cell_queue_st.h"
#include "core/or/channel.h"
#include "core/or/clpacket.h"

#define DSTPORT "7000"

int get_socket(void);
void fill_icell_clpacket(struct clpacket *clpkt, cell_t *cell);
void fill_ocell_clpacket(struct clpacket *clpkt, channel_t *chan, packed_cell_t *cell);
void send_clpacket(struct clpacket *clpkt);

#endif
