#pragma once

#include "Prerequisites.h"

class Device;
class DeviceContext;
class Texture;


// =================================================================================
// CLASE: DEPTH STENCIL VIEW
// =================================================================================

/**
 * @class DepthStencilView
 * @brief Clase encargada de gestionar la vista de profundidad/esténcil en Direct3D 11.
 *
 * Proporciona una interfaz para crear, aplicar y liberar un objeto @c ID3D11DepthStencilView,
 * fundamental para el buffer de profundidad (Z-Buffer) en la etapa Output-Merger.
 *
 * @note Esta clase no administra la memoria de la Textura asociada.
 */
class DepthStencilView {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Construye un objeto vacío sin asignar recursos.
     */
    DepthStencilView() = default;


    /**
     * @brief Destructor trivial.
     * @note No libera recursos automáticamente. Se debe llamar a @c destroy().
     */
    ~DepthStencilView() = default;


    // -----------------------------------------------------------------------------
    // INICIALIZACIÓN
    // -----------------------------------------------------------------------------

    /**
     * @brief Crea la vista de profundidad/esténcil a partir de una textura válida.
     *
     * Inicializa internamente un @c ID3D11DepthStencilView asociado al recurso dado.
     * @param device Referencia al dispositivo gráfico responsable de la creación.
     * @param depthStencil Textura que actuará como buffer de profundidad.
     * @param format Formato DXGI de la vista (ej: @c DXGI_FORMAT_D24_UNORM_S8_UINT).
     * @return @c S_OK si la creación se realizó con éxito.
     */
    HRESULT
        init(Device& device,
            Texture& depthStencil,
            DXGI_FORMAT format);


    // -----------------------------------------------------------------------------
    // OPERACIONES
    // -----------------------------------------------------------------------------

    /**
     * @brief Actualiza el estado de la vista.
     * @note Actualmente es un marcador para futura expansión lógica.
     */
    void
        update()
    {
    }


    /**
     * @brief Enlaza la vista de profundidad/esténcil al contexto de render.
     *
     * Invoca a @c OMSetRenderTargets para adjuntar la vista al pipeline.
     * @param deviceContext Contexto de dispositivo donde se activará la vista.
     */
    void
        render(DeviceContext& deviceContext);


    /**
     * @brief Libera la vista de profundidad/esténcil asociada.
     * Restablece el puntero interno a @c nullptr.
     */
    void
        destroy();


public:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO (Recursos COM)
    // -----------------------------------------------------------------------------

    /**
     * @brief Puntero al recurso @c ID3D11DepthStencilView de DirectX.
     * @note Gestionado manualmente vía init() y destroy().
     */
    ID3D11DepthStencilView* m_depthStencilView = nullptr;

};