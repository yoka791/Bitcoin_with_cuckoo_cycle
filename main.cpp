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

	//assert(edge_precentage >= 0 && edge_precentage <= 100);

	array<edge_t, 42> solution_arr;
	const std::string str = "5b7a4494ac602f4ddfbd5fbd180a5d670978b765d487c3680a50f5c03572f600";
	CuckooMiner cuc_miner(str, 70);
	if (cuc_miner.isSolutionFound())
	{
		cuc_miner.getSolution(solution_arr);
		printf("nonces added to array\n");
		if (verify(solution_arr, str, 70)) {
			printf("verified solution\n");
		} else {
			printf("error - solution not veified\n");
		}
	}

	////
	
	/*
	string line;
	ifstream myfile("/home/admin1/test_file.txt");
	if (myfile.is_open())
	{
	while (getline(myfile, line))
	{
	//cout << line << '\n';
	CuckooMiner cuc_miner(line, 50);
	if (cuc_miner.isSolutionFound()) {
		//printf("found solution\n");
		array<edge_t, 42> solution_arr;
		cuc_miner.getSolution(solution_arr);
		if (verify(solution_arr, line, 50)) {
			//printf("verified solution\n");
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