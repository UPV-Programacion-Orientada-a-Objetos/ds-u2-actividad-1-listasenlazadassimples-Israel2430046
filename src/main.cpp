#include <iostream>
#include <cstring>
#include <cstdlib>

#include "GestorSensores.hpp"
#include "SensorTemperatura.hpp"
#include "SensorPresion.hpp"
#include "ComunicacionSerial.hpp"

// Evitamos 'using namespace std;' como se solicita
using std::cout;
using std::cerr;
using std::cin;
using std::endl;

/**
 *Muestra el menú principal
 */
void mostrarMenu() {
    cout << "\n========================================" << endl;
    cout << "  Sistema IoT de Monitoreo Polimórfico" << endl;
    cout << "========================================" << endl;
    cout << "1. Crear Sensor de Temperatura (FLOAT)" << endl;
    cout << "2. Crear Sensor de Presión (INT)" << endl;
    cout << "3. Registrar Lectura en Sensor" << endl;
    cout << "4. Conectar con Arduino/ESP32 (Puerto Serial)" << endl;
    cout << "5. Procesar Lecturas (Polimorfismo)" << endl;
    cout << "6. Ver Estado de Sensores" << endl;
    cout << "7. Cerrar Sistema (Liberar Memoria)" << endl;
    cout << "========================================" << endl;
    cout << "Seleccione una opción: ";
}

/**
 *  Lee un string seguro de la entrada
 *  tamanio Tamaño máximo del buffer
 */
void leerString(char* buffer, int tamanio) {
    cin.getline(buffer, tamanio);
    // Limpiar espacios en blanco al inicio
    int i = 0;
    while (buffer[i] == ' ' && buffer[i] != '\0') {
        i++;
    }
    if (i > 0) {
        for (int j = 0; j < (int)std::strlen(buffer) - i; j++) {
            buffer[j] = buffer[j + i];
        }
        buffer[std::strlen(buffer) - i] = '\0';
    }
}


void crearSensor(GestorSensores& gestor, bool esTemperatura) {
    char nombreSensor[50];
    cout << "\nIngrese el identificador del sensor (ej: T-001, P-105): ";
    leerString(nombreSensor, sizeof(nombreSensor));

    if (std::strlen(nombreSensor) == 0) {
        cout << "[Error] El nombre del sensor no puede estar vacío." << endl;
        return;
    }

    SensorBase* nuevoSensor = nullptr;
    if (esTemperatura) {
        nuevoSensor = new SensorTemperatura(nombreSensor);
    } else {
        nuevoSensor = new SensorPresion(nombreSensor);
    }

    gestor.agregarSensor(nuevoSensor);
}


void registrarLectura(GestorSensores& gestor) {
    if (gestor.estaVacio()) {
        cout << "[Advertencia] No hay sensores registrados." << endl;
        return;
    }

    char nombreSensor[50];
    cout << "\nIngrese el identificador del sensor: ";
    leerString(nombreSensor, sizeof(nombreSensor));

    SensorBase* sensor = gestor.buscarSensor(nombreSensor);
    if (sensor == nullptr) {
        cout << "[Error] Sensor '" << nombreSensor << "' no encontrado." << endl;
        return;
    }

    double valor;
    cout << "Ingrese el valor de la lectura: ";
    cin >> valor;
    cin.ignore(); // Limpiar el buffer

    sensor->registrarLectura(valor);
}


void conectarArduino(GestorSensores& gestor) {
    if (gestor.estaVacio()) {
        cout << "[Advertencia] No hay sensores registrados. Cree al menos uno." << endl;
        return;
    }

    cout << "\n========== Conexión Arduino/ESP32 ==========" << endl;
    
    char puerto[256];
    int velocidad;

    #ifdef _WIN32
        cout << "Ingrese el puerto COM (ej: COM3): ";
    #else
        cout << "Ingrese el puerto (ej: /dev/ttyUSB0 o /dev/ttyACM0): ";
    #endif
    
    leerString(puerto, sizeof(puerto));

    cout << "Ingrese la velocidad en baudios (9600, 115200, etc): ";
    cin >> velocidad;
    cin.ignore();

    ComunicacionSerial serial;
    if (!serial.conectar(puerto, velocidad)) {
        cerr << "[Error] No se pudo establecer conexión con el puerto serial." << endl;
        return;
    }

    cout << "\n[Sistema] Esperando datos del Arduino/ESP32..." << endl;
    cout << "[Sistema] Presione Ctrl+C para detener." << endl;
    cout << "[Formato esperado: ID:valor (ej: T-001:25.5)]" << endl;
    cout << "=============================================" << endl;

    char buffer[256];
    char linea[256];
    int posicion = 0;

    // Simulación de lectura de datos
    cout << "\n[Simulación de lecturas Arduino]" << endl;
    cout << "Ingrese lecturas en formato 'ID:valor' (escribe 'salir' para terminar):" << endl;

    while (true) {
        cout << "> ";
        leerString(linea, sizeof(linea));

        if (std::strcmp(linea, "salir") == 0) {
            break;
        }

        // Parsear formato "ID:valor"
        char* separador = std::strchr(linea, ':');
        if (separador == nullptr) {
            cout << "[Error] Formato inválido. Use 'ID:valor'" << endl;
            continue;
        }

        char idSensor[50];
        char valorStr[50];
        int idLen = separador - linea;

        std::strncpy(idSensor, linea, idLen);
        idSensor[idLen] = '\0';

        std::strncpy(valorStr, separador + 1, sizeof(valorStr) - 1);
        valorStr[sizeof(valorStr) - 1] = '\0';

        SensorBase* sensor = gestor.buscarSensor(idSensor);
        if (sensor == nullptr) {
            cout << "[Error] Sensor '" << idSensor << "' no encontrado." << endl;
            continue;
        }

        double valor = std::atof(valorStr);
        sensor->registrarLectura(valor);
    }

    serial.desconectar();
}

/**
 * @brief Función principal
 */
int main() {
    cout << "\n╔════════════════════════════════════════════╗" << endl;
    cout << "║  Sistema IoT de Monitoreo Polimórfico     ║" << endl;
    cout << "║  Gestión de Sensores con Listas Enlazadas ║" << endl;
    cout << "║  Compatible con Arduino/ESP32             ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;

    GestorSensores gestor;
    int opcion;
    bool ejecutando = true;

    while (ejecutando) {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(); // Limpiar el buffer de entrada

        switch (opcion) {
            case 1:
                cout << "\n--- Crear Sensor de Temperatura ---" << endl;
                crearSensor(gestor, true);
                break;

            case 2:
                cout << "\n--- Crear Sensor de Presión ---" << endl;
                crearSensor(gestor, false);
                break;

            case 3:
                cout << "\n--- Registrar Lectura ---" << endl;
                registrarLectura(gestor);
                break;

            case 4:
                cout << "\n--- Conectar con Arduino/ESP32 ---" << endl;
                conectarArduino(gestor);
                break;

            case 5:
                gestor.procesarTodosSensores();
                break;

            case 6:
                gestor.imprimirTodosSensores();
                break;

            case 7:
                cout << "\n--- Cerrando Sistema ---" << endl;
                ejecutando = false;
                break;

            default:
                cout << "[Error] Opción no válida. Intente nuevamente." << endl;
                break;
        }
    }

    // El destructor del gestor se encargará de liberar toda la memoria
    cout << "\n[Sistema] Aplicación finalizada correctamente." << endl;
    return 0;
}
