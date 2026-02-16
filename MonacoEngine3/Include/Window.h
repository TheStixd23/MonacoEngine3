#pragma once

#include "Prerequisites.h"
#include <string>


// =================================================================================
// CLASE: WINDOW (Win32 Wrapper)
// =================================================================================

/**
 * @class Window
 * @brief Abstracción de una ventana de aplicación Win32 utilizada por el motor gráfico.
 *
 * Esta clase encapsula la creación, manejo y destrucción de una ventana en sistemas Windows.
 * Sirve como contenedor de bajo nivel sobre la API Win32 y punto de entrada para el renderizado.
 */
class Window {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Constructor por defecto.
     * @details Inicializa la clase sin crear la ventana. Se requiere llamar a `init()`.
     */
    Window() = default;


    /**
     * @brief Destructor por defecto.
     * @note No destruye automáticamente la ventana del SO. Llamar a `destroy()`.
     */
    ~Window() = default;


    // -----------------------------------------------------------------------------
    // CICLO DE VIDA (Life Cycle)
    // -----------------------------------------------------------------------------

    /**
     * @brief Crea e inicializa la ventana principal de la aplicación.
     *
     * @param hInstance Identificador de la instancia de la aplicación (WinMain).
     * @param nCmdShow Comando que especifica cómo debe mostrarse la ventana (ej.: SW_SHOW).
     * @param wndproc Función de callback que manejará los mensajes de ventana (Win32 WNDPROC).
     * @return `S_OK` si la ventana fue creada con éxito.
     */
    HRESULT
        init(HINSTANCE hInstance,
            int nCmdShow,
            WNDPROC wndproc);


    /**
     * @brief Actualiza el estado de la ventana y procesa los mensajes del sistema.
     * Debe llamarse en cada iteración del bucle principal (Message Pump).
     */
    void
        update();


    /**
     * @brief Punto de entrada para operaciones de renderizado asociadas a la ventana.
     * Lugar para llamadas de preparación del frame o sincronización (Present).
     */
    void
        render();


    /**
     * @brief Libera los recursos asociados a la ventana y la destruye.
     * Cierra la ventana y limpia las estructuras internas.
     */
    void
        destroy();


public:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO PÚBLICOS
    // -----------------------------------------------------------------------------

    /**
     * @brief Manejador de la ventana Win32 (Handle).
     * Identificador único utilizado para interactuar con la API del SO.
     */
    HWND m_hWnd = nullptr;


    /**
     * @brief Ancho actual de la ventana en píxeles.
     */
    unsigned int m_width = 0;


    /**
     * @brief Altura actual de la ventana en píxeles.
     */
    unsigned int m_height = 0;


private:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO PRIVADOS
    // -----------------------------------------------------------------------------

    /**
     * @brief Identificador de instancia de la aplicación.
     * Proporcionado por el sistema operativo al iniciar la aplicación.
     */
    HINSTANCE m_hInst = nullptr;


    /**
     * @brief Estructura que define el área cliente de la ventana.
     * Almacena coordenadas de posición y tamaño utilizable.
     */
    RECT m_rect{};


    /**
     * @brief Nombre por defecto de la ventana (Título en la barra).
     */
    std::string m_windowName = "MonacoEngine3";

};