#ifndef GESTOR_SENSORES_HPP
#define GESTOR_SENSORES_HPP

#include <iostream>
#include "SensorBase.hpp"

/**
 * @brief Nodo para la lista polimórfica de sensores
 */
struct NodoSensor {
    SensorBase* sensor;      ///< Puntero a sensor (polimórfico)
    NodoSensor* siguiente;   ///< Puntero al siguiente nodo

    /**
     * @brief Constructor del nodo
     * @param s Puntero a un SensorBase
     */
    NodoSensor(SensorBase* s) : sensor(s), siguiente(nullptr) {}
};

/**
 * @brief Gestor polimórfico de sensores
 * 
 * Gestiona una lista de sensores de diferentes tipos de forma
 * polimórfica. Implementa la Regla de los Tres/Cinco.
 */
class GestorSensores {
private:
    NodoSensor* cabeza;     ///< Puntero al primer sensor
    int cantidad;           ///< Cantidad de sensores

public:
    /**
     * @brief Constructor por defecto
     */
    GestorSensores() : cabeza(nullptr), cantidad(0) {}

    /**
     * @brief Destructor - Libera todos los sensores
     */
    ~GestorSensores() {
        limpiar();
    }

    /**
     * @brief Constructor de copia (Regla de los Tres)
     * @param otro Referencia a otro GestorSensores
     */
    GestorSensores(const GestorSensores& otro) : cabeza(nullptr), cantidad(0) {
        // Copiar sensores (nota: esto requeriría métodos de clonación)
        std::cout << "[Advertencia] Constructor de copia no completamente implementado." << std::endl;
    }

    /**
     * @brief Operador de asignación (Regla de los Tres)
     * @param otro Referencia a otro GestorSensores
     * @return Referencia al gestor asignado
     */
    GestorSensores& operator=(const GestorSensores& otro) {
        if (this != &otro) {
            limpiar();
        }
        return *this;
    }

    /**
     * @brief Agrega un sensor a la lista
     * @param sensor Puntero al sensor a agregar
     */
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

    /**
     * @brief Busca un sensor por nombre
     * @param nombre Nombre del sensor a buscar
     * @return Puntero al sensor si existe, nullptr en caso contrario
     */
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

    /**
     * @brief Ejecuta el procesamiento polimórfico de todos los sensores
     */
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

    /**
     * @brief Imprime información de todos los sensores
     */
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

    /**
     * @brief Obtiene la cantidad de sensores
     * @return Cantidad de sensores registrados
     */
    int obtenerCantidad() const {
        return cantidad;
    }

    /**
     * @brief Verifica si el gestor está vacío
     * @return true si no hay sensores, false en caso contrario
     */
    bool estaVacio() const {
        return cabeza == nullptr;
    }

private:
    /**
     * @brief Limpia todos los sensores
     */
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
