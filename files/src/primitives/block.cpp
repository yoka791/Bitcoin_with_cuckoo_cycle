// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <utilstrencodings.h>
#include <crypto/common.h>

uint256 CBlockHeader::GetHash() const
{
	CBlockHeaderNoCycle block_no_cycle;
	block_no_cycle.SetNull();
	
	block_no_cycle.nVersion = nVersion;
	block_no_cycle.hashPrevBlock = hashPrevBlock;
    block_no_cycle.hashMerkleRoot = hashMerkleRoot;
    block_no_cycle.nTime = nTime;
    block_no_cycle.nBits = nBits;
    block_no_cycle.nNonce = nNonce;
	return block_no_cycle.GetHash();
}

uint256 CBlockHeaderNoCycle::GetHash() const
{
	return SerializeHash(*this);
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
	s << "cycle=\n";
	for (const auto& edge : cycle_arr) {
		s << strprintf("0x%08x", edge);
	}
	s << "\n";	
    return s.str();
}
