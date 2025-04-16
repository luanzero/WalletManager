#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "wallet.h"


class User {
private:
    std::string username;
    std::string hashedPassword;
    std::string role; // "user" hoac "admin"
    std::string fullName;
    std::string email;
    Wallet wallet;
    bool forcePasswordChange;;

public:
    User(std::string uname, std::string hashPass, std::string r, std::string fname, std::string mail);

    std::string getUsername() const;
    std::string getPassword() const;
    std::string getRole() const;
    std::string getEmail() const;
    std::string getFullName() const;
    Wallet& getWallet();
    const Wallet& getWallet() const;
    bool isForcePasswordChange() const;
    void viewProfile() const;
    void setForcePasswordChange(bool value);
    void updateInfo();
    void changePassword();
};

User* findUserByUsername(std::vector<User>& users, const std::string& username);
void listAllUsers(const std::vector<User>& users);
void createUserByAdmin(std::vector<User>& users);
void updateUserInfoByAdmin(std::vector<User>& users);
void manageUserWallet(std::vector<User>& users);

#endif // USER_H
