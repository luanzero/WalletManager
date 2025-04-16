#include "transaction.h"
#include <iostream>
#include <iomanip>

Transaction::Transaction(std::string from, std::string to, int amt, std::string time, std::string stat)
    : fromWalletId(from), toWalletId(to), amount(amt), timestamp(time), status(stat) {}

std::string Transaction::getFromWalletId() const { return fromWalletId; }
std::string Transaction::getToWalletId() const { return toWalletId; }
int Transaction::getAmount() const { return amount; }
std::string Transaction::getTimestamp() const { return timestamp; }
std::string Transaction::getStatus() const { return status; }

void Transaction::print() const {
    std::cout << std::left << std::setw(20) << timestamp
              << std::setw(15) << fromWalletId
              << std::setw(15) << toWalletId
              << std::setw(10) << amount
              << std::setw(10) << status << std::endl;
}
