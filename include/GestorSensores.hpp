#ifndef GESTOR_SENSORES_HPP
#define GESTOR_SENSORES_HPP

#include <iostream>
#include "SensorBase.hpp"

// Nodo para la lista polimórfica de sensores
struct NodoSensor {
    SensorBase* sensor;      ///< Puntero a sensor (polimórfico)
    NodoSensor* siguiente;   ///< Puntero al siguiente nodo

    /**
     *  Constructor del nodo
     * s Puntero a un SensorBase
     */
    NodoSensor(SensorBase* s) : sensor(s), siguiente(nullptr) {}
};

/**
 *  Gestor polimórfico de sensores
 * 
 * Gestiona una lista de sensores de diferentes tipos de forma
 * polimórfica. Implementa la Regla de los Tres/Cinco.
 */
class GestorSensores {
private:
    NodoSensor* cabeza;     ///< Puntero al primer sensor
    int cantidad;           ///< Cantidad de sensores

public:
    /** Constructor por defecto
     */
    GestorSensores() : cabeza(nullptr), cantidad(0) {}

    /** Destructor - Libera todos los sensores*/
    ~GestorSensores() {
        limpiar();
    }

    /** Constructor de copia (Regla de los Tres)  otro Referencia a otro GestorSensores
     */
    GestorSensores(const GestorSensores& otro) : cabeza(nullptr), cantidad(0) {
        // Copiar sensores (nota: esto requeriría métodos de clonación)
        std::cout << "[Advertencia] Constructor de copia no completamente implementado." << std::endl;
    }


    GestorSensores& operator=(const GestorSensores& otro) {
        if (this != &otro) {
            limpiar();
        }
        return *this;
    }

 
    void agregarSensor(SensorBase* sensor) {
        if (sensor == nullptr) {
            std::cout << "[Error] Intento de agregar sensor nulo." << std::endl;
            return;
        }

        NodoSensor* nuevoNodo = new NodoSensor(sensor);
        
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            NodoSensor* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
        cantidad++;
        std::cout << "[Log] Sensor '" << sensor->obtenerNombre() 
                  << "' insertado en la lista de gestión." << std::endl;
    }


    SensorBase* buscarSensor(const char* nombre) {
        NodoSensor* actual = cabeza;
        while (actual != nullptr) {
            if (std::strcmp(actual->sensor->obtenerNombre(), nombre) == 0) {
                return actual->sensor;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }


    void procesarTodosSensores() {
        if (cabeza == nullptr) {
            std::cout << "[Advertencia] No hay sensores registrados." << std::endl;
            return;
        }

        std::cout << "\n--- Ejecutando Polimorfismo ---" << std::endl;
        
        NodoSensor* actual = cabeza;
        while (actual != nullptr) {
            actual->sensor->procesarLectura();
            actual = actual->siguiente;
        }
    }


    void imprimirTodosSensores() const {
        if (cabeza == nullptr) {
            std::cout << "[Sistema] No hay sensores registrados." << std::endl;
            return;
        }

        std::cout << "\n--- Estado de Sensores ---" << std::endl;
        NodoSensor* actual = cabeza;
        int contador = 1;
        while (actual != nullptr) {
            std::cout << "\n[Sensor #" << contador << "]" << std::endl;
            actual->sensor->imprimirInfo();
            actual = actual->siguiente;
            contador++;
        }
    }


    int obtenerCantidad() const {
        return cantidad;
    }


    bool estaVacio() const {
        return cabeza == nullptr;
    }

private:

    void limpiar() {
        std::cout << "\n--- Liberación de Memoria en Cascada ---" << std::endl;
        
        NodoSensor* actual = cabeza;
        while (actual != nullptr) {
            NodoSensor* temp = actual;
            actual = actual->siguiente;
            
            std::cout << "[Destructor General] Liberando Nodo: " 
                      << temp->sensor->obtenerNombre() << "." << std::endl;
            delete temp->sensor;  // Llama al destructor virtual correcto
            delete temp;
        }
        cabeza = nullptr;
        cantidad = 0;
        std::cout << "Sistema cerrado. Memoria limpia." << std::endl;
    }
};

#endif // GESTOR_SENSORES_HPP
