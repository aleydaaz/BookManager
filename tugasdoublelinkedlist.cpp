#include <iostream>
#include <string>
#include <fstream>
#include <algorithm> 

using namespace std;

struct buku {
    string judul;
    string author;
    int tahun;
    buku *next;
    buku *prev;
};

// Fungsi untuk menghapus spasi di awal dan akhir string
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Fungsi membuat node buku baru
buku *buatBuku(string judul, string author, int tahun) {
    return new buku{judul, author, tahun, NULL, NULL};
}

// Tambah buku di depan
void SisipDepan(buku **head, buku **tail, string judul, string author, int tahun) {
    buku *newBuku = buatBuku(judul, author, tahun);
    newBuku->next = *head;
    if (*head != NULL) {
        (*head)->prev = newBuku;
        *head = newBuku;
    } else {
        *head = *tail = newBuku;
    }
    cout << "Buku berhasil ditambahkan dari depan.\n";
}

// Tambah buku di belakang
void SisipBelakang(buku **head, buku **tail, string judul, string author, int tahun) {
    buku *newBuku = buatBuku(judul, author, tahun);
    if (*tail != NULL) {
        newBuku->prev = *tail;
        (*tail)->next = newBuku;
        *tail = newBuku;
    } else {
        *head = *tail = newBuku;
    }
    cout << "Buku berhasil ditambahkan dari belakang.\n";
}

// Hapus buku berdasarkan judul
void HapusBuku(buku **head, buku **tail, string judul) {
    buku *bantu = *head;
    while (bantu != NULL) {
        if (bantu->judul == judul) {
            if (bantu->prev != NULL)
                bantu->prev->next = bantu->next;
            else
                *head = bantu->next;

            if (bantu->next != NULL)
                bantu->next->prev = bantu->prev;
            else
                *tail = bantu->prev;

            delete bantu;
            cout << "Buku \"" << judul << "\" berhasil dihapus.\n";
            return;
        }
        bantu = bantu->next;
    }
    cout << "Buku tidak ditemukan.\n";
}

// Cetak daftar buku
void cetakBuku(buku *head) {
    if (head == NULL) {
        cout << "Daftar buku kosong!" << endl;
        return;
    }
    cout << "\n== Daftar Buku ==\n";
    buku *bantu = head;
    while (bantu != NULL) {
        cout << "Judul  : " << bantu->judul << endl;
        cout << "Penulis: " << bantu->author << endl;
        cout << "Tahun  : " << bantu->tahun << endl;
        cout << "------------------------\n";
        bantu = bantu->next;
    }
}

// Cari berdasarkan judul
void cariJudul(buku *head, const string &judulInput) {
    string judul = trim(judulInput);
    buku *bantu = head;
    while (bantu != NULL) {
        if (trim(bantu->judul) == judul) {
            cout << "Buku ditemukan:\n";
            cout << "Judul  : " << bantu->judul << endl;
            cout << "Penulis: " << bantu->author << endl;
            cout << "Tahun  : " << bantu->tahun << endl;
            return;
        }
        bantu = bantu->next;
    }
    cout << "Buku dengan judul \"" << judul << "\" tidak ditemukan.\n";
}

// Cari berdasarkan penulis
void cariAuthor(buku *head, const string &authorInput) {
    string author = trim(authorInput);
    buku *bantu = head;
    bool ketemu = false;
    while (bantu != NULL) {
        if (bantu->author == author) {
            cout << "Judul  : " << bantu->judul << endl;
            cout << "Tahun  : " << bantu->tahun << endl;
            cout << "------------------------\n";
            ketemu = true;
        }
        bantu = bantu->next;
    }
    if (!ketemu)
        cout << "Buku dari penulis \"" << author << "\" tidak ditemukan.\n";
}

// Simpan ke file
void simpanKeFile(buku *head, const string &namaFile) {
    ofstream file(namaFile);
    if (!file) {
        cout << "Gagal membuka file untuk menyimpan data!\n";
        return;
    }

    buku *bantu = head;
    while (bantu != NULL) {
        file << bantu->judul << "|" << bantu->author << "|" << bantu->tahun << "\n";
        bantu = bantu->next;
    }

    file.close();
    cout << "Data berhasil disimpan ke file.\n";
}

// Baca dari file
void bacaDariFile(buku **head, buku **tail, const string &namaFile) {
    ifstream file(namaFile);
    if (!file) {
        return;
    }

    string line, judul, author;
    int tahun;

    while (getline(file, line)) {
        size_t pos1 = line.find("|");
        size_t pos2 = line.rfind("|");
        if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2)
            continue;

        judul = line.substr(0, pos1);
        author = line.substr(pos1 + 1, pos2 - pos1 - 1);
        tahun = stoi(line.substr(pos2 + 1));

        SisipBelakang(head, tail, judul, author, tahun);
    }

    file.close();
}

// Hapus semua node (pembersih memori)
void hapusSemua(buku **head) {
    buku *bantu = *head;
    while (bantu != NULL) {
        buku *hapus = bantu;
        bantu = bantu->next;
        delete hapus;
    }
    *head = NULL;
}

int main() {
    buku *head = NULL;
    buku *tail = NULL;
    int pilih, tahun;
    string judul, author;
    string namaFile = "data_buku.txt";

    bacaDariFile(&head, &tail, namaFile);

    do {
        cout << "\n== MENU MANAJEMEN DAFTAR BUKU ==\n";
        cout << "1. Tambah Buku di Depan\n";
        cout << "2. Tambah Buku di Belakang\n";
        cout << "3. Hapus Buku berdasarkan Judul\n";
        cout << "4. Tampilkan Daftar Buku\n";
        cout << "5. Cari Buku berdasarkan Judul\n";
        cout << "6. Cari Buku berdasarkan Penulis\n";
        cout << "7. Keluar\n";
        cout << "Pilih menu : ";
        cin >> pilih;
        cin.ignore();

        switch (pilih) {
            case 1:
                cout << "Masukkan Judul  : ";
                getline(cin, judul);
                cout << "Masukkan Penulis: ";
                getline(cin, author);
                cout << "Masukkan Tahun  : ";
                cin >> tahun;
                cin.ignore();
                SisipDepan(&head, &tail, judul, author, tahun);
                break;
            case 2:
                cout << "Masukkan Judul  : ";
                getline(cin, judul);
                cout << "Masukkan Penulis: ";
                getline(cin, author);
                cout << "Masukkan Tahun  : ";
                cin >> tahun;
                cin.ignore();
                SisipBelakang(&head, &tail, judul, author, tahun);
                break;
            case 3:
                cout << "Masukkan Judul Buku yang akan dihapus: ";
                getline(cin, judul);
                HapusBuku(&head, &tail, judul);
                break;
            case 4:
                cetakBuku(head);
                break;
            case 5:
                cout << "Masukkan Judul Buku yang dicari: ";
                getline(cin, judul);
                cariJudul(head, judul);
                break;
            case 6:
                cout << "Masukkan Nama Penulis yang dicari: ";
                getline(cin, author);
                cariAuthor(head, author);
                break;
            case 7:
                simpanKeFile(head, namaFile);
                hapusSemua(&head);
                cout << "Program manajemen daftar buku ditutup.\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (pilih != 7);

    return 0;
}
