#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

class Naipe {
private:
    int numero;
    int palo;
public:
    Naipe() : numero(0), palo(0) {}
    Naipe(int num, int p) {
        numero = num;
        palo = p;
    }
    int getVal() const {
        if (numero > 10) return 10;
        return numero; 
    }
    int getNumero() const { return numero; }
    void muestra() const {
        string nombresPalo[4] = {"♢", "♡", "♧", "♤"};
        string nombreValor = to_string(numero);
        if (numero == 1) nombreValor = "As";
        else if (numero == 11) nombreValor = "J";
        else if (numero == 12) nombreValor = "Q";
        else if (numero == 13) nombreValor = "K";
        cout << nombreValor << nombresPalo[palo];
    }
};

class Mazo {
private:
    Naipe naipes[52];
    int indiceActual;
public:
    Mazo() {
        indiceActual = 0;
        int index = 0;
        for (int p = 0; p < 4; p++) {
            for (int n = 1; n <= 13; n++) {
                naipes[index] = Naipe(n, p);
                index++;
            }
        }
        baraja();
    }
    void baraja() {
        for (int i = 0; i < 52; i++) {
            int j = rand() % 52;
            Naipe temp = naipes[i];
            naipes[i] = naipes[j];
            naipes[j] = temp;
        }
        indiceActual = 0;
    }
    Naipe daNaipe() {
        if (indiceActual >= 52) {
            baraja();
        }
        return naipes[indiceActual++];
    }
};

class Mano {
private:
    Naipe naipes[10];
    int cantidad;
public:
    Mano() {
        cantidad = 0;
    }
    void añadeNaipe(Naipe c) {
        if (cantidad < 10) {
            naipes[cantidad++] = c;
        }
    }
    int obtenPuntaje() const {
        int total = 0;
        int ases = 0;
        for (int i = 0; i < cantidad; i++) {
            int val = naipes[i].getVal();
            if (naipes[i].getNumero() == 1) {
                ases++;
                total += 11; 
            } else {
                total += val;
            }
        }
        while (total > 21 && ases > 0) {
            total -= 10;
            ases--;
        }
        return total;
    }
    int obtenPuntajeVisible() const {
        if (cantidad < 2) return 0;
        int val = naipes[1].getVal();
        if (naipes[1].getNumero() == 1) return 11;
        return val;
    }
    void muestraMano(bool ocultarPrimera = false) const {
        for (int i = 0; i < cantidad; i++) {
            cout << " - ";
            if (i == 0 && ocultarPrimera) {
                cout << "[Carta Oculta]" << endl;
            } else {
                naipes[i].muestra();
                cout << endl;
            }
        }
    }
};

int main() {
    srand(time(0)); 
    
    cout << "PRUEBA A: Valores del 1 al 13\n";
    for (int i = 1; i <= 13; i++) {
        Naipe cartaA(i, 0);
        cout << "Valor numérico: " << cartaA.getNumero() << endl;
    }
    cout << "\n";
    
    cout << "PRUEBA B: Las 52 cartas del mazo\n";
    int contadorCartasB = 0;
    for (int p = 0; p < 4; p++) {
        for (int n = 1; n <= 13; n++) {
            Naipe cartaB(n, p);
            contadorCartasB++;
            cout << "Carta " << contadorCartasB << ": ";
            cartaB.muestra();
            cout << endl;
        }
    }
    cout << "\n";

    cout << "PRUEBA C: 13 cartas del mismo símbolo\n";
    for (int n = 1; n <= 13; n++) {
        Naipe cartaC(n, 0); 
        cartaC.muestra();
        cout << endl;
    }    
    
    Mazo mazo;
    int fichas = 500;
    char continua = 's';

    cout << "       JUEGO DEL 21\n";
    cout << "-----------------------------------------\n";

    char opcionLoad; 
    cout << "¿Deseas cargar una partida anterior (s/n)? ";
    cin >> opcionLoad;
    if (opcionLoad == 's' || opcionLoad == 'S') {
        ifstream memoria("progreso.txt"); 
        if (memoria.is_open()) {
            memoria >> fichas;
            memoria.close();
            cout << "Partida cargada correctamente.\n";
        } else {
            cout << "No hay partidas guardadas. Iniciando con 500 fichas.\n";
        }
    }
    cout << "-----------------------------------------\n\n";

    while ((continua == 's' || continua == 'S') && fichas > 0) {
        Mano usuario;
        Mano casa;
        int apuesta = 0;

        cout << "Tienes " << fichas << " fichas.\n";
        
        do {
            cout << "¿Cuántas fichas deseas apostar? ";
            cin >> apuesta;
            if (apuesta <= 0) {
                cout << "La apuesta debe ser mayor a 0.\n";
            } else if (apuesta > fichas) {
                cout << "No tienes suficientes fichas.\n";
            }
        } while (apuesta <= 0 || apuesta > fichas);

        cout << "\n--- Iniciando ronda (Apuesta: " << apuesta << " fichas) ---\n\n";

        usuario.añadeNaipe(mazo.daNaipe());
        usuario.añadeNaipe(mazo.daNaipe());
        casa.añadeNaipe(mazo.daNaipe());
        casa.añadeNaipe(mazo.daNaipe());

        cout << "--- Tus cartas ---\n";
        usuario.muestraMano();
        cout << "Tu total actual: " << usuario.obtenPuntaje() << "\n\n";

        cout << "--- Cartas de la Casa ---\n";
        casa.muestraMano(true);
        cout << "Total visible de la casa: " << casa.obtenPuntajeVisible() << "\n\n";

        char opcion;
        bool jPaso = false;
        
        while (usuario.obtenPuntaje() < 21) {
            cout << "¿Deseas pedir otra carta (s/n)? ";
            cin >> opcion;
            if (opcion == 's' || opcion == 'S') {
                Naipe nueva = mazo.daNaipe();
                cout << "\nRecibiste: ";
                nueva.muestra();
                cout << endl;
                usuario.añadeNaipe(nueva);
                cout << "Nuevo total: " << usuario.obtenPuntaje() << "\n\n";
                
                if (usuario.obtenPuntaje() > 21) {
                    cout << "Te pasaste de 21. Has perdido " << apuesta << " fichas.\n";
                    fichas -= apuesta;
                    jPaso = true;
                    break;
                }
            } else {
                break;
            }
        }

        if (!jPaso) {
            cout << "\n--- Turno de la Casa ---\n";
            cout << "Revelando cartas de la casa:\n";
            casa.muestraMano(false);
            cout << "Total inicial de la casa: " << casa.obtenPuntaje() << "\n\n";

            while (casa.obtenPuntaje() < 17) {
                Naipe nuevaCasa = mazo.daNaipe();
                cout << "La casa pide y recibe: ";
                nuevaCasa.muestra();
                cout << endl;
                casa.añadeNaipe(nuevaCasa);
                cout << "Nuevo total de la casa: " << casa.obtenPuntaje() << "\n\n";
            }

            int ptUsuario = usuario.obtenPuntaje();
            int ptCasa = casa.obtenPuntaje();

            cout << "       RESULTADOS\n";
            cout << "Tu total: " << ptUsuario << endl;
            cout << "Total de la casa: " << ptCasa << endl;
            cout << "-------------------------\n";

            if (ptCasa > 21) {
                cout << "La casa se pasó. Ganaste " << apuesta << " fichas.\n";
                fichas += apuesta;
            } else if (ptUsuario > ptCasa) {
                cout << "Le ganaste a la casa. Ganaste " << apuesta << " fichas.\n";
                fichas += apuesta;
            } else if (ptUsuario < ptCasa) {
                cout << "Gana la casa. Perdiste " << apuesta << " fichas.\n";
                fichas -= apuesta;
            } else {
                cout << "Empate. Recuperas tus fichas.\n";
            }
        }

        if (fichas <= 0) {
            cout << "\nTe has quedado sin fichas. Fin del juego.\n";
            break;
        }

        char opcionSave;
        cout << "¿Deseas guardar tu partida actual (s/n)? ";
        cin >> opcionSave;
        if (opcionSave == 's' || opcionSave == 'S') {
            ofstream memoria("progreso.txt");
            if (memoria.is_open()) {
                memoria << fichas;
                memoria.close();
                cout << "Partida guardada con éxito.\n";
            } else {
                cout << "Error al intentar guardar.\n";
            }
        }

        cout << "\n¿Deseas jugar otra ronda (s/n)? ";
        cin >> continua;
        cout << "\n-----------------------------------------\n\n";
    }

    cout << "Gracias por jugar. Te retiras con " << fichas << " fichas.\n";
    return 0;
}