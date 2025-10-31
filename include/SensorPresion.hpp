#ifndef SENSOR_PRESION_HPP
#define SENSOR_PRESION_HPP

#include "SensorBase.hpp"
#include "ListaSensor.hpp"

/**
 * @brief Sensor especializado en mediciones de presión
 * 
 * Hereda de SensorBase e implementa los métodos virtuales puros.
 * Utiliza ListaSensor<int> para almacenar el historial de mediciones.
 */
class SensorPresion : public SensorBase {
private:
    ListaSensor<int> historial;  ///< Historial de lecturas de presión

public:
    /**
     * @brief Constructor de SensorPresion
     * @param nom Nombre identificador del sensor
     */
    SensorPresion(const char* nom) : SensorBase(nom) {
        std::cout << "[SensorPresion] Sensor '" << nombre << "' creado." << std::endl;
    }

    /**
     * @brief Destructor - Libera el historial
     */
    ~SensorPresion() {
        std::cout << "[Destructor SensorPresion] Liberando Lista Interna de '" 
                  << nombre << "'..." << std::endl;
    }

    /**
     * @brief Registra una lectura de presión
     * @param valor Valor de presión en int (Pa)
     */
    void registrarLectura(double valor) override {
        int presionInt = static_cast<int>(valor);
        std::cout << "[" << nombre << "] Registrando lectura: " << presionInt << " Pa" << std::endl;
        historial.insertar(presionInt);
    }

    /**
     * @brief Procesa las lecturas de presión
     * 
     * Calcula el promedio de todas las lecturas de presión.
     */
    void procesarLectura() override {
        if (historial.estaVacia()) {
            std::cout << "[SensorPresion] " << nombre << " - Sin lecturas registradas." << std::endl;
            return;
        }

        std::cout << "\n-> Procesando Sensor " << nombre << "..." << std::endl;
        
        double promedio = historial.calcularPromedio();
        std::cout << "[Sensor Presion] Promedio calculado sobre " 
                  << historial.obtenerCantidad() << " lectura(s): " 
                  << promedio << " Pa" << std::endl;
    }

    /**
     * @brief Imprime información del sensor y su historial
     */
    void imprimirInfo() const override {
        std::cout << "\n=== Sensor Presion ===" << std::endl;
        std::cout << "Nombre: " << nombre << std::endl;
        std::cout << "Cantidad de lecturas: " << historial.obtenerCantidad() << std::endl;
        std::cout << "Historial: ";
        historial.imprimir();
    }

    /**
     * @brief Obtiene la cantidad de lecturas registradas
     * @return Cantidad de lecturas
     */
    int obtenerCantidadLecturas() const {
        return historial.obtenerCantidad();
    }
};

#endif // SENSOR_PRESION_HPP
