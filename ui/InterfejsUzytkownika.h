#pragma once
#include <iostream>
#include <string>
#include <limits>
#include "../MenedzerFloty.h"
#include "../Statystyki.h"
using namespace std;

// ============================================================
//  Widok - odpowiada za komunikacje z uzytkownikiem.
//  Nie zawiera logiki biznesowej - deleguje do MenedzerFloty.
// ============================================================
class InterfejsUzytkownika {
private:
    MenedzerFloty& menedzer;

    // Czysci bufor cin po blednym wejsciu
    void czyscBufor() const {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Pobiera liczbe calkowita z walidacja
    int pobierzInt(const string& prompt) const {
        int val;
        while (true) {
            cout << prompt;
            if (cin >> val) return val;
            czyscBufor();
            cout << "Blad: podaj liczbe calkowita.\n";
        }
    }

    // Pobiera liczbe rzeczywista z walidacja
    double pobierzDouble(const string& prompt) const {
        double val;
        while (true) {
            cout << prompt;
            if (cin >> val) return val;
            czyscBufor();
            cout << "Blad: podaj liczbe.\n";
        }
    }

public:
    InterfejsUzytkownika(MenedzerFloty& m) : menedzer(m) {}

    void wyswietlGlowneMenu() const {
        cout << "\n===== SYSTEM ZARZADZANIA FLOTA =====\n";
        cout << " 1. Sprawdz pojazdy\n";
        cout << " 2. Wypozycz pojazd\n";
        cout << " 3. Zwroc pojazd\n";
        cout << " 4. Dodaj pojazd\n";
        cout << " 5. Usun pojazd\n";
        cout << " 6. Wyświetl historie Wypozyczen\n";
        cout << " 7. Status techniczny pojazdow\n";
        cout << " 8. Aktualizuj przebieg\n";
        cout << " 9. Dodaj klienta\n";
        cout << "10. Statystyki\n";
        cout << " 0. Wyjscie\n";
        cout << "=====================================\n";
    }

    int pobierzWyborUzytkownika() const {
        return pobierzInt("Wybierz opcje: ");
    }

    void wyswietlKomunikat(const string& tresc) const {
        cout << tresc << "\n";
    }

    // ========== POJAZDY ==========

    void sprawdzPojazdy() const {
        const vector<Pojazd*>& pojazdy = menedzer.getPojazdy();
        if (pojazdy.empty()) {
            wyswietlKomunikat("Brak pojazdow w systemie.");
            return;
        }
        cout << "\n===== LISTA POJAZDOW =====\n";
        for (const Pojazd* p : pojazdy) {
            cout << "ID: "       << p->getId()
                 << " | Typ: "  << p->getTyp()
                 << " | "       << p->getMarka() << " " << p->getModel()
                 << " | Rok: "  << p->getRok()
                 << " | Przebieg: " << p->getPrzebieg() << " km"
                 << " | Status: "   << p->getStatus() << "\n";
        }
    }

    void dodajPojazd() {
        int typ = pobierzInt("Typ pojazdu (1-Samochod, 2-Motocykl, 3-Dostawczy): ");

        // ID generowane automatycznie - max istniejacych + 1
        int    id = menedzer.nastepneIdPojazdu();
        cout << "Nowe ID pojazdu: " << id << "\n";
        string marka, model, status;

        cout << "Marka: ";   cin >> marka;
        cout << "Model: ";   cin >> model;
        int rok      = pobierzInt("Rok produkcji: ");
        int przebieg = pobierzInt("Przebieg (km): ");
        cout << "Status (dostepny/serwis): "; cin >> status;

        if (typ == 1) {
            string silnik;
            cout << "Silnik (np. 2.0 TDI): "; cin >> silnik;
            int drzwi = pobierzInt("Liczba drzwi: ");
            menedzer.dodajPojazd(new Samochod(id, marka, model, rok, przebieg, status, drzwi, silnik));

        } else if (typ == 2) {
            int pojemnosc = pobierzInt("Pojemnosc silnika (cm3): ");
            menedzer.dodajPojazd(new Motocykl(id, marka, model, rok, przebieg, status, pojemnosc));

        } else if (typ == 3) {
            string silnik;
            cout << "Silnik (np. 1.9 TDI): "; cin >> silnik;
            int ladownosc = pobierzInt("Ladownosc (kg): ");
            menedzer.dodajPojazd(new Dostawczy(id, marka, model, rok, przebieg, status, ladownosc, silnik));

        } else {
            wyswietlKomunikat("Nieznany typ pojazdu - anulowano.");
        }
    }

    void usunPojazd() {
        sprawdzPojazdy();
        if (menedzer.getPojazdy().empty()) return;
        int id = pobierzInt("\nPodaj ID pojazdu do usuniecia: ");
        menedzer.usunPojazd(id);
    }

    void wypozyczPojazd() {
        // Pokazujemy tylko dostepne pojazdy
        vector<Pojazd*> dostepne = menedzer.wyszukajDostepne();
        if (dostepne.empty()) {
            wyswietlKomunikat("Brak dostepnych pojazdow.");
            return;
        }

        cout << "\n===== DOSTEPNE POJAZDY =====\n";
        for (const Pojazd* p : dostepne) {
            cout << "ID: " << p->getId()
                 << " | " << p->getMarka() << " " << p->getModel()
                 << " | " << p->getTyp() << "\n";
        }

        cout << "\n===== KLIENCI =====\n";
        for (const Klient& k : menedzer.getKlienci()) {
            cout << "ID: " << k.idKlienta
                 << " | " << k.getImieNazwisko()
                 << " | Prawo jazdy: " << k.nrPrawaJazdy << "\n";
        }

        int idPojazdu = pobierzInt("\nID pojazdu: ");
        int idKlienta = pobierzInt("ID klienta: ");

        string dataOd, dataDo;
        cout << "Data od  (RRRR-MM-DD): "; cin >> dataOd;
        cout << "Data do  (RRRR-MM-DD): "; cin >> dataDo;

        double koszt = pobierzDouble("Koszt calkowity (PLN): ");

        menedzer.wypozyczPojazd(idKlienta, idPojazdu, dataOd, dataDo, koszt);
    }

    void zwrocPojazd() {
        // Pokazujemy tylko aktywne wypozyczenia
        const vector<Wypozyczenie>& lista = menedzer.getWypozyczenia();
        bool byloAktywne = false;

        cout << "\n===== AKTYWNE WYPOZYCZENIA =====\n";
        for (const Wypozyczenie& w : lista) {
            if (!w.oddany) {
                cout << "ID: "          << w.idWypozyczenia
                     << " | Pojazd ID: "<< w.idPojazdu
                     << " | Klient ID: "<< w.idKlienta
                     << " | Od: "       << w.dataOd
                     << " | Do: "       << w.dataDo << "\n";
                byloAktywne = true;
            }
        }

        if (!byloAktywne) {
            wyswietlKomunikat("Brak aktywnych wypozyczen.");
            return;
        }

        int id = pobierzInt("\nID wypozyczenia do zwrotu: ");
        menedzer.zwrocPojazd(id);
    }

    void historia() const {
        Statystyki::generujraport(menedzer.getWypozyczenia());
    }

    void sprawdzStatusTechniczny() const {
        const vector<Pojazd*>& pojazdy = menedzer.getPojazdy();
        if (pojazdy.empty()) { wyswietlKomunikat("Brak pojazdow."); return; }

        cout << "\n===== STATUS TECHNICZNY =====\n";
        for (const Pojazd* p : pojazdy) {
            cout << "ID: " << p->getId()
                 << " | " << p->getMarka() << " " << p->getModel()
                 << " | Status: " << p->getStatus() << "\n";

            const vector<Serwis>& historia = p->getHistoriaSerwisowa();
            if (historia.empty()) {
                cout << "  (brak historii serwisowej)\n";
            } else {
                for (const Serwis& s : historia) {
                    cout << "  >> Przeglad: " << s.dataPrzegladu
                         << " | Usterka: "   << s.opisUsterki
                         << " | Sprawny: "   << (s.czySprawny ? "Tak" : "Nie") << "\n";
                }
            }
        }
    }

    void aktualizujPrzebieg() {
        sprawdzPojazdy();
        if (menedzer.getPojazdy().empty()) return;

        int id           = pobierzInt("\nID pojazdu: ");
        int nowyPrzebieg = pobierzInt("Nowy przebieg (km): ");
        menedzer.zaktualizujPrzebieg(id, nowyPrzebieg);
    }

    // ========== KLIENCI ==========

    void dodajKlienta() {
        // ID generowane automatycznie - max istniejacych + 1
        int id = menedzer.nastepneIdKlienta();
        cout << "Nowe ID klienta: " << id << "\n";
        string imie, nazwisko, nrPrawaJazdy;

        cout << "Imie: ";           cin >> imie;
        cout << "Nazwisko: ";       cin >> nazwisko;
        cout << "Nr prawa jazdy: "; cin >> nrPrawaJazdy;

        if (menedzer.dodajKlienta(Klient(id, imie, nazwisko, nrPrawaJazdy)))
            wyswietlKomunikat("Klient dodany.");
    }

    // ========== STATYSTYKI ==========

    void pokazStatystyki() const {
        Statystyki::generujraport(menedzer.getWypozyczenia());
        Statystyki::pokazNajczestszUsterki(menedzer.getPojazdy());
    }

    // ========== GLOWNA PETLA ==========

    void uruchom() {
        int wybor;
        do {
            wyswietlGlowneMenu();
            wybor = pobierzWyborUzytkownika();

            switch (wybor) {
                case 1:  sprawdzPojazdy();           break;
                case 2:  wypozyczPojazd();           break;
                case 3:  zwrocPojazd();              break;
                case 4:  dodajPojazd();              break;
                case 5:  usunPojazd();               break;
                case 6:  historia();                 break;
                case 7:  sprawdzStatusTechniczny();  break;
                case 8:  aktualizujPrzebieg();       break;
                case 9:  dodajKlienta();             break;
                case 10: pokazStatystyki();          break;
                case 0:  wyswietlKomunikat("Koniec programu."); break;
                default: wyswietlKomunikat("Niepoprawna opcja. Wybierz 0-10.");
            }
        } while (wybor != 0);
    }
};
