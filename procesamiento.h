#ifndef procesamiento_h
#define procesamiento_h
#include <vector>
#include <string> 

using namespace std; 
// Estructura para guarda cada captura 
struct CapturaTactil{
    int id;
    vector<vector<int>> matriz;
};
// declaración de las funciones
vector<CapturaTactil> leer_json(const string& ruta_archivo);
bool validar_matrices(const vector<CapturaTactil>& capturas);
vector<vector<int>> interpolacion_bilineal(const vector<vector<int>>& matriz_original);
bool enviar_matriz_http(int capture_id, const vector<vector<int>>& matriz_ampliada);
#endif 