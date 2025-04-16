#include "utils.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <filesystem>
using namespace std;

string hashPassword(const string& password) {
    std::hash<std::string> hasher;
    size_t hashed = hasher(password);
    std::stringstream ss;
    ss << std::hex << hashed;
    return ss.str();
}

std::string generateOTP() {
    srand(time(nullptr));
    int otp = 100000 + rand() % 900000;
    return std::to_string(otp);
}

std::string generateRandomPassword() {
    srand(time(nullptr));
    std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string pass = "";
    for (int i = 0; i < 8; i++) {
        pass += chars[rand() % chars.size()];
    }
    return pass;
}

std::string getCurrentTimestamp() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

void logFailedTransaction(const string& reason,
                          const string& fromWallet,
                          const string& toWallet,
                          int amount,
                          const string& timestamp) {
    std::filesystem::create_directories("logs");
    ofstream log("logs/failed_tx.txt", ios::app);

    if (log.is_open()) {
        log << "[" << timestamp << "] "
            << "FROM: " << fromWallet
            << ", TO: " << toWallet
            << ", AMOUNT: " << amount
            << ", REASON: " << reason << "\n";
        log.close();
    }
}
