#include "cuckoo.h"
edge_t _sipnode(siphash_keys *keys, edge_t nonce, u32 uorv)
{
	edge_t res = siphash24(keys, 2*nonce + uorv) & MODULU;   //& for modolu
	return res;
}

node_t sipnode(siphash_keys *keys, edge_t nonce, u32 uorv)
{
	node_t res = _sipnode(keys, nonce, uorv) << 1 | uorv;   //2h(..,..)+1/0
	return res;
}

void setKeysFromHeader(const std::string &header, siphash_keys *keys)
{
  setkeys(keys, header.c_str());
}

bool verify(const std::array<edge_t, 42> solution_arr, const std::string& header, const uint &edge_precentage)
{
	siphash_keys sip_keys;
	setKeysFromHeader(header, &sip_keys);
	u64 edges_num = edge_precentage * (u64)NNODES / 100;
	node_t even_nodes[ PROOFSIZE];
	node_t odd_nodes[PROOFSIZE];
	node_t xor0 = 0, xor1 = 0;
	for (u32 n = 0; n < PROOFSIZE; n++) {
		if (solution_arr[n] > edges_num)  //no. of edge does not exist
			return false;
		if (n && solution_arr[n] <= solution_arr[n - 1])
			return false;
		xor0 ^= even_nodes[n] = sipnode(&sip_keys, solution_arr[n], 0);
		xor1 ^= odd_nodes[n] = sipnode(&sip_keys, solution_arr[n], 1);
	}
	if (xor0 | xor1)              // optional check for obviously bad proofs
		return false;

	node_t first_even_index = even_nodes[0];
	int index = 0;
	for (size_t i = 0; i < PROOFSIZE; i++)
	{
		if (i != 0) {
			node_t even_val_to_search = even_nodes[index];
			even_nodes[index] = 0;
			size_t j;
			for (j = 0; j < PROOFSIZE; j++)//even
			{
				if (even_nodes[j] == even_val_to_search) {
					index = j;
					break;
				}
			}
			if (j == PROOFSIZE) {
				return even_val_to_search == first_even_index ? true : false;
			}
		}
		even_nodes[index] = 0;

		node_t odd_val_to_search = odd_nodes[index];
		odd_nodes[index] = 0;
		size_t k;
		for (k = 0; k < PROOFSIZE; k++)//odd
		{
			if (odd_nodes[k] == odd_val_to_search)
			{
				index = k;
				odd_nodes[k] = 0;
				break;
			}
		}
	}
}


