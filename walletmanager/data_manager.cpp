#include "data_manager.h"
#include <fstream>
#include <filesystem>
#include <iostream>
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
    return true;
}

// Load dữ liệu ví tổng
bool DataManager::loadGlobalWallet(Wallet& wallet, const string& filename) {
    return true;
}
