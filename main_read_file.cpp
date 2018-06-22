#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include "CuckooMiner.h"

using namespace std;
//SHOW;SHOW_CYCLES_NONCES;PROOFSIZE=2;NODEBITS=4;%(PreprocessorDefinitions)

int main(int argc, char **argv) {
	string line;
	//ifstream myfile("/home/admin1/test_file.txt");
	ifstream myfile("/home/admin1/final_project/test_file_200.txt");
	ofstream csv_file;
	//csv_file.open("/home/admin1/final_project/data_500MB.csv");
	if (myfile.is_open()){
		while (getline(myfile, line))
		{
			CuckooMiner cuc_miner(line, 70, csv_file);
			if (cuc_miner.isSolutionFound()) {
				array<edge_t, 42> solution_arr;
				cuc_miner.getSolution(solution_arr);
			}
		}
		myfile.close();
	} else cout << "Unable to open file";
	
}
