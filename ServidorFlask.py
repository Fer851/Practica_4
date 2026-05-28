from flask import Flask, request, jsonify
import numpy as np 
import matplotlib
matplotlib.use('Agg') # obliga a trabajar en segundo plano
import matplotlib.pyplot as plt 
import os

app = Flask(__name__)
#crar la ruta donde el servidor escuchará (HTTP POST)
@app.route('/upload', methods=['POST'])
def upload_matrix():
    try:
        # 1. Recibir el JSON desde C++
        datos = request.get_json()
        capture_id = datos['capture_id']
        width = datos['width']
        height = datos['height']
        matriz_data = datos['data']

        # 2. Reconstruir la matriz gigante usando NumPy
        matriz = np.array(matriz_data)

        # 3. Generar la imagen con la configuración exacta del profesor
        plt.figure(figsize=(6, 6))
        plt.imshow(matriz, cmap="inferno")
        plt.colorbar(label="Presion")
        plt.title(f"Mapa tactil - Captura {capture_id}")

        # 4. Guardar la imagen automáticamente
        nombre_archivo = f"capture_{capture_id}.png"
        plt.savefig(nombre_archivo)
        
        # Limpiar la memoria gráfica para que no explote al hacer 50 fotos
        plt.close() 

        print(f"[OK] Imagen {nombre_archivo} generada con éxito. ({width}x{height})")
        return jsonify({"status": "ok", "message": f"Imagen {capture_id} guardada"}), 200

    except Exception as e:
        print(f"[ERROR] Fallo al procesar la peticion: {e}")
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    print("=== SERVIDOR PYTHON INICIADO ===")
    print("Esperando matrices en http://127.0.0.1:5000/upload")
    # Arrancamos el servidor en el puerto 5000
    app.run(host='0.0.0.0', port=5000)