# 🧾 HỆ THỐNG ĐĂNG NHẬP – ĐĂNG KÝ VÀ QUẢN LÝ VÍ ĐIỂM

## 🧩 Giới thiệu dự án

Dự án xây dựng một hệ thống phần mềm bằng C++ phục vụ cho việc:
- Đăng ký và đăng nhập tài khoản người dùng với xác thực bảo mật cao (có OTP).
- Phân quyền người dùng (thông thường và quản lý).
- Quản lý thông tin cá nhân, thay đổi mật khẩu, theo dõi lịch sử hoạt động.
- Hệ thống ví điểm với khả năng chuyển điểm giữa các ví, có xác thực OTP, bảo đảm tính toàn vẹn (giao dịch atom).
- Lưu trữ thông tin vào tệp và sao lưu dữ liệu đầy đủ.

## 👨‍💻 Thành viên và phân công công việc

| Họ tên           |Mã Sinh Viên|Email     | Nhiệm vụ chính |
|--------------------|---------------|---------------|--------------------------------------------------------------------------------|
| Trần Văn Luân      | K24DTCN070 |tranluan89@gmail.com | Thiết kế cấu trúc dữ liệu, viết chức năng đăng ký, đăng nhập, thay đổi mật khẩu, OTP, xử lý xác thực 2 lớp, bảo mật hệ thống |
| Nguyễn Thành Phi  | K24DTCN076|  thanhphichv96@gmail.com | Xây dựng tính năng ví điểm, giao dịch điểm, kiểm tra số dư, xử lý atomic         |
| Nguyễn Thành Nam   | K24DTCN074| anthony307193@gmail.com |Phân quyền người dùng, chức năng quản lý, lịch sử giao dịch                                    |
| Nguyễn Trọng Khang      |  K24DTCN062 | trongkhang185@gmail.com |Lưu trữ dữ liệu vào tệp, quản lý backup, hướng dẫn sử dụng, viết tài liệu      |

## 📌 Đặc tả chức năng

### 1. Tài khoản người dùng
#### 1.1. Đăng ký tài khoản
##### Mô tả:
Cho phép người dùng mới tạo tài khoản với tên đăng nhập duy nhất. Hệ thống tự sinh mật khẩu ban đầu và yêu cầu xác nhận OTP trước khi hoàn tất đăng ký.

##### Dữ liệu đầu vào:
- `username`: Tên đăng nhập duy nhất
- `email`: Địa chỉ email để gửi OTP
- `fullName`: Họ tên đầy đủ

##### Luồng xử lý:
1. Kiểm tra `username` đã tồn tại chưa.
2. Sinh mật khẩu ngẫu nhiên (tự động).
3. Gửi mã OTP đến email hoặc console.
4. Người dùng nhập mã OTP để xác thực.
5. Nếu hợp lệ, tạo mới tài khoản:
   - Băm mật khẩu bằng SHA-256
   - Đặt `forcePasswordChange = true`
   - Gán `role = "user"`
   - Tạo ví điểm rỗng kèm theo

##### Ràng buộc:
- `username` phải duy nhất
- Email hợp lệ (định dạng chuẩn)

#### 1.2. Đăng nhập
##### Mô tả:
Cho phép người dùng đã có tài khoản truy cập hệ thống.

##### Dữ liệu đầu vào:
- `username`
- `password` (đã băm để so sánh)

##### Luồng xử lý:
1. Tìm người dùng theo `username`.
2. So sánh `hashedPassword`.
3. Nếu đúng, kiểm tra `forcePasswordChange`:
   - Nếu `true` → chuyển sang quy trình đổi mật khẩu.
4. Nếu thành công → nạp thông tin ví, vai trò, lịch sử giao dịch.

##### Ràng buộc:
- Sai mật khẩu → cho phép nhập lại nhiều lần
- Cần cập nhật mật khẩu mới nếu đang dùng mật khẩu tự sinh

#### 1.3. Cập nhật thông tin cá nhân
##### Mô tả:
Cho phép người dùng thay đổi thông tin cá nhân (email, họ tên) sau khi xác thực bằng OTP.

##### Dữ liệu đầu vào:
- `email`, `fullName` mới
- Mã OTP xác nhận

##### Luồng xử lý:
1. Gửi mã OTP đến email hiện tại.
2. Người dùng nhập OTP.
3. Nếu hợp lệ → cập nhật `email`, `fullName`.

##### Ràng buộc:
- OTP chỉ có hiệu lực trong thời gian giới hạn
- Email mới phải hợp lệ

#### 1.4. Đổi mật khẩu
##### Mô tả:
Cho phép người dùng thay đổi mật khẩu vì lý do bảo mật hoặc do hệ thống yêu cầu.

##### Dữ liệu đầu vào:
- `oldPassword`
- `newPassword`
- Mã OTP

##### Luồng xử lý:
1. Kiểm tra mật khẩu cũ có khớp không.
2. Gửi mã OTP.
3. Người dùng nhập OTP.
4. Nếu đúng:
   - Cập nhật `hashedPassword` mới.
   - Đặt `forcePasswordChange = false`.

##### Ràng buộc:
- Mật khẩu mới không được trùng với mật khẩu cũ
- OTP phải hợp lệ

#### 1.5. Phân quyền người dùng

##### Mô tả:
Cho phép xác định và xử lý hành vi khác nhau tùy theo vai trò người dùng: `"user"` hoặc `"admin"`.

##### Vai trò:
- `"user"`:
  - Truy cập và cập nhật thông tin cá nhân
  - Xem ví điểm
  - Thực hiện giao dịch điểm
- `"admin"`:
  - Tạo tài khoản hộ người dùng
  - Xem và quản lý tất cả người dùng
  - Xem và can thiệp vào ví điểm người khác
  - Cập nhật thông tin hộ (yêu cầu OTP người dùng)

##### Ràng buộc:
- Phải đăng nhập để xác định vai trò
- Các thao tác yêu cầu quyền `admin` phải có phân quyền kiểm tra rõ ràng

### 2. Bảo mật
#### 2.1. Lưu mật khẩu an toàn

##### Mô tả:
Mật khẩu người dùng không được lưu dưới dạng thô (plain text) mà phải băm (hash) bằng thuật toán bảo mật.

##### Yêu cầu:
- Sử dụng thuật toán SHA-256 (có thể mở rộng thành SHA-512 hoặc thêm salt trong tương lai).
- Mỗi khi người dùng đăng ký hoặc đổi mật khẩu, hệ thống thực hiện:
  ```cpp
  std::string hashedPassword = hashFunction(plainPassword);
  ```

##### Lý do bảo mật:
- Ngăn việc rò rỉ mật khẩu thực tế khi tệp dữ liệu bị truy cập trái phép.
- Hạn chế tấn công dò mật khẩu bằng từ điển (dictionary attack).

#### 2.2. Xác thực bằng OTP (One-Time Password)

##### Mô tả:
Hệ thống sinh và gửi mã OTP để xác minh các hành động quan trọng từ người dùng.

##### Quy trình:
1. Hệ thống tạo mã OTP ngẫu nhiên gồm 6 chữ số:
   ```cpp
   std::string otp = generateOTP(); // random 6 digits
   ```
2. Gửi OTP đến địa chỉ email đã đăng ký (nếu chưa có SMTP thì in ra console).
3. Lưu mã OTP vào bộ nhớ tạm cùng timestamp.
4. Khi người dùng nhập OTP:
   - So sánh với mã đã sinh.
   - Kiểm tra thời gian hiệu lực (ví dụ 3 phút).
   - Nếu khớp → cho phép thực hiện hành động.

##### Ứng dụng:
- Đăng ký tài khoản.
- Đổi mật khẩu.
- Cập nhật thông tin cá nhân.
- Giao dịch ví điểm.

##### Yêu cầu:
- OTP hết hạn sau một thời gian nhất định (timeout ~180 giây).
- Chỉ cho phép nhập lại OTP tối đa 3 lần.

#### 2.3. Bảo vệ hành động nhạy cảm

##### Danh sách hành động yêu cầu OTP:
| Hành động                         | Yêu cầu OTP | Ghi chú thêm                               |
|----------------------------------|-------------|--------------------------------------------|
| Đăng ký tài khoản                | ❌          | Không cần OTP    |
| Đổi mật khẩu                     | ❌          | Không cần OTP                  |
| Cập nhật thông tin cá nhân       | ✅          | Ngăn chỉnh sửa thông tin email/Họ tên      |
| Giao dịch chuyển điểm            | ✅          | Tránh lạm dụng và giả mạo giao dịch        |
| Tạo tài khoản hộ (admin)         | ❌          | Không cần OTP nhưng cần quyền admin        |
| Cập nhật hộ thông tin (admin)    | ✅          | Cần OTP của người dùng bị sửa              |


#### 2.4. Bảo mật lớp ứng dụng

##### Biện pháp:
- Không in trực tiếp mật khẩu vào console hoặc file log.
- Ẩn đầu vào khi nhập mật khẩu từ bàn phím.
- Kiểm tra quyền (`role`) trước khi thực thi hành động có giới hạn.
- Mỗi hành động bảo mật cần có **log hoặc lịch sử ghi nhận** (nếu mở rộng hệ thống).

### 3. Quản lý ví điểm
#### 3.1. Khởi tạo ví điểm

##### Mô tả:
Mỗi người dùng sẽ được tạo một ví điểm duy nhất khi đăng ký thành công.

##### Dữ liệu đầu vào:
- Không có (tự động sinh trong quá trình đăng ký)

##### Luồng xử lý:
1. Sinh `walletId` duy nhất (có thể dùng UUID hoặc kết hợp `username`).
2. Gán `balance = 0`.
3. Gán `transactionHistory = []`.

##### Ràng buộc:
- Một người dùng chỉ được có một ví duy nhất.
- Ví gắn chặt với tài khoản người dùng.

---

#### 3.2. Kiểm tra số dư

##### Mô tả:
Kiểm tra xem số điểm hiện có trong ví có đủ để thực hiện giao dịch không.

##### Dữ liệu đầu vào:
- `walletId`
- `amount`: số điểm cần kiểm tra

##### Luồng xử lý:
1. Truy xuất ví theo `walletId`.
2. So sánh `wallet.balance >= amount`.

##### Ràng buộc:
- Nếu không đủ điểm → dừng giao dịch, trả thông báo lỗi.

#### 3.3. Giao dịch chuyển điểm

##### Mô tả:
Cho phép người dùng chuyển điểm từ ví của mình đến ví người khác.

##### Dữ liệu đầu vào:
- `fromWalletId`
- `toWalletId`
- `amount`
- Mã OTP từ người gửi

##### Luồng xử lý:
1. Kiểm tra sự tồn tại của cả hai ví.
2. Kiểm tra `fromWallet.balance >= amount`.
3. Gửi mã OTP đến người gửi.
4. Người gửi nhập OTP để xác thực.
5. Nếu OTP đúng:
   - Trừ `amount` khỏi `fromWallet`.
   - Cộng `amount` vào `toWallet`.
   - Tạo đối tượng `Transaction` và ghi vào lịch sử của cả hai ví.

##### Ràng buộc:
- Cấm chuyển điểm đến chính ví mình.
- Số điểm giao dịch phải > 0.
- Chỉ thực hiện nếu OTP hợp lệ và còn hiệu lực.

#### 3.4. Giao dịch atomic (Atomic Transaction)

##### Mô tả:
Đảm bảo giao dịch chuyển điểm được thực hiện toàn vẹn, không sai lệch trạng thái nếu một bước thất bại.

##### Dữ liệu đầu vào:
- Cùng đầu vào như giao dịch chuyển điểm.

##### Luồng xử lý:
1. Thực hiện `fromWallet.balance -= amount`.
2. Nếu thành công, tiếp tục `toWallet.balance += amount`.
3. Nếu bước 2 thất bại → hoàn tác bước 1 (rollback).
4. Ghi trạng thái vào `Transaction.status`:
   - `"success"` nếu cả 2 bước đều thành công.
   - `"failed"` nếu một bước bị lỗi.

##### Ràng buộc:
- Tính toàn vẹn dữ liệu phải được bảo đảm tuyệt đối.
- Giao dịch không để lại trạng thái trung gian.

#### 3.5. Lịch sử giao dịch

##### Mô tả:
Ghi lại mọi giao dịch mà ví đã thực hiện hoặc nhận.

##### Dữ liệu đầu vào:
- `walletId`

##### Luồng xử lý:
1. Truy xuất `wallet.transactionHistory`.
2. Hiển thị danh sách `Transaction` theo thời gian.

##### Ràng buộc:
- Người dùng chỉ xem được lịch sử ví của mình.
- Quản trị viên (`admin`) có thể xem lịch sử của bất kỳ ví nào.

## 📋 Bảng Cấu trúc Dữ liệu

Bảng dưới đây mô tả chi tiết các lớp dữ liệu chính được sử dụng trong hệ thống: `User`, `Wallet`, `Transaction`, `OTPManager`, và `DataManager`.  
Mỗi bảng con tương ứng với một lớp, liệt kê các thuộc tính hoặc phương thức chính, cùng với kiểu dữ liệu và mô tả rõ ràng giúp lập trình viên dễ dàng triển khai, mở rộng và bảo trì hệ thống.

---

### 1. `User`

> Lớp `User` lưu trữ thông tin tài khoản người dùng, bao gồm thông tin đăng nhập, vai trò và ví điểm gắn liền với người đó. Ngoài ra, người dùng còn có thể được yêu cầu đổi mật khẩu khi hệ thống sinh mật khẩu tự động.

| STT | Tên trường           | Kiểu dữ liệu       | Mô tả chi tiết                                         |
|-----|----------------------|--------------------|--------------------------------------------------------|
| 1   | username             | std::string        | Tên đăng nhập duy nhất                                |
| 2   | hashedPassword       | std::string        | Mật khẩu đã băm (SHA-256)                             |
| 3   | role                 | std::string        | Vai trò người dùng: `"user"` hoặc `"admin"`           |
| 4   | fullName             | std::string        | Họ tên đầy đủ                                          |
| 5   | email                | std::string        | Địa chỉ email để gửi mã OTP                           |
| 6   | forcePasswordChange | bool               | Cờ yêu cầu đổi mật khẩu nếu là mật khẩu tự sinh       |
| 7   | wallet               | Wallet             | Ví điểm gắn với người dùng                            |

---

### 2. `Wallet`

> Lớp `Wallet` đại diện cho một ví điểm gắn với mỗi người dùng. Mỗi ví có mã định danh duy nhất, số dư điểm và danh sách giao dịch từng thực hiện.

| STT | Tên trường           | Kiểu dữ liệu       | Mô tả chi tiết                                         |
|-----|----------------------|--------------------|--------------------------------------------------------|
| 1   | walletId             | std::string        | Mã định danh ví duy nhất                               |
| 2   | balance              | int                | Số điểm hiện tại trong ví                              |
| 3   | transactionHistory   | std::vector<Transaction> | Danh sách các giao dịch đã thực hiện              |

---

### 3. `Transaction`

> Lớp `Transaction` ghi lại một giao dịch điểm giữa hai ví. Thông tin gồm mã ví nguồn, ví đích, số điểm, thời gian và trạng thái giao dịch.

| STT | Tên trường           | Kiểu dữ liệu       | Mô tả chi tiết                                         |
|-----|----------------------|--------------------|--------------------------------------------------------|
| 1   | fromWalletId         | std::string        | Mã ví nguồn                                            |
| 2   | toWalletId           | std::string        | Mã ví đích                                             |
| 3   | amount               | int                | Số điểm giao dịch                                      |
| 4   | timestamp            | std::string        | Thời điểm thực hiện giao dịch                          |
| 5   | status               | std::string        | Trạng thái: `"success"`, `"failed"`, `"pending"`       |

---

### 4. `OTPManager`

> Lớp `OTPManager` đảm nhiệm việc sinh và xác thực mã OTP dùng để bảo vệ các thao tác nhạy cảm như cập nhật thông tin hoặc thực hiện giao dịch điểm.

| STT | Tên hàm              | Kiểu trả về        | Mô tả chi tiết                                         |
|-----|----------------------|--------------------|--------------------------------------------------------|
| 1   | generateOTP          | std::string        | Tạo mã OTP cho người dùng                              |
| 2   | validateOTP          | bool               | Xác thực mã OTP đã nhập                                |

---

### 5. `DataManager`

> Lớp `DataManager` hỗ trợ lưu trữ và quản lý dữ liệu người dùng, bao gồm chức năng sao lưu và phục hồi để đảm bảo an toàn dữ liệu.

| STT | Tên hàm              | Kiểu trả về        | Mô tả chi tiết                                         |
|-----|----------------------|--------------------|--------------------------------------------------------|
| 1   | saveUser             | bool               | Lưu thông tin người dùng vào tệp                       |
| 2   | loadUsers            | bool               | Tải toàn bộ người dùng từ tệp                          |
| 3   | backupData           | bool               | Tạo bản sao lưu dữ liệu                                |
| 4   | restoreBackup        | bool               | Phục hồi dữ liệu từ bản sao lưu                        |


## 📥 Cách tải và dịch chương trình

### Yêu cầu:
- Compiler hỗ trợ C++11 hoặc cao hơn (g++, clang++)

### Tải source code:
```bash
git clone https://github.com/luanzero/WalletManager.git
cd WalletManager
```

### Dịch chương trình:
```bash
Chạy CodeBlocks: 
- Chọn File Open chọn tới thư mục dự án chọn walletmanager.cbp 
- Sau đó nhấn Build > Build hoặc phím tắt F9.
- Kiểm tra cửa sổ Build log xem có lỗi không.
Note: Download bản CodeBlocks có cả mingw: https://www.codeblocks.org/downloads/binaries/
```

## ▶️ Cách chạy chương trình

Sau khi dịch thành công, chạy:

```bash
Nhấn Build > Run hoặc phím tắt Ctrl + F10 hoặc F10.
```

#### HƯỚNG DẪN SỬ DỤNG HỆ THỐNG QUẢN LÝ VÍ ĐIỂM

##### 1. Giới thiệu chung

Phần mềm này là hệ thống quản lý ví điểm cho người dùng, gồm hai loại tài khoản: **Admin (quản lý)** và **User (người dùng thường)**. Hệ thống lưu trữ dữ liệu trong thư mục `data` và có chức năng backup tại thư mục `backup`.

##### 2. Khởi động chương trình

- Khi chạy chương trình, hệ thống sẽ tự động tạo các thư mục `data`, `backup`, và `logs` nếu chưa có.
- Dữ liệu người dùng, giao dịch và ví tổng sẽ được load tự động từ:
    - `data/users.txt`
    - `data/transactions.txt`
    - `data/global_wallet.txt`

##### 3. Màn hình chính
Sau khi chạy, bạn sẽ thấy menu chính:
===== HE THONG VI DIEM =====
1. Dang ky tai khoan
2. Dang nhap
0. Thoat
Chon:
###### 3.1. Đăng ký tài khoản mới

- Nhấn `1`, nhập:
    - **Tên đăng nhập**
    - **Email**
    - **Họ tên**
    - **Mật khẩu** (nhập 2 lần để xác nhận)
- Nếu tên đăng nhập đã tồn tại hoặc mật khẩu nhập lại không trùng khớp, hệ thống sẽ báo lỗi.

##### 3.2. Đăng nhập

- Nhấn `2`, nhập:
    - **Tên đăng nhập**
    - **Mật khẩu**
- Nếu thông tin chính xác, sẽ chuyển tới menu:
    - **Admin**: Menu quản lý
    - **User thường**: Menu người dùng

##### 4. Chức năng người dùng (User)
Khi đăng nhập bằng tài khoản thường, bạn sẽ thấy menu:
===== MENU NGUOI DUNG =====
1. Cap nhat thong tin ca nhan
2. Thay doi mat khau
3. Xem vi diem va lich su giao dich
4. Chuyen diem
5. Xem thong tin ca nhan
0. Dang xuat
Chon:
- **1. Cập nhật thông tin cá nhân:** Thay đổi thông tin hồ sơ cá nhân.
- **2. Thay đổi mật khẩu:** Đổi mật khẩu đăng nhập.
- **3. Xem ví điểm & lịch sử giao dịch:** Xem số dư ví, các giao dịch đã thực hiện.
- **4. Chuyển điểm:** Chuyển điểm cho tài khoản khác.
- **5. Xem thông tin cá nhân:** Xem lại thông tin hồ sơ.
- **0. Đăng xuất:** Thoát tài khoản.
##### 5. Chức năng quản lý (Admin)
Nếu đăng nhập với quyền admin, bạn sẽ thấy menu:
===== MENU QUAN LY =====
1. Xem danh sach tai khoan
2. Tao tai khoan ho nguoi dung
3. Cap nhat thong tin ho
4. Quan ly vi diem nguoi dung
5. Xem vi tong
6. Nap diem vao vi tong (SYSTEM)
7. Phat diem tu vi tong cho nguoi dung
8. Sao luu du lieu
9. Phuc hoi du lieu tu file
0. Dang xuat
Chon:
**1. Xem danh sách tài khoản:** Liệt kê toàn bộ user.
**2. Tạo tài khoản hộ người dùng:** Admin tạo user mới cho người khác.
**3. Cập nhật thông tin hộ:** Sửa thông tin của user khác.
**4. Quản lý ví điểm người dùng:** Quản lý số dư ví của user.
**5. Xem ví tổng:** Xem số dư ví tổng (GLOBAL).
**6. Nạp điểm vào ví tổng (SYSTEM):** Nhập số điểm nạp cho ví tổng.
**7. Phát điểm từ ví tổng cho người dùng:** Phát điểm từ ví tổng cho user bất kỳ.
**8. Sao lưu dữ liệu:** Backup dữ liệu vào thư mục `backup`.
**9. Phục hồi dữ liệu từ file:** Khôi phục dữ liệu từ file backup.
**0. Đăng xuất:** Thoát tài khoản admin.

##### 6. Các thao tác khác

- **Chuyển điểm:** Chỉ chuyển cho user khác đã có tài khoản trong hệ thống.
- **Backup và Restore:** File backup lưu theo timestamp (dấu thời gian thực hiện).

##### 7. Lưu ý

- Dữ liệu lưu trong thư mục `data` dưới dạng file `.txt`.
- Mỗi khi nạp, chuyển hoặc phát điểm đều lưu lịch sử giao dịch.
- Hãy backup thường xuyên để tránh mất mát dữ liệu.
- Các chức năng chi tiết hơn nằm ở các file khác như `user.h`, `wallet.h`...

##### 8. Gợi ý sử dụng nhanh

1. **Chạy chương trình** → chọn `1` để đăng ký tài khoản admin đầu tiên.
2. Đăng nhập bằng admin, tạo thêm user nếu muốn.
3. Admin nạp điểm vào ví tổng, sau đó phát cho user.
4. User đăng nhập và chuyển điểm cho nhau, xem lịch sử điểm.



## 🔐 Tệp tin và thư viện kèm theo

- `main.cpp` – chương trình chính
- `user.h / user.cpp` – quản lý người dùng
- `wallet.h / wallet.cpp` – quản lý ví
- `transaction.h / transaction.cpp` – xử lý giao dịch
- `utils.h / utils.cpp` – tiện ích (hash, sinh OTP,...)
- `data_manager.h / data_manager.cpp` – Quản lý dữ liệu...
- `data/` – thư mục chứa tệp dữ liệu người dùng và ví
- `backup/` – thư mục chứa bản sao lưu dữ liệu

## 📚 Tài liệu tham khảo

1. Tài liệu về OTP:  
   - [Wikipedia - One-time password](https://en.wikipedia.org/wiki/One-time_password)  
   - [RFC 4226 - HOTP: An HMAC-Based One-Time Password Algorithm](https://datatracker.ietf.org/doc/html/rfc4226)  
   - [RFC 6238 - TOTP: Time-Based One-Time Password Algorithm](https://datatracker.ietf.org/doc/html/rfc6238)

2. Tài liệu về bảo mật mật khẩu:
   - [OWASP Password Storage Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Password_Storage_Cheat_Sheet.html)
   - StackOverflow, GeeksForGeeks: các thuật toán băm như SHA-256 trong C++
