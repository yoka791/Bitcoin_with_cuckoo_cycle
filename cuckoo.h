#ifndef _CUCKOO_H
#define _CUCKOO_H

#include <stdint.h> // for types uint32_t,uint64_t
#include <string.h> // for functions strlen, memset
#include "siphash.h"
#include "sha256.h"

// proof-of-work parameters
#ifndef NODEBITS
// the main parameter is the 2-log of the graph size,
// which is the size in bits of the node identifiers
#define NODEBITS 22
#endif
#ifndef PROOFSIZE
// the next most important parameter is the (even) length
// of the cycle to be found. a minimum of 12 is recommended
#define PROOFSIZE 42
#endif

#if NODEBITS > 31
typedef u64 node_t;
typedef u64 edge_t;

#else
typedef u32 node_t;
typedef u32 edge_t;
#endif

// number of edges
#define NNODES ((node_t)1 << NODEBITS)
#define NNODES_2 (NNODES >> (node_t)1)
// used to mask siphash output
#define MODULU ((edge_t)NNODES_2 - 1)

// generate edge endpoint in cuckoo graph without partition bit
edge_t _sipnode(siphash_keys *keys, edge_t nonce, u32 uorv);

// generate edge endpoint in cuckoo graph
node_t sipnode(siphash_keys *keys, edge_t nonce, u32 uorv);

// convenience function for extracting siphash keys from header
void setKeysFromHeader(const std::string &header, siphash_keys *keys);
#endif //_CUCKOO_H
