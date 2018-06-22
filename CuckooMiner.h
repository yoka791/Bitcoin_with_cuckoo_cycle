#ifndef _CUCKOO_MINER_H
#define _CUCKOO_MINER_H
#include <string>
#include <array>
#include "cuckoo.h"
#include <assert.h>
#include <list>
#include <vector>
#include <time.h>
#include <iostream>
#include <fstream>
#include <set>
using namespace std;

class CuckooMiner {
private:
	void makeMinning();
	uint path(node_t *cuckoo_table, node_t value_from_table, node_t *path_arr);
	void genSolution(node_t *even_path_to_root, uint even_root_index, node_t *odd_path_to_root, uint odd_root_index);

public:
	explicit CuckooMiner(const std::string &header, uint edge_precentage, string _csv_file_path);
    ~CuckooMiner();
	bool isSolutionFound() const;
	bool getSolution(std::array<edge_t, 42> &sol_arr) const;

private:
	bool is_solution_found = false;
	std::array<edge_t, 42> solution_arr;
	siphash_keys sip_keys;
	u64 edges_num;
    ofstream csv_file;
    string csv_file_path;
    node_t *cuckoo_table;
    clock_t start = clock();
};
#endif //_CUCKOO_MINER_H

