#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <unistd.h>
#include "CuckooMiner.h"

using namespace std;

int main(int argc, char **argv) {
	int c;
	string header = "hello world";
	uint edge_precentage  = 70;

	while ((c = getopt(argc, argv, "e:h:")) != -1) {
		switch (c) {
		case 'e':
			edge_precentage = atoi(optarg);
			break;
		case 'h':
			header = optarg;
			break;
		}
	}
	assert(edge_precentage >= 0 && edge_precentage <= 100);
	printf("Looking for %d-cycle on cuckoo%d(\"%s\") with %d%% edges\n",
		PROOFSIZE, NODEBITS, header.c_str(), edge_precentage);

	array<edge_t, 42> solution_arr;
	CuckooMiner cuc_miner(header, edge_precentage);
	if (cuc_miner.isSolutionFound())
	{
		cuc_miner.getSolution(solution_arr);
		printf("nonces added to array\n");
		if (verify(solution_arr, header)) {
			printf("verified solution\n");
		}
	}
}