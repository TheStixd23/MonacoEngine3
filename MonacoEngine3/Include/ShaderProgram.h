#pragma once

#include "Prerequisites.h"
#include "InputLayout.h"
#include <string>
#include <vector>

class Device;
class DeviceContext;


// =================================================================================
// CLASE: SHADER PROGRAM
// =================================================================================

/**
 * @class ShaderProgram
 * @brief Clase que gestiona los Shaders de Vértice y Píxel y el Layout de Entrada asociado.
 *
 * Contiene métodos para compilar shaders a partir de archivos HLSL, crear los recursos nativos
 * de DirectX (ID3D11VertexShader, ID3D11PixelShader) y vincularlos al pipeline.
 */
class ShaderProgram {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Constructor por defecto.
     */
    ShaderProgram() = default;


    /**
     * @brief Destructor por defecto.
     */
    ~ShaderProgram() = default;


    // -----------------------------------------------------------------------------
    // CICLO DE VIDA (Life Cycle)
    // -----------------------------------------------------------------------------

    /**
     * @brief Inicializa el programa de shaders completo.
     *
     * Compila los shaders (VS y PS) desde un archivo y crea el InputLayout.
     * @param device Referencia al objeto Device de DirectX.
     * @param fileName Ruta del archivo que contiene el código fuente HLSL.
     * @param Layout Vector de estructuras que describen los elementos del vértice.
     * @return HRESULT El código de resultado de la operación (S_OK si es exitosa).
     */
    HRESULT
        init(Device& device,
            const std::string& fileName,
            std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);


    /**
     * @brief Lógica de actualización (generalmente vacía para shaders estáticos).
     */
    void
        update();


    /**
     * @brief Limpia y libera todos los recursos de shaders y los blobs compilados.
     */
    void
        destroy();


    // -----------------------------------------------------------------------------
    // OPERACIONES DE PIPELINE (Binding)
    // -----------------------------------------------------------------------------

    /**
     * @brief Vincula todos los shaders (VS y PS) y el InputLayout al pipeline.
     * @param deviceContext Contexto del dispositivo para establecer los recursos.
     */
    void
        render(DeviceContext& deviceContext);


    /**
     * @brief Vincula un tipo específico de shader al pipeline de renderizado.
     * @param deviceContext Contexto del dispositivo.
     * @param type Especifica el tipo de shader a vincular (e.g., VertexShader, PixelShader).
     */
    void
        render(DeviceContext& deviceContext,
            ShaderType type);


    // -----------------------------------------------------------------------------
    // CREACIÓN DE RECURSOS (Interno/Avanzado)
    // -----------------------------------------------------------------------------

    /**
     * @brief Crea el InputLayout de vértices usando los datos del Vertex Shader compilado.
     * @param device Referencia al objeto Device de DirectX.
     * @param Layout Vector de descripción de elementos.
     * @return HRESULT El código de resultado.
     */
    HRESULT
        CreateInputLayout(Device& device,
            std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);


    /**
     * @brief Crea un recurso de shader nativo de DirectX a partir de datos ya compilados.
     * Usa los blobs internos `m_vertexShaderData` o `m_pixelShaderData`.
     * @param device Referencia al objeto Device de DirectX.
     * @param type Especifica el tipo de shader a crear (VS o PS).
     * @return HRESULT El código de resultado.
     */
    HRESULT
        CreateShader(Device& device,
            ShaderType type);


    /**
     * @brief Compila un shader desde un archivo y luego crea el recurso de DirectX.
     * @param device Referencia al objeto Device de DirectX.
     * @param type Especifica el tipo de shader a compilar y crear.
     * @param fileName Ruta del archivo HLSL.
     * @return HRESULT El código de resultado.
     */
    HRESULT
        CreateShader(Device& device,
                    ShaderType type,
                    const std::string& fileName);


    /**
     * @brief Compila el código fuente del shader desde un archivo a un buffer de bytecodes.
     * Función auxiliar de bajo nivel para compilar HLSL.
     * @param szFileName Nombre del archivo del shader (HLSL).
     * @param szEntryPoint Nombre de la función principal (e.g., "VSMain", "PSMain").
     * @param szShaderModel Modelo del shader (e.g., "vs_5_0", "ps_5_0").
     * @param ppBlobOut Puntero de salida para el búfer de datos compilado (ID3DBlob).
     * @return HRESULT El código de resultado.
     */
    HRESULT
        CompileShaderFromFile(char* szFileName,
            LPCSTR szEntryPoint,
            LPCSTR szShaderModel,
            ID3DBlob** ppBlobOut);


public:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO PÚBLICOS (Recursos Nativos)
    // -----------------------------------------------------------------------------

    /** @brief Puntero al objeto nativo ID3D11VertexShader de DirectX. */
    ID3D11VertexShader* m_VertexShader = nullptr;


    /** @brief Puntero al objeto nativo ID3D11PixelShader de DirectX. */
    ID3D11PixelShader* m_PixelShader = nullptr;


    /** @brief Objeto que gestiona el layout de entrada de los vértices asociado. */
    InputLayout m_inputLayout;


private:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO PRIVADOS (Blobs & Metadata)
    // -----------------------------------------------------------------------------

    /** @brief Nombre del archivo desde el que se cargaron los shaders. */
    std::string m_shaderFileName;


    /** @brief Búfer de datos (blob) del Vertex Shader compilado. Necesario para el InputLayout. */
    ID3DBlob* m_vertexShaderData = nullptr;


    /** @brief Búfer de datos (blob) del Pixel Shader compilado. */
    ID3DBlob* m_pixelShaderData = nullptr;

};