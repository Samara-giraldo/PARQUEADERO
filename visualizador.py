import os
os.add_dll_directory(os.getcwd())
import tkinter as tk
import parqueadero  # Importa el wrapper SWIG que acabas de compilar

# Instancia de la clase C++ envuelta por SWIG
logica_cxx = parqueadero.Parqueaderologica()

root = tk.Tk()
root.title("Visualizador de Parqueadero - SWIG + C++")
root.geometry("750x600")
root.configure(bg="#1e272e")

celdas_widgets = {}

lbl_titulo = tk.Label(
    root, 
    text="MONITOR DE CELDAS EN TIEMPO REAL (SWIG + C++)", 
    font=("Arial", 14, "bold"), 
    bg="#1e272e", 
    fg="#00d8d6"
)
lbl_titulo.pack(pady=15)

frame_grid = tk.Frame(root, bg="#1e272e")
frame_grid.pack()

# Crear cuadrícula visual de 40 celdas
for i in range(1, 41):
    fila = (i - 1) // 5
    columna = (i - 1) % 5
    
    lbl = tk.Label(
        frame_grid, 
        text=f"Celda {i}\nLIBRE", 
        width=13, 
        height=3, 
        bg="#05c46b", # Verde para libre
        fg="white", 
        font=("Arial", 9, "bold"),
        relief="groove",
        bd=2
    )
    lbl.grid(row=fila, column=columna, padx=4, pady=4)
    celdas_widgets[i] = lbl

def actualizar_interfaz():
    try:
        for i in range(1, 41):
            # Llama al método C++ a través de SWIG
            estado = logica_cxx.obtenerEstadoCelda(i)
            
            if estado == "LIBRE":
                celdas_widgets[i].config(bg="#05c46b", text=f"Celda {i}\nLIBRE")
            else:
                celdas_widgets[i].config(bg="#ff3f34", text=f"Celda {i}\n{estado}")
    except Exception as e:
        pass
            
    root.after(500, actualizar_interfaz)

actualizar_interfaz()
root.mainloop()