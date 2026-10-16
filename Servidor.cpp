#include <iostream>
#include <string>
#include <map>
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

int main() {
    WSADATA wsaData;
    SOCKET servidor_fd, nuevo_socket;
    struct sockaddr_in direccion;
    int addrlen = sizeof(direccion);

    // Mapa para llevar el control: qué placa está ocupando qué celda
    // Si la celda está libre vale "", si está ocupada guarda la placa.
    std::string celdas[41]; 
    for(int i = 0; i <= 40; i++) celdas[i] = "LIBRE";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[SERVIDOR] Error al inicializar Winsock.\n";
        return 1;
    }

    servidor_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor_fd == INVALID_SOCKET) {
        std::cerr << "[SERVIDOR] Error al crear el socket.\n";
        WSACleanup();
        return 1;
    }

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(PORT);

    if (bind(servidor_fd, (struct sockaddr*)&direccion, sizeof(direccion)) == SOCKET_ERROR) {
        std::cerr << "[SERVIDOR] Error en bind.\n";
        closesocket(servidor_fd);
        WSACleanup();
        return 1;
    }

    if (listen(servidor_fd, 3) == SOCKET_ERROR) {
        std::cerr << "[SERVIDOR] Error en listen.\n";
        closesocket(servidor_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "[SERVIDOR] Esperando conexión del cliente...\n";
    nuevo_socket = accept(servidor_fd, (struct sockaddr*)&direccion, &addrlen);
    if (nuevo_socket == INVALID_SOCKET) {
        std::cerr << "[SERVIDOR] Error en accept.\n";
        closesocket(servidor_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "[SERVIDOR] ¡Conectado! Administrando las 40 celdas del parqueadero...\n\n";

    while (true) {
        VehiculoPacket paqueteRecibido;
        int valread = recv(nuevo_socket, reinterpret_cast<char*>(&paqueteRecibido), sizeof(VehiculoPacket), 0);
        
        if (valread > 0) {
            int c = paqueteRecibido.celda;
            std::string placaActual(paqueteRecibido.placa);

            // Verificamos si la celda ya estaba ocupada o si simulamos entrada/salida
            std::cout << "[ESTADO PARQUEADERO] Recibido carro con Placa: " << placaActual 
                      << " | Celda asignada: " << c << std::endl;

            if (c >= 1 && c <= 40) {
                if (celdas[c] == "LIBRE") {
                    celdas[c] = placaActual;
                    std::cout << " -> [ENTRADA] La celda " << c << " ahora esta OCUPADA por " << placaActual << "\n";
                } else {
                    std::cout << " -> [SALIDA] La celda " << c << " ha quedado LIBRE (Vehiculo salio).\n";
                    celdas[c] = "LIBRE";
                }
            }
            std::cout << "--------------------------------------------------\n";
        } else {
            std::cout << "[SERVIDOR] Cliente desconectado.\n";
            break;
        }
    }

    closesocket(nuevo_socket);
    closesocket(servidor_fd);
    WSACleanup();
    return 0;
}