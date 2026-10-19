import os
import sys

os.add_dll_directory(os.getcwd())

import tkinter as tk
import parqueadero

logica_cxx = parqueadero.ParqueaderoLogica()

root = tk.Tk()
root.title("Sistema de Parqueaderos")
root.geometry("880x750")
root.configure(bg="#f4f6f7") # Fondo suave / claro pastel

celdas_widgets = {}

# Título estilizado
lbl_titulo = tk.Label(
    root, 
    text="PARQUEADERO", 
    font=("Segoe UI", 22, "bold"), 
    bg="#f4f6f7", 
    fg="#2c3e50"
)
lbl_titulo.pack(pady=12)

# Contadores
lbl_info = tk.Label(
    root, 
    text="Ocupadas: 0    |    Libres: 40", 
    font=("Segoe UI", 12, "bold"), 
    bg="#f4f6f7", 
    fg="#34495e"
)
lbl_info.pack(pady=2)

# Leyenda limpia sin el texto adicional al lado de rojo
lbl_leyenda = tk.Label(
    root, 
    text="VERDE: LIBRE    |    ROJO: OCUPADO", 
    font=("Segoe UI", 9, "italic"), 
    bg="#f4f6f7", 
    fg="#7f8c8d"
)
lbl_leyenda.pack(pady=6)

# Contenedor de la cuadrícula
frame_grid = tk.Frame(root, bg="#eaeded", bd=1, relief="solid")
frame_grid.pack(pady=10, padx=10)

# Colores Pastel
COLOR_VERDE_PASTEL = "#a8e6cf" # Verde pastel suave
COLOR_ROJO_PASTEL  = "#ff8b94" # Rojo / Rosa pastel suave
COLOR_TEXTO_LIBRE  = "#1b5e20" # Texto verde oscuro para legibilidad
COLOR_TEXTO_OCUPADO = "#78281f" # Texto rojo oscuro para legibilidad

# Crear las 40 celdas
for i in range(1, 41):
    fila = (i - 1) // 5
    columna = (i - 1) % 5
    
    lbl = tk.Label(
        frame_grid, 
        text=f"CELDA {i}\nLIBRE", 
        width=15, 
        height=3, 
        bg=COLOR_VERDE_PASTEL, 
        fg=COLOR_TEXTO_LIBRE, 
        font=("Segoe UI", 8, "bold"),
        relief="flat",
        bd=1
    )
    lbl.grid(row=fila, column=columna, padx=6, pady=6)
    celdas_widgets[i] = lbl

def actualizar_interfaz():
    ocupadas_total = 0
    libres_total = 0
    
    try:
        for i in range(1, 41):
            estado_raw = logica_cxx.obtenerEstadoCelda(i)
            estado = str(estado_raw).strip()
            
            if "std::string" in estado or "at 0x" in estado:
                estado = "LIBRE"

            if estado == "" or estado == "LIBRE":
                celdas_widgets[i].config(
                    bg=COLOR_VERDE_PASTEL, 
                    fg=COLOR_TEXTO_LIBRE, 
                    text=f"CELDA {i}\nLIBRE"
                )
                libres_total += 1
            else:
                if "|" in estado:
                    partes = estado.split("|")
                    placa = partes[0]
                    hora = partes[1]
                    texto_celda = f"CELDA {i}\n{placa}\n{hora}"
                else:
                    texto_celda = f"CELDA {i}\n{estado}"

                celdas_widgets[i].config(
                    bg=COLOR_ROJO_PASTEL, 
                    fg=COLOR_TEXTO_OCUPADO, 
                    text=texto_celda
                )
                ocupadas_total += 1
                
        lbl_info.config(text=f"Ocupadas: {ocupadas_total}    |    Libres: {libres_total}")
    except Exception as e:
        pass
            
    root.after(300, actualizar_interfaz)

actualizar_interfaz()
root.mainloop()