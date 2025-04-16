#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include "transaction.h"

class Wallet {
private:
    std::string walletId;
    int balance;
    std::vector<Transaction> transactionHistory;

public:
    Wallet();
    Wallet(std::string id);

    std::string getWalletId() const;
    int getBalance() const;
    void addTransaction(const Transaction& tx);

    void viewWallet() const;
    void transferPoints(std::vector<class User>& users);
    void adjustBalance(int amount);
    std::vector<Transaction>& getTransactionHistory();
    const std::vector<Transaction>& getTransactionHistory() const;
    void setBalance(int newBalance);
};

#endif
