// author: tko
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream>
#include <string>

#include <vector>
#include <memory>
#include <stdexcept>

#include "SHA256.h"

#include "common.hpp"

using namespace std;

class BlockChain
{
public:
    BlockChain(int genesis = 1);
    Block getBlock(int index);
    // getBlock(string hash); //not implemented
    int getNumOfBlocks(void);
    // length of blocks

    int addBlock(int index, string prevHash, string hash, string nonce, vector<string> &merkle);
    // create new blocks

    string getLatestBlockHash(void);
    // void toString(void);

private:
    vector<unique_ptr<Block>> blockchain; // vector that is the blockchain
};

// If integer passed into constructor is 0, it the first node and creates the genesis block
BlockChain::BlockChain(int genesis)
{
    if (genesis == 0)
    {
        vector<string> v;
        v.push_back("Genesis Block!");
        // string header = to_string(0) + string("00000000000000") + getMerkleRoot(v);
        auto hash_nonce_pair = findHash(0, string("00000000000000"), v);

        this->blockchain.push_back(std::make_unique<Block>(0, string("00000000000000"), hash_nonce_pair.first, hash_nonce_pair.second, v));
        // printf("Created blockchain!\n");
    }
}
// Gets block based on the index
Block BlockChain::getBlock(int index)
{
    for (int i = 0; i < blockchain.size(); i++)
    {
        if (blockchain[i]->getIndex() == index)
        {
            return *(blockchain[i]);
        }
    }
    throw invalid_argument("Index does not exist.");
}

// returns number of blocks
int BlockChain::getNumOfBlocks(void)
{
    return this->blockchain.size();
}

// checks whether data fits with the right hash -> add block
// valod_proof, proof of work
int BlockChain::addBlock(int index, string prevHash, string hash, string nonce, vector<string> &merkle)
{
    string header = to_string(index) + prevHash + getMerkleRoot(merkle) + nonce;

    SHA256 sha;

    sha.update(header);

    uint8_t *digest = sha.digest();

    string hashed_header = SHA256::toString(digest);

    delete[] digest;

    if ((!(hashed_header.compare(hash))) && (hash.substr(0, 4) == "0000") && (index == blockchain.size()))
    {
        // printf("Block hashes match --- Adding Block %s \n", hash.c_str());
        this->blockchain.push_back(std::make_unique<Block>(index, prevHash, hash, nonce, merkle));
        return 1;
    }
    // cout << "Hash doesn't match criteria\n";
    return 0;
}

// returns hash of the latest block, used for finding the previousHash when mining new block
string BlockChain::getLatestBlockHash(void)
{
    return this->blockchain[blockchain.size() - 1]->getHash();
}

#endif