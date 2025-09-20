/* 
    VICTOR MENJÓN FANDOS: 930870
    DAVID CARRASCO ALONSO: 927253
*/

#include <iostream>
#include <fstream> 

using namespace std;

// CONSTANTES Y TIPOS

enum tpEstadoCelda {
    NO_USADA, 
    VACIA, 
    OCUPADA
}; 

enum tpDireccion {
    superiorIzquierda=0, 
    superior, 
    superiorDerecha, 
    izquierda, 
    derecha, 
    inferiorIzquierda, 
    inferior, 
    inferiorDerecha
}; 

// Máximo múmero de filas y columnas del tablero
const int MAXDIM = 25;

// Tipo tablero
struct tpTablero{
    int nfils; // Número de filas del tablero
    int ncols; // Número de columnas del tablero
    tpEstadoCelda matriz[MAXDIM][MAXDIM];   // Una celda puede estar vacía, ocupada o no formar parte 
                                            // del tablero
                                            // Las coordenadas de la casilla de arriba a la izquierda son (0,0)
};

// Tipo posición
struct tpPosicion {     // una posición dentro del tablero
    int x; 
    int y; 
};

// Tipo movimientoPieza 
struct tpMovimientoPieza {   // especificamos un movimiento con la celda origen y la celda destino del mismo
    tpPosicion origen; 
    tpPosicion destino; 
};

// Tipo tpListaPosiciones
struct tpListaPosiciones {
    tpPosicion posiciones[MAXDIM*MAXDIM]; 
    int numPos = 0; 
}; 

// Tipo tpListaMovimientos 
// Nota: esta lista es para los movimientos de la solución 
//      (dependiendo de cómo abordéis el diseño, puede ser necesaria una lista de mayor tamaño)
struct tpListaMovimientos {
    tpMovimientoPieza movs[MAXDIM*MAXDIM]; // como cada movimiento elimina una pieza, el número de movimientos de la solución está acotado
    int numMovs = 0; 
};

// Tipo tpMovimientosValidos
struct tpMovimientosValidos {
    // cada valor de tpDireccion se corresponderá con una posición de este vector
    bool validos[8]; 
};


// Códigos numéricos de colores - auxiliares para utilizar al mostrar el tablero por pantalla
const int NEGRO    = 40;
const int ROJO     = 41;
const int VERDE    = 42;
const int AMARILLO = 43;
const int AZUL     = 44;
const int MAGENTA  = 45;
const int CIAN     = 46;
const int BLANCO   = 47;


// FUNCIONES / PROCEDIMIENTOS 

// Pre: true
// Post: lee la configuración y el estado del tablero del fichero de configuración que se le pasa como argumento
//      inicializando tablero y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarTablero(const string nombreFichero, tpTablero &tablero);

// Pre: true 
// Post: lee los movimientos válidos del fichero que se le pasa como argumento 
//      inicializando la estructura y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarMovimientosValidos(const string nombreFichero, tpMovimientosValidos &movimientos);

// Pre: tablero contiene el estado actual de la ejecución de la búsqueda de la solución
// Post: Se ha mostrado el tablero por pantalla
void mostrarTablero(const tpTablero & tablero);

// Pre: tablero contiene el estado inicial del que se parte para la búsqueda, 
//      movimientosValidos contiene los movimientos que están permitidos, 
//      solucionParcial contiene la solución actual como lista de movimientos, En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
// Post: solucionParcial contendrá la lista de movimientos completa (si no se llega a una solución, estará vacía, numMovs == 0)
//       Devuelve 1 si encuentra solución, -1 si no la encuentra.
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const int retardo=0);


// Pre: listaMovimientos contiene la lista de movimientos con la solucion 
// Post: escribe la lista de movimientos en el fichero que se le pasa como argumento siguiendo el 
//      formato especificado en el guión (si está vacía, se escribe un -1 en el fichero)
void escribeListaMovimientos (string nombreFichero, const tpListaMovimientos &solucion); 

//////////////////////////////////////////////////////////////////////////////
// Salida con atributos
//
// La cadena "<ESC>[{attr1};...;{attrn}m" sirve para asignar atributos a la salida
//
// Por ejemplo: 
//    cout << "\033[34;42m" << "Hola" << "\033[0m";
// escribe en azul con fondo verde, y después resetea los atributos

// Lista de atributos estándar:
// 0 Resetea todos los atributos
// 1 brillante
// 2 tenue
// 4 subrayado
// 5 parpadeo
// 7 invertido
// 8 oculto

// Colores de primer plano
// 30 negro
// 31 rojo
// 32 verde
// 33 amarillo
// 34 azul
// 35 magenta
// 36 cian
// 37 blanco

// Colores de fondo
// 40 negro
// 41 rojo
// 42 verde
// 43 amarillo
// 44 azul
// 45 magenta
// 46 cian
// 47 blanco