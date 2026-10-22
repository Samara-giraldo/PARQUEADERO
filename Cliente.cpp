#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

struct VehiculoPacket {
    char placa[10];
    char hora[10];
    int celda;
    int esSalida;
};

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
        std::cerr << "[CLIENTE] No se pudo conectar al servidor. Asegúrate de que esté encendido.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[CLIENTE] Conectado al Servidor.\n";
    std::cout << "[CLIENTE] Iniciando generador automatico de placas (2 a 5 segundos)...\n\n";

    srand(time(0));

    // Bucle infinito de generación automática
    while (true) {
        // Generar pausa aleatoria entre 2 y 5 segundos
        int delay = 2 + (rand() % 4); 
        std::this_thread::sleep_for(std::chrono::seconds(delay));

        VehiculoPacket paquete;
        paquete.celda = 1 + (rand() % 40); // Celda aleatoria entre 1 y 40

        // 70% de probabilidad de generar una entrada, 30% de generar una salida
        int accion = rand() % 10;
        
        if (accion < 7) { 
            // SIMULAR ENTRADA
            std::string letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            std::string placa = "";
            for(int i = 0; i < 3; i++) placa += letras[rand() % 26]; // 3 Letras
            for(int i = 0; i < 3; i++) placa += std::to_string(rand() % 10); // 3 Números

            std::string horaStr = obtenerHoraActual();

            strcpy_s(paquete.placa, placa.c_str());
            strcpy_s(paquete.hora, horaStr.c_str());
            paquete.esSalida = 0;

            std::cout << "[GENERADO] ENTRADA -> Placa: " << placa << " | Celda: " << paquete.celda << " | Esperó: " << delay << "s\n";
        } else {
            // SIMULAR SALIDA
            strcpy_s(paquete.placa, "LIBRE");
            strcpy_s(paquete.hora, "--:--:--");
            paquete.esSalida = 1;

            std::cout << "[GENERADO] SALIDA  -> Celda: " << paquete.celda << " liberada. | Esperó: " << delay << "s\n";
        }

        // Enviar datos al servidor
        send(sock, reinterpret_cast<char*>(&paquete), sizeof(VehiculoPacket), 0);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}