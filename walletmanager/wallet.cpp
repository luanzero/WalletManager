#include "wallet.h"
#include "utils.h"
#include "user.h"
#include <iostream>
#include "data_manager.h"
using namespace std;

Wallet::Wallet() : walletId(""), balance(0) {}
Wallet::Wallet(string id) : walletId(id), balance(0) {}

string Wallet::getWalletId() const { return walletId; }
int Wallet::getBalance() const { return balance; }
void Wallet::adjustBalance(int amount) { balance += amount; }
void Wallet::setBalance(int newBalance) {
    balance = newBalance;
}
vector<Transaction>& Wallet::getTransactionHistory() { return transactionHistory; }
// Lịch sử giao dịch của ví
const std::vector<Transaction>& Wallet::getTransactionHistory() const {
}

// Thêm giao dịch
void Wallet::addTransaction(const Transaction& tx) {
}


// Hiển thị thông tin ví
void Wallet::viewWallet() const {
}

// Chuyển điểm của user đang đăng nhập đến user trong hệ thống
void Wallet::transferPoints(vector<User>& users) {
    string toUsername;
    int amount;

    // 1_ Tìm, mở ví A: ví hiện tại (this)

    cout << "Nhap ten nguoi nhan: ";
    getline(cin, toUsername);
    User* recipient = findUserByUsername(users, toUsername);

    // 2_ Tìm, mở ví B (đích)
    if (!recipient || recipient->getWallet().getWalletId() == this->walletId) {
        cout << "Nguoi nhan khong hop le (khong the chuyen cho chinh minh).\n";
        return;
    }

    Wallet& toWallet = recipient->getWallet();

    cout << "So diem muon chuyen: ";
    cin >> amount; cin.ignore();
    if (amount <= 0) {
        cout << "So diem khong hop le.\n";
        return;
    }

    string otp = generateOTP();
    cout << "Ma OTP: " << otp << endl;
    string enteredOtp;
    cout << "Nhap OTP: ";
    getline(cin, enteredOtp);
    if (enteredOtp != otp) {
        cout << "OTP sai. Huy giao dich.\n";
        return;
    }

    // 3_ Giao dịch: đảm bảo tính toàn vẹn (atomic)
    string time = getCurrentTimestamp();
    Transaction tx(walletId, toWallet.getWalletId(), amount, time, "success");

    // Sao lưu số dư ban đầu để rollback nếu cần
    int backupSender = balance;
    int backupReceiver = toWallet.getBalance();

    try {
        // 3_1 Kiểm tra số dư
        if (balance < amount) {
            throw runtime_error("So du khong du. Huy giao dich.");
        }

        // 3_2 Thực hiện trừ – cộng điểm
        balance -= amount;
        toWallet.adjustBalance(amount);

        addTransaction(tx);
        toWallet.addTransaction(tx);

        cout << "Chuyen " << amount << " diem toi " << toUsername << " thanh cong!\n";

        // Lưu lịch sử ngay sau giao dịch thành công
        extern vector<User> users;
        DataManager::saveTransactions(users, "data/transactions.txt");

    } catch (const exception& e) {
        // Khôi phục số dư ban đầu nếu có lỗi
        balance = backupSender;
        toWallet.setBalance(backupReceiver);
        cout << e.what() << endl;
        logFailedTransaction(e.what(), walletId, toWallet.getWalletId(), amount, time);
    }
}

