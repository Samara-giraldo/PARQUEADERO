#ifndef PARQUEADERO_H
#define PARQUEADERO_H

#include <string>

class ParqueaderoLogica {
private:
    std::string celdas[41]; // 40 celdas

public:
    ParqueaderoLogica() {
        for(int i = 1; i <= 40; i++) {
            celdas[i] = "LIBRE";
        }
    }

    void actualizarCelda(int celda, std::string placa) {
        if(celda >= 1 && celda <= 40) {
            celdas[celda] = placa;
        }
    }

    std::string obtenerEstadoCelda(int celda) {
        if(celda >= 1 && celda <= 40) {
            return celdas[celda];
        }
        return "INVALIDA";
    }
};

#endif