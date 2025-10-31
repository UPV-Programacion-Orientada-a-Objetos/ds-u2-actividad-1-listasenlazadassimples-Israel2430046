#ifndef SENSOR_TEMPERATURA_HPP
#define SENSOR_TEMPERATURA_HPP

#include "SensorBase.hpp"
#include "ListaSensor.hpp"


class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float> historial;  ///< Historial de lecturas de temperatura

public:

    SensorTemperatura(const char* nom) : SensorBase(nom) {
        std::cout << "[SensorTemperatura] Sensor '" << nombre << "' creado." << std::endl;
    }

   
    ~SensorTemperatura() {
        std::cout << "[Destructor SensorTemperatura] Liberando Lista Interna de '" 
                  << nombre << "'..." << std::endl;
    }


    void registrarLectura(double valor) override {
        float temperaturaFloat = static_cast<float>(valor);
        std::cout << "[" << nombre << "] Registrando lectura: " << temperaturaFloat << "°C" << std::endl;
        historial.insertar(temperaturaFloat);
    }

    void procesarLectura() override {
        if (historial.estaVacia()) {
            std::cout << "[SensorTemperatura] " << nombre << " - Sin lecturas registradas." << std::endl;
            return;
        }

        float minimo = historial.obtenerMinimo();
        
        std::cout << "\n-> Procesando Sensor " << nombre << "..." << std::endl;
        std::cout << "[Sensor Temp] Lectura más baja (" << minimo << "°C) eliminada." << std::endl;
        
        historial.eliminarMinimo();

        if (!historial.estaVacia()) {
            double promedio = historial.calcularPromedio();
            std::cout << "[Sensor Temp] Promedio calculado sobre " 
                      << historial.obtenerCantidad() << " lectura(s): " 
                      << promedio << "°C" << std::endl;
        } else {
            std::cout << "[Sensor Temp] Sin lecturas restantes." << std::endl;
        }
    }


    void imprimirInfo() const override {
        std::cout << "\n=== Sensor Temperatura ===" << std::endl;
        std::cout << "Nombre: " << nombre << std::endl;
        std::cout << "Cantidad de lecturas: " << historial.obtenerCantidad() << std::endl;
        std::cout << "Historial: ";
        historial.imprimir();
    }


    int obtenerCantidadLecturas() const {
        return historial.obtenerCantidad();
    }
};

#endif // SENSOR_TEMPERATURA_HPP
