#include "block.h"

#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

Block::Block(const std::string& prevHash, const std::vector<Txn>& txns, int difficulty)
: prevHash_{prevHash}, txns_{txns}, difficulty_{difficulty}
, nonce_{0}, timeMs_{std::chrono::steady_clock::now().time_since_epoch().count() / 1000000}, hash_{mine()} 
{}

auto Block::mine() -> std::string {
    std::string targetPrefix (difficulty_, '0');
    while (hash_.substr(0, difficulty_) != targetPrefix) {
        ++nonce_;
        hash_ = genHash();
    }
    return hash_;
}

auto Block::genHash() -> std::string {
    std::stringstream ss {};
    ss << timeMs_;
    for (const auto& txn : txns_) {
        ss << txn.sender_ << txn.receiver_ << txn.amount_;
    }
    ss << prevHash_ << nonce_;
    return genSha256Hash(ss.str());
}

auto Block::genSha256Hash(const std::string& raw) -> std::string {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(raw.c_str()), raw.size(), hash);
    std::stringstream ss {};
    for (int i {0}; i < sizeof(hash); ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int) hash[i];
    }
    return ss.str();
}