#include "user.h"
#include "wallet.h"
#include "transaction.h"
#include "utils.h"
#include "data_manager.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<User> users;
User* currentUser = nullptr;
Wallet globalWallet("GLOBAL");
void registerAccount();
void login();
void userMenu();
void adminMenu();
void viewGlobalWallet();
void topUpGlobalWallet();
void distributePointsFromGlobal();
void backupUserData();
void restoreUserData();
int main() {
    int choice;
    std::filesystem::create_directories("data");
    std::filesystem::create_directories("backup");
    std::filesystem::create_directories("logs");
    DataManager::loadUsers(users, "data/users.txt");
    DataManager::loadTransactions(users, "data/transactions.txt");
    DataManager::loadGlobalWallet(globalWallet, "data/global_wallet.txt");
    do {
        cout << "\n===== HE THONG VI DIEM =====\n";
        cout << "1. Dang ky tai khoan\n";
        cout << "2. Dang nhap\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: registerAccount(); break;
            case 2: login(); break;
        }
    } while (choice != 0);
    return 0;
}

void registerAccount() {
    string username, email, fullName;
    cout << "Ten dang nhap: "; getline(cin, username);
    cout << "Email: "; getline(cin, email);
    cout << "Ho ten: "; getline(cin, fullName);
    if (findUserByUsername(users, username)) {
        cout << "Ten dang nhap da ton tai!\n";
        return;
    }
    string password1, password2;
    cout << "Nhap mat khau: ";
    getline(cin, password1);
    cout << "Nhap lai mat khau: ";
    getline(cin, password2);

    if (password1 != password2) {
        cout << "Mat khau nhap lai khong khop. Huy dang ky.\n";
        return;
    }
    User newUser(username, hashPassword(password1), "admin", fullName, email);
    users.push_back(newUser);
    DataManager::saveUsers(users, "data/users.txt");
}

void login() {
    string username, password;
    cout << "Ten dang nhap: "; getline(cin, username);
    cout << "Mat khau: "; getline(cin, password);
    User* user = findUserByUsername(users, username);
    if (!user || user->getPassword() != hashPassword(password)) {
        cout << "Sai thong tin dang nhap!\n";
        return;
    }
    currentUser = user;
    cout << "Dang nhap thanh cong!\n";

    if (currentUser->isForcePasswordChange()) {
        cout << "Ban dang su dung mat khau tu sinh. Vui long doi mat khau ngay bay gio.\n";
        currentUser->changePassword();
    }

    if (currentUser->getRole() == "admin")
        adminMenu();
    else
        userMenu();
}

void userMenu() {
    int choice;
    do {
        cout << "\n===== MENU NGUOI DUNG =====\n";
        cout << "1. Cap nhat thong tin ca nhan\n";
        cout << "2. Thay doi mat khau\n";
        cout << "3. Xem vi diem va lich su giao dich\n";
        cout << "4. Chuyen diem\n";
        cout << "5. Xem thong tin ca nhan\n";
        cout << "0. Dang xuat\n";
        cout << "Chon: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: currentUser->updateInfo(); break;
            case 2: currentUser->changePassword(); break;
            case 3: currentUser->getWallet().viewWallet(); break;
            case 4:
                currentUser->getWallet().transferPoints(users);
                DataManager::saveTransactions(users, "data/transactions.txt");
                break;
            case 5: currentUser->viewProfile(); break;
        }
    } while (choice != 0);
    currentUser = nullptr;
}

void adminMenu() {
    int choice;
    do {
        cout << "\n===== MENU QUAN LY =====\n";
        cout << "1. Xem danh sach tai khoan\n";
        cout << "2. Tao tai khoan ho nguoi dung\n";
        cout << "3. Cap nhat thong tin ho\n";
        cout << "4. Quan ly vi diem nguoi dung\n";
        cout << "5. Xem vi tong\n";
        cout << "6. Nap diem vao vi tong (SYSTEM)\n";
        cout << "7. Phat diem tu vi tong cho nguoi dung\n";
        cout << "8. Sao luu du lieu\n";
        cout << "9. Phuc hoi du lieu tu file\n";
        cout << "0. Dang xuat\n";
        cout << "Chon: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: listAllUsers(users); break;
            case 2:
                createUserByAdmin(users);
                DataManager::saveUsers(users, "data/users.txt");
                break;
            case 3:
                updateUserInfoByAdmin(users);
                DataManager::saveUsers(users, "data/users.txt");
                break;
            case 4: manageUserWallet(users); break;
            case 5: viewGlobalWallet(); break;
            case 6:
                topUpGlobalWallet();
                DataManager::saveGlobalWallet(globalWallet, "data/global_wallet.txt");
                break;
            case 7:
                distributePointsFromGlobal();
                DataManager::saveGlobalWallet(globalWallet, "data/global_wallet.txt");
                DataManager::saveGlobalWallet(globalWallet, "data/global_wallet.txt");
                break;
            case 8: backupUserData(); break;
            case 9: restoreUserData(); break;
        }
    } while (choice != 0);
    currentUser = nullptr;
}

void viewGlobalWallet() {
    globalWallet.viewWallet();
}

void topUpGlobalWallet() {
    int amount;
    cout << "Nhap so diem muon nap vao vi tong: ";
    cin >> amount; cin.ignore();
    if (amount <= 0) {
        cout << "So diem khong hop le.\n";
        return;
    }

    string time = getCurrentTimestamp();
    Transaction tx("SYSTEM", globalWallet.getWalletId(), amount, time, "topup");

    globalWallet.adjustBalance(amount);
    globalWallet.addTransaction(tx);

    cout << "Nap diem vao vi tong thanh cong.\n";
}

void distributePointsFromGlobal() {
    string username;
    int amount;
    cout << "Nhap ten nguoi nhan: "; getline(cin, username);
    User* user = findUserByUsername(users, username);
    if (!user) {
        cout << "Khong tim thay nguoi dung.\n";
        return;
    }

    cout << "Nhap so diem muon phat: "; cin >> amount; cin.ignore();
    if (amount <= 0 || amount > globalWallet.getBalance()) {
        cout << "So diem khong hop le hoac vuot qua vi tong.\n";
        return;
    }

    string time = getCurrentTimestamp();
    Transaction tx(globalWallet.getWalletId(), user->getWallet().getWalletId(), amount, time, "distribute");

    globalWallet.adjustBalance(-amount);
    user->getWallet().adjustBalance(amount);

    globalWallet.addTransaction(tx);
    user->getWallet().addTransaction(tx);

    cout << "Da phat " << amount << " diem cho " << username << ".\n";
}

void backupUserData() {
    string timestamp = getCurrentTimestamp();
    string timeStr = timestamp;
    std::replace(timeStr.begin(), timeStr.end(), ' ', '_');
    std::replace(timeStr.begin(), timeStr.end(), ':', '-');

    string userBackup = "backup/users_backup_" + timeStr + ".bak";
    string transBackup = "backup/transactions_backup_" + timeStr + ".bak";
    string globalBackup = "backup/global_wallet_backup_" + timeStr + ".bak";

    bool ok1 = DataManager::backupData("data/users.txt", userBackup);
    bool ok2 = DataManager::backupData("data/transactions.txt", transBackup);
    bool ok3 = DataManager::backupData("data/global_wallet.txt", globalBackup);

    if (ok1) cout << "Da sao luu users.txt → " << userBackup << endl;
    else cout << "Loi khi sao luu users.txt\n";

    if (ok2) cout << "Da sao luu transactions.txt → " << transBackup << endl;
    else cout << "Loi khi sao luu giao dich\n";

    if (ok3) cout << "Da sao luu global_wallet.txt → " << globalBackup << endl;
    else cout << "Loi khi sao luu vi tong\n";
}

void restoreUserData() {
    string fileUser, fileTx;
    cout << "Nhap duong dan file backup users: ";
    getline(cin, fileUser);
    cout << "Nhap duong dan file backup giao dich: ";
    getline(cin, fileTx);

    if (DataManager::restoreBackup(fileUser, "data/users.txt"))
        cout << "Phuc hoi users.txt thanh cong.\n";
    else
        cout << "Loi khi phuc hoi users.txt\n";

    if (DataManager::restoreBackup(fileTx, "data/transactions.txt"))
        cout << "Phuc hoi giao dich thanh cong.\n";
    else
        cout << "Loi khi phuc hoi giao dich\n";

    users.clear();
    DataManager::loadUsers(users, "data/users.txt");
    DataManager::loadTransactions(users, "data/transactions.txt");
    DataManager::loadGlobalWallet(globalWallet, "data/global_wallet.txt");
}
