#pragma once

#include "Prerequisites.h"
#include <string>


// =================================================================================
// ENUMERACIONES
// =================================================================================

/**
 * @enum ResourceType
 * @brief Categorías de recursos disponibles en el motor.
 */
enum class ResourceType {
    Unknown,    ///< Tipo no identificado o inicializado.
    Model3D,    ///< Malla tridimensional (ej. .obj, .fbx).
    Texture,    ///< Imagen o textura (ej. .png, .dds).
    Sound,      ///< Archivo de audio.
    Shader,     ///< Programa de sombreado (Vertex, Pixel, etc.).
    Material    ///< Definición de material y propiedades de superficie.
};


/**
 * @enum ResourceState
 * @brief Representa el estado actual del ciclo de vida de un recurso.
 */
enum class ResourceState {
    Unloaded,   ///< El recurso no reside en memoria (RAM/VRAM).
    Loading,    ///< El recurso está en proceso de carga.
    Loaded,     ///< El recurso está completamente cargado y listo para usarse.
    Failed      ///< Hubo un error al intentar cargar o inicializar el recurso.
};


// =================================================================================
// CLASE BASE: IRESOURCE
// =================================================================================

/**
 * @class IResource
 * @brief Interfaz base abstracta para todos los recursos gestionados por el motor.
 */
class IResource {

public:

    /**
     * @brief Constructor de la interfaz de recurso.
     * @param name Nombre identificador del recurso.
     */
    IResource(const std::string& name)
        : m_name(name)
        , m_filePath("")
        , m_type(ResourceType::Unknown)
        , m_state(ResourceState::Unloaded)
        , m_id(GenerateID())
    {
        // El cuerpo del constructor está vacío porque usamos lista de inicialización arriba
    }


    /**
     * @brief Destructor virtual.
     */
    virtual ~IResource() = default;


    // -----------------------------------------------------------------------------
    // MÉTODOS VIRTUALES PUROS (Lógica principal)
    // -----------------------------------------------------------------------------

    /**
     * @brief Inicializa el recurso en la API gráfica (GPU).
     * @return `true` si la inicialización en GPU fue exitosa, `false` en caso contrario.
     */
    virtual bool init() = 0;


    /**
     * @brief Carga los datos del recurso desde el disco a la memoria del sistema (RAM).
     * @param filename Ruta absoluta o relativa al archivo en el disco.
     * @return `true` si la lectura del archivo fue exitosa.
     */
    virtual bool load(const std::string& filename) = 0;


    /**
     * @brief Libera la memoria utilizada por el recurso (RAM y VRAM).
     */
    virtual void unload() = 0;


    /**
     * @brief Obtiene el tamaño aproximado del recurso en bytes.
     * @return Tamaño en bytes ocupado por el recurso.
     */
    virtual size_t getSizeInBytes() const = 0;


    // -----------------------------------------------------------------------------
    // SETTERS (Modificadores)
    // -----------------------------------------------------------------------------

    /** @brief Establece la ruta del archivo asociado al recurso. */
    void SetPath(const std::string& path) {
        m_filePath = path;
    }


    /** @brief Define el tipo de recurso. */
    void SetType(ResourceType t) {
        m_type = t;
    }


    /** @brief Actualiza el estado actual del recurso. */
    void SetState(ResourceState s) {
        m_state = s;
    }


    // -----------------------------------------------------------------------------
    // GETTERS (Accesores)
    // -----------------------------------------------------------------------------

    /** @return Nombre del recurso. */
    const std::string& GetName() const {
        return m_name;
    }


    /** @return Ruta del archivo fuente. */
    const std::string& GetPath() const {
        return m_filePath;
    }


    /** @return Tipo de recurso. */
    ResourceType GetType() const {
        return m_type;
    }


    /** @return Estado actual de carga/vida. */
    ResourceState GetState() const {
        return m_state;
    }


    /** @return Identificador único (ID) generado automáticamente. */
    uint64_t GetID() const {
        return m_id;
    }


protected:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO
    // -----------------------------------------------------------------------------

    std::string    m_name;      ///< Nombre del recurso (clave de búsqueda).
    std::string    m_filePath;  ///< Ubicación en disco.
    ResourceType   m_type;      ///< Categoría del recurso.
    ResourceState  m_state;     ///< Estado actual.
    uint64_t       m_id;        ///< ID único de instancia.


private:

    /**
     * @brief Generador interno de IDs únicos.
     * @return Un nuevo ID incremental cada vez que se llama.
     */
    static uint64_t GenerateID()
    {
        static uint64_t nextID = 1;
        return nextID++;
    }

};