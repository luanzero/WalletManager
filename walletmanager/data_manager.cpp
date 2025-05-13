#include "data_manager.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

using namespace std;

// Lưu dữ liệu của users
bool DataManager::saveUsers(const vector<User>& users, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cout << "Khong the mo file de ghi: " << filename << endl;
        return false;
    }

    for (const auto& user : users) {
        out << user.getUsername() << '|'
            << user.getPassword() << '|'
            << user.getRole() << '|'
            << user.getFullName() << '|'
            << user.getEmail() << '|'
            << user.getWallet().getWalletId() << '|'
            << user.getWallet().getBalance() << '\n';
    }
    out.close();
    return true;
}

// Load dữ liệu users
bool DataManager::loadUsers(vector<User>& users, const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    users.clear();
    string line;
    while (getline(in, line)) {
        istringstream ss(line);
        string username, password, role, fullName, email, walletId;
        int balance;

        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, role, '|');
        getline(ss, fullName, '|');
        getline(ss, email, '|');
        getline(ss, walletId, '|');
        ss >> balance;

        User user(username, password, role, fullName, email);
        user.getWallet().adjustBalance(balance);
        users.push_back(user);
    }
    in.close();
    return true;
}

// Backup data
bool DataManager::backupData(const string& sourceFile, const string& backupDir) {
    if (!filesystem::exists(sourceFile)) {
        cout << "Khong the mo file de ghi: " << sourceFile << endl;
        return false;
    }
    filesystem::create_directories(backupDir);
    string backupFile = backupDir + "/backup_" + to_string(time(nullptr)) + ".bak";
    return filesystem::copy_file(sourceFile, backupFile, filesystem::copy_options::overwrite_existing);
}

// Restore data từ backup
bool DataManager::restoreBackup(const string& backupFile, const string& targetFile) {
    if (!filesystem::exists(backupFile)) return false;
    return filesystem::copy_file(backupFile, targetFile, filesystem::copy_options::overwrite_existing);
}

// Lưu dữ liệu giao dịch xuống file
bool DataManager::saveTransactions(const std::vector<User>& users, const std::string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Khong the ghi file giao dich: " << filename << endl;
        return false;
    }

    for (const auto& user : users) {
        for (const auto& tx : user.getWallet().getTransactionHistory()) {
            out << tx.getTimestamp() << '|'
                << tx.getFromWalletId() << '|'
                << tx.getToWalletId() << '|'
                << tx.getAmount() << '|'
                << tx.getStatus() << '\n';
        }
    }

    out.close();
    return true;
}

// Load dữ liệu giao dịch chuyển điểm
bool DataManager::loadTransactions(std::vector<User>& users, const std::string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    string line;
    while (getline(in, line)) {
        istringstream ss(line);
        string timestamp, fromId, toId, status;
        int amount;

        getline(ss, timestamp, '|');
        getline(ss, fromId, '|');
        getline(ss, toId, '|');
        ss >> amount;
        ss.ignore();
        getline(ss, status);

        Transaction tx(fromId, toId, amount, timestamp, status);

        for (auto& user : users) {
            if (user.getWallet().getWalletId() == fromId || user.getWallet().getWalletId() == toId) {
                user.getWallet().addTransaction(tx);
            }
        }
    }

    in.close();
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