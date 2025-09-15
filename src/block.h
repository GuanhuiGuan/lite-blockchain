#pragma once

#include "txn.h"

#include <vector>
#include <chrono>

class Block {
public:
    std::string prevHash_;
    std::vector<Txn> txns_;
    int difficulty_; // num of prefixing 0s for the current hash
    int nonce_;
    long timeMs_;
    std::string hash_;

    Block(const std::string& prevHash, const std::vector<Txn>& txns, int difficulty);

    // set hash
    auto mine() -> std::string;
    // gen hash
    auto genHash() -> std::string;
    // transform concat str to str of integers
    auto genSha256Hash(const std::string& raw) -> std::string;
};