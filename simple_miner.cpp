// Cuckoo Cycle, a memory-hard proof-of-work
// Copyright (c) 2013-2016 John Tromp

#include "cuckoo.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <set>

// assume EDGEBITS < 31
#define NNODES (2 * NEDGES)
#define MAXPATHLEN 8192

class cuckoo_ctx {
public:
  siphash_keys sip_keys;
  edge_t easiness;
  node_t *cuckoo;

  cuckoo_ctx(const char* header, const u32 headerlen, const u32 nonce, edge_t easy_ness) {
    ((u32 *)header)[headerlen/sizeof(u32)-1] = htole32(nonce); // place nonce at end and convert to little endian
    setheader(header, headerlen, &sip_keys);
    easiness = easy_ness;
    cuckoo = (node_t *)calloc(1+NNODES, sizeof(node_t));  //2^27 +1
    assert(cuckoo != 0);
  }
  ~cuckoo_ctx() {
    free(cuckoo);
  }
};
//int path(node_t *cuckoo, node_t u - value from table, node_t *us - us[0] original index) {
int path(node_t *cuckoo, node_t u, node_t *us) {
  int nu;
  for (nu = 0; u; u = cuckoo[u]) {
    if (++nu >= MAXPATHLEN) {
      while (nu-- && us[nu] != u) ;
      if (nu < 0)
        printf("maximum path length exceeded\n");
      else printf("illegal % 4d-cycle\n", MAXPATHLEN-nu);
      exit(0);
    }
    us[nu] = u;
  }
  return nu;
}

typedef std::pair<node_t,node_t> edge;

void solution(cuckoo_ctx *ctx, node_t *us, int nu, node_t *vs, int nv) {
  std::set<edge> cycle;
  unsigned n;
  cycle.insert(edge(*us, *vs));
  while (nu--)
    cycle.insert(edge(us[(nu+1)&~1], us[nu|1])); // u's in even position; v's in odd
  while (nv--)
    cycle.insert(edge(vs[nv|1], vs[(nv+1)&~1])); // u's in odd position; v's in even
  printf("Solution");
  for (edge_t nonce = n = 0; nonce < ctx->easiness; nonce++) {
    edge e(sipnode(&ctx->sip_keys, nonce, 0), sipnode(&ctx->sip_keys, nonce, 1));
    if (cycle.find(e) != cycle.end()) {
      printf(" %x", nonce);
      cycle.erase(e);
    }
  }
  printf("\n");
}

void worker(cuckoo_ctx *ctx) {
  node_t *cuckoo = ctx->cuckoo;
  node_t even_path_to_root[MAXPATHLEN], odd_path_to_root[MAXPATHLEN];
  for (node_t nonce = 0; nonce < ctx->easiness; nonce++) {
    node_t even_node_index = sipnode(&ctx->sip_keys, nonce, 0);
    if (even_node_index == 0) continue; // reserve 0 as nil; odd_node_index guaranteed non-zero
    node_t odd_node_index = sipnode(&ctx->sip_keys, nonce, 1);
	node_t value_from_even_table = cuckoo[even_node_index];
	node_t value_from_odd_table = cuckoo[odd_node_index];
    even_path_to_root[0] = even_node_index;
    odd_path_to_root[0] = odd_node_index;
#ifdef SHOW
    for (unsigned j=1; j<NNODES; j++)
      if (!cuckoo[j]) printf("%2d:   ",j);
      else           printf("%2d:%02d ",j,cuckoo[j]);
    printf(" %x (%d,%d)\n", nonce,*even_path_to_root,*odd_path_to_root);
#endif
	int even_root_index = path(cuckoo, value_from_even_table, even_path_to_root);  //finding the path to the root, save in even_path_to_root from node to its root, return its index in even_path_to_root/odd_path_to_root
	int odd_root_index = path(cuckoo, value_from_odd_table, odd_path_to_root);
    if (even_path_to_root[even_root_index] == odd_path_to_root[odd_root_index]) {  // found cycle
      int min = even_root_index < odd_root_index ? even_root_index : odd_root_index;
      for (even_root_index -= min, odd_root_index -= min; even_path_to_root[even_root_index] != odd_path_to_root[odd_root_index]; even_root_index++, odd_root_index++) ;  //for calculating length
      int len = even_root_index + odd_root_index + 1;
      printf("% 4d-cycle found at %d%%\n", len, (int)(nonce*100L/ctx->easiness));
      if (len == PROOFSIZE)
        solution(ctx, even_path_to_root, even_root_index, odd_path_to_root, odd_root_index);
      continue;
    }
    if (even_root_index < odd_root_index) {  //revrese shorter path
      while (even_root_index--)
        cuckoo[even_path_to_root[even_root_index+1]] = even_path_to_root[even_root_index];
      cuckoo[even_node_index] = odd_node_index;
    } else {
      while (odd_root_index--)
        cuckoo[odd_path_to_root[odd_root_index+1]] = odd_path_to_root[odd_root_index];
      cuckoo[odd_node_index] = even_node_index;
    }
  }
}

// arbitrary length of header hashed into siphash key
#define HEADERLEN 80

int main(int argc, char **argv) {
  char header[HEADERLEN];
  memset(header, 0, HEADERLEN);
  int c, easipct = 50;
  u32 nonce = 0;
  while ((c = getopt (argc, argv, "e:h:")) != -1) {
    switch (c) {
      case 'e':
        easipct = atoi(optarg);
        break;
      case 'h':
        memcpy(header, optarg, strlen(optarg));
        break;
    }
  }
  assert(easipct >= 0 && easipct <= 100);
  printf("Looking for %d-cycle on cuckoo%d(\"%s\") with %d%% edges\n",
               PROOFSIZE, EDGEBITS+1, header, easipct);
  u64 easiness = easipct * (u64)NNODES / 100;
  cuckoo_ctx ctx(header, sizeof(header), nonce, easiness);

  worker(&ctx);
}
