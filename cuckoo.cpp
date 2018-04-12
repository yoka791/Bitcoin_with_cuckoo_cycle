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
  std::string sha_output = sha256(header);
  setkeys(keys, sha_output.c_str());
}

bool verify(const std::array<edge_t, 42> solution_arr, const std::string& header)
{
	siphash_keys sip_keys;
	setKeysFromHeader(header, &sip_keys);

	node_t uvs[2 * PROOFSIZE];
	node_t xor0 = 0, xor1 = 0;
	for (u32 n = 0; n < PROOFSIZE; n++) {
		if (solution_arr[n] > MODULU)  //no. of edge does not exist
			return false;
		if (n && solution_arr[n] <= solution_arr[n - 1])
			return false;
		xor0 ^= uvs[2 * n] = sipnode(&sip_keys, solution_arr[n], 0);
		xor1 ^= uvs[2 * n + 1] = sipnode(&sip_keys, solution_arr[n], 1);
	}
	if (xor0 | xor1)              // optional check for obviously bad proofs
		return false;
	u32 n = 0, i = 0, j;
	do {                        // follow cycle
		for (u32 k = j = i; (k = (k + 2) % (2 * PROOFSIZE)) != i; ) {
			if (uvs[k] == uvs[i]) { // find other edge endpoint identical to one at i
				if (j != i)           // already found one before
					return false;
				j = k;
			}
		}
		if (j == i) return false;  // no matching endpoint
		i = j ^ 1;
		n++;
	} while (i != 0);           // must cycle back to start or we would have found branch
	return n == PROOFSIZE ? true : false;
}


