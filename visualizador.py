import os
import sys

# Asegurar carga de DLLs locales para SWIG
os.add_dll_directory(os.getcwd())

import tkinter as tk
import parqueadero  # Módulo SWIG compilado

# Instanciar la clase de C++
logica_cxx = parqueadero.ParqueaderoLogica()

root = tk.Tk()
root.title("Sistema de Parqueaderos ITM")
root.geometry("800x680")
root.configure(bg="#ecf0f1")

celdas_widgets = {}

# Encabezado idéntico al modelo de referencia
lbl_titulo = tk.Label(
    root, 
    text="PARQUEADERO ITM", 
    font=("Arial", 18, "bold"), 
    bg="#ecf0f1", 
    fg="#2c3e50"
)
lbl_titulo.pack(pady=10)

# Contadores superiores de ocupadas y libres
lbl_info = tk.Label(
    root, 
    text="Ocupadas: 0    Libres: 40", 
    font=("Arial", 11, "bold"), 
    bg="#ecf0f1", 
    fg="#34495e"
)
lbl_info.pack(pady=2)

lbl_leyenda = tk.Label(
    root, 
    text="VERDE: LIBRE    ROJO: OCUPADA", 
    font=("Arial", 9, "italic"), 
    bg="#ecf0f1", 
    fg="#7f8c8d"
)
lbl_leyenda.pack(pady=5)

# Contenedor de la cuadrícula
frame_grid = tk.Frame(root, bg="#ecf0f1")
frame_grid.pack(pady=10)

# Crear las 40 celdas con el diseño limpio
for i in range(1, 41):
    fila = (i - 1) // 5
    columna = (i - 1) % 5
    
    lbl = tk.Label(
        frame_grid, 
        text=f"CELDA {i}\nLIBRE", 
        width=14, 
        height=3, 
        bg="#2ecc71", # Verde inicial (Libre)
        fg="white", 
        font=("Arial", 9, "bold"),
        relief="raised",
        bd=2
    )
    lbl.grid(row=fila, column=columna, padx=5, pady=5)
    celdas_widgets[i] = lbl

def actualizar_interfaz():
    ocupadas_total = 0
    libres_total = 0
    
    try:
        for i in range(1, 41):
            # Obtener estado desde C++ y limpiar cualquier formato de puntero SWIG
            estado_raw = logica_cxx.obtenerEstadoCelda(i)
            estado = str(estado_raw).strip()
            
            # Si SWIG devuelve un objeto string con dirección de memoria, lo filtramos limpiamente
            if "std::string" in estado or "at 0x" in estado:
                estado = "LIBRE"

            if estado == "" or estado == "LIBRE":
                celdas_widgets[i].config(bg="#2ecc71", text=f"CELDA {i}\nLIBRE") # Verde
                libres_total += 1
            else:
                celdas_widgets[i].config(bg="#e74c3c", text=f"CELDA {i}\n{estado}") # Rojo con la placa
                ocupadas_total += 1
                
        # Actualizar los contadores superiores
        lbl_info.config(text=f"Ocupadas: {ocupadas_total}    Libres: {libres_total}")
        
    except Exception as e:
        pass
            
    root.after(400, actualizar_interfaz)

actualizar_interfaz()
root.mainloop()