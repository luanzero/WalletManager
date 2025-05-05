#include "data_manager.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

using namespace std;

// Lưu dữ liệu của users
bool DataManager::saveUsers(const vector<User>& users, const string& filename) {
    return true;
}

// Load dữ liệu users
bool DataManager::loadUsers(vector<User>& users, const string& filename) {
    return true;
}

// Backup data
bool DataManager::backupData(const string& sourceFile, const string& backupDir) {
}

// Restore data từ backup
bool DataManager::restoreBackup(const string& backupFile, const string& targetFile) {
}

// Lưu dữ liệu giao dịch xuống file
bool DataManager::saveTransactions(const std::vector<User>& users, const std::string& filename) {
    return true;
}

// Load dữ liệu giao dịch chuyển điểm
bool DataManager::loadTransactions(std::vector<User>& users, const std::string& filename) {
    return true;
}

// Lưu data của ví tổng xuống file
bool DataManager::saveGlobalWallet(const Wallet& wallet, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) return false;

    out << wallet.getWalletId() << "|" << wallet.getBalance() << "\n";
    for (const auto& tx : wallet.getTransactionHistory()) {
        out << tx.getTimestamp() << "|" << tx.getFromWalletId() << "|"
            << tx.getToWalletId() << "|" << tx.getAmount() << "|"
            << tx.getStatus() << "\n";
    }

    out.close();
    return true;
}

// Load dữ liệu ví tổng
bool DataManager::loadGlobalWallet(Wallet& wallet, const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) return false;

    string line;
    getline(in, line);
    istringstream ss(line);
    string walletId;
    int balance;
    getline(ss, walletId, '|');
    ss >> balance;

    wallet = Wallet(walletId);
    wallet.adjustBalance(balance);

    while (getline(in, line)) {
        istringstream txs(line);
        string ts, from, to, status;
        int amt;
        getline(txs, ts, '|');
        getline(txs, from, '|');
        getline(txs, to, '|');
        txs >> amt;
        txs.ignore(); getline(txs, status);

        Transaction tx(from, to, amt, ts, status);
        wallet.addTransaction(tx);
    }

    in.close();
    return true;
}