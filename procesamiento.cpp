#include "procesamiento.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

// Alias para usar la librería más comodo
using json = nlohmann::json; 

vector<CapturaTactil> leer_json(const string& ruta_archivo){
    vector<CapturaTactil> capturas_leidas;
    ifstream archivo(ruta_archivo);
    if(!archivo.is_open()){
        cerr << "error al abrir el archivo JSON: " << ruta_archivo << endl;
        return capturas_leidas; // devuelve vector vacío si falla
    }
    json datos_json;
    archivo >> datos_json;
    
    // Recorrer el arrat "captures" del JSON
    for(const auto& item: datos_json["captures"]){
        CapturaTactil cap;
        cap.id = item["id"];
        //Extraer la matriz bidimensional directamente 
        cap.matriz = item["matrix"].get<vector<vector<int>>>();
        capturas_leidas.push_back(cap);
    }
    archivo.close();
    return capturas_leidas;    
}
bool validar_matrices(const vector<CapturaTactil>& capturas) {
    for (const auto& cap : capturas){
        //verificar que tenga exactamente 16 filas 
        if (cap.matriz.size() != 16){
            cerr << "error en captura ID " << cap.id << ": No tiene 16 filas. " << endl;
            return false;
        }
        //Verificar que cada fila tenga exactamente 16 columnas
        for (const auto& fila: cap.matriz){
            if (fila.size() != 16) {
                cerr <<"error en captura ID" << cap.id << ": Una fila no tiene 16 columnas. " << endl; 
                return false; 
            }
        }
    }
    cout << "Exito: todas las " << capturas.size() << "capturas tienen el tamaño correcto (16x16). " << endl;
    return true;
 }

    //Funcion de interpolación manual
    vector<vector<int>> interpolacion_bilineal(const vector<vector<int>>& matriz_original) {
        int tam_viejo = 16;
        int tam_nuevo = 128;
        // creamos la nueva matriz gigante de 128x128 llena de ceros
        vector<vector<int>> matriz_ampliada(tam_nuevo, vector<int>(tam_nuevo, 0));
        for (int i = 0; i < tam_nuevo; ++i) {
        for (int j = 0; j < tam_nuevo; ++j) {
            
            // Mapear la coordenada actual a la matriz pequeña
            float x = i * (tam_viejo - 1) / (float)(tam_nuevo - 1);
            float y = j * (tam_viejo - 1) / (float)(tam_nuevo - 1);

            // Encontrar las 4 esquinas más cercanas (los 4 vecinos)
            int x1 = (int)x;
            int y1 = (int)y;
            // Usamos min() para no salirnos de los bordes de la matriz de 16x16
            int x2 = min(x1 + 1, tam_viejo - 1);
            int y2 = min(y1 + 1, tam_viejo - 1);

            // Calcular las distancias a esos vecinos (los pesos)
            float dx = x - x1;
            float dy = y - y1;

            // Sacar los valores de presión de los 4 vecinos originales
            float p11 = matriz_original[x1][y1];
            float p12 = matriz_original[x1][y2];
            float p21 = matriz_original[x2][y1];
            float p22 = matriz_original[x2][y2];

            // Interpolar horizontalmente (eje X)
            float r1 = p11 * (1.0f - dx) + p21 * dx;
            float r2 = p12 * (1.0f - dx) + p22 * dx;

            // Interpolar verticalmente (eje Y) para el resultado final
            float resultado = r1 * (1.0f - dy) + r2 * dy;

            // Guardar el resultado en la matriz gigante (redondeando)
            matriz_ampliada[i][j] = (int)(resultado + 0.5f);
        }
    }
    
    return matriz_ampliada;   
}
// Función para enviar la matriz mediante HTTP POST (Fase 4)
bool enviar_matriz_http(int capture_id, const vector<vector<int>>& matriz_ampliada) {
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(!curl) {
        cerr << "Error al iniciar cURL." << endl;
        return false;
    }

    // Construir el JSON con el formato exacto que pide el profesor
    json j;
    j["capture_id"] = capture_id;
    j["width"] = 128;
    j["height"] = 128;
    j["data"] = matriz_ampliada;
    
    // Convertir el objeto JSON a texto (string)
    string json_str = j.dump();

    // Configurar las cabeceras HTTP
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Configurar cURL para enviar al servidor Python local
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5000/upload");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());

    // Ejecutar el envío
    res = curl_easy_perform(curl);
    
    // Limpiar memoria
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    if(res != CURLE_OK) {
        cerr << "\nFallo al enviar a Python: " << curl_easy_strerror(res) << endl;
        return false;
    }
    
    return true;
}