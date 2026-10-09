#include <iostream>
#include <locale.h>

using namespace std;

// Declaramos que la función menú existe en otra parte
void menu(); 

int main() {
    setlocale(LC_ALL, "spanish");
    
    menu(); // Llama al menú y arranca el programa
}