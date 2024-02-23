#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>

using namespace std;

namespace Shiphers {

    class Coder {
    private:
        int lKey[5];
        int lsize;

    public:
        string text, dec_line;

        Coder() {
            lsize = 5;
            for (int i = 0; i < 5; i++) {
                lKey[i] = i;
            }
            for (int i = 0; i < 5; i++) {
                srand(static_cast<unsigned int>(time(0)));
                swap(lKey[i], lKey[rand() % 5]);
            }
        }

        Coder(int* lKey_par) {
            lsize = 5;
            for (int i = 0; i < 5; i++) {
                lKey[i] = lKey_par[i];
            }
        }

        void print_key() {
            cout << "Key:" << endl;
            for (int i = 0; i < 5; i++) {
                cout << lKey[i];
            }
            cout << endl;
        }

        string encode(string filem) {
            long i, j;
            string text, line1, line = "", encstr;
            ifstream ifs(filem.c_str());
            if (!ifs) {
                cout << "Error opening file" << endl;
            }
            else {
                while (getline(ifs, line1))
                    text += line1;
                ifs.close();
                for (i = 0; i < lsize; i++) {
                    for (j = 0; j + lKey[i] < text.length(); j += 5) {
                        line += text[lKey[i] + j];
                    }
                }
            }
            return line;
        }

        string decode(string enc_line) {
            int i, j, iter = 0, k = 0;
            int cols, rows = lsize;
            char** matr;
            string text, dec_line = "", line = "";
            int razmer = static_cast<int>(enc_line.length());
            if ((razmer % lsize) != 0) {
                cols = ((razmer / lsize) + 1);
            }
            else {
                cols = (razmer / lsize);
            }

            matr = new char* [rows];
            for (i = 0; i < rows; i++) {
                matr[i] = new char[cols];
                for (j = 0; j < cols; j++) {
                    matr[i][j] = 0;
                }
            }

            for (i = 0; i < rows; i++) {
                if (lKey[i] < razmer % lsize) {
                    iter = cols;
                }
                else
                    iter = cols - 1;
                for (j = 0; j < iter; j++) {
                    matr[lKey[i]][j] = enc_line[k];
                    k++;
                }
            }
            int count = 0;
            for (j = 0; j < cols; j++) {
                for (i = 0; i < rows; i++) {
                    if (count < razmer) {
                        dec_line += matr[i][j];
                        count++;
                    }
                }
            }
            return dec_line;
        }
    };

    // Function for fast exponentiation with modulo
    int power(int a, int x, int p) {
        int result = 1;
        while (x != 0) {
            if (x % 2 == 0) {
                a = (a * a) % p;
                x = x / 2;
            }
            else {
                x--;
                result = (result * a) % p;
            }
        }
        return result;
    }

    class Shamir {
    private:
        int private_key_C;
        int private_key_D;

    public:
        int p = 107;

        Shamir(int key_C, int key_D) : private_key_C(key_C), private_key_D(key_D) {}

        int pre_code(int num) {
            int a = power(num, private_key_C, p);
            return a;
        }

        int post_code(int num) {
            int a = power(num, private_key_D, p);
            return a;
        }

        string encode(string simple_text, Shamir& b) {
            string text = simple_text;
            for (char& a : text) {
                a = pre_code(a - 47);
            }
            for (char& a : text) {
                a = b.pre_code(a);
            }
            return text;
        }

        string decode(string simple_text, Shamir& a) {
            string text = simple_text;
            for (char& a1 : text) {
                a1 = static_cast<char>(a.post_code(a1));
            }
            for (char& a1 : text) {
                a1 = static_cast<char>(post_code(a1) + 47);
            }
            return text;
        }
    };

    int extendedEuclidean(int U, int V, int restU, int restV) {
        if (V == 0)
            return restU;
        int q = U / V;
        return extendedEuclidean(V, U % V, restV, restU - q * restV);
    }

    class RSA {
    private:
        const int p = 7, q = 13;
        int f_eler;
        int key_d;

    public:
        int expon;
        int n;

        RSA(int key_e) :expon(key_e), n(0) {
            f_eler = (p - 1) * (q - 1);
            int n = p * q;
            int V = expon;
            int U = f_eler;

            if (U < V) {
                key_d = extendedEuclidean(V, U, 1, 0);
                if (key_d < 0)
                    key_d += V;
            }
            else {
                key_d = extendedEuclidean(U, V, 0, 1);
                if (key_d < 0)
                    key_d += U;
            }
        }

        vector<int> encode(string simple_text, int b_e, int b_n) {
            vector<int> res;
            string text = simple_text;

            for (char& a : text) {
                res.push_back(power(a, b_e, b_n));
            }
            return res;
        }

        string decode(vector<int> chiper_text) {
            string text = "";

            for (int s : chiper_text) {
                text += power(s, key_d, n);
            }

            return text;
        }
    };

    class ElGamal {
    private:
        long private_key_a = 357;

    public:
        long P = 1003;
        long g = 2;
        long public_key_b;
        int C1 = 0;

        ElGamal() {
            srand(static_cast<unsigned int>(time(0)));
            (void)rand();
            private_key_a = rand() % (P - 1);
            public_key_b = power(g, private_key_a, P);
        }

        ElGamal(int key) : private_key_a(key) {
            public_key_b = power(g, key, P);
        }

        void ax_by_c(long A, long* X, long B, long* Y, long C) {
            long x1, x2, y1, y2;
            x1 = 1; x2 = 0; y1 = 1; y2 = 0;
            long e1, e2, e3, e4, k, r;
            while (1) {
                r = A % B;
                if (r == 0) break;
                k = (A - r) / B;
                e1 = x2; e2 = x1 - x2 * k;
                e3 = y1; e4 = y2 - y1 * k;
                x1 = e1; x2 = e2;
                y2 = e3; y1 = e4;
                A = B; B = r;
            }
            (*Y) = y1 * C; (*X) = x2 * C;
        }

        string encode(string in, long public_key_b) {
            char out[500] = "";
            int r = 7;
            int C2;
            int X = 0;
            srand((unsigned)time(NULL));
            int i = 0, n = static_cast<int>(in.length());
            while (i < n) {
                char c = in[i];
                X = c;
                r = (long)((double)rand() / (double)RAND_MAX * (P - 2) + 1);
                C1 = power(g, r, P);
                C2 = power(public_key_b, r, P);
                C2 = (C2 * X) % P;
                char tmp[500];
                sprintf_s(tmp, "%d %d ", C1, C2);
                strcat_s(out, tmp);
                i++;
            }
            return out;
        }

        string decode(string out, int C1) {
            string in = "";
            long C2;
            long X = 0, t, Y;
            int k = 0;
            stringstream str;
            str << out;
            while (str >> C1 >> C2) {
                t = power(C1, private_key_a, P);
                ax_by_c(t, &X, P, &Y, C2);
                while (X < 0) X += P;
                X = X % P;
                in += (char)(X);
                k++;
                if (k == 6) { k = 0; }
            }
            return in;
        }
    };

    class DiffiHell {
    private:
        int private_key;
        void generate_public_key() {
            public_key = power(g, private_key, p);
        }

        int secret_key;

    public:
        int public_key;
        int p = 23;
        int g = 5;

        DiffiHell() {
            srand(static_cast<unsigned int>(time(0)));
            (void)rand();
            private_key = rand() % 99 + 1;
            generate_public_key();
        }

        DiffiHell(int key) {
            private_key = key;
            generate_public_key();
        }

        void make_secret_key(int k) {
            secret_key = power(k, private_key, p);
        }

        string encode(string simple_text) {
            string text = simple_text;
            for (char& a : text) {
                a += secret_key;
            }
            return text;
        }

        string decode(string c_text) {
            string text = c_text;
            for (char& a : text) {
                a -= secret_key;
            }
            return text;
        }
    };
}

void printVar(int var) {
    setlocale(LC_ALL, "");
    switch (var) {
    case 1: {
        setlocale(LC_ALL, "ru-RU");

        int lKey_par[5];

        for (int i = 0; i < 5; i++) {
            cout << "KEY[" << i + 1 << "]=";
            cin >> lKey_par[i];
        }
        Shiphers::Coder obj(lKey_par);
        obj.print_key();
        cout << "Encoded text: " << obj.encode("encode.txt") << endl;
        cout << "Decoded text: " << obj.decode(obj.encode("encode.txt")) << endl;

        break;
    }
    case 2: {
        string text;
        cout << "Enter text for encryption:" << endl;
        cin >> text;

        Shiphers::Shamir a(5, 85);
        Shiphers::Shamir b(17, 25);
        text = a.encode(text, b);
        cout << text << endl;
        std::cout << b.decode(text, a) << std::endl;
        text = b.decode(text, a);
        cout << text << endl;

        break;
    }
    case 3: {
        string text;
        cout << "Enter text for encryption:" << endl;
        cin >> text;
        Shiphers::RSA a(7);
        Shiphers::RSA b(7);
        std::vector<int32_t> e_text = a.encode(text, b.expon, b.n);
        for (int32_t a : e_text) {
            cout << a << ' ';
        }
        cout << endl;
        cout << "Text: " << b.decode(e_text) << endl;

        break;
    }
    case 4: {
        string out;
        string text;
        cout << "Enter text for encryption:" << endl;
        cin >> text;
        Shiphers::ElGamal a(30);
        Shiphers::ElGamal b(50);
        out = a.encode(text, b.public_key_b);
        cout << out << endl;
        text = b.decode(out, a.C1);
        cout << text;

        break;
    }
    case 5: {
        string out;
        string text;
        cout << "Enter text for encryption:" << endl;
        cin >> text;
        Shiphers::DiffiHell a(6);
        Shiphers::DiffiHell b(15);
        a.make_secret_key(b.public_key);
        b.make_secret_key(a.public_key);
        out = a.encode(text);

        cout << out << endl;
        text = b.decode(out);
        cout << text;

        break;
    }
    default:
        cout << "Invalid option =(";
        break;
    }
}

int main() {
    setlocale(LC_ALL, "ru-RU");
    int n;
    cout << "1. Simple permutation cipher" << endl;
    cout << "2. Shamir's cipher" << endl;
    cout << "3. RSA cipher" << endl;
    cout << "4. ElGamal cipher" << endl;
    cout << "5. Diffie-Hellman cipher" << endl;

    cout << "Enter the cipher number:";
    cin >> n;
    printVar(n);

    return 0;
}
