#include <bits/stdc++.h>
using namespace std;

class Bit {
private:
    int value;

public:
    Bit(int v = 0) {
        value = v & 1;
    }

    int get() const {
        return value;
    }

    Bit operator+(const Bit& other) const {
        return Bit(value ^ other.value);
    }

    Bit operator*(const Bit& other) const {
        return Bit(value & other.value);
    }
};

class BinaryVector {
private:
    vector<Bit> data;

public:
    BinaryVector() {}

    BinaryVector(int n) {
        data.assign(n, Bit(0));
    }

    int size() const {
        return data.size();
    }

    void set(int i, int value) {
        data[i] = Bit(value);
    }

    int get(int i) const {
        return data[i].get();
    }

    int weight() const {
        int cnt = 0;
        for (int i = 0; i < size(); i++) {
            cnt += get(i);
        }
        return cnt;
    }

    string toString() const {
        string s = "";
        for (int i = 0; i < size(); i++) {
            s += char(get(i) + '0');
        }
        return s;
    }

    BinaryVector operator+(const BinaryVector& other) const {
        BinaryVector res(size());
        for (int i = 0; i < size(); i++) {
            res.set(i, get(i) ^ other.get(i));
        }
        return res;
    }
};

class BinaryMatrix {
private:
    int rows, cols;
    vector<vector<Bit>> a;

public:
    BinaryMatrix(int r = 0, int c = 0) {
        rows = r;
        cols = c;
        a.assign(rows, vector<Bit>(cols, Bit(0)));
    }

    void set(int i, int j, int value) {
        a[i][j] = Bit(value);
    }

    int get(int i, int j) const {
        return a[i][j].get();
    }

    BinaryVector multiply(const BinaryVector& v) const {
        BinaryVector res(cols);
        for (int j = 0; j < cols; j++) {
            int sum = 0;
            for (int i = 0; i < rows; i++) {
                sum ^= (v.get(i) & get(i, j));
            }
            res.set(j, sum);
        }
        return res;
    }

    void print() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << get(i, j);
            }
            cout << "\n";
        }
    }
};

BinaryVector numberToBinaryVector(int mask, int k) {
    BinaryVector v(k);
    for (int i = 0; i < k; i++) {
        int bit = (mask >> (k - 1 - i)) & 1;
        v.set(i, bit);
    }
    return v;
}

int hammingDistance(const BinaryVector& a, const BinaryVector& b) {
    int d = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a.get(i) != b.get(i)) {
            d++;
        }
    }
    return d;
}

int main() {
    // Tối ưu I/O cho ICPC
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int mode;
    int k, n;

    // Đọc liên tục đến khi hết file input (chuẩn chấm tự động của ICPC)
    while (cin >> mode >> k >> n) {
        BinaryMatrix G(k, n);

        for (int i = 0; i < k; i++) {
            string s;
            cin >> s;
            for (int j = 0; j < n; j++) {
                G.set(i, j, s[j] - '0');
            }
        }

        vector<BinaryVector> messages;
        vector<BinaryVector> codewords;

        int total = 1 << k;

        for (int mask = 0; mask < total; mask++) {
            BinaryVector u = numberToBinaryVector(mask, k);
            BinaryVector c = G.multiply(u);

            messages.push_back(u);
            codewords.push_back(c);
        }

        int dmin = n + 1;

        // Vòng lặp từ 1 để bỏ qua vector 0 (chỉ xét trọng số của từ mã khác 0)
        for (int i = 1; i < total; i++) {
            dmin = min(dmin, codewords[i].weight());
        }

        int detect = dmin - 1;
        int correct = (dmin - 1) / 2;

        if (mode == 1) {
            // Mode 1: Chuẩn đầu ra ICPC / Trắc nghiệm (chỉ in đáp án)
            cout << total << "\n";

            for (int i = 0; i < total; i++) {
                cout << codewords[i].toString() << "\n";
            }

            cout << dmin << "\n";
            cout << detect << "\n";
            cout << correct << "\n";
        } else {
            // Mode khác: Dạng lời giải tự luận đầy đủ
            cout << "MA TRAN SINH G:\n";
            G.print();

            cout << "\nDANH SACH CAC TU MA:\n";
            cout << "Thong diep u -> Tu ma c = uG -> Trong so\n";

            for (int i = 0; i < total; i++) {
                cout << messages[i].toString()
                     << " -> "
                     << codewords[i].toString()
                     << " -> "
                     << codewords[i].weight()
                     << "\n";
            }

            cout << "\nVi ma la ma khoi tuyen tinh nen d_min bang trong so nho nhat cua cac tu ma khac 0.\n";
            cout << "d_min = " << dmin << "\n";

            cout << "\nKha nang phat hien loi:\n";
            cout << "Ma phat hien duoc toi da d_min - 1 = "
                 << dmin << " - 1 = " << detect << " loi.\n";

            cout << "\nKha nang sua loi:\n";
            cout << "Ma sua duoc toi da floor((d_min - 1) / 2) = "
                 << correct << " loi.\n";
        }
        // Thêm dòng ngăn cách nếu cần nạp nhiều test liên tiếp
        if (mode != 1) cout << "--------------------------------\n"; 
    }

    return 0;
}
