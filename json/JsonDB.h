#pragma once
#include <fstream>
#include <sstream>
#include "../class/Pojazdy.h"

using namespace std;

class JsonDB
{
public:

    static void saveVehicle(const Samochod& s)
    {
        ofstream file("json/pojazdy.json", ios::app);

        file << "{";
        file << "\"typ\":\"samochod\",";
        file << "\"id\":" << s.getId() << ",";
        file << "\"marka\":\"" << s.getMarka() << "\",";
        file << "\"model\":\"" << s.getModel() << "\",";
        file << "\"rok\":" << s.getRok() << ",";
        file << "\"przebieg\":" << s.getPrzebieg() << ",";
        file << "\"status\":\"" << s.getStatus() << "\",";
        file << "\"silnik\":\"" << s.getSilnik() << "\",";
        file << "\"drzwi\":" << s.getDrzwi();
        file << "}" << endl;
    }

    static void saveVehicle(const Motocykl& m)
    {
        ofstream file("json/pojazdy.json", ios::app);

        file << "{";
        file << "\"typ\":\"motocykl\",";
        file << "\"id\":" << m.getId() << ",";
        file << "\"marka\":\"" << m.getMarka() << "\",";
        file << "\"model\":\"" << m.getModel() << "\",";
        file << "\"rok\":" << m.getRok() << ",";
        file << "\"przebieg\":" << m.getPrzebieg() << ",";
        file << "\"status\":\"" << m.getStatus() << "\",";
        file << "\"pojemnosc\":" << m.getPojemnosc();
        file << "}" << endl;
    }

    static void saveVehicle(const Dostawczy& d)
    {
        ofstream file("json/pojazdy.json", ios::app);

        file << "{";
        file << "\"typ\":\"dostawczy\",";
        file << "\"id\":" << d.getId() << ",";
        file << "\"marka\":\"" << d.getMarka() << "\",";
        file << "\"model\":\"" << d.getModel() << "\",";
        file << "\"rok\":" << d.getRok() << ",";
        file << "\"przebieg\":" << d.getPrzebieg() << ",";
        file << "\"status\":\"" << d.getStatus() << "\",";
        file << "\"silnik\":\"" << d.getSilnik() << "\",";
        file << "\"ladownosc\":" << d.getLadownosc();
        file << "}" << endl;
    }
};