#ifndef SENSOR_BASE_HPP
#define SENSOR_BASE_HPP

#include <iostream>
#include <cstring>

/**
 * @brief Clase base abstracta para sensores
 * 
 * Define la interfaz que deben implementar todos los sensores
 * del sistema. Utiliza métodos virtuales puros para forzar
 * la implementación en las clases derivadas.
 */
class SensorBase {
protected:
    char nombre[50];  ///< Identificador del sensor

public:
    /**
     * @brief Constructor de SensorBase
     * @param nom Nombre identificador del sensor
     */
    SensorBase(const char* nom) {
        std::strncpy(nombre, nom, sizeof(nombre) - 1);
        nombre[sizeof(nombre) - 1] = '\0';
    }

    /**
     * @brief Destructor virtual para permitir polimorfismo correcto
     */
    virtual ~SensorBase() {}

    /**
     * @brief Método virtual puro para procesar lecturas
     */
    virtual void procesarLectura() = 0;

    /**
     * @brief Método virtual puro para imprimir información del sensor
     */
    virtual void imprimirInfo() const = 0;

    /**
     * @brief Obtiene el nombre del sensor
     * @return Puntero al nombre del sensor
     */
    virtual const char* obtenerNombre() const {
        return nombre;
    }

    /**
     * @brief Registra una lectura (método virtual)
     * @param valor Valor de la lectura (genérico)
     */
    virtual void registrarLectura(double valor) = 0;
};

#endif // SENSOR_BASE_HPP
