# Binary Linear Block Code Simulator

## Giới thiệu
Chương trình mô phỏng mã khối tuyến tính nhị phân (Binary Linear Block Codes).

Chương trình cho phép:
- Sinh tất cả các từ mã từ ma trận sinh G
- Tính trọng số Hamming
- Tính khoảng cách tối thiểu d_min
- Xác định khả năng:
  - phát hiện lỗi
  - sửa lỗi

Hỗ trợ:
- Chế độ ICPC/output ngắn gọn
- Chế độ in lời giải chi tiết

---

# Ý tưởng thuật toán

Cho ma trận sinh:

G(k × n)

Mỗi thông điệp nhị phân u có độ dài k sẽ sinh ra từ mã:

c = uG (mod 2)

Chương trình:
1. Sinh toàn bộ 2^k thông điệp
2. Nhân với ma trận G
3. Sinh danh sách từ mã
4. Tính:
   - trọng số từng từ mã
   - d_min
   - số lỗi phát hiện được
   - số lỗi sửa được

---

# Công thức sử dụng

## Khoảng cách tối thiểu
Với mã tuyến tính:

d_min = trọng số nhỏ nhất của từ mã khác 0

---

## Khả năng phát hiện lỗi

Phát hiện tối đa:

detect = d_min - 1

---

## Khả năng sửa lỗi

Sửa tối đa:

correct = floor((d_min - 1) / 2)

---

# Cấu trúc chương trình

## Class Bit
Biểu diễn 1 bit nhị phân:
- phép XOR
- phép AND

---

## Class BinaryVector
Biểu diễn vector nhị phân:
- lưu dữ liệu
- tính trọng số
- cộng vector modulo 2

---

## Class BinaryMatrix
Biểu diễn ma trận nhị phân:
- lưu ma trận sinh
- nhân vector với ma trận

---

# Định dạng Input

Dữ liệu gồm nhiều test liên tiếp.

Mỗi test:

mode k n
<k dòng ma trận>

Trong đó:
- mode = 1:
  - chỉ in output chuẩn ICPC
- mode != 1:
  - in lời giải chi tiết
- k:
  - số bit thông điệp
- n:
  - độ dài từ mã

Ví dụ:

1 2 3
101
011

---

# Định dạng Output

## Mode 1
In:
1. số lượng từ mã
2. danh sách từ mã
3. d_min
4. số lỗi phát hiện được
5. số lỗi sửa được

Ví dụ:

4
000
011
101
110
2
1
0

---

## Mode 2
In đầy đủ:
- ma trận sinh
- bảng thông điệp
- từ mã
- trọng số
- phân tích d_min
- khả năng phát hiện/sửa lỗi

---

# Độ phức tạp

Số lượng thông điệp:
2^k

Độ phức tạp:
O(2^k × k × n)

Phù hợp với:
- bài tập học phần
- mô phỏng mã hóa
- kiểm thử ICPC nhỏ

---

# Biên dịch và chạy

## Compile

g++ Main.cpp -o Main

---

## Chạy bằng terminal

./Main

---

## Chạy với file input/output

- chạy trên cmd
Main < Testcase\input > Testcase\output (Ví dụ: Main < Testcase\test01.in > Testcase\test01.out)
- chạy trên terminal trong vsc
cmd /c "Main < Testcase\test01.in > Testcase\test01.out"

---

# Ví dụ

## Input

1 2 3
101
011

## Output

4
000
011
101
110
2
1
0

---

# Bộ test tiêu biểu

## test01
- Ma trận đơn vị
- d_min = 1

## test02
- Mã parity cơ bản

## test03
- Mã Hamming (7,4)

## test04
- Khoảng cách lớn## test05
- Dòng toàn 0

## test06
- Hai dòng trùng nhau

## test07
- Mode 2 chi tiết

## test08
- d_min = 3

## test09
- Sửa được nhiều lỗi

## test10
- Nhiều test liên tiếp
