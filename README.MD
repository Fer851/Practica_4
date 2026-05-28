# Práctica de Sistemas Operativos — C++ y Python

## 1. Descripción
Este repositorio contiene la Práctica 4 de Sistemas Operativos, cuyo objetivo es aplicar conceptos de comunicación entre procesos en red (arquitectura Cliente-Servidor) y manipulación algorítmica de memoria. 

El sistema procesa matrices táctiles de baja resolución provenientes de un gripper robótico, hay que aumentar su resolución mediante **interpolación bilineal programada en C++**, y transmite los datos mediante peticiones HTTP POST a un **servidor Python** que genera representaciones de mapas térmicos. El proyecto está diseñado para ser totalmente autocontenido.

## 2. Estructura del repositorio
```text
.
|--- main.cpp   # Código fuente principal en C++ (Control de flujo)
|--- procesamiento.cpp   # Implementación algorítmica y de red
|--- procesamiento.h     # Fichero de cabecera y estructuras de datos
|--- ServidorFlask.py    # Microservicio en Python (Servidor receptor)
|--- tactile_captures_50.json # Datos de entrada del sensor táctil (50 muestras)
|--- Practica_4_Informe_Fernando_Moran.pdf # Informe técnico del proyecto
|--- README.md   # Este documento

## 3.Requisitos del Sistema
* Sistema operativo: Linux (UBUNTU)
* Compilador: g++ con soporte C++17 
* Intérprete: python3
* Instalación de herramientas básicas:
 **sudo apt update**
 **sudo apt install -y build-essential

## 4. Dependencias Externas
El proyecto presciende de librerías de visión artificial pesadas en favor de herramientas ligeras de serialización, red y visualización científica.

Intalación: 
	**sudo apt update**
	**sudo apt install -y nlohmann-json3-dev libcurl4-openssl-dev python3-flask**

## 5. Compilación
El proyecto se directamente: 
	* Servidor: **python3 ServidorFlask.py
	* Cliente: **g++ main.cpp procesamiento.cpp -o cliente -lcurl** 
	* Ejecutable: **./cliente**

## 6. Salida esperada
Al ejecutar correctamente el programa, el sistema finaliza sin errores y almacena 50 archivos PNG en el directorio raíz.

- Salida en Terminal 1 (Python):

=== SERVIDOR PYTHON INICIADO ===
Esperando matrices en [http://127.0.0.1:5000/upload](http://127.0.0.1:5000/upload)
[OK] Imagen capture_0.png generada con éxito. (128x128)
127.0.0.1 - - [27/May/2026 22:35:51] "POST /upload HTTP/1.1" 200 -
Salida en Terminal 2 (C++):

=== INICIANDO SISTEMA TACTIL ===
1. Leyendo archivo JSON...
2. Validando dimensiones de las matrices...
Exito: todas las 50 capturas tienen el tamaño correcto (16x16).
3. Interpolando y enviando las 50 capturas...
[C++] Captura 0 enviada correctamente.
...
=== PROCESO COMPLETADO CON EXITO ===

## 7. Depuración
Para compilar con símbolos de depuración (modo Debug):

**Bash**
g++ -g main.cpp procesamiento.cpp -o cliente_debug -lcurl
Ejecutar con gdb para trazar la memoria:

**Bash**
gdb ./cliente_debug

## 8. Problemas comunes
* Fallo al enviar a Python: Olvidar activar el servidor Python antes de ejecutar el cliente en C++. . 

## 9. Autoría
* Alumno: Fernando Morán Fernández
