#include "cuckoo.h"
edge_t _sipnode(siphash_keys *keys, edge_t nonce, u32 uorv) {
	edge_t res = siphash24(keys, 2*nonce + uorv) & MODULU;   //& for modolu
	return res;
}

node_t sipnode(siphash_keys *keys, edge_t nonce, u32 uorv) {
	node_t res = _sipnode(keys, nonce, uorv) << 1 | uorv;   //2h(..,..)+1/0
	return res;
}

void setKeysFromHeader(const std::string &header, siphash_keys *keys) {
  std::string sha_output = sha256(header);
  setkeys(keys, sha_output.c_str());
}

