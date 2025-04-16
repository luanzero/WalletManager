#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "user.h"
#include <vector>

class DataManager {
public:
    static bool saveUsers(const std::vector<User>& users, const std::string& filename);
    static bool loadUsers(std::vector<User>& users, const std::string& filename);
    static bool backupData(const std::string& sourceFile, const std::string& backupDir);
    static bool restoreBackup(const std::string& backupFile, const std::string& targetFile);
    static bool saveTransactions(const std::vector<User>& users, const std::string& filename);
    static bool loadTransactions(std::vector<User>& users, const std::string& filename);
    static bool saveGlobalWallet(const Wallet& wallet, const std::string& filename);
    static bool loadGlobalWallet(Wallet& wallet, const std::string& filename);
};

#endif
