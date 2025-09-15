#pragma once

#include <string>
#include <openssl/rsa.h>

class Txn {
public:
    // public for convenience
    std::string sender_;
    std::string receiver_;
    float amount_;
    int nonce_;

    std::string signature_;
    unsigned int signLen_; // length of signature

    Txn(const std::string& sender, const std::string& receiver, float amount, int nonce);
    // sign with sender's private key
    auto sign(RSA* privateKey) -> void;
    // verify with sender's public key
    auto validSign(RSA* publicKey) const -> bool;
    // is the txn valid (valid amount & nonce & sign)
    auto valid(RSA* publicKey) const -> bool;
private:
    auto genRawData() const;
};