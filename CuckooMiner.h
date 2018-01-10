#ifndef _CUCKOO_MINER_H
#define _CUCKOO_MINER_H
#include <string>
#include <array>
#include "cuckoo.h"
#include <assert.h>

using namespace std;
class CuckooMiner {
private:
	void genSolution();
public:
	explicit CuckooMiner(const string &header, uint edge_precentage);
	void makeMinning();
	bool isSolutionFound();
	bool getSolution(array<edge_t, 42> &sol_arr);

private:
	bool is_solution_found = false;
	array<edge_t, 42> solution;
	siphash_keys sip_keys;
	u64 edges_num;
	node_t *cuckoo_table;
};
#endif //_CUCKOO_MINER_H

