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
    string otp = generateOTP();
    cout << "Ma OTP: " << otp << endl;
    string enteredOtp;
    cout << "Nhap OTP: "; getline(cin, enteredOtp);
    if (enteredOtp != otp) {
        cout << "OTP sai. Huy cap nhat.\n";
        return;
    }
    cout << "Nhap ho ten moi: "; getline(cin, fullName);
    cout << "Nhap email moi: "; getline(cin, email);
    cout << "Cap nhat thanh cong.\n";
}

// Đổi mật khẩu của user
void User::changePassword() {
    string oldPass, newPass1, newPass2;

    cout << "Nhap mat khau cu: ";
    getline(cin, oldPass);

    if (hashPassword(oldPass) != hashedPassword) {
        cout << "Sai mat khau cu!\n";
        return;
    }

    cout << "Nhap mat khau moi: ";
    getline(cin, newPass1);
    cout << "Nhap lai mat khau moi: ";
    getline(cin, newPass2);

    if (newPass1 != newPass2) {
        cout << "Mat khau moi khong khop. Huy thay doi.\n";
        return;
    }

    hashedPassword = hashPassword(newPass1);
    forcePasswordChange = false;
    cout << "Doi mat khau thanh cong.\n";
}

// Tìm user by username
User* findUserByUsername(vector<User>& users, const string& username) {
    for (auto& user : users) {
        if (user.getUsername() == username) return &user;
    }
    return nullptr;
}

// Hiển thị danh sách của user
void listAllUsers(const vector<User>& users) {
    cout << "\nDanh sach tai khoan:\n";
    for (const auto& user : users) {
        cout << "- " << user.getUsername() << " (" << user.getRole() << ")\n";
    }
}

// Admin tạo user mới
void createUserByAdmin(vector<User>& users) {
    string username, email, fullName;
    cout << "Ten dang nhap: "; getline(cin, username);
    cout << "Email: "; getline(cin, email);
    cout << "Ho va ten: "; getline(cin, fullName);
    if (findUserByUsername(users, username)) {
        cout << "Ten dang nhap da ton tai!\n";
        return;
    }
    string password = generateRandomPassword();
    User newUser(username, hashPassword(password), "user", fullName, email);
    newUser.setForcePasswordChange(true);
    users.push_back(newUser);
    cout << "Tao tai khoan thanh cong. Mat khau: " << password << endl;
}

// Admin cập nhật thông tin của user
void updateUserInfoByAdmin(vector<User>& users) {
    string username;
    cout << "Nhap username can cap nhat: "; getline(cin, username);
    User* target = findUserByUsername(users, username);
    if (!target) {
        cout << "Khong tim thay nguoi dung.\n";
        return;
    }
    string otp = generateOTP();
    cout << "OTP xac thuc (nguoi dung cung cap): " << otp << endl;
    string enteredOtp;
    cout << "Nhap OTP: "; getline(cin, enteredOtp);
    if (enteredOtp != otp) {
        cout << "OTP sai. Khong the cap nhat.\n";
        return;
    }
    string newFullName, newEmail;
    cout << "Ho ten moi: "; getline(cin, newFullName);
    cout << "Email moi: "; getline(cin, newEmail);
    *target = User(target->getUsername(), target->getPassword(), target->getRole(), newFullName, newEmail);
    cout << "Cap nhat thanh cong.\n";
}

// Quản lý ví của user
void manageUserWallet(vector<User>& users) {
    string username;
    cout << "Nhap username can xem vi: "; getline(cin, username);
    User* target = findUserByUsername(users, username);
    if (!target) {
        cout << "Khong tim thay nguoi dung.\n";
        return;
    }
    target->getWallet().viewWallet();
}

// Hiển thị thông tin của user
void User::viewProfile() const {
    cout << "\n===== THONG TIN CA NHAN =====\n";
    cout << "Ten dang nhap: " << username << endl;
    cout << "Ho ten       : " << fullName << endl;
    cout << "Email        : " << email << endl;
    cout << "Vai tro      : " << role << endl;
}
