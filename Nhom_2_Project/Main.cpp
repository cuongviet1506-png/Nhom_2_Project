/*
 * HUFFMAN CODING - Ly thuyet Thong tin
 * Chuan ICPC + Menu lua chon output
 *
 * INPUT (stdin hoac file .in):
 *   Mode 1: Nhap tan so
 *     Mode 1
 *     n
 *     ky_tu_1 tan_so_1
 *     ky_tu_2 tan_so_2
 *     ...
 *   Mode 2: Nhap xac suat
 *     Mode 2
 *     n
 *     ky_tu_1 xac_suat_1
 *     ...
 *   Mode 3: Nhap chuoi van ban
 *     Mode 3
 *     chuoi_van_ban
 *
 * OUTPUT (stdout hoac file .out):
 *   0 = ICPC chuan (chi ket qua)
 *   1 = Loi giai tu luan day du
 *   2 = Ket qua trac nghiem (A/B/C/D)
 */

#include <bits/stdc++.h>
using namespace std;

// ==================== CAU TRUC DU LIEU ====================

struct HuffNode {
    char ch;
    double freq;
    int left, right; // index trong mang nodes
    HuffNode(char c, double f) : ch(c), freq(f), left(-1), right(-1) {}
    HuffNode(double f, int l, int r) : ch('\0'), freq(f), left(l), right(r) {}
};

vector<HuffNode> nodes;
map<char, string> huffCode;
map<char, double> charFreq;  // xac suat goc
int outputMode = 0; // 0=ICPC, 1=Tu luan, 2=Trac nghiem

// ==================== XAY DUNG CAY HUFFMAN ====================

void buildHuffman(vector<pair<double, int>>& symbols) {
    // Min-heap: (freq, node_index)
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;
    for (auto& s : symbols)
        pq.push(s);

    while (pq.size() > 1) {
        auto [f1, i1] = pq.top(); pq.pop();
        auto [f2, i2] = pq.top(); pq.pop();
        double newFreq = f1 + f2;
        nodes.push_back(HuffNode(newFreq, i1, i2));
        int newIdx = nodes.size() - 1;
        pq.push({newFreq, newIdx});
    }
}

void generateCodes(int idx, string code) {
    if (idx == -1) return;
    HuffNode& node = nodes[idx];
    if (node.left == -1 && node.right == -1) {
        huffCode[node.ch] = code.empty() ? "0" : code; // truong hop chi co 1 ky tu
        return;
    }
    generateCodes(node.left,  code + "0");
    generateCodes(node.right, code + "1");
}

// ==================== IN CAY HUFFMAN (dang text) ====================

void printTree(int idx, string prefix, bool isLeft, ostream& out) {
    if (idx == -1) return;
    HuffNode& node = nodes[idx];
    out << prefix;
    out << (isLeft ? "|-- " : "\\-- ");
    if (node.left == -1 && node.right == -1)
        out << "[" << node.ch << " | p=" << fixed << setprecision(4) << node.freq << "]";
    else
        out << "(p=" << fixed << setprecision(4) << node.freq << ")";
    out << "\n";
    string newPrefix = prefix + (isLeft ? "|   " : "    ");
    printTree(node.left,  newPrefix, true,  out);
    printTree(node.right, newPrefix, false, out);
}

// ==================== TINH DO DAI TRUNG BINH ====================

double avgCodeLength() {
    double avg = 0;
    for (auto& [ch, code] : huffCode)
        avg += charFreq[ch] * code.size();
    return avg;
}

double entropy() {
    double H = 0;
    for (auto& [ch, p] : charFreq)
        if (p > 0) H -= p * log2(p);
    return H;
}

double efficiency() {
    double L = avgCodeLength();
    double H = entropy();
    return (L > 0) ? (H / L) * 100.0 : 0;
}

// ==================== OUTPUT MODE 0: ICPC CHUAN ====================

void outputICPC(ostream& out) {
    // In bang ma
    out << huffCode.size() << "\n";
    for (auto& [ch, code] : huffCode) {
        if (ch == ' ') out << "SPACE";
        else out << ch;
        out << " " << code << "\n";
    }
    out << fixed << setprecision(4) << avgCodeLength() << "\n";
}

// ==================== OUTPUT MODE 1: LOI GIAI TU LUAN ====================

void outputTuLuan(ostream& out) {
    out << "============================================================\n";
    out << "         LOI GIAI MA HOA HUFFMAN - LY THUYET THONG TIN\n";
    out << "============================================================\n\n";

    // Buoc 1: Xac suat
    out << "BUOC 1: BANG XAC SUAT CAC KY TU\n";
    out << string(50, '-') << "\n";
    out << left << setw(10) << "Ky tu" << setw(15) << "Xac suat" << "\n";
    out << string(25, '-') << "\n";
    for (auto& [ch, p] : charFreq) {
        if (ch == ' ') out << left << setw(10) << "SPACE";
        else out << left << setw(10) << ch;
        out << setw(15) << fixed << setprecision(6) << p << "\n";
    }
    out << "\n";

    // Buoc 2: Giai thich thuat toan
    out << "BUOC 2: THUAT TOAN XAY DUNG CAY HUFFMAN\n";
    out << string(50, '-') << "\n";
    out << "- Khoi tao: Moi ky tu la mot nut la voi trong so = xac suat.\n";
    out << "- Lap: Lay 2 nut co xac suat nho nhat, gop thanh nut cha.\n";
    out << "- Lap lai cho den khi chi con 1 nut (goc cay).\n";
    out << "- Gan ma: Nut trai = '0', nut phai = '1'.\n\n";

    // Buoc 3: Cay Huffman
    out << "BUOC 3: CAY HUFFMAN\n";
    out << string(50, '-') << "\n";
    if (!nodes.empty())
        printTree(nodes.size() - 1, "", false, out);
    out << "\n";

    // Buoc 4: Bang ma
    out << "BUOC 4: BANG MA HUFFMAN\n";
    out << string(50, '-') << "\n";
    out << left << setw(10) << "Ky tu"
        << setw(12) << "Xac suat"
        << setw(15) << "Ma Huffman"
        << setw(10) << "Do dai" << "\n";
    out << string(47, '-') << "\n";
    for (auto& [ch, code] : huffCode) {
        if (ch == ' ') out << left << setw(10) << "SPACE";
        else out << left << setw(10) << ch;
        out << setw(12) << fixed << setprecision(4) << charFreq[ch];
        out << setw(15) << code;
        out << setw(10) << code.size() << "\n";
    }
    out << "\n";

    // Buoc 5: Ket qua
    out << "BUOC 5: CAC THONG SO DANH GIA\n";
    out << string(50, '-') << "\n";
    double L = avgCodeLength();
    double H = entropy();
    double eff = efficiency();
    out << fixed << setprecision(4);
    out << "Entropy nguon H(S)          = " << H << " bits/ky tu\n";
    out << "Do dai ma trung binh L      = " << L << " bits/ky tu\n";
    out << "Hieu qua ma hoa             = " << eff << " %\n";
    out << "Du phu (Redundancy)         = " << (L - H) << " bits/ky tu\n\n";

    out << "NHAN XET:\n";
    if (eff >= 99.0)
        out << "- Ma hoa Huffman dat hieu qua toi uu (gan bang entropy).\n";
    else if (eff >= 90.0)
        out << "- Ma hoa Huffman dat hieu qua cao.\n";
    else
        out << "- Ma hoa Huffman co the cai thien them voi cac ky thuat khac.\n";
    out << "- Ma Huffman la ma phi doan (prefix-free): khong co ma nao la\n";
    out << "  tien to cua ma khac, dam bao giai ma duy nhat.\n";
}

// ==================== OUTPUT MODE 2: TRAC NGHIEM ====================

void outputTracNghiem(ostream& out) {
    out << "============================================================\n";
    out << "         CAU HOI TRAC NGHIEM - MA HOA HUFFMAN\n";
    out << "============================================================\n\n";

    double L = avgCodeLength();
    double H = entropy();
    double eff = efficiency();

    // Sap xep ky tu theo xac suat
    vector<pair<double,char>> sorted_chars;
    for (auto& [ch, p] : charFreq) sorted_chars.push_back({p, ch});
    sort(sorted_chars.rbegin(), sorted_chars.rend());

    int qNum = 1;

    // Cau 1: Ma cua ky tu xac suat cao nhat
    char topChar = sorted_chars[0].second;
    string topCode = huffCode[topChar];
    out << "Cau " << qNum++ << ": Ma Huffman cua ky tu '"
        << (topChar==' '?"SPACE":string(1,topChar))
        << "' (xac suat = " << fixed << setprecision(4) << charFreq[topChar] << ") la?\n";
    // Tao 4 dap an: dap an dung + 3 nhieu
    vector<string> opts = {topCode};
    set<string> used; used.insert(topCode);
    // Lay ma cua ky tu khac
    for (auto& [ch, code] : huffCode) {
        if (used.find(code) == used.end() && opts.size() < 4) {
            opts.push_back(code);
            used.insert(code);
        }
    }
    // Neu khong du 4 thi them ma gia
    while (opts.size() < 4) {
        string fake = "";
        for (int i = 0; i < (int)topCode.size() + 1; i++) fake += (char)('0' + rand()%2);
        if (used.find(fake) == used.end()) { opts.push_back(fake); used.insert(fake); }
    }
    // Xao tron
    int correctIdx = 0;
    shuffle(opts.begin(), opts.end(), default_random_engine(42));
    for (int i = 0; i < 4; i++) if (opts[i] == topCode) correctIdx = i;
    char letters[] = {'A','B','C','D'};
    for (int i = 0; i < 4; i++)
        out << "  " << letters[i] << ". " << opts[i] << "\n";
    out << "=> Dap an: " << letters[correctIdx] << "\n\n";

    // Cau 2: Do dai ma trung binh
    out << "Cau " << qNum++ << ": Do dai ma trung binh L cua ma Huffman la?\n";
    double L2 = round(L * 10000) / 10000.0;
    vector<double> Lopts = {L2, round((L+0.5)*10000)/10000.0,
                             round((L-0.3)*10000)/10000.0,
                             round((L+1.0)*10000)/10000.0};
    shuffle(Lopts.begin(), Lopts.end(), default_random_engine(43));
    int Lcorrect = 0;
    for (int i = 0; i < 4; i++) if (abs(Lopts[i]-L2)<1e-6) Lcorrect = i;
    for (int i = 0; i < 4; i++)
        out << "  " << letters[i] << ". " << fixed << setprecision(4) << Lopts[i] << " bits/ky tu\n";
    out << "=> Dap an: " << letters[Lcorrect] << "\n\n";

    // Cau 3: Entropy
    out << "Cau " << qNum++ << ": Entropy H(S) cua nguon thong tin la?\n";
    double H2 = round(H * 10000) / 10000.0;
    vector<double> Hopts = {H2, round((H+0.4)*10000)/10000.0,
                             round((H-0.2)*10000)/10000.0,
                             round((H+0.8)*10000)/10000.0};
    shuffle(Hopts.begin(), Hopts.end(), default_random_engine(44));
    int Hcorrect = 0;
    for (int i = 0; i < 4; i++) if (abs(Hopts[i]-H2)<1e-6) Hcorrect = i;
    for (int i = 0; i < 4; i++)
        out << "  " << letters[i] << ". " << fixed << setprecision(4) << Hopts[i] << " bits/ky tu\n";
    out << "=> Dap an: " << letters[Hcorrect] << "\n\n";

    // Cau 4: Hieu qua ma hoa
    out << "Cau " << qNum++ << ": Hieu qua ma hoa Huffman la?\n";
    double e2 = round(eff * 100) / 100.0;
    vector<double> eopts = {e2, round((eff-5)*100)/100.0,
                             round((eff+3)*100)/100.0,
                             round((eff-10)*100)/100.0};
    shuffle(eopts.begin(), eopts.end(), default_random_engine(45));
    int ecorrect = 0;
    for (int i = 0; i < 4; i++) if (abs(eopts[i]-e2)<1e-4) ecorrect = i;
    for (int i = 0; i < 4; i++)
        out << "  " << letters[i] << ". " << fixed << setprecision(2) << eopts[i] << " %\n";
    out << "=> Dap an: " << letters[ecorrect] << "\n\n";

    // Cau 5: Tinh chat ma phi doan
    out << "Cau " << qNum++ << ": Ma Huffman la ma phi doan (prefix-free). Dieu nay co nghia la?\n";
    out << "  A. Moi ky tu co do dai ma bang nhau\n";
    out << "  B. Khong co ma nao la tien to (prefix) cua ma khac\n";
    out << "  C. Ma chi gom toan so 0\n";
    out << "  D. Do dai ma bang entropy nguon\n";
    out << "=> Dap an: B\n\n";

    // Cau 6: Ky tu nao co ma dai nhat
    char longestChar = ' ';
    size_t maxLen = 0;
    for (auto& [ch, code] : huffCode)
        if (code.size() > maxLen) { maxLen = code.size(); longestChar = ch; }
    out << "Cau " << qNum++ << ": Ky tu nao co ma Huffman DAI NHAT?\n";
    vector<char> charOpts = {longestChar};
    set<char> usedC; usedC.insert(longestChar);
    for (auto& [ch, p] : charFreq)
        if (usedC.find(ch)==usedC.end() && charOpts.size()<4)
            { charOpts.push_back(ch); usedC.insert(ch); }
    shuffle(charOpts.begin(), charOpts.end(), default_random_engine(46));
    int ccorrect = 0;
    for (int i = 0; i < (int)charOpts.size(); i++)
        if (charOpts[i] == longestChar) ccorrect = i;
    for (int i = 0; i < (int)charOpts.size(); i++)
        out << "  " << letters[i] << ". '"
            << (charOpts[i]==' '?"SPACE":string(1,charOpts[i])) << "'\n";
    out << "=> Dap an: " << letters[ccorrect] << "\n\n";
}

// ==================== DOC INPUT ====================

void readInput(istream& in) {
    int mode;
    string modeStr;
    in >> modeStr >> mode;

    nodes.clear();
    huffCode.clear();
    charFreq.clear();

    int n;
    vector<pair<double, int>> symbols;

    if (mode == 1) {
        // Mode 1: Tan so
        in >> n;
        long long totalFreq = 0;
        vector<pair<char,long long>> rawFreqs;
        for (int i = 0; i < n; i++) {
            char ch; long long f;
            in >> ch >> f;
            rawFreqs.push_back({ch, f});
            totalFreq += f;
        }
        for (auto& [ch, f] : rawFreqs) {
            double p = (double)f / totalFreq;
            charFreq[ch] = p;
            nodes.push_back(HuffNode(ch, p));
            symbols.push_back({p, (int)nodes.size()-1});
        }
    } else if (mode == 2) {
        // Mode 2: Xac suat
        in >> n;
        for (int i = 0; i < n; i++) {
            char ch; double p;
            in >> ch >> p;
            charFreq[ch] = p;
            nodes.push_back(HuffNode(ch, p));
            symbols.push_back({p, (int)nodes.size()-1});
        }
    } else {
        // Mode 3: Chuoi van ban
        string text;
        in >> ws;
        getline(in, text);
        map<char, int> freq;
        for (char c : text) freq[c]++;
        for (auto& [ch, f] : freq) {
            double p = (double)f / text.size();
            charFreq[ch] = p;
            nodes.push_back(HuffNode(ch, p));
            symbols.push_back({p, (int)nodes.size()-1});
        }
    }

    buildHuffman(symbols);
    if (!nodes.empty())
        generateCodes(nodes.size()-1, "");
}

// ==================== MENU ====================

void showMenu() {
    cerr << "\n=== CHUONG TRINH MA HOA HUFFMAN ===\n";
    cerr << "Chon che do output:\n";
    cerr << "  0 - ICPC chuan (chi ket qua)\n";
    cerr << "  1 - Loi giai tu luan day du\n";
    cerr << "  2 - Ket qua trac nghiem\n";
    cerr << "Nhap lua chon (0/1/2): ";
    cin >> outputMode;
    if (outputMode < 0 || outputMode > 2) outputMode = 0;

    cerr << "\nChon nguon input:\n";
    cerr << "  0 - Nhap tu ban phim (stdin)\n";
    cerr << "  1 - Doc tu file .in\n";
    cerr << "Nhap lua chon (0/1): ";
}

// ==================== MAIN ====================

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    bool interactiveMenu = (argc == 1);
    string inputFile = "", outputFile = "";

    // Neu co argument: ./huffman input.in [output.out] [outputMode]
    if (argc >= 2) inputFile = argv[1];
    if (argc >= 3) outputFile = argv[2];
    if (argc >= 4) outputMode = atoi(argv[3]);

    if (interactiveMenu) {
        showMenu();
        int inputChoice;
        cin >> inputChoice;
        if (inputChoice == 1) {
            cerr << "Nhap ten file .in: ";
            cin >> inputFile;
        }
        cerr << "Nhap ten file output (de trong = stdout): ";
        cin >> outputFile;
        if (outputFile == "-") outputFile = "";
    }

    // Mo input
    istream* in = &cin;
    ifstream fin;
    if (!inputFile.empty()) {
        fin.open(inputFile);
        if (!fin) { cerr << "Loi: Khong mo duoc file " << inputFile << "\n"; return 1; }
        in = &fin;
    }

    // Mo output
    ostream* out = &cout;
    ofstream fout;
    if (!outputFile.empty() && outputFile != "-") {
        fout.open(outputFile);
        if (!fout) { cerr << "Loi: Khong mo duoc file " << outputFile << "\n"; return 1; }
        out = &fout;
    }

    readInput(*in);

    switch (outputMode) {
        case 0: outputICPC(*out); break;
        case 1: outputTuLuan(*out); break;
        case 2: outputTracNghiem(*out); break;
        default: outputICPC(*out);
    }

    return 0;
}
