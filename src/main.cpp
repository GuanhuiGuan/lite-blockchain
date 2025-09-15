#include "block.h"

#include <iostream>
#include <openssl/rsa.h>

void testTxn() {
    auto key {RSA_generate_key(2048, RSA_F4, nullptr, nullptr)};
    auto key2 {RSA_generate_key(2048, RSA_F4, nullptr, nullptr)};

    Txn txn {"Alice", "Bob", -1, 1};
    std::cout << txn.valid(key) << std::endl;
    txn.amount_ = 1.3;
    txn.sign(key);
    std::cout << txn.valid(key) << std::endl;
    std::cout << txn.valid(key2) << std::endl;

    RSA_free(key);
    RSA_free(key2);
}

void testBlock() {
    auto key {RSA_generate_key(2048, RSA_F4, nullptr, nullptr)};
    std::vector<Txn> txns {
        {"Alice", "Bob", 9.9, 1},
        {"Bob", "Alice", 10.5, 2},
    };
    for (auto& txn : txns) txn.sign(key);
    Block block {"", txns, 4};
    std::cout << block.hash_ << std::endl;

    RSA_free(key);
}

int main() {
    // testTxn();
    testBlock();

    return 0;
}