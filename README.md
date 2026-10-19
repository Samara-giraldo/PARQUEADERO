# PARQUEADERO
# Sistema de Parqueadero — C++ y Python

Este proyecto es el desarrollo final para administrar un parqueadero de **40 celdas**. Utiliza una arquitectura **Cliente-Servidor** conectada por red en **C++**, guardado automático de datos para no perder información y una **interfaz gráfica en Python** bonita y fácil de entender.

---

## 1. Descripción del Proyecto

El objetivo es controlar la entrada, salida y el estado en tiempo real de 40 puestos de parqueadero. 

El programa principal se ejecuta en un servidor hecho en C++ que recibe las peticiones, revisa si una celda está libre u ocupada y guarda el estado en un archivo de texto. Además, cuenta con una pantalla interactiva hecha en Python con tonos pastel que muestra los 40 puestos, cambiando a color verde cuando están libres y a color rosado/rojo cuando están ocupados, junto con la placa del carro y la hora de entrada.

---

## 2. Herramientas Utilizadas y Por Qué las Usé

| Herramienta | Para qué se usó | Explicación Sencilla |
| :--- | :--- | :--- |
| **C++** | Servidor, Cliente y Lógica | Es un lenguaje rápido y eficiente que permite manejar la memoria y las conexiones de red de forma directa. |
| **Winsock2** | Conexión TCP/IP | Es la librería de Windows que permite enviar mensajes entre el programa cliente y el servidor a través de la red. |
| **Python y Tkinter** | Pantalla / Interfaz Gráfica | Permite crear ventanas bonitas, botones y cuadros de colores de forma fácil y rápida para el usuario. |
| **SWIG** | Unión entre C++ y Python | Es la herramienta que traduce el código de C++ para que Python lo pueda leer y usar directamente. |
| **Archivo `estado.txt`** | Guardado de Datos | Sirve para que, si el programa se apaga o se cierra, no se borre la información de los carros parqueados. |
| **Git y GitHub** | Control de Versiones | Para guardar el historial de cambios del proyecto y tener las carpetas ordenadas en internet. |

---

## 3. Cómo Funciona el Sistema

```text
 ┌─────────────────────────┐
 │    Cliente TCP (C++)    │ (Punto de entrada: recibe la placa y la celda)
 └────────────┬────────────┘
              │ Envia datos por red 
              ▼
 ┌─────────────────────────┐        ┌─────────────────────────┐
 │   Servidor TCP (C++)    │ ─────> │ Archivo `estado.txt`    │ (Guarda los datos)
 └────────────┬────────────┘        └─────────────────────────┘
              │
              │ Conecta mediante SWIG
              ▼
 ┌─────────────────────────┐
 │ Visualizador en Python  │ (Muestra los 40 puestos con colores pastel)
 └─────────────────────────┘

##Estructura

 PARQUEADERO/
 
├── src/                          # Código fuente en C++ y archivo de SWIG
│   ├── Servidor.cpp              # Servidor que atiende las conexiones
│   ├── Cliente.cpp               # Cliente para registrar entradas y salidas
│   ├── Parqueadero.h             # Lógica principal de las celdas
│   └── parqueadero.i             # Archivo de interfaz para SWIG
├── python/                       # Código de la pantalla en Python
│   └── visualizador.py           # Ventana gráfica hecha en Tkinter
├── docs/                         # Documentación del proyecto
│   └── arquitectura.md           # Explicación de la arquitectura
├── setup.py                      # Script para compilar el enlace de SWIG
├── .gitignore                    # Archivos temporales que Git debe ignorar
└── README.md                     # Explicación principal del repositorio