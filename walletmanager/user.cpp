#include "user.h"
#include "utils.h"
#include <iostream>

using namespace std;

User::User(string uname, string hashPass, string r, string fname, string mail)
    : username(uname), hashedPassword(hashPass), role(r), fullName(fname), email(mail), wallet(uname + "_wallet"), forcePasswordChange(false) {}

string User::getUsername() const { return username; }
string User::getPassword() const { return hashedPassword; }
string User::getRole() const { return role; }
string User::getEmail() const { return email; }
string User::getFullName() const { return fullName; }
bool User::isForcePasswordChange() const {
    return forcePasswordChange;
}
void User::setForcePasswordChange(bool value) {
    forcePasswordChange = value;
}
Wallet& User::getWallet() { return wallet; }
const Wallet& User::getWallet() const {
    return wallet;
}

// Cập nhật thông tin user
void User::updateInfo() {
}

// Đổi mật khẩu của user
void User::changePassword() {
}

// Tìm user by username
User* findUserByUsername(vector<User>& users, const string& username) {
}

// Hiển thị danh sách của user
void listAllUsers(const vector<User>& users) {
}

// Admin tạo user mới
void createUserByAdmin(vector<User>& users) {
}

// Admin cập nhật thông tin của user
void updateUserInfoByAdmin(vector<User>& users) {
}

// Quản lý ví của user
void manageUserWallet(vector<User>& users) {
}

// Hiển thị thông tin của user
void User::viewProfile() const {
}
