#pragma once

#include "Prerequisites.h"
#include <string>
#include <array>

class Device;
class DeviceContext;


// =================================================================================
// CLASE: TEXTURE (2D & Cubemap)
// =================================================================================

/**
 * @class Texture
 * @brief Encapsula una textura 2D en Direct3D 11, incluyendo su recurso y vista como Shader Resource.
 *
 * Esta clase administra texturas que pueden provenir de:
 * - Archivos de imagen (png, jpg, dds).
 * - Texturas creadas en memoria (RTV, DSV).
 * - Cubemaps (Skyboxes).
 */
class Texture {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Constructor por defecto.
     */
    Texture() = default;


    /**
     * @brief Destructor por defecto.
     * @note No libera recursos automáticamente; llamar a destroy().
     */
    ~Texture() = default;


    // -----------------------------------------------------------------------------
    // INICIALIZACIÓN (Creation)
    // -----------------------------------------------------------------------------

    /**
     * @brief Inicializa una textura cargada desde archivo.
     *
     * Crea un recurso de textura a partir de una imagen y genera su SRV.
     * @param device Dispositivo con el que se creará la textura.
     * @param textureName Nombre o ruta del archivo de textura.
     * @param extensionType Tipo de extensión de archivo (ej. PNG, JPG, DDS).
     * @return @c S_OK si fue exitoso.
     */
    HRESULT
        init(Device& device,
            const std::string& textureName,
            ExtensionType extensionType);


    /**
     * @brief Inicializa una textura creada desde memoria (Procedural / Render Target).
     *
     * Crea un recurso de textura 2D vacío con un tamaño y formato especificados.
     * Útil para render targets, depth buffers o texturas dinámicas.
     *
     * @param device Dispositivo con el que se creará la textura.
     * @param width Ancho de la textura en píxeles.
     * @param height Alto de la textura en píxeles.
     * @param Format Formato DXGI de la textura.
     * @param BindFlags Banderas de enlace (ej. SHADER_RESOURCE, RENDER_TARGET).
     * @param sampleCount Número de muestras para MSAA (default 1).
     * @param qualityLevels Niveles de calidad soportados para MSAA.
     * @return @c S_OK si fue exitoso.
     */
    HRESULT
        init(Device& device,
            unsigned int width,
            unsigned int height,
            DXGI_FORMAT Format,
            unsigned int BindFlags,
            unsigned int sampleCount = 1,
            unsigned int qualityLevels = 0);


    /**
     * @brief Inicializa una textura a partir de otra existente (Copy / View).
     *
     * Crea una nueva textura basada en la descripción de @p textureRef.
     * @param device Dispositivo con el que se creará la textura.
     * @param textureRef Referencia a otra textura existente.
     * @param format Nuevo formato DXGI de la textura.
     * @return @c S_OK si fue exitoso.
     */
    HRESULT
        init(Device& device,
            Texture& textureRef,
            DXGI_FORMAT format);


    /**
     * @brief Crea un Cubemap (Skybox) a partir de 6 texturas individuales.
     *
     * @param device Dispositivo gráfico.
     * @param deviceContext Contexto para comandos de copia.
     * @param facePaths Array de 6 rutas a las texturas (Right, Left, Top, Bottom, Front, Back).
     * @param generateMips Si es true, genera mipmaps automáticamente.
     * @return S_OK si el cubemap se creó correctamente.
     */
    HRESULT
        CreateCubemap(Device& device,
                      DeviceContext& deviceContext,
                      const std::array<std::string, 6>& facePaths,
                      bool generateMips = false);


    // -----------------------------------------------------------------------------
    // OPERACIONES (Runtime)
    // -----------------------------------------------------------------------------

    /**
     * @brief Actualiza el contenido de la textura.
     * @note Actualmente es un marcador (placeholder).
     */
    void
        update();


    /**
     * @brief Asigna la textura al pipeline de render (Pixel Shader).
     *
     * Llama a @c PSSetShaderResources.
     * @param deviceContext Contexto donde se aplicará la textura.
     * @param StartSlot Slot inicial de vinculación.
     * @param NumViews Número de vistas a asignar (normalmente 1).
     */
    void
        render(DeviceContext& deviceContext,
            unsigned int StartSlot,
            unsigned int NumViews);


    /**
     * @brief Libera los recursos de la textura (Texture2D y SRV).
     * Establece los punteros internos a @c nullptr.
     */
    void
        destroy();


    // -----------------------------------------------------------------------------
    // UTILIDADES (Helpers)
    // -----------------------------------------------------------------------------

    /**
     * @brief Helper para crear una vista (SRV) de una cara específica de un Cubemap.
     * Útil para renderizar caras individuales de un entorno dinámico.
     *
     * @param device Puntero nativo al dispositivo.
     * @param cubemapTex Puntero nativo a la textura del cubemap.
     * @param format Formato DXGI de la vista.
     * @param faceIndex Índice de la cara (0-5).
     * @param mipLevels Niveles de mipmap a incluir en la vista.
     * @return Puntero al SRV creado o nullptr si falla.
     */
    ID3D11ShaderResourceView* CreateCubemapFaceSRV(ID3D11Device* device,
        ID3D11Texture2D* cubemapTex,
        DXGI_FORMAT format,
        UINT faceIndex,
        UINT mipLevels = 1)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC d{};
        d.Format = format;
        d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
        d.Texture2DArray.MostDetailedMip = 0;
        d.Texture2DArray.MipLevels = mipLevels;
        d.Texture2DArray.FirstArraySlice = faceIndex; // Cara específica
        d.Texture2DArray.ArraySize = 1;               // Solo esa cara

        ID3D11ShaderResourceView* srv = nullptr;
        if (FAILED(device->CreateShaderResourceView(cubemapTex, &d, &srv)))
        {
            return nullptr;
        }

        return srv;
    }


public:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO (Recursos Nativos)
    // -----------------------------------------------------------------------------

    /** @brief Recurso base de la textura en GPU. */
    ID3D11Texture2D* m_texture = nullptr;


    /** @brief Vista de la textura como recurso de shader (SRV). */
    ID3D11ShaderResourceView* m_textureFromImg = nullptr;


    /** @brief Nombre o ruta de la textura (si proviene de archivo). */
    std::string m_textureName;

};