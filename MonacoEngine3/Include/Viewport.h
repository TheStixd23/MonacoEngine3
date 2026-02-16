#pragma once

#include "Prerequisites.h"

class Window;
class DeviceContext;


// =================================================================================
// CLASE: VIEWPORT (Pipeline Rasterizer Stage)
// =================================================================================

/**
 * @class Viewport
 * @brief Encapsula la estructura D3D11_VIEWPORT y proporciona métodos para su inicialización y vinculación.
 *
 * Un viewport define qué porción del Render Target se mapea al espacio de coordenadas de clip
 * normalizado (NDC). Es esencial para definir el área de dibujo en la ventana.
 */
class Viewport {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Constructor por defecto.
     */
    Viewport() = default;


    /**
     * @brief Destructor por defecto.
     */
    ~Viewport() = default;


    // -----------------------------------------------------------------------------
    // INICIALIZACIÓN
    // -----------------------------------------------------------------------------

    /**
     * @brief Inicializa el viewport basándose en las dimensiones de una ventana existente.
     *
     * Utiliza el ancho y alto del área cliente de la ventana para configurar el viewport.
     * @param window Referencia constante al objeto Window cuyas dimensiones se utilizarán.
     * @return HRESULT Siempre devuelve S_OK (operación local).
     */
    HRESULT
        init(const Window& window);


    /**
     * @brief Inicializa el viewport con dimensiones específicas manualmente.
     *
     * Configura el ancho, alto, y rango de profundidad (0.0 a 1.0).
     * @param width El ancho del viewport en píxeles.
     * @param height La altura del viewport en píxeles.
     * @return HRESULT Siempre devuelve S_OK.
     */
    HRESULT
        init(unsigned int width,
            unsigned int height);


    // -----------------------------------------------------------------------------
    // OPERACIONES DE PIPELINE
    // -----------------------------------------------------------------------------

    /**
     * @brief Lógica de actualización.
     * @note Generalmente vacía para un viewport estático, salvo en redimensionado dinámico.
     */
    void
        update();


    /**
     * @brief Vincula el viewport al pipeline de renderizado (Rasterizer Stage).
     *
     * Llama a RSSetViewports en el DeviceContext para establecer este viewport como activo.
     * @param deviceContext Referencia al contexto del dispositivo.
     */
    void
        render(DeviceContext& deviceContext);


    /**
     * @brief Limpia y libera recursos.
     * @note Implementación vacía ya que D3D11_VIEWPORT es una estructura POD (Plain Old Data) sin recursos COM.
     */
    void
        destroy()
    {
    }


public:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO (Nativos)
    // -----------------------------------------------------------------------------

    /**
     * @brief Estructura nativa de DirectX que contiene la definición del viewport.
     * (TopLeftX, TopLeftY, Width, Height, MinDepth, MaxDepth).
     */
    D3D11_VIEWPORT m_viewport{};

};