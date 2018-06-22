#include "CuckooMiner.h"
#include <limits.h>

// assume EDGEBITS < 31
#define MAXPATHLEN 8192
CuckooMiner::CuckooMiner(const std::string &header, uint edge_precentage, string _csv_file_path)
{
	assert(edge_precentage <= 100);
        csv_file_path = _csv_file_path;
	this->edges_num = edge_precentage * (u64)NNODES / 100;
	setKeysFromHeader(header, &sip_keys);
	cuckoo_table = (node_t *)calloc(1 + NNODES, sizeof(node_t));  //2^27 +1
	assert(cuckoo_table != 0);
	makeMinning();
}

CuckooMiner::~CuckooMiner()
{
    free(cuckoo_table);
}

void
CuckooMiner::genSolution(node_t *even_path_to_root, uint even_root_index, node_t *odd_path_to_root, uint odd_root_index) {
	typedef std::pair<node_t, node_t> edge;
	std::set<edge> cycle;
	unsigned n;
	cycle.insert(edge(*even_path_to_root, *odd_path_to_root));
	while (even_root_index--) {
		node_t node1_even = even_path_to_root[(even_root_index + 1)&~1];
		node_t node2_even = even_path_to_root[even_root_index | 1];
		cycle.insert(edge(node1_even, node2_even));
	}
	while (odd_root_index--) {
		node_t node1_odd = odd_path_to_root[odd_root_index | 1];
		node_t node2_odd = odd_path_to_root[(odd_root_index + 1)&~1];
		cycle.insert(edge(node1_odd, node2_odd)); // u's in odd position; v's in even
	}
	//printf("Solution");
	uint counter = 0;
	for (edge_t nonce = n = 0; (nonce < edges_num) && (counter < PROOFSIZE); nonce++) {
		edge e(sipnode(&sip_keys, nonce, 0), sipnode(&sip_keys, nonce, 1));
		if (cycle.find(e) != cycle.end()) {
			#ifdef SHOW_CYCLES_NONCES
			printf(" 0x%x, ", nonce);
			#endif
			if (PROOFSIZE != 2) cycle.erase(e);
			solution_arr[counter] = nonce;
			counter++;
		}
	}
	//printf("\n");
    clock_t stop = clock();
    int elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
	int minutes = (int) ((elapsed / (1000*60)) % 60);
	int seconds = (int) (elapsed / 1000) % 60 ;
	double pre = ((double)solution_arr[41]/(edge_t)NNODES)*100.0;
	printf("Solution found: Time elapsed is %dm%ds, edge_precentage is: %f\n", minutes, seconds, pre);
        csv_file.open(csv_file_path, std::ios::app);
	csv_file << minutes <<":" << seconds << ", " << pre << "\n";
        csv_file.close();
}

uint
CuckooMiner::path(node_t *cuckoo_table, node_t value_from_table, node_t *path_arr)
{
	int path_counter;
	for (path_counter = 0; value_from_table != 0; value_from_table = cuckoo_table[value_from_table]) {
		if (++path_counter >= MAXPATHLEN) {
			while (path_counter-- && path_arr[path_counter] != value_from_table);
			if (path_counter < 0)
				printf("maximum path length exceeded\n");
			else printf("illegal % 4d-cycle\n", MAXPATHLEN - path_counter);
			exit(0);
		}
		path_arr[path_counter] = value_from_table;
	}
	return path_counter;
}

void
CuckooMiner::makeMinning()
{
	node_t even_path_to_root[MAXPATHLEN], odd_path_to_root[MAXPATHLEN];
	for (node_t nonce = 0; nonce < edges_num; nonce++) {
		node_t even_node_index = sipnode(&sip_keys, nonce, 0);
		if (even_node_index == 0) continue; // reserve 0 as nil; odd_node_index guaranteed non-zero
		node_t odd_node_index = sipnode(&sip_keys, nonce, 1);
		node_t value_from_even_table = cuckoo_table[even_node_index];
		node_t value_from_odd_table = cuckoo_table[odd_node_index];
		even_path_to_root[0] = even_node_index;
		odd_path_to_root[0] = odd_node_index;
	#ifdef SHOW_GRAPTH
		for (unsigned j = 1; j<NNODES; j++)
			if (!cuckoo_table[j]) printf("%2d:   ", j);
			else           printf("%2d:%02d ", j, cuckoo_table[j]);
			printf(" %x (%d,%d)\n", nonce, *even_path_to_root, *odd_path_to_root);
	#endif
			uint even_root_index = path(cuckoo_table, value_from_even_table, even_path_to_root);  //finding the path to the root, save in even_path_to_root from node to its root, return its index in even_path_to_root/odd_path_to_root
			uint odd_root_index = path(cuckoo_table, value_from_odd_table, odd_path_to_root);
			if (even_path_to_root[even_root_index] == odd_path_to_root[odd_root_index]) {  // found cycle
				uint min = even_root_index < odd_root_index ? even_root_index : odd_root_index;
				for (even_root_index -= min, odd_root_index -= min;
					 even_path_to_root[even_root_index] != odd_path_to_root[odd_root_index];
					 even_root_index++, odd_root_index++);  //for calculating length
				int len = even_root_index + odd_root_index + 1;
	#ifdef SHOW_CYCLES
				printf("% 4d-cycle found\n", len);
	#endif
				if (len == PROOFSIZE) {
					genSolution(even_path_to_root, even_root_index, odd_path_to_root, odd_root_index);
					is_solution_found = true;
					return;
				}
				continue;
			}
			//reverse shorter path and add edge to table
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
bool
CuckooMiner::isSolutionFound() const
{
	return is_solution_found;
}

bool
CuckooMiner::getSolution(std::array<edge_t, 42> &sol_arr) const 
{
	if (!is_solution_found) return false;
	for (uint i = 0; i < 42; i++) {
		sol_arr[i] = solution_arr[i];
	}
	return true;
}
