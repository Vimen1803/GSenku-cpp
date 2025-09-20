/* 
    VICTOR MENJÓN FANDOS: 930870
    DAVID CARRASCO ALONSO: 927253
*/

#include "GSenku.hpp"        // Incluye la cabecera del proyecto con definiciones de tipos y constantes
#include <iostream>           // Para entrada y salida estándar
#include <fstream>            // Para manejo de archivos
#include <thread>             // Para usar hilos (retardos)
#include <chrono>             // Para manipulación de tiempo

using namespace std;         // Evita tener que escribir std::

// Pre: true
// Post: Lee el tablero desde archivo y lo inicializa, devolviendo true si fue exitoso
bool inicializarTablero(const string nombreFichero, tpTablero &tablero){
    ifstream archivo(nombreFichero);             // Abre el archivo
    if(!archivo.is_open()) return false;         // Si falla, retorna false

    int dimX, dimY;                              // Variables para dimensiones del tablero
    archivo >> dimX >> dimY;                     // Lee dimensiones del tablero
    if (dimX > MAXDIM || dimY > MAXDIM) return false; // Verifica si son válidas

    tablero.nfils = dimX;                        // Guarda número de filas
    tablero.ncols = dimY;                        // Guarda número de columnas

    for(int i = 0; i < dimX; i++){               // Recorre filas
        for(int j = 0; j < dimY; j++){           // Recorre columnas
            char c;                              // Carácter leído del archivo
            archivo >> c;                        // Lee el carácter correspondiente a cada celda
            switch(c){
                case '-': tablero.matriz[i][j] = NO_USADA; break;   // Celda no usada
                case 'o': tablero.matriz[i][j] = OCUPADA; break;     // Celda con ficha
                case 'x': tablero.matriz[i][j] = VACIA; break;       // Celda vacía
            }
        }
    }
    archivo.close();                             // Cierra el archivo
    return true;                                 // Retorna éxito
}

// Pre: true
// Post: Inicializa los movimientos válidos leyendo desde archivo
bool inicializarMovimientosValidos(const string nombreFichero, tpMovimientosValidos &movimientos) {
    ifstream archivo(nombreFichero);             // Abre archivo de movimientos
    if(!archivo.is_open()) return false;         // Verifica apertura

    char matriz[3][3];                           // Matriz auxiliar 3x3
    for(int i = 0; i < 3; i++){                  // Recorre filas
        for(int j = 0; j < 3; j++){              // Recorre columnas
            archivo >> matriz[i][j];            // Lee la matriz de movimientos válidos
        }
    }
    archivo.close();                             // Cierra archivo

    // Asigna valores booleanos a cada dirección válida
    movimientos.validos[0] = (matriz[0][0] == '+'); // superior izquierda
    movimientos.validos[1] = (matriz[0][1] == '+'); // superior
    movimientos.validos[2] = (matriz[0][2] == '+'); // superior derecha
    movimientos.validos[3] = (matriz[1][0] == '+'); // izquierda
    movimientos.validos[4] = (matriz[1][2] == '+'); // derecha
    movimientos.validos[5] = (matriz[2][0] == '+'); // inferior izquierda
    movimientos.validos[6] = (matriz[2][1] == '+'); // inferior
    movimientos.validos[7] = (matriz[2][2] == '+'); // inferior derecha

    return true;                                  // Todo correcto
}

// Pre: tablero contiene el estado actual
// Post: Muestra el tablero por consola con colores
void mostrarTablero(const tpTablero &tablero) {
    for(int i = 0; i < tablero.nfils; i++){       // Recorre filas
        for(int j = 0; j < tablero.ncols; j++){   // Recorre columnas
            switch(tablero.matriz[i][j]){         // Imprime según el contenido de la celda
                case NO_USADA: cout << "\033[30;41m - \033[0m"; break;  // Rojo
                case VACIA:    cout << "\033[30;42m x \033[0m"; break;  // Verde
                case OCUPADA:  cout << "\033[30;44m o \033[0m"; break;  // Azul
            }
        }
        cout << "\033[0m\n";                      // Salto de línea
    }
    cout << endl;                                 // Línea adicional
}

// Verifica si una posición está dentro del tablero y es válida
bool esPosicionValida(const tpTablero &tablero, int x, int y){
    return x >= 0 && x < tablero.nfils && y >= 0 && y < tablero.ncols && tablero.matriz[x][y] != NO_USADA;
}

// Calcula nueva posición moviéndose en una dirección dada cierta cantidad de pasos
tpPosicion moverEnDireccion(const tpPosicion &pos, tpDireccion dir, int pasos = 1){
    tpPosicion nueva = pos;                      // Copia posición original
    switch(dir){
        case superiorIzquierda: nueva.x -= pasos; nueva.y -= pasos; break; // Mueve en diagonal arriba-izquierda
        case superior: nueva.x -= pasos; break;                              // Mueve hacia arriba
        case superiorDerecha: nueva.x -= pasos; nueva.y += pasos; break;   // Mueve en diagonal arriba-derecha
        case izquierda: nueva.y -= pasos; break;                            // Mueve a la izquierda
        case derecha: nueva.y += pasos; break;                              // Mueve a la derecha
        case inferiorIzquierda: nueva.x += pasos; nueva.y -= pasos; break; // Mueve en diagonal abajo-izquierda
        case inferior: nueva.x += pasos; break;                             // Mueve hacia abajo
        case inferiorDerecha: nueva.x += pasos; nueva.y += pasos; break;  // Mueve en diagonal abajo-derecha
    }
    return nueva;                                   // Devuelve la nueva posición
}

// Verifica si se puede mover en una dirección desde una posición
bool SiMueve(const tpTablero &tablero, const tpMovimientosValidos &movValidos, const tpPosicion &origen, tpDireccion dir) {
    if (!movValidos.validos[dir]) return false;  // Movimiento no permitido en esa dirección

    tpPosicion medio = moverEnDireccion(origen, dir, 1);   // Calcula posición intermedia
    tpPosicion destino = moverEnDireccion(origen, dir, 2); // Calcula destino final

    // Verifica si las posiciones son válidas y cumplen reglas del juego
    if(!esPosicionValida(tablero, medio.x, medio.y) || !esPosicionValida(tablero, destino.x, destino.y)) return false;
    if(tablero.matriz[origen.x][origen.y] != OCUPADA) return false;   // Debe haber ficha en origen
    if(tablero.matriz[medio.x][medio.y] != OCUPADA) return false;     // Debe haber ficha para saltar
    if(tablero.matriz[destino.x][destino.y] != VACIA) return false;   // Destino debe estar vacío

    return true; // Movimiento posible
}

// Realiza un movimiento en el tablero
void Mover(tpTablero &tablero, const tpPosicion &origen, tpDireccion dir) {
    tpPosicion medio = moverEnDireccion(origen, dir, 1);     // Posición intermedia
    tpPosicion destino = moverEnDireccion(origen, dir, 2);   // Posición destino

    tablero.matriz[origen.x][origen.y] = VACIA;              // Vacía origen
    tablero.matriz[medio.x][medio.y] = VACIA;                // Quita ficha intermedia
    tablero.matriz[destino.x][destino.y] = OCUPADA;          // Coloca ficha en destino
}

// Revierte un movimiento realizado
void deshacerMovimiento(tpTablero &tablero, const tpPosicion &origen, tpDireccion dir) {
    tpPosicion medio = moverEnDireccion(origen, dir, 1);     // Posición intermedia
    tpPosicion destino = moverEnDireccion(origen, dir, 2);   // Posición destino

    tablero.matriz[origen.x][origen.y] = OCUPADA;            // Restaura ficha en origen
    tablero.matriz[medio.x][medio.y] = OCUPADA;              // Restaura ficha intermedia
    tablero.matriz[destino.x][destino.y] = VACIA;            // Vacía destino
}

// Cuenta el número de piezas en el tablero
int contadorPiezas(const tpTablero &tablero) {
    int contador = 0;                                      // Inicializa contador
    for(int i = 0; i < tablero.nfils; i++){               // Recorre filas
        for(int j = 0; j < tablero.ncols; j++){           // Recorre columnas
            if(tablero.matriz[i][j] == OCUPADA) contador++; // Suma si hay ficha
        }
    }
    return contador;                                      // Devuelve total
}

// Busca recursivamente una solución para el juego
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const int retardo) {
    if (contadorPiezas(tablero) == 1) {
        return 1; // Solución encontrada
    }

    for(int i = 0; i < tablero.nfils; i++){                       // Recorre filas
        for(int j = 0; j < tablero.ncols; j++){                   // Recorre columnas
            tpPosicion origen = {i, j};                           // Define origen
            if(tablero.matriz[i][j] == OCUPADA){                 // Solo si hay ficha
                for(int d = 0; d < 8; d++){                       // Recorre direcciones
                    tpDireccion dir = static_cast<tpDireccion>(d); // Convierte índice a dirección
                    if(SiMueve(tablero, movValidos, origen, dir)){
                        Mover(tablero, origen, dir);                 // Realiza movimiento
                        solucionParcial.movs[solucionParcial.numMovs++] = {origen, moverEnDireccion(origen, dir, 2)}; // Guarda en solución
                        if(retardo > 0){
                            mostrarTablero(tablero);               // Muestra tablero si hay retardo
                            this_thread::sleep_for(chrono::milliseconds(retardo)); // Pausa
                        }
                        int res = buscaSolucion(tablero, movValidos, solucionParcial, retardo); // Llamada recursiva
                        if(res == 1) return 1;                      // Solución encontrada
                        solucionParcial.numMovs--;                 // Deshace si no funcionó
                        deshacerMovimiento(tablero, origen, dir);  // Revierte el movimiento
                    }
                }
            }
        }
    }
    return -1; // No hay solución
}

// Escribe la lista de movimientos en un archivo
void escribeListaMovimientos(string nombreFichero, const tpListaMovimientos &solucion) {
    ofstream fichero(nombreFichero);                    // Abre archivo para escritura
    if(!fichero.is_open()) return;                      // Verifica apertura

    if(solucion.numMovs == 0){
        fichero << -1 << endl;                          // Sin solución
    }else{
        for(int i = 0; i < solucion.numMovs; i++){      // Recorre movimientos
            const tpMovimientoPieza &mov = solucion.movs[i]; // Toma movimiento actual
            fichero << mov.origen.x << "," << mov.origen.y << ":" << mov.destino.x << "," << mov.destino.y << endl; // Escribe en formato
        }
    }
    fichero.close();                                   // Cierra archivo
}