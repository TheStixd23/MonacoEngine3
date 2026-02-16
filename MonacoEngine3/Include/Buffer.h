#pragma once

#include "Prerequisites.h"
#include "MeshComponent.h"

class Device;
class DeviceContext;


// =================================================================================
// CLASE: BUFFER (Recurso GPU)
// =================================================================================

/**
 * @class Buffer
 * @brief Encapsula un recurso @c ID3D11Buffer de DirectX 11.
 *
 * Esta clase gestiona la creación, actualización y vinculación de buffers de hardware.
 * @note La instancia maneja un único buffer; su rol se define en la inicialización.
 */
class Buffer {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Constructor por defecto.
     */
    Buffer() = default;


    /**
     * @brief Destructor.
     * @note No libera el recurso automáticamente.
     */
    ~Buffer() = default;


    // -----------------------------------------------------------------------------
    // INICIALIZACIÓN (Creation)
    // -----------------------------------------------------------------------------

    /**
     * @brief Inicializa el buffer como Vertex Buffer o Index Buffer a partir de una malla.
     *
     * Crea el buffer en GPU y sube los datos contenidos en el @c MeshComponent.
     * @param device Dispositivo DirectX utilizado para crear el recurso.
     * @param mesh Componente de malla con los datos de origen (vértices/índices).
     * @param bindFlag Bandera que define el tipo: @c D3D11_BIND_VERTEX_BUFFER o @c D3D11_BIND_INDEX_BUFFER.
     * @return @c S_OK si la creación fue exitosa, o un código de error HRESULT.
     */
    HRESULT
        init(Device& device,
            const MeshComponent& mesh,
            unsigned int bindFlag);


    /**
     * @brief Inicializa el buffer como Constant Buffer.
     *
     * Reserva memoria en GPU para un buffer de constantes dinámico/estático.
     * @param device Dispositivo DirectX utilizado para crear el recurso.
     * @param ByteWidth Tamaño del buffer en bytes (debe ser múltiplo de 16).
     * @return @c S_OK si la creación fue exitosa.
     */
    HRESULT
        init(Device& device,
            unsigned int ByteWidth);


    /**
     * @brief Método interno para la creación de bajo nivel del buffer.
     * @param device Dispositivo DirectX.
     * @param desc Descripción completa de la configuración del buffer.
     * @param initData Puntero a los datos iniciales (opcional).
     * @return @c S_OK si se crea correctamente.
     */
    HRESULT
        createBuffer(Device& device,
            D3D11_BUFFER_DESC& desc,
            D3D11_SUBRESOURCE_DATA* initData);


    // -----------------------------------------------------------------------------
    // OPERACIONES (Update & Render)
    // -----------------------------------------------------------------------------

    /**
     * @brief Actualiza los datos del buffer en la GPU.
     * Utiliza @c UpdateSubresource para transferir nuevos datos desde la CPU.
     */
    void
        update(DeviceContext& deviceContext,
            ID3D11Resource* pDstResource,
            unsigned int DstSubresource,
            const D3D11_BOX* pDstBox,
            const void* pSrcData,
            unsigned int SrcRowPitch,
            unsigned int SrcDepthPitch);


    /**
     * @brief Vincula el buffer al pipeline de renderizado.
     * Configura el buffer en la etapa correspondiente (Input Assembler o Shaders).
     */
    void
        render(DeviceContext& deviceContext,
            unsigned int StartSlot,
            unsigned int NumBuffers,
            bool setPixelShader = false,
            DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);


    /**
     * @brief Libera el recurso COM y reinicia el estado interno.
     */
    void
        destroy();


private:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO PRIVADOS
    // -----------------------------------------------------------------------------

    /** @brief Puntero al recurso de buffer de DirectX 11. */
    ID3D11Buffer* m_buffer = nullptr;


    /** @brief Tamaño de un elemento individual (Stride), usado para Vertex Buffers. */
    unsigned int m_stride = 0;


    /** @brief Desplazamiento en bytes (Offset) desde el inicio del buffer. */
    unsigned int m_offset = 0;


    /** @brief Bandera de vinculación que indica el tipo de buffer. */
    unsigned int m_bindFlag = 0;

};