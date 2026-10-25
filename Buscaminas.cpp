#include <iostream>
#include <fstream>   // Para el historial (leer y escribir archivo)
#include <cstdlib>   // rand y srand
#include <ctime>     // time
using namespace std;

const int MAX = 9;
const int MINAS = 3;
typedef char Matriz[MAX][MAX];

// Pide un numero entero. Si el jugador escribe una letra, avisa y vuelve a pedir.
int leerNumero(const char* mensaje) {
    int n;
    cout << mensaje;
    while (!(cin >> n)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Eso no es un numero.\n" << mensaje;
    }
    return n;
}

// Lo uso para filas y columnas hasta que sean validas.
void pedirTamano(int& filas, int& columnas) {
    while (true) {
        filas = leerNumero("Filas (3 a 9): ");
        columnas = leerNumero("Columnas (3 a 9): ");
        if (filas >= 3 && filas <= 9 && columnas >= 3 && columnas <= 9) return;
        cout << "El tamano debe estar entre 3 y 9.\n";
    }
}

// Se hace una matriz con simbolos
void crearMatriz(Matriz m, int filas, int columnas, char simbolo) {
    for (int f = 0; f < filas; f++)
        for (int c = 0; c < columnas; c++)
            m[f][c] = simbolo;
}

// Aqui escondo las minas 
void colocarMinas(Matriz minas, int filas, int columnas) {
    int colocadas = 0;
    while (colocadas < MINAS) {
        int f = rand() % filas;
        int c = rand() % columnas;
        if (minas[f][c] != '*') {
            minas[f][c] = '*';
            colocadas++;
        }
    }
}

// Se enseña la matriz con su cantidad de filas y columnas
void mostrarTablero(Matriz m, int filas, int columnas) {
    cout << "\n    ";
    for (int c = 0; c < columnas; c++) cout << c + 1 << " ";
    cout << "\n";
    for (int f = 0; f < filas; f++) {
        cout << " " << f + 1 << "  ";
        for (int c = 0; c < columnas; c++) cout << m[f][c] << " ";
        cout << "\n";
    }
}

// Al escoger la casilla, aca la abre y muestra true si habu¿ia una mina
bool abrirCasilla(Matriz minas, Matriz tablero, int f, int c) {
    if (minas[f][c] == '*') {
        tablero[f][c] = '*';
        return true;
    }
    tablero[f][c] = '+';
    return false;
}

//Pasa las minas de nuestra matriz secreta a la que usamos para ver en donde estaban al ganar o perder
void revelarMinas(Matriz minas, Matriz tablero, int filas, int columnas) {
    for (int f = 0; f < filas; f++)
        for (int c = 0; c < columnas; c++)
            if (minas[f][c] == '*') tablero[f][c] = '*';
}

// Muestra si ganamos si el numero de casillas cerradas es igual a minas o sea 3
bool hayVictoria(Matriz tablero, int filas, int columnas) {
    int cerradas = 0;
    for (int f = 0; f < filas; f++)
        for (int c = 0; c < columnas; c++)
            if (tablero[f][c] == '?') cerradas++;
    return cerradas == MINAS;
}

// Agrega una linea al final de historial.txt.
void guardarHistorial(int filas, int columnas, bool gano, int intentos) {
    ofstream archivo("historial.txt", ios::app);
    archivo << filas << " " << columnas << " " << (gano ? "gano" : "perdio") << " " << intentos << "\n";
}

// Muestra nuestro historial, y suma uno cuando empecemos otra partida
void mostrarResumen() {
    int partidas = 0, ganadas = 0, perdidas = 0;
    int filas, columnas, intentos;
    char resultado[10];
    ifstream archivo("historial.txt");
    while (archivo >> filas >> columnas >> resultado >> intentos) {
        partidas++;
        if (resultado[0] == 'g') ganadas++;
        else perdidas++;
    }
    cout << "Partidas: " << partidas << " | Ganadas: " << ganadas << " | Perdidas: " << perdidas << "\n";
}

int main() {
    srand(time(0));
    mostrarResumen();

    int filas, columnas;
    pedirTamano(filas, columnas);
    Matriz minas, tablero;
    crearMatriz(minas, filas, columnas, '.');
    crearMatriz(tablero, filas, columnas, '?');
    colocarMinas(minas, filas, columnas);

    int intentos = 0;
    while (true) {
        mostrarTablero(tablero, filas, columnas);

        int f = leerNumero("Fila (0 para salir, -1 para ver minas): ");
        if (f == 0) {
            cout << "Juego terminado.\n";
            return 0;
        }
        
        if (f == -1) {
            mostrarTablero(minas, filas, columnas);
            continue;
        }
        int c = leerNumero("Columna: ");

        if (f < 1 || f > filas || c < 1 || c > columnas) {
            cout << "Posicion fuera del tablero.\n";
            continue;
        }
        if (tablero[f - 1][c - 1] != '?') {
            cout << "Esa casilla ya esta abierta.\n";
            continue;
        }

        intentos++;
        if (abrirCasilla(minas, tablero, f - 1, c - 1)) {
            revelarMinas(minas, tablero, filas, columnas);
            cout << "\nPisaste una mina\n";
            mostrarTablero(tablero, filas, columnas);
            guardarHistorial(filas, columnas, false, intentos);
            break;
        }
        if (hayVictoria(tablero, filas, columnas)) {
            revelarMinas(minas, tablero, filas, columnas);
            cout << "\nGanaste\n";
            mostrarTablero(tablero, filas, columnas);
            guardarHistorial(filas, columnas, true, intentos);
            break;
        }
    }
    return 0;
}
