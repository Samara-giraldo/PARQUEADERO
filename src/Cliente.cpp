#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ctime>
#include <iomanip>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

struct VehiculoPacket {
    char placa[10];
    char hora[10];
    int celda;
    int esSalida;
};

// Función para obtener la hora actual en formato HH:MM:SS
std::string obtenerHoraActual() {
    time_t ahora = time(0);
    tm* ltm = localtime(&ahora);
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << ltm->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << ltm->tm_min << ":"
       << std::setfill('0') << std::setw(2) << ltm->tm_sec;
    return ss.str();
}

int main() {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[CLIENTE] Error al inicializar Winsock.\n";
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "[CLIENTE] Error al crear socket.\n";
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "[CLIENTE] No se pudo conectar al servidor. Asegúrate de que est\u00e9 encendido.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[CLIENTE] \u00a1Conectado al Servidor de Parqueadero!\n\n";

    while (true) {
        VehiculoPacket paquete;
        std::string opcion;

        std::cout << "--- MENU PARQUEADERO ---\n";
        std::cout << "1. Ingresar Veh\u00edculo\n";
        std::cout << "2. Registrar Salida\n";
        std::cout << "3. Salir\n";
        std::cout << "Seleccione una opci\u00f3n: ";
        std::cin >> opcion;

        if (opcion == "3") break;

        if (opcion == "1") {
            std::cout << "Ingrese la placa del veh\u00edculo (ej: ABC123): ";
            std::string placaStr;
            std::cin >> placaStr;

            std::cout << "Ingrese el n\u00famero de celda (1 a 40): ";
            std::cin >> paquete.celda;

            if (paquete.celda < 1 || paquete.celda > 40) {
                std::cout << "[!] Celda inv\u00e1lida. Debe ser entre 1 y 40.\n\n";
                continue;
            }

            std::string horaStr = obtenerHoraActual();

            // Copiar a arreglos de caracteres fijos del paquete
            strcpy_s(paquete.placa, placaStr.c_str());
            strcpy_s(paquete.hora, horaStr.c_str());
            paquete.esSalida = 0; // 0 significa entrada

            send(sock, reinterpret_cast<char*>(&paquete), sizeof(VehiculoPacket), 0);
            std::cout << "[CLIENTE ENVIADO] Placa: " << placaStr << " | Hora: " << horaStr << " | Celda: " << paquete.celda << "\n\n";

        } else if (opcion == "2") {
            std::cout << "Ingrese el n\u00famero de celda a liberar (1 a 40): ";
            std::cin >> paquete.celda;

            if (paquete.celda < 1 || paquete.celda > 40) {
                std::cout << "[!] Celda inv\u00e1lida.\n\n";
                continue;
            }

            strcpy_s(paquete.placa, "LIBRE");
            strcpy_s(paquete.hora, "--:--:--");
            paquete.esSalida = 1; // 1 significa salida

            send(sock, reinterpret_cast<char*>(&paquete), sizeof(VehiculoPacket), 0);
            std::cout << "[CLIENTE ENVIADO] Celda " << paquete.celda << " liberada.\n\n";
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}