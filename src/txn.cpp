#include "txn.h"

#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <iostream>

/***** private *****/

auto Txn::genRawData() const {
    return sender_ + receiver_ + std::to_string(amount_) + std::to_string(nonce_);
}

/***** public *****/

Txn::Txn(const std::string& sender, const std::string& receiver, float amount, int nonce)
: sender_{sender}, receiver_{receiver}, amount_{amount}, nonce_{nonce} {}

auto Txn::sign(RSA* privateKey) -> void {
    // hash
    auto raw {genRawData()};
    unsigned char hash [SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(raw.c_str()), raw.size(), hash);
    // sign
    unsigned char sign [RSA_size(privateKey)];
    unsigned int signLen;
    auto signResp {RSA_sign(NID_sha256, hash, sizeof(hash), sign, &signLen, privateKey)};
    if (signResp != 1) {
        std::cerr << "Bad sign resp: " << signResp << std::endl;
        return;
    }
    // store
    signature_.assign(reinterpret_cast<char*>(sign), signLen);
}
    
auto Txn::validSign(RSA* publicKey) const -> bool {
    ERR_clear_error(); // clear SSL error stack

    // hash
    auto raw {genRawData()};
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(raw.c_str()), raw.size(), hash);
    // verify
    auto verifyResp {RSA_verify(NID_sha256, hash, sizeof(hash), 
        reinterpret_cast<const unsigned char*>(signature_.c_str()), signature_.size(), 
        publicKey)};
    if (verifyResp != 1) {
        auto errCode {ERR_get_error()};
        auto errMsg {ERR_error_string(errCode, nullptr)};
        std::cerr << "Failed verification: " << errCode << ", " << errMsg << std::endl;
        return false;
    }
    std::cout << "Successful verification" << std::endl;
    return true;
}

auto Txn::valid(RSA* publicKey) const -> bool {
    if (amount_ < 0 || nonce_ < 0) return false;
    return validSign(publicKey);
}