#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "CuckooMiner.h"

using namespace std;
//SHOW;SHOW_CYCLES_NONCES;PROOFSIZE=2;NODEBITS=4;%(PreprocessorDefinitions)

int main(int argc, char **argv) {
	/*int c;
	string header = "hello world";
	uint edge_precentage  = 50;

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
	*/
	array<edge_t, 42> solution_arr;
	const std::string str = "0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206";
	CuckooMiner cuc_miner(str, 50);
	if (cuc_miner.isSolutionFound())
	{
		cuc_miner.getSolution(solution_arr);
		printf("nonces added to array\n");
		if (verify(solution_arr, str)) {
			printf("verified solution\n");
		} else {
			printf("error - solution not veified\n");
		}
	}

	////

	/*string line;
	ifstream myfile("/home/admin1/test_file.txt");
	if (myfile.is_open())
	{
	while (getline(myfile, line))
	{
	//cout << line << '\n';
	CuckooMiner cuc_miner(line, edge_precentage);
	if (cuc_miner.isSolutionFound()) {
	//printf("found solution\n");
	array<edge_t, 42> solution_arr;
	cuc_miner.getSolution(solution_arr);
	if (verify(solution_arr, line)) {
	printf("verified solution\n");
	}
	else {
	printf("error - solution not veified\n");
	}
	}
	else {
	//printf("solution not found\n");
	}
	}
	myfile.close();
	}

	else cout << "Unable to open file";
	*/
}