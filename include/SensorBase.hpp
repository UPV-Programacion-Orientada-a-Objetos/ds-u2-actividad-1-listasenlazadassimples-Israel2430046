#ifndef SENSOR_BASE_HPP
#define SENSOR_BASE_HPP

#include <iostream>
#include <cstring>

/**

 * Define la interfaz que deben implementar todos los sensores
 * del sistema. Utiliza métodos virtuales puros para forzar
 * la implementación en las clases derivadas.
 */
class SensorBase {
protected:
    char nombre[50];  ///< Identificador del sensor

public:
    /** nom Nombre identificador del sensor
     */
    SensorBase(const char* nom) {
        std::strncpy(nombre, nom, sizeof(nombre) - 1);
        nombre[sizeof(nombre) - 1] = '\0';
    }

    /** Destructor virtual para permitir polimorfismo correcto
     */
    virtual ~SensorBase() {}

    /** Método virtual puro para procesar lecturas
     */
    virtual void procesarLectura() = 0;

    /** Método virtual puro para imprimir información del sensor
     */
    virtual void imprimirInfo() const = 0;

    /** Obtiene el nombre del sensor Puntero al nombre del sensor
     */
    virtual const char* obtenerNombre() const {
        return nombre;
    }

    /** Registra una lectura (método virtual) valor Valor de la lectura (genérico)
     */
    virtual void registrarLectura(double valor) = 0;
};

#endif // SENSOR_BASE_HPP
