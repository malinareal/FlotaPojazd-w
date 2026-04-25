#include <filesystem>
#include <iostream>
#include "MenedzerFloty.h"
#include "ui/InterfejsUzytkownika.h"

using namespace std;

int main() {
    cout << "Katalog roboczy: " << filesystem::current_path() << "\n";

    MenedzerFloty menedzer;
    InterfejsUzytkownika ui(menedzer);
    ui.uruchom();

    return 0;
}
