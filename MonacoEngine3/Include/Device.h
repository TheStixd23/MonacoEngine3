#pragma once

#include "Prerequisites.h"

// =================================================================================
// CLASE: DEVICE (Direct3D 11 Wrapper)
// =================================================================================

/**
 * @class Device
 * @brief Clase responsable de administrar el dispositivo gráfico Direct3D 11.
 *
 * Facilita la creación y control de un objeto @c ID3D11Device.
 * Actúa como una fábrica para generar recursos (Texturas, Shaders, Buffers).
 */
class Device {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Construye un objeto sin inicializar.
     */
    Device() = default;


    /**
     * @brief Destructor trivial.
     * @note No libera automáticamente los recursos. Invocar @c destroy().
     */
    ~Device() = default;


    // -----------------------------------------------------------------------------
    // CICLO DE VIDA
    // -----------------------------------------------------------------------------

    /**
     * @brief Configura el dispositivo Direct3D 11.
     * Crea el @c ID3D11Device y las feature levels necesarias.
     */
    void
        init();


    /**
     * @brief Aplica cambios en el estado del dispositivo.
     */
    void
        update();


    /**
     * @brief Lanza las rutinas de renderizado asociadas al dispositivo.
     */
    void
        render();


    /**
     * @brief Libera todos los recursos creados y cierra el dispositivo.
     * Establece @c m_device en @c nullptr.
     */
    void
        destroy();


    // -----------------------------------------------------------------------------
    // FÁBRICA DE RECURSOS (Resource Creation)
    // -----------------------------------------------------------------------------

    /**
     * @brief Crea una vista de render target (RTV).
     * @param pResource Recurso de origen (Textura/Buffer).
     * @param pDesc Descriptor opcional para la vista (nullptr para default).
     * @param ppRTView Referencia de salida para la interfaz creada.
     * @return @c S_OK si la creación fue exitosa.
     */
    HRESULT
        CreateRenderTargetView(ID3D11Resource* pResource,
                                const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
                                ID3D11RenderTargetView** ppRTView);


    /**
     * @brief Crea una textura 2D en memoria de GPU.
     * @param pDesc Estructura con las características de la textura.
     * @param pInitialData Datos iniciales opcionales para llenar la textura.
     * @param ppTexture2D Referencia de salida para la textura resultante.
     * @return @c S_OK si la creación fue exitosa.
     */
    HRESULT
        CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
                        const D3D11_SUBRESOURCE_DATA* pInitialData,
                        ID3D11Texture2D** ppTexture2D);


    /**
     * @brief Crea una vista de Depth Stencil (DSV).
     * @param pResource Recurso base (Textura de profundidad).
     * @param pDesc Descriptor opcional de la vista.
     * @param ppDepthStencilView Referencia de salida para la vista resultante.
     * @return @c S_OK si la creación fue exitosa.
     */
    HRESULT
        CreateDepthStencilView(ID3D11Resource* pResource,
                                const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
                                ID3D11DepthStencilView** ppDepthStencilView);


    /**
     * @brief Crea un shader de vértices desde bytecode compilado.
     * @param pShaderBytecode Puntero al blob del shader compilado.
     * @param BytecodeLength Tamaño del bytecode.
     * @param pClassLinkage Enlace de clases dinámico (opcional).
     * @param ppVertexShader Salida del shader creado.
     */
    HRESULT
        CreateVertexShader(const void* pShaderBytecode,
                            SIZE_T BytecodeLength,
                            ID3D11ClassLinkage* pClassLinkage,
                            ID3D11VertexShader** ppVertexShader);


    /**
     * @brief Genera un layout de entrada para la etapa de ensamblado de vértices.
     * Define cómo se mapean los datos del buffer a los inputs del shader.
     * @param pInputElementDescs Array de descripciones de elementos.
     * @param NumElements Cantidad de elementos en el array.
     * @param pShaderBytecodeWithInputSignature Bytecode del VS para validación.
     * @param BytecodeLength Tamaño del bytecode.
     * @param ppInputLayout Salida del layout creado.
     */
    HRESULT
        CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                            UINT NumElements,
                            const void* pShaderBytecodeWithInputSignature,
                            SIZE_T BytecodeLength,
                            ID3D11InputLayout** ppInputLayout);


    /**
     * @brief Crea un shader de píxeles desde bytecode compilado.
     * @param pShaderBytecode Puntero al blob del shader compilado.
     * @param BytecodeLength Tamaño del bytecode.
     * @param pClassLinkage Enlace de clases dinámico (opcional).
     * @param ppPixelShader Salida del shader creado.
     */
    HRESULT
        CreatePixelShader(const void* pShaderBytecode,
                            SIZE_T BytecodeLength,
                            ID3D11ClassLinkage* pClassLinkage,
                            ID3D11PixelShader** ppPixelShader);


    /**
     * @brief Reserva un buffer en GPU para vértices, índices o constantes.
     * @param pDesc Descripción del buffer (uso, tamaño, flags).
     * @param pInitialData Datos para inicializar el buffer (opcional).
     * @param ppBuffer Salida del buffer creado.
     */
    HRESULT
        CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
                    const D3D11_SUBRESOURCE_DATA* pInitialData,
                    ID3D11Buffer** ppBuffer);


    /**
     * @brief Crea un estado de muestreo de texturas (Sampler State).
     * Configura filtros, modos de direccionamiento (wrap, clamp), etc.
     * @param pSamplerDesc Descripción del estado del sampler.
     * @param ppSamplerState Salida del estado creado.
     */
    HRESULT
        CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
                           ID3D11SamplerState** ppSamplerState);


public:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO
    // -----------------------------------------------------------------------------

    /**
     * @brief Puntero al objeto @c ID3D11Device subyacente.
     * Inicializado en @c init() y liberado en @c destroy().
     */
    ID3D11Device* m_device = nullptr;

};