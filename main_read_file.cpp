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

int main(int argc, char **argv) {
    string headers_file_path;
    string csv_file_path;
    int c;
    while ((c = getopt(argc, argv, "f:d:")) != -1) {
        switch (c) {
        case 'f':
            headers_file_path = optarg;
            break;
        case 'd':
            csv_file_path = optarg;
            break;
        }
    }

    ofstream csv_file(csv_file_path);
    if (!csv_file.is_open()) {
        cout << "Unable to open csv file";
        return 1;
    }
    csv_file.close();

    ifstream myfile(headers_file_path);
    if (!myfile.is_open()) {
        cout << "Unable to open headers file";
        return 1;
    }
    
    string line;
	while (getline(myfile, line))
	{
		CuckooMiner cuc_miner(line, 70, csv_file_path);
		if (cuc_miner.isSolutionFound()) {
			array<edge_t, PROOFSIZE> solution_arr;
			cuc_miner.getSolution(solution_arr);
            if (verify(solution_arr, line, 70)) {
                printf("solution verified\n");
            } else {
                printf("error - solution not verified\n");
                return 1;
            }
		}
	}
	myfile.close();
	
}
