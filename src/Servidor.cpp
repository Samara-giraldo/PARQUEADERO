#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#define PORT 8080

struct VehiculoPacket {
    char placa[10];
    char hora[10];
    int celda;
    int esSalida;
};

// Función para guardar el estado actual en el disco
void guardarEstadoDisco(const std::string celdas[]) {
    std::ofstream archivo("estado.txt");
    for (int i = 1; i <= 40; i++) {
        archivo << i << ":" << celdas[i] << "\n";
    }
    archivo.close();
}

// Función para CARGAR el estado previo si ya existe un estado.txt
void cargarEstadoDisco(std::string celdas[]) {
    std::ifstream archivo("estado.txt");
    if (!archivo.is_open()) {
        // Si no existe, todas inician en LIBRE
        for (int i = 1; i <= 40; i++) celdas[i] = "LIBRE";
        guardarEstadoDisco(celdas);
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        size_t separador = linea.find(":");
        if (separador != std::string::npos) {
            int num = std::stoi(linea.substr(0, separador));
            std::string estado = linea.substr(separador + 1);
            if (num >= 1 && num <= 40) {
                celdas[num] = estado;
            }
        }
    }
    archivo.close();
}

int main() {
    WSADATA wsaData;
    SOCKET servidor_fd, nuevo_socket;
    struct sockaddr_in direccion;
    int addrlen = sizeof(direccion);

    std::string celdas[41];
    
    // CARGAR ESTADO PREVIO (mantiene las placas aunque apagues todo)
    cargarEstadoDisco(celdas);

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    servidor_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(PORT);

    bind(servidor_fd, (struct sockaddr*)&direccion, sizeof(direccion));
    listen(servidor_fd, 3);

    std::cout << "[SERVIDOR] Guardado previo cargado. Esperando conexiones...\n";

    while (true) {
        nuevo_socket = accept(servidor_fd, (struct sockaddr*)&direccion, &addrlen);
        if (nuevo_socket == INVALID_SOCKET) continue;
        
        std::cout << "[SERVIDOR] ¡Cliente conectado!\n";

        VehiculoPacket paquete;
        while (recv(nuevo_socket, reinterpret_cast<char*>(&paquete), sizeof(VehiculoPacket), 0) > 0) {
            int c = paquete.celda;
            std::string placa(paquete.placa);
            std::string horaIngreso(paquete.hora);

            if (c >= 1 && c <= 40) {
                if (paquete.esSalida == 1) {
                    celdas[c] = "LIBRE";
                } else {
                    if (horaIngreso.empty()) horaIngreso = "14:35:10";
                    celdas[c] = placa + "|" + horaIngreso;
                }
                guardarEstadoDisco(celdas);
            }
        }
        closesocket(nuevo_socket);
    }

    closesocket(servidor_fd);
    WSACleanup();
    return 0;
}