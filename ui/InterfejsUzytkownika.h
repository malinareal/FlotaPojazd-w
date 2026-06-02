#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
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

    // NOWA FUNKCJA: Bezpieczne pobieranie tekstu ze spacjami (np. "Aston Martin", "Jan Maria")
    string pobierzTekst(const string& prompt) const {
        string wejscie;
        cout << prompt;
        // std::ws usuwa z bufora 'entery' pozostale po cin >>, chroniac przed bledem
        getline(cin >> ws, wejscie); 
        return wejscie;
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
        cout << " 6. Wyswietl historie Wypozyczen\n";
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
                 << " | Typ: "   << p->getTyp()
                 << " | "        << p->getMarka() << " " << p->getModel()
                 << " | Rok: "   << p->getRok()
                 << " | Przebieg: " << p->getPrzebieg() << " km"
                 << " | Status: "   << p->getStatus() << "\n";
        }
    }

    void dodajPojazd() {
        bool dodano = false;

        while (!dodano) {
            cout << "\n--- DODAWANIE NOWEGO POJAZDU ---\n";
            int typ = pobierzInt("Typ pojazdu (1-Samochod, 2-Motocykl, 3-Dostawczy, 0-Anuluj): ");
            
            if (typ == 0) {
                wyswietlKomunikat("Anulowano dodawanie pojazdu.");
                return; 
            }

            int id = menedzer.nastepneIdPojazdu();
            cout << "Nowe ID pojazdu: " << id << "\n";

            // Uzywamy nowej funkcji pobierzTekst()
            string marka  = pobierzTekst("Marka: ");
            string model  = pobierzTekst("Model: ");
            int rok       = pobierzInt("Rok produkcji: ");
            int przebieg  = pobierzInt("Przebieg (km): ");
            string status = pobierzTekst("Status (dostepny/serwis): ");

            Pojazd* nowyPojazd = nullptr; 

            if (typ == 1) {
                string silnik = pobierzTekst("Silnik (np. 2.0 TDI): ");
                int drzwi = pobierzInt("Liczba drzwi: ");
                nowyPojazd = new Samochod(id, marka, model, rok, przebieg, status, drzwi, silnik);

            } else if (typ == 2) {
                int pojemnosc = pobierzInt("Pojemnosc silnika (cm3): ");
                nowyPojazd = new Motocykl(id, marka, model, rok, przebieg, status, pojemnosc);

            } else if (typ == 3) {
                string silnik = pobierzTekst("Silnik (np. 1.9 TDI): ");
                int ladownosc = pobierzInt("Ladownosc (kg): ");
                nowyPojazd = new Dostawczy(id, marka, model, rok, przebieg, status, ladownosc, silnik);

            } else {
                wyswietlKomunikat("Nieznany typ pojazdu. Sprobuj ponownie.");
                continue; 
            }

            if (nowyPojazd != nullptr) {
                try {
                    menedzer.dodajPojazd(nowyPojazd);
                    wyswietlKomunikat("Sukces! Pojazd zostal dodany do floty.");
                    dodano = true; 
                } catch (const exception& e) {                
                    wyswietlKomunikat(string("Wystapil blad: ") + e.what());
                    wyswietlKomunikat("Wprowadz dane poprawnie.");
                }
            }
        }
    }

    void usunPojazd() {
        sprawdzPojazdy();
        if (menedzer.getPojazdy().empty()) return;
        int id = pobierzInt("\nPodaj ID pojazdu do usuniecia: ");
        
        try {
            menedzer.usunPojazd(id);
            wyswietlKomunikat("Sukces! Pojazd zostal usuniety.");
        } catch (const exception& e) {
            wyswietlKomunikat(string("Blad: ") + e.what());
        }
    }

    void wypozyczPojazd() {
        // Pokazujemy tylko dostepne pojazdy
        vector<const Pojazd*> dostepne = menedzer.wyszukajDostepne();
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

        string dataOd = pobierzTekst("Data od  (RRRR-MM-DD): ");
        string dataDo = pobierzTekst("Data do  (RRRR-MM-DD): ");

        double koszt = pobierzDouble("Koszt calkowity (PLN): ");

        try {
            menedzer.wypozyczPojazd(idKlienta, idPojazdu, dataOd, dataDo, koszt);
            wyswietlKomunikat("Sukces! Wypozyczenie zostalo zarejestrowane.");
        } catch (const exception& e) {
            wyswietlKomunikat(string("Blad: ") + e.what());
        }
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
        
        try {
            menedzer.zwrocPojazd(id);
            wyswietlKomunikat("Sukces! Pojazd zostal zwrocony do floty.");
        } catch (const exception& e) {
            wyswietlKomunikat(string("Blad: ") + e.what());
        }
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
        
        try {
            menedzer.zaktualizujPrzebieg(id, nowyPrzebieg);
            wyswietlKomunikat("Sukces! Przebieg zostal zaktualizowany.");
        } catch (const exception& e) {
            wyswietlKomunikat(string("Blad: ") + e.what());
        }
    }

    // ========== KLIENCI ==========

    void dodajKlienta() {
        bool dodano = false;

        while (!dodano) {
            cout << "\n--- DODAWANIE NOWEGO KLIENTA ---\n";
            cout << "(Wpisz '0' jako imie, aby anulowac)\n";

            int id = menedzer.nastepneIdKlienta();
            cout << "Nowe ID klienta: " << id << "\n";

            // Uzywamy nowej funkcji pobierzTekst()
            string imie = pobierzTekst("Imie: ");
            
            if (imie == "0") {
                wyswietlKomunikat("Anulowano dodawanie klienta.");
                return;
            }

            string nazwisko     = pobierzTekst("Nazwisko: ");
            string nrPrawaJazdy = pobierzTekst("Nr prawa jazdy: ");

            if (imie.length() < 2 || nazwisko.length() < 2) {
                wyswietlKomunikat("Blad: Imie i nazwisko musza miec przynajmniej 2 znaki. Sprobuj ponownie.");
                continue; 
            }

            try {
                menedzer.dodajKlienta(Klient(id, imie, nazwisko, nrPrawaJazdy));
                wyswietlKomunikat("Sukces! Klient zostal dodany.");
                dodano = true; 
            } catch (const exception& e) {
                wyswietlKomunikat(string("Blad podczas dodawania klienta: ") + e.what());
            }
        }
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
