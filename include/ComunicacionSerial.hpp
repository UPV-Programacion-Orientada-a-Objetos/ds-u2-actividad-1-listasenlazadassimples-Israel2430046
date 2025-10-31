#ifndef COMUNICACION_SERIAL_HPP
#define COMUNICACION_SERIAL_HPP

#include <iostream>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
    typedef HANDLE PuertoSerial;
#else
    #include <termios.h>
    #include <fcntl.h>
    #include <unistd.h>
    typedef int PuertoSerial;
#endif

/**
 * @brief Clase para gestionar comunicación serial
 */
class ComunicacionSerial {
private:
    PuertoSerial puerto;
    bool conectado;
    char puertoNombre[256];

public:
   
    ComunicacionSerial() : conectado(false) {
        std::memset(puertoNombre, 0, sizeof(puertoNombre));
        
        #ifdef _WIN32
            puerto = INVALID_HANDLE_VALUE;
        #else
            puerto = -1;
        #endif
    }


    ~ComunicacionSerial() {
        desconectar();
    }


    bool conectar(const char* puerto_nombre, int velocidad = 9600) {
        std::strncpy(puertoNombre, puerto_nombre, sizeof(puertoNombre) - 1);
        puertoNombre[sizeof(puertoNombre) - 1] = '\0';

        #ifdef _WIN32
            return conectarWindows(velocidad);
        #else
            return conectarLinux(velocidad);
        #endif
    }


    int leer(char* buffer, int tamanio) {
        if (!conectado) {
            std::cerr << "[Error] Puerto no conectado." << std::endl;
            return -1;
        }

        #ifdef _WIN32
            DWORD bytesLeidos = 0;
            if (ReadFile(puerto, buffer, tamanio, &bytesLeidos, nullptr)) {
                return bytesLeidos;
            }
        #else
            int bytesLeidos = ::read(puerto, buffer, tamanio);
            return bytesLeidos;
        #endif

        return -1;
    }


    bool escribir(const char* datos, int tamanio) {
        if (!conectado) {
            std::cerr << "[Error] Puerto no conectado." << std::endl;
            return false;
        }

        #ifdef _WIN32
            DWORD bytesEscritos = 0;
            return WriteFile(puerto, datos, tamanio, &bytesEscritos, nullptr);
        #else
            return (::write(puerto, datos, tamanio) > 0);
        #endif
    }


    void desconectar() {
        if (conectado) {
            #ifdef _WIN32
                if (puerto != INVALID_HANDLE_VALUE) {
                    CloseHandle(puerto);
                }
            #else
                if (puerto != -1) {
                    close(puerto);
                }
            #endif
            conectado = false;
            std::cout << "[ComunicacionSerial] Puerto cerrado." << std::endl;
        }
    }


    bool estaConectado() const {
        return conectado;
    }

private:
    #ifdef _WIN32
    bool conectarWindows(int velocidad) {
        puerto = CreateFileA(
            puertoNombre,
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (puerto == INVALID_HANDLE_VALUE) {
            std::cerr << "[Error] No se pudo abrir el puerto " << puertoNombre << std::endl;
            return false;
        }

        DCB dcbSerialParams = {0};
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        if (!GetCommState(puerto, &dcbSerialParams)) {
            std::cerr << "[Error] No se pudo obtener el estado del puerto." << std::endl;
            CloseHandle(puerto);
            puerto = INVALID_HANDLE_VALUE;
            return false;
        }

        dcbSerialParams.BaudRate = velocidad;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;

        if (!SetCommState(puerto, &dcbSerialParams)) {
            std::cerr << "[Error] No se pudo configurar el puerto." << std::endl;
            CloseHandle(puerto);
            puerto = INVALID_HANDLE_VALUE;
            return false;
        }

        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;

        if (!SetCommTimeouts(puerto, &timeouts)) {
            std::cerr << "[Error] No se pudo establecer timeouts." << std::endl;
            CloseHandle(puerto);
            puerto = INVALID_HANDLE_VALUE;
            return false;
        }

        conectado = true;
        std::cout << "[ComunicacionSerial] Conectado a " << puertoNombre 
                  << " a " << velocidad << " baudios (Windows)." << std::endl;
        return true;
    }
    #else
    bool conectarLinux(int velocidad) {
        puerto = open(puertoNombre, O_RDWR | O_NOCTTY | O_NDELAY);

        if (puerto == -1) {
            std::cerr << "[Error] No se pudo abrir el puerto " << puertoNombre << std::endl;
            return false;
        }

        fcntl(puerto, F_SETFL, 0);

        termios tty;
        if (tcgetattr(puerto, &tty) != 0) {
            std::cerr << "[Error] No se pudo obtener atributos del puerto." << std::endl;
            close(puerto);
            puerto = -1;
            return false;
        }

        speed_t baud = B9600;
        if (velocidad == 115200) baud = B115200;
        else if (velocidad == 57600) baud = B57600;
        else if (velocidad == 38400) baud = B38400;
        else if (velocidad == 19200) baud = B19200;

        cfsetospeed(&tty, baud);
        cfsetispeed(&tty, baud);

        tty.c_cflag |= (CLOCAL | CREAD);
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;

        tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_oflag &= ~OPOST;

        if (tcsetattr(puerto, TCSANOW, &tty) != 0) {
            std::cerr << "[Error] No se pudo configurar el puerto." << std::endl;
            close(puerto);
            puerto = -1;
            return false;
        }

        conectado = true;
        std::cout << "[ComunicacionSerial] Conectado a " << puertoNombre 
                  << " a " << velocidad << " baudios (Linux)." << std::endl;
        return true;
    }
    #endif
};

#endif 
