import serial
import time
from datetime import datetime


PUERTO = 'COM3'  # Puerto serial donde está conectado el Arduino
BAUDIOS = 9600 # Velocidad de baudios del Arduino

# Crear un nombre de archivo único con fecha y hora
nombre_archivo = f"magnetometro_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"

try:
    with serial.Serial(PUERTO, BAUDIOS, timeout=1) as ser, open(nombre_archivo, "w") as archivo:
        print(f"📥 Guardando datos en: {nombre_archivo}")
        time.sleep(2)  # Espera para que Arduino comience a enviar datos

        # Lee la primera línea (cabecera) y guárdala
        cabecera = ser.readline().decode("utf-8").strip()
        archivo.write(cabecera + "\n")
        print(f"🔤 Encabezado: {cabecera}")

        while True:
            linea = ser.readline().decode("utf-8").strip()
            if linea:
                print(linea)
                archivo.write(linea + "\n")

except serial.SerialException:
    print("⚠ No se pudo abrir el puerto. ¿Está ocupado o mal configurado?")
except KeyboardInterrupt:
    print("\n✅ Captura detenida por el usuario.")
    print(f"📁 Datos guardados en: {nombre_archivo}")