#include "txn.h"

#include <iostream>
#include <openssl/rsa.h>

void testTxn() {
    auto key {RSA_generate_key(2048, RSA_F4, nullptr, nullptr)};

    Txn txn {"Alice", "Bob", -1, 1};
    std::cout << txn.valid(key) << std::endl;
    txn.amount_ = 1.3;
    txn.sign(key);
    std::cout << txn.valid(key) << std::endl;

    RSA_free(key);
}

int main() {
    testTxn();

    return 0;
}