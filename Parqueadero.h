#ifndef PARQUEADEROLOGICA_H
#define PARQUEADEROLOGICA_H

#include <string>
#include <fstream>
#include <sstream>

class ParqueaderoLogica {
private:
    std::string ultimaRespuesta; // Variable miembro para mantener la memoria viva en C++
public:
    const char* obtenerEstadoCelda(int celda) {
        std::ifstream archivo("estado.txt");
        if (!archivo.is_open()) {
            ultimaRespuesta = "LIBRE";
            return ultimaRespuesta.c_str();
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            size_t separador = linea.find(":");
            if (separador != std::string::npos) {
                int num = std::stoi(linea.substr(0, separador));
                std::string estado = linea.substr(separador + 1);
                if (num == celda) {
                    ultimaRespuesta = estado;
                    return ultimaRespuesta.c_str();
                }
            }
        }
        ultimaRespuesta = "LIBRE";
        return ultimaRespuesta.c_str();
    }
};

#endif