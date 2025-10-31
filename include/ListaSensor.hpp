#ifndef LISTA_SENSOR_HPP
#define LISTA_SENSOR_HPP

#include <iostream>
#include <cstring>

/**
 * @brief Nodo genérico para la Lista Enlazada Simple
 * @tparam T Tipo de dato que almacena el nodo
 */
template <typename T>
struct Nodo {
    T dato;              ///< Dato genérico almacenado
    Nodo<T>* siguiente;  ///< Puntero al siguiente nodo

    /**
     * @brief Constructor del Nodo
     * @param valor Valor a almacenar en el nodo
     */
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

/**
 * @brief Lista Enlazada Simple Genérica
 * @tparam T Tipo de dato que almacena la lista
 * 
 * Implementa operaciones básicas de una lista enlazada simple
 * incluyendo inserción, búsqueda y eliminación de nodos.
 * Cumple con la Regla de los Tres: Destructor, Constructor de Copia
 * y Operador de Asignación.
 */
template <typename T>
class ListaSensor {
private:
    Nodo<T>* cabeza;     ///< Puntero al primer nodo
    int cantidad;        ///< Cantidad de elementos en la lista

public:
    /**
     * @brief Constructor por defecto
     */
    ListaSensor() : cabeza(nullptr), cantidad(0) {}

    /**
     * @brief Destructor - Libera toda la memoria de la lista
     */
    ~ListaSensor() {
        limpiar();
    }

    /**
     * @brief Constructor de copia (Regla de los Tres)
     * @param otra Referencia a otra ListaSensor
     */
    ListaSensor(const ListaSensor& otra) : cabeza(nullptr), cantidad(0) {
        Nodo<T>* actual = otra.cabeza;
        while (actual != nullptr) {
            insertar(actual->dato);
            actual = actual->siguiente;
        }
    }

    /**
     * @brief Operador de asignación (Regla de los Tres)
     * @param otra Referencia a otra ListaSensor
     * @return Referencia a la lista asignada
     */
    ListaSensor& operator=(const ListaSensor& otra) {
        if (this != &otra) {
            limpiar();
            cantidad = 0;
            Nodo<T>* actual = otra.cabeza;
            while (actual != nullptr) {
                insertar(actual->dato);
                actual = actual->siguiente;
            }
        }
        return *this;
    }

    /**
     * @brief Inserta un elemento al final de la lista
     * @param valor Valor a insertar
     */
    void insertar(T valor) {
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);
        
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            Nodo<T>* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
        cantidad++;
        std::cout << "[Log] Nodo insertado. Cantidad actual: " << cantidad << std::endl;
    }

    /**
     * @brief Busca un elemento en la lista
     * @param valor Valor a buscar
     * @return true si el elemento existe, false en caso contrario
     */
    bool buscar(T valor) const {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->dato == valor) {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    /**
     * @brief Obtiene el primer elemento
     * @return Referencia al primer elemento
     * @throw Si la lista está vacía
     */
    T& obtenerPrimero() {
        if (cabeza == nullptr) {
            throw std::runtime_error("Lista vacía");
        }
        return cabeza->dato;
    }

    /**
     * @brief Obtiene el elemento mínimo
     * @return Elemento mínimo de la lista
     * @throw Si la lista está vacía
     */
    T obtenerMinimo() const {
        if (cabeza == nullptr) {
            throw std::runtime_error("Lista vacía");
        }
        
        T minimo = cabeza->dato;
        Nodo<T>* actual = cabeza->siguiente;
        
        while (actual != nullptr) {
            if (actual->dato < minimo) {
                minimo = actual->dato;
            }
            actual = actual->siguiente;
        }
        return minimo;
    }

    /**
     * @brief Elimina el elemento mínimo de la lista
     * @return true si se eliminó correctamente, false si lista está vacía
     */
    bool eliminarMinimo() {
        if (cabeza == nullptr) {
            return false;
        }

        if (cantidad == 1) {
            T valorEliminado = cabeza->dato;
            delete cabeza;
            cabeza = nullptr;
            cantidad = 0;
            std::cout << "[Log] Nodo<T> " << valorEliminado << " liberado." << std::endl;
            return true;
        }

        // Si el mínimo es la cabeza
        if (cabeza->dato <= (cabeza->siguiente != nullptr ? cabeza->siguiente->dato : cabeza->dato)) {
            Nodo<T>* temp = cabeza;
            T valorEliminado = cabeza->dato;
            cabeza = cabeza->siguiente;
            delete temp;
            cantidad--;
            std::cout << "[Log] Nodo<T> " << valorEliminado << " liberado." << std::endl;
            return true;
        }

        // Buscar el mínimo en el resto
        Nodo<T>* actual = cabeza;
        Nodo<T>* anterior = nullptr;
        T minimo = cabeza->dato;
        Nodo<T>* nodoMinimo = cabeza;
        Nodo<T>* anteriorMinimo = nullptr;

        while (actual != nullptr) {
            if (actual->dato < minimo) {
                minimo = actual->dato;
                nodoMinimo = actual;
                anteriorMinimo = anterior;
            }
            anterior = actual;
            actual = actual->siguiente;
        }

        if (anteriorMinimo != nullptr) {
            anteriorMinimo->siguiente = nodoMinimo->siguiente;
        }
        delete nodoMinimo;
        cantidad--;
        std::cout << "[Log] Nodo<T> " << minimo << " liberado." << std::endl;
        return true;
    }

    /**
     * @brief Calcula el promedio de los elementos
     * @return Promedio de los elementos
     * @throw Si la lista está vacía
     */
    double calcularPromedio() const {
        if (cabeza == nullptr) {
            throw std::runtime_error("Lista vacía");
        }

        T suma = 0;
        Nodo<T>* actual = cabeza;
        int contador = 0;

        while (actual != nullptr) {
            suma += actual->dato;
            contador++;
            actual = actual->siguiente;
        }

        return static_cast<double>(suma) / contador;
    }

    /**
     * @brief Obtiene la cantidad de elementos
     * @return Cantidad de elementos en la lista
     */
    int obtenerCantidad() const {
        return cantidad;
    }

    /**
     * @brief Imprime todos los elementos de la lista
     */
    void imprimir() const {
        if (cabeza == nullptr) {
            std::cout << "[Lista vacía]" << std::endl;
            return;
        }

        Nodo<T>* actual = cabeza;
        std::cout << "Elementos: ";
        while (actual != nullptr) {
            std::cout << actual->dato << " ";
            actual = actual->siguiente;
        }
        std::cout << std::endl;
    }

    /**
     * @brief Verifica si la lista está vacía
     * @return true si está vacía, false en caso contrario
     */
    bool estaVacia() const {
        return cabeza == nullptr;
    }

private:
    /**
     * @brief Limpia todos los nodos de la lista
     */
    void limpiar() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
        cabeza = nullptr;
        cantidad = 0;
    }
};

#endif // LISTA_SENSOR_HPP
