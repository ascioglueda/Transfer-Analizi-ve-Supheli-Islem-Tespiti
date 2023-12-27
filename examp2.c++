#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

ostream& customSetw(ostream& os, int width) {
    int spaceCount = width - os.tellp();
    for (int i = 0; i < spaceCount; ++i) {
        os << ' ';
    }
    return os;
}


struct KisiFirmaBilgisi {
    int id;
    string ad;
    double vergiBeyani;
    double maxIsKapasitesi;
};

void dosyaOku(KisiFirmaBilgisi kisilerFirmalar[], int& kisiFirmaSayisi, int transferler[][100], int& transferSayisi, ifstream& kisilerFirmalarDosya, ifstream& transferDosya);
void toplamTransfer(int transferler[][100], int transferSayisi);
void transferListele(int transferler[][100], int transferSayisi, KisiFirmaBilgisi kisilerFirmalar[]);
void supheliBul(KisiFirmaBilgisi kisilerFirmalar[], int kisiFirmaSayisi, int transferler[][100], int transferSayisi);

int main() {
    const int MAX_KISI_FIRMA = 100;
    const int MAX_TRANSFER = 100;

    KisiFirmaBilgisi kisilerFirmalar[MAX_KISI_FIRMA];
    int kisiFirmaSayisi = 0;

    int transferler[MAX_TRANSFER][100];
    int transferSayisi = 0;

    ifstream kisilerFirmalarDosya("kisilervefirmalar.txt");
    ifstream transferDosya("transfer.txt");

    if (!kisilerFirmalarDosya.is_open() || !transferDosya.is_open()) {
        cerr << "Dosyalar acilamadi!" << endl;
        exit(EXIT_FAILURE);
    }

    dosyaOku(kisilerFirmalar, kisiFirmaSayisi, transferler, transferSayisi, kisilerFirmalarDosya, transferDosya);

    kisilerFirmalarDosya.close();
    transferDosya.close();

    toplamTransfer(transferler, transferSayisi);
    transferListele(transferler, transferSayisi, kisilerFirmalar);
    supheliBul(kisilerFirmalar, kisiFirmaSayisi, transferler, transferSayisi);

    return 0;
}

void dosyaOku(KisiFirmaBilgisi kisilerFirmalar[], int& kisiFirmaSayisi, int transferler[][100], int& transferSayisi, ifstream& kisilerFirmalarDosya, ifstream& transferDosya) {
    int id;
    string ad;
    double vergiBeyani, maxIsKapasitesi;

    while (kisiFirmaSayisi < 100 && kisilerFirmalarDosya >> id >> ad >> vergiBeyani >> maxIsKapasitesi) {
        kisilerFirmalar[kisiFirmaSayisi].id = id;
        kisilerFirmalar[kisiFirmaSayisi].ad = ad;
        kisilerFirmalar[kisiFirmaSayisi].vergiBeyani = vergiBeyani;
        kisilerFirmalar[kisiFirmaSayisi].maxIsKapasitesi = maxIsKapasitesi;
        kisiFirmaSayisi++;
    }

    int deger;
    string satir;
    while (getline(transferDosya, satir) && transferSayisi < 100) {
        istringstream satirStream(satir);
        int index = 0;

        while (satirStream >> deger && index < 100) {
            transferler[transferSayisi][index++] = deger;
        }

        transferSayisi++;
    }
}

void toplamTransfer(int transferler[][100], int transferSayisi) {
    int toplam = 0;

    for (int i = 0; i < transferSayisi; ++i) {
        for (int j = 0; j < 100; ++j) {
            toplam += transferler[i][j];
        }
    }

    cout << "Toplam Transfer: " << toplam << endl;
}

void transferListele(int transferler[][100], int transferSayisi, KisiFirmaBilgisi kisilerFirmalar[]) {
    cout << "Transfer Listesi:" << endl;

    for (int i = 0; i < transferSayisi; ++i) {
        bool transferVar = false;  // Bu transferde veri var mı?

        for (int j = 0; j < 100; ++j) {
            if (transferler[i][j] != 0) {
                transferVar = true;
                customSetw(cout, 10) << transferler[i][j] << " ";
            }
        }

        if (transferVar) { 
            cout <<"\n\n"; // Eğer bu transferde veri varsa
            cout << "\n  (Gonderen: " << kisilerFirmalar[i].ad << ", Alan: " << kisilerFirmalar[i].ad << ")" << endl;
        }
    }
}


void supheliBul(KisiFirmaBilgisi kisilerFirmalar[], int kisiFirmaSayisi, int transferler[][100], int transferSayisi) {
    ofstream supheliDosya("SupheliListesi.txt");

    if (!supheliDosya.is_open()) {
        cerr << "Supheli listesi dosyasi acilamadi!" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Supheli Bulundu mu: ";

    for (int i = 0; i < kisiFirmaSayisi; ++i) {
        double vergiBeyani = kisilerFirmalar[i].vergiBeyani;
        double maxIsKapasitesi = kisilerFirmalar[i].maxIsKapasitesi;
        double toplamTransfer = 0;

        for (int j = 0; j < transferSayisi; ++j) {
            toplamTransfer += transferler[j][i];
        }

        if ((vergiBeyani <= maxIsKapasitesi) && (vergiBeyani == toplamTransfer)) {
            cout << "Hayir" << endl;
        } else if (vergiBeyani > 2 * maxIsKapasitesi) {
            cout << "Evet" << endl;
            supheliDosya << "ID: " << kisilerFirmalar[i].id << ", Ad: " << kisilerFirmalar[i].ad << endl;
        } else if (vergiBeyani != toplamTransfer) {
            cout << "Evet" << endl;
            supheliDosya << "ID: " << kisilerFirmalar[i].id << ", Ad: " << kisilerFirmalar[i].ad << endl;
        }
    }

    supheliDosya.close();
}
