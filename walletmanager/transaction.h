#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    std::string fromWalletId;
    std::string toWalletId;
    int amount;
    std::string timestamp;
    std::string status; // "success", "failed", "pending"

public:
    Transaction(std::string from, std::string to, int amt, std::string time, std::string stat);

    std::string getFromWalletId() const;
    std::string getToWalletId() const;
    int getAmount() const;
    std::string getTimestamp() const;
    std::string getStatus() const;
    void print() const;
};

#endif
