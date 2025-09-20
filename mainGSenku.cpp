/* 
    VICTOR MENJÓN FANDOS: 930870
    DAVID CARRASCO ALONSO: 927253
*/

#include "GSenku.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 4){
        cerr << "Uso: " << argv[0] << " ficheroTablero ficheroMovimientos retardo [ficheroSalida]" << endl;
        return 1;
    }

    string ficheroTablero = argv[1];
    string ficheroMovimientos = argv[2];
    int retardo = stoi(argv[3]);
    string ficheroSalida = (argc >= 5) ? argv[4] : "resultado.txt";
    tpTablero tablero;
    tpMovimientosValidos movValidos;
    tpListaMovimientos solucion;

    if(!inicializarTablero(ficheroTablero, tablero)){
        cerr << "Error al leer el fichero de tablero: " << ficheroTablero << endl;
        return 1;
    }

    if(!inicializarMovimientosValidos(ficheroMovimientos, movValidos)){
        cerr << "Error al leer el fichero de movimientos: " << ficheroMovimientos << endl;
        return 1;
    }

    if(retardo > 0){
        mostrarTablero(tablero);
        std::this_thread::sleep_for(std::chrono::milliseconds(retardo));
    }

    int resultado = buscaSolucion(tablero, movValidos, solucion, retardo);

    escribeListaMovimientos(ficheroSalida, solucion);

    if(resultado == 1){
        cout << "Solucion encontrada." << endl;
    } 
    else{
        cout << "No se encontro solucion." << endl;
    }

    return 0;
}
