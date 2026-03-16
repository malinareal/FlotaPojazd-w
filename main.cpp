#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "historia/Historia.h"
#include "class/Pojazdy.h"
#include "json/JsonDB.h"

using namespace std;

// ======== PLACEHOLDER FUNKCJI ========

void wypozyczPojazd() {
    cout << "Funkcja wypozyczPojazd() jeszcze nie jest zaimplementowana.\n";
}

void zwrocPojazd() {
    cout << "Funkcja zwrocPojazd() jeszcze nie jest zaimplementowana.\n";
}

void sprawdzPojazdy() {
    cout << "Funkcja sprawdzPojazdy() jeszcze nie jest zaimplementowana.\n";
}

void dodajPojazd()
{
    int typ;

    cout << "Typ pojazdu:\n";
    cout << "1 - Samochod\n";
    cout << "2 - Motocykl\n";
    cout << "3 - Dostawczy\n";

    cin >> typ;

    int id;
    string marka;
    string model;
    int rok;
    int przebieg;
    string status;

    cout << "ID: ";
    cin >> id;

    cout << "Marka: ";
    cin >> marka;

    cout << "Model: ";
    cin >> model;

    cout << "Rok: ";
    cin >> rok;

    cout << "Przebieg: ";
    cin >> przebieg;

    cout << "Status: ";
    cin >> status;

    if (typ == 1)
    {
        int drzwi;
        string silnik;

        cout << "Silnik: ";
        cin >> silnik;

        cout << "Liczba drzwi: ";
        cin >> drzwi;

        Samochod auto1(id, marka, model, rok, przebieg, status, drzwi, silnik);
        JsonDB::saveVehicle(auto1);
    }

    else if (typ == 2)
    {
        int pojemnosc;

        cout << "Pojemnosc silnika: ";
        cin >> pojemnosc;

        Motocykl moto1(id, marka, model, rok, przebieg, status, pojemnosc);
        JsonDB::saveVehicle(moto1);
    }

    else if (typ == 3)
    {
        int ladownosc;
        string silnik;

        cout << "Silnik: ";
        cin >> silnik;

        cout << "Ladownosc: ";
        cin >> ladownosc;

        Dostawczy van1(id, marka, model, rok, przebieg, status, ladownosc, silnik);
        JsonDB::saveVehicle(van1);
    }

    cout << "Pojazd zapisany\n";
}

void usunPojazd() {
    cout << "Funkcja usunPojazd() jeszcze nie jest zaimplementowana.\n";
}

void historiaWypozyczen() {
    cout << "Funkcja historiaWypozyczen() jeszcze nie jest zaimplementowana.\n";
}

void sprawdzStatusTechniczny() {
    cout << "Funkcja sprawdzStatusTechniczny() jeszcze nie jest zaimplementowana.\n";
}

void aktualizujPrzebieg() {
    cout << "Funkcja aktualizujPrzebieg() jeszcze nie jest zaimplementowana.\n";
}

// ======== MENU ========

void pokazMenu() {

    cout << "\n===== SYSTEM ZARZADZANIA FLOTA =====\n";
    cout << "1. Sprawdz pojazdy\n";
    cout << "2. Wypozycz pojazd\n";
    cout << "3. Zwroc pojazd\n";
    cout << "4. Dodaj pojazd\n";
    cout << "5. Usun pojazd\n";
    cout << "6. Historia wypozyczen\n";
    cout << "7. Sprawdz status techniczny\n";
    cout << "8. Aktualizuj przebieg\n";
    cout << "0. Wyjscie\n";
}

// ======== MAIN ========

int main() {
    cout << std::filesystem::current_path() << endl;
    int wybor;

    do {

        pokazMenu();
        cout << "Wybierz opcje: ";
        cin >> wybor;

        switch(wybor) {

            case 1:
                sprawdzPojazdy();
                break;

            case 2:
                wypozyczPojazd();
                break;

            case 3:
                zwrocPojazd();
                break;

            case 4:
                dodajPojazd();
                break;

            case 5:
                usunPojazd();
                break;

            case 6:
                historiaWypozyczen();
                break;

            case 7:
                sprawdzStatusTechniczny();
                break;

            case 8:
                aktualizujPrzebieg();
                break;

            case 0:
                cout << "Koniec programu.\n";
                break;

            default:
                cout << "Niepoprawna opcja.\n";
        }

    } while(wybor != 0);

    return 0;
}
