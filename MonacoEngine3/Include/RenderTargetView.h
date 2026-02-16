#pragma once

#include "Prerequisites.h"

class Device;
class DeviceContext;
class Texture;
class DepthStencilView;


// =================================================================================
// CLASE: RENDER TARGET VIEW (RTV)
// =================================================================================

/**
 * @class RenderTargetView
 * @brief Encapsula la creación, gestión y uso de un Render Target View (RTV) en Direct3D 11.
 *
 * Esta clase gestiona el ciclo de vida del RTV, desde su inicialización hasta su destrucción.
 * Permite configurarlo para renderizar sobre el Back Buffer o sobre Texturas personalizadas.
 */
class RenderTargetView {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Crea un objeto vacío de RenderTargetView.
     * @details No realiza ninguna inicialización; se requiere llamar a `init()` antes de usarlo.
     */
    RenderTargetView() = default;


    /**
     * @brief Destructor trivial del objeto.
     * @note El recurso asociado no se libera automáticamente. Invocar `destroy()`.
     */
    ~RenderTargetView() = default;


    // -----------------------------------------------------------------------------
    // INICIALIZACIÓN (Creation)
    // -----------------------------------------------------------------------------

    /**
     * @brief Construye un Render Target View a partir del back buffer.
     * @param device Dispositivo Direct3D responsable de la creación.
     * @param backBuffer Textura que representa el back buffer del swap chain.
     * @param Format Formato en el que se definirá el RTV (ej: `DXGI_FORMAT_R8G8B8A8_UNORM`).
     * @return Devuelve `S_OK` en caso de éxito.
     */
    HRESULT
        init(Device& device,
            Texture& backBuffer,
            DXGI_FORMAT Format);


    /**
     * @brief Inicializa un RTV a partir de una textura arbitraria.
     * Útil para render targets secundarios (Deferred rendering, Shadow Maps).
     * @param device Dispositivo que gestiona la creación del recurso.
     * @param inTex Textura destino donde se dibujará la salida.
     * @param ViewDimension Dimensión de la vista (ej: `D3D11_RTV_DIMENSION_TEXTURE2D`).
     * @param Format Formato deseado para la vista.
     * @return `S_OK` si la operación se completa con éxito.
     */
    HRESULT
        init(Device& device,
            Texture& inTex,
            D3D11_RTV_DIMENSION ViewDimension,
            DXGI_FORMAT Format);


    // -----------------------------------------------------------------------------
    // OPERACIONES (Pipeline & Logic)
    // -----------------------------------------------------------------------------

    /**
     * @brief Punto de extensión para actualizar parámetros internos del RTV.
     */
    void
        update();


    /**
     * @brief Aplica el RTV al pipeline de render y lo limpia con un color específico.
     * @param deviceContext Contexto de dispositivo donde se establecerá la vista.
     * @param depthStencilView Vista de Depth Stencil a enlazar junto al RTV.
     * @param numViews Número de vistas a establecer (habitualmente 1).
     * @param ClearColor Array de 4 floats (RGBA) para limpiar el fondo.
     */
    void
        render(DeviceContext& deviceContext,
                DepthStencilView& depthStencilView,
                unsigned int numViews,
                const float ClearColor[4]);


    /**
     * @brief Asigna el RTV al pipeline sin realizar operaciones de limpieza.
     * @param deviceContext Contexto del dispositivo donde se establecerá.
     * @param numViews Número de vistas a utilizar (generalmente 1).
     */
    void
        render(DeviceContext& deviceContext,
            unsigned int numViews);


    /**
     * @brief Libera el recurso `ID3D11RenderTargetView` asociado.
     * Reinicia el puntero interno a `nullptr`.
     */
    void
        destroy();


private:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO PRIVADOS
    // -----------------------------------------------------------------------------

    /**
     * @brief Puntero COM al recurso de vista de destino de renderizado.
     * Será válido tras una inicialización exitosa.
     */
    ID3D11RenderTargetView* m_renderTargetView = nullptr;

};