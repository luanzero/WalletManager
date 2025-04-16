#include "wallet.h"
#include "utils.h"
#include "user.h"
#include <iostream>
#include "data_manager.h"
using namespace std;

Wallet::Wallet() : walletId(""), balance(0) {}
Wallet::Wallet(string id) : walletId(id), balance(0) {}

string Wallet::getWalletId() const { return walletId; }
int Wallet::getBalance() const { return balance; }
void Wallet::adjustBalance(int amount) { balance += amount; }
void Wallet::setBalance(int newBalance) {
    balance = newBalance;
}
vector<Transaction>& Wallet::getTransactionHistory() { return transactionHistory; }
// Lịch sử giao dịch của ví
const std::vector<Transaction>& Wallet::getTransactionHistory() const {
}

// Thêm giao dịch
void Wallet::addTransaction(const Transaction& tx) {
}


// Hiển thị thông tin ví
void Wallet::viewWallet() const {
}

// Chuyển điểm của user đang đăng nhập đến user trong hệ thống
void Wallet::transferPoints(vector<User>& users) {
}

