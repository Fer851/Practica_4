#include <iostream>
#include "procesamiento.h"

using namespace std; 
int main(){
    cout << "=== INICIANDO SISTEMA TACTIL ===" << endl;
    // Nombre del archivo de datos
    string archivo_datos = "tactile_captures_50.json";
    // Lectura del JSON
    cout << "\n1. Leyendo archivo JSON..." << endl;
    vector<CapturaTactil> capturas = leer_json(archivo_datos);
    if (capturas.empty()) {
        cerr << "No se encontraron datos o el archivo no existe. " << endl; 
        return 1;
    }
    // Validacion de matrices 
    cout << "\n2. Validando dimensiones de las matrices..." << endl;
    if (!validar_matrices(capturas)){
        cerr << "Fallo wn la validacion de datos. " << endl;
        return 1;
    }
    cout <<"\n3. Interpolando y enviando prueba de la primera captura... " << endl;
   // Bucle para procesar todas las matrices de la 0 a la 49
    for (const auto& cap : capturas) {
        
        // 1. Interpolar a 128x128
        vector<vector<int>> matriz_ampliada = interpolacion_bilineal(cap.matriz);
        
        // 2. Enviar por HTTP
        if (enviar_matriz_http(cap.id, matriz_ampliada)) {
            cout << "[C++] Captura " << cap.id << " enviada correctamente." << endl;
        } else {
            cerr << "[C++] Error al enviar la captura " << cap.id << endl;
        }
    }

    cout << "\n=== PROCESO COMPLETADO CON EXITO ===" << endl;

    return 0;
}