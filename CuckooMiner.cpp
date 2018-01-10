#include "CuckooMiner.h"

// assume EDGEBITS < 31
#define MAXPATHLEN 8192

CuckooMiner::CuckooMiner(const string &header, uint edge_precentage) {
	assert(edge_precentage <= 100);
	this->edges_num = edge_precentage * (u64)NNODES / 100;
	setKeysFromHeader(header, &sip_keys);
	cuckoo_table = (node_t *)calloc(1 + NNODES, sizeof(node_t));  //2^27 +1
	assert(cuckoo_table != 0);
}

void
CuckooMiner::makeMinning() {
node_t even_path_to_root[MAXPATHLEN], odd_path_to_root[MAXPATHLEN];
for (node_t nonce = 0; nonce < edges_num; nonce++) {
	node_t even_node_index = sipnode(&sip_keys, nonce, 0);
	if (even_node_index == 0) continue; // reserve 0 as nil; odd_node_index guaranteed non-zero
	node_t odd_node_index = sipnode(&sip_keys, nonce, 1);
	node_t value_from_even_table = cuckoo_table[even_node_index];
	node_t value_from_odd_table = cuckoo_table[odd_node_index];
	even_path_to_root[0] = even_node_index;
	odd_path_to_root[0] = odd_node_index;
#ifdef SHOW
	for (unsigned j = 1; j<NNODES; j++)
		if (!cuckoo_table[j]) printf("%2d:   ", j);
		else           printf("%2d:%02d ", j, cuckoo_table[j]);
		printf(" %x (%d,%d)\n", nonce, *even_path_to_root, *odd_path_to_root);
#endif
		int even_root_index = path(cuckoo_table, value_from_even_table, even_path_to_root);  //finding the path to the root, save in even_path_to_root from node to its root, return its index in even_path_to_root/odd_path_to_root
		int odd_root_index = path(cuckoo_table, value_from_odd_table, odd_path_to_root);
		if (even_path_to_root[even_root_index] == odd_path_to_root[odd_root_index]) {  // found cycle
			int min = even_root_index < odd_root_index ? even_root_index : odd_root_index;
			for (even_root_index -= min, odd_root_index -= min; even_path_to_root[even_root_index] != odd_path_to_root[odd_root_index]; even_root_index++, odd_root_index++);  //for calculating length
			int len = even_root_index + odd_root_index + 1;
			printf("% 4d-cycle found\n", len);
			if (len == PROOFSIZE)
				solution(ctx, even_path_to_root, even_root_index, odd_path_to_root, odd_root_index);
			continue;
		}
		if (even_root_index < odd_root_index) {  //revrese shorter path
			while (even_root_index--)
				cuckoo_table[even_path_to_root[even_root_index + 1]] = even_path_to_root[even_root_index];
			cuckoo_table[even_node_index] = odd_node_index;
		}
		else {
			while (odd_root_index--)
				cuckoo_table[odd_path_to_root[odd_root_index + 1]] = odd_path_to_root[odd_root_index];
			cuckoo_table[odd_node_index] = even_node_index;
		}
}
}