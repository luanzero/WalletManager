#ifndef UTILS_H
#define UTILS_H

#include "utils.h"
#include "user.h"
#include "wallet.h"
#include "transaction.h"

#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <filesystem>
using namespace std;
class User;
std::string hashPassword(const std::string& password);
std::string generateOTP();
std::string generateRandomPassword();
std::string getCurrentTimestamp();
void logFailedTransaction(const std::string& reason,
                          const std::string& fromWallet,
                          const std::string& toWallet,
                          int amount,
                          const std::string& timestamp);
#endif
