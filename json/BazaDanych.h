#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "../class/Pojazdy.h"
#include "../class/Klient.h"
#include "../class/Wypozyczenie.h"
using namespace std;

class BazaDanych {
public:

    // ============================================================
    //  Inicjalizacja - tworzy pliki JSON jesli nie istnieja
    // ============================================================
    static void inicjalizuj() {
        stworzJesliNieIstnieje("json/pojazdy.json");
        stworzJesliNieIstnieje("json/klienci.json");
        stworzJesliNieIstnieje("json/wypozyczenia.json");
    }

    // ============================================================
    //  ZAPIS CALOSCI - atomowy zapis wszystkich danych
    //  Uzyj po kazdej operacji modyfikujacej dane.
    // ============================================================
    static void zapiszWszystkoDoJSON(const vector<Pojazd*>& pojazdy,
                                     const vector<Klient>&  klienci,
                                     const vector<Wypozyczenie>& wypozyczenia) {
        zapiszPojazdy(pojazdy);
        zapiszKlientow(klienci);
        zapiszWypozyczenia(wypozyczenia);
    }

    // ============================================================
    //  ZAPIS POJEDYNCZEGO REKORDU - tylko przy dodawaniu
    // ============================================================
    static void zapiszPojazd(const Pojazd* p) {
        ofstream file("json/pojazdy.json", ios::app);
        if (!file.is_open()) { cerr << "Blad: nie mozna otworzyc pojazdy.json\n"; return; }
        file << serializujPojazd(p) << "\n";
    }

    static void zapiszKlienta(const Klient& k) {
        ofstream file("json/klienci.json", ios::app);
        if (!file.is_open()) { cerr << "Blad: nie mozna otworzyc klienci.json\n"; return; }
        file << serializujKlienta(k) << "\n";
    }

    static void zapiszWypozyczenie(const Wypozyczenie& w) {
        ofstream file("json/wypozyczenia.json", ios::app);
        if (!file.is_open()) { cerr << "Blad: nie mozna otworzyc wypozyczenia.json\n"; return; }
        file << serializujWypozyczenie(w) << "\n";
    }

    // ============================================================
    //  ODCZYT
    // ============================================================
    static vector<Pojazd*> wczytajPojazdy() {
        vector<Pojazd*> pojazdy;
        string calosc = czytajPlik("json/pojazdy.json");
        if (calosc.empty()) return pojazdy;

        size_t pos = 0;
        while ((pos = calosc.find("{", pos)) != string::npos) {
            size_t end = calosc.find("}", pos);
            if (end == string::npos) break;
            string obj = calosc.substr(pos, end - pos + 1);

            string typ     = znajdzStr(obj, "typ");
            int    id      = znajdzInt(obj, "id");
            string marka   = znajdzStr(obj, "marka");
            string model   = znajdzStr(obj, "model");
            int    rok     = znajdzInt(obj, "rok");
            int    przebieg= znajdzInt(obj, "przebieg");
            string status  = znajdzStr(obj, "status");

            if (typ == "samochod") {
                pojazdy.push_back(new Samochod(id, marka, model, rok, przebieg, status,
                                               znajdzInt(obj, "drzwi"),
                                               znajdzStr(obj, "silnik")));
            } else if (typ == "motocykl") {
                pojazdy.push_back(new Motocykl(id, marka, model, rok, przebieg, status,
                                               znajdzInt(obj, "pojemnosc")));
            } else if (typ == "dostawczy") {
                pojazdy.push_back(new Dostawczy(id, marka, model, rok, przebieg, status,
                                                znajdzInt(obj, "ladownosc"),
                                                znajdzStr(obj, "silnik")));
            }
            pos = end + 1;
        }
        return pojazdy;
    }

    static vector<Klient> wczytajKlientow() {
        vector<Klient> klienci;
        string calosc = czytajPlik("json/klienci.json");
        if (calosc.empty()) return klienci;

        size_t pos = 0;
        while ((pos = calosc.find("{", pos)) != string::npos) {
            size_t end = calosc.find("}", pos);
            if (end == string::npos) break;
            string obj = calosc.substr(pos, end - pos + 1);

            klienci.push_back(Klient(
                znajdzInt(obj, "idKlienta"),
                znajdzStr(obj, "imie"),
                znajdzStr(obj, "nazwisko"),
                znajdzStr(obj, "nrPrawaJazdy")
            ));
            pos = end + 1;
        }
        return klienci;
    }

    static vector<Wypozyczenie> wczytajWypozyczenia() {
        vector<Wypozyczenie> lista;
        string calosc = czytajPlik("json/wypozyczenia.json");
        if (calosc.empty()) return lista;

        size_t pos = 0;
        while ((pos = calosc.find("{", pos)) != string::npos) {
            size_t end = calosc.find("}", pos);
            if (end == string::npos) break;
            string obj = calosc.substr(pos, end - pos + 1);

            string koszt_str = znajdzStr(obj, "koszt");
            double koszt = koszt_str.empty() ? 0.0 : stod(koszt_str);
            bool   oddany = (znajdzStr(obj, "oddany") == "true");

            lista.push_back(Wypozyczenie(
                znajdzInt(obj, "idWypozyczenia"),
                znajdzInt(obj, "idPojazdu"),
                znajdzInt(obj, "idKlienta"),
                znajdzStr(obj, "dataOd"),
                znajdzStr(obj, "dataDo"),
                koszt,
                oddany
            ));
            pos = end + 1;
        }
        return lista;
    }

private:

    // ============================================================
    //  SERIALIZACJA
    // ============================================================
    static string serializujPojazd(const Pojazd* p) {
        string out = "{";
        out += "\"typ\":\"" + p->getTyp() + "\",";
        out += "\"id\":"    + to_string(p->getId()) + ",";
        out += "\"marka\":\"" + p->getMarka() + "\",";
        out += "\"model\":\"" + p->getModel() + "\",";
        out += "\"rok\":"     + to_string(p->getRok()) + ",";
        out += "\"przebieg\":" + to_string(p->getPrzebieg()) + ",";
        out += "\"status\":\"" + p->getStatus() + "\"";

        const Samochod* s = dynamic_cast<const Samochod*>(p);
        if (s) {
            out += ",\"silnik\":\"" + s->getSilnik() + "\"";
            out += ",\"drzwi\":"    + to_string(s->getDrzwi());
        }
        const Motocykl* m = dynamic_cast<const Motocykl*>(p);
        if (m) {
            out += ",\"pojemnosc\":" + to_string(m->getPojemnosc());
        }
        const Dostawczy* d = dynamic_cast<const Dostawczy*>(p);
        if (d) {
            out += ",\"silnik\":\"" + d->getSilnik() + "\"";
            out += ",\"ladownosc\":" + to_string(d->getLadownosc());
        }
        out += "}";
        return out;
    }

    static string serializujKlienta(const Klient& k) {
        string out = "{";
        out += "\"idKlienta\":"    + to_string(k.idKlienta) + ",";
        out += "\"imie\":\""       + k.imie + "\",";
        out += "\"nazwisko\":\""   + k.nazwisko + "\",";
        out += "\"nrPrawaJazdy\":\"" + k.nrPrawaJazdy + "\"";
        out += "}";
        return out;
    }

    static string serializujWypozyczenie(const Wypozyczenie& w) {
        string out = "{";
        out += "\"idWypozyczenia\":" + to_string(w.idWypozyczenia) + ",";
        out += "\"idPojazdu\":"      + to_string(w.idPojazdu) + ",";
        out += "\"idKlienta\":"      + to_string(w.idKlienta) + ",";
        out += "\"dataOd\":\""       + w.dataOd + "\",";
        out += "\"dataDo\":\""       + w.dataDo + "\",";
        out += "\"koszt\":"          + to_string(w.koszt) + ",";
        out += "\"oddany\":"         + string(w.oddany ? "true" : "false");
        out += "}";
        return out;
    }

    // ============================================================
    //  ZAPIS CALYCH PLIKOW (nadpisuje)
    // ============================================================
    static void zapiszPojazdy(const vector<Pojazd*>& pojazdy) {
        ofstream file("json/pojazdy.json");
        if (!file.is_open()) { cerr << "Blad zapisu pojazdy.json\n"; return; }
        for (const Pojazd* p : pojazdy)
            file << serializujPojazd(p) << "\n";
    }

    static void zapiszKlientow(const vector<Klient>& klienci) {
        ofstream file("json/klienci.json");
        if (!file.is_open()) { cerr << "Blad zapisu klienci.json\n"; return; }
        for (const Klient& k : klienci)
            file << serializujKlienta(k) << "\n";
    }

    static void zapiszWypozyczenia(const vector<Wypozyczenie>& wypozyczenia) {
        ofstream file("json/wypozyczenia.json");
        if (!file.is_open()) { cerr << "Blad zapisu wypozyczenia.json\n"; return; }
        for (const Wypozyczenie& w : wypozyczenia)
            file << serializujWypozyczenie(w) << "\n";
    }

    // ============================================================
    //  POMOCNICZE
    // ============================================================
    static string czytajPlik(const string& sciezka) {
        ifstream file(sciezka);
        if (!file.is_open()) return "";
        string linia, calosc;
        while (getline(file, linia)) calosc += linia;
        return calosc;
    }

    static void stworzJesliNieIstnieje(const string& sciezka) {
        ifstream test(sciezka);
        if (!test.is_open()) {
            ofstream nowy(sciezka);
        }
    }

    // Wyciaga wartosc stringa z JSON np. "marka":"Toyota" -> "Toyota"
    static string znajdzStr(const string& obj, const string& klucz) {
        size_t pos = obj.find("\"" + klucz + "\"");
        if (pos == string::npos) return "";
        pos = obj.find(":", pos) + 1;
        while (pos < obj.size() && obj[pos] == ' ') pos++;

        if (pos < obj.size() && obj[pos] == '\"') {
            pos++;
            size_t end = obj.find("\"", pos);
            if (end == string::npos) return "";
            return obj.substr(pos, end - pos);
        }
        // wartosc bez cudzyslowow (true/false)
        size_t end = pos;
        while (end < obj.size() && obj[end] != ',' && obj[end] != '}') end++;
        return obj.substr(pos, end - pos);
    }

    // Wyciaga wartosc int z JSON np. "id":5 -> 5
    static int znajdzInt(const string& obj, const string& klucz) {
        string val = znajdzStr(obj, klucz);
        if (val.empty()) return 0;
        try { return stoi(val); } catch (...) { return 0; }
    }
};
