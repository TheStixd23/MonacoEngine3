#pragma once
#include "Prerequisites.h"
#include "IResource.h"

/**
 * @class ResourceManager
 * @brief Gestor centralizado de recursos (Assets) del motor.
 *
 * Implementa el patrón **Singleton** para garantizar un único punto de acceso global.
 * Utiliza el patrón Flyweight para evitar duplicidad en la carga de recursos:
 * si se solicita un recurso que ya existe en memoria, devuelve la instancia existente
 * en lugar de crear una nueva.
 */
class ResourceManager {
public:
    /** @brief Constructor por defecto. */
    ResourceManager()  = default;
    
    /** @brief Destructor por defecto. */
    ~ResourceManager() = default;

    /**
     * @brief Obtiene la instancia única del gestor (Singleton).
     * @return Referencia estática a la instancia del ResourceManager.
     */
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // Deshabilitar copia y asignación para mantener la integridad del Singleton.
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    /**
     * @brief Obtiene un recurso del caché o lo carga desde disco si no existe.
     *
     * Este es el método principal para solicitar recursos.
     * 1. Busca en el mapa si la `key` ya existe.
     * 2. Si existe y está cargado, devuelve el puntero compartido.
     * 3. Si no, crea una nueva instancia, llama a `load()` e `init()`.
     * 4. Almacena el nuevo recurso en el mapa y lo devuelve.
     *
     * @tparam T Tipo del recurso a cargar (debe heredar de @ref IResource).
     * @tparam Args Tipos de los argumentos adicionales para el constructor del recurso.
     *
     * @param key Identificador único para el recurso (usualmente el nombre o ruta relativa).
     * @param filename Ruta del archivo en disco que se usará para `load()`.
     * @param args Argumentos variadicos que se pasan al constructor de `T` (Perfect Forwarding).
     *
     * @return Puntero compartido (`std::shared_ptr`) al recurso, o `nullptr` si falla la carga/inicialización.
     */
    template<typename T, typename... Args>
    std::shared_ptr<T> GetOrLoad(const std::string& key,
                                 const std::string& filename,
                                 Args&&... args) {
        static_assert(std::is_base_of<IResource, T>::value, "El tipo T debe heredar de IResource");
        
        // 1. ¿Ya existe el recurso en el caché?
        auto it = m_resources.find(key);
        if (it != m_resources.end()) {
            // Intentar castear al tipo correcto
            auto existing = std::dynamic_pointer_cast<T>(it->second);
            // Verificar si el recurso es válido y está cargado
            if (existing && existing->GetState() == ResourceState::Loaded) {
                return existing;
            }
        }

        // 2. No existe o no está cargado -> crearlo y cargarlo
        std::shared_ptr<T> resource = std::make_shared<T>(key, std::forward<Args>(args)...);

        // Intentar cargar desde archivo
        if (!resource->load(filename)) {
            // Aquí se podría loguear un error específico
            return nullptr;
        }

        // Inicializar recursos de GPU (Buffers, Texturas, etc.)
        if (!resource->init()) {
            return nullptr;
        }

        // 3. Guardar en el caché y devolver
        m_resources[key] = resource;
        return resource;
    }

    /**
     * @brief Busca un recurso ya cargado en el caché.
     *
     * A diferencia de @ref GetOrLoad, este método no intenta cargar el recurso si falta.
     * Útil para comprobar existencia sin efectos secundarios.
     *
     * @tparam T Tipo al cual castear el recurso.
     * @param key Clave única del recurso.
     * @return Puntero compartido al recurso si existe, `nullptr` en caso contrario.
     */
    template<typename T>
    std::shared_ptr<T> Get(const std::string& key) const
    {
        auto it = m_resources.find(key);
        if (it == m_resources.end()) return nullptr;

        return std::dynamic_pointer_cast<T>(it->second);
    }

    /**
     * @brief Libera un recurso específico de la memoria.
     *
     * Llama a `unload()` en el recurso y lo elimina del mapa de gestión.
     * Nota: Si otros sistemas mantienen `shared_ptr` a este recurso, la memoria no se liberará
     * completamente hasta que esas referencias desaparezcan.
     *
     * @param key Clave del recurso a liberar.
     */
    void Unload(const std::string& key)
    {
        auto it = m_resources.find(key);
        if (it != m_resources.end()) {
            it->second->unload();
            m_resources.erase(it);
        }
    }

    /**
     * @brief Libera todos los recursos gestionados.
     *
     * Método de limpieza general, útil al cerrar el juego o cambiar de nivel/escena.
     */
    void UnloadAll()
    {
        for (auto& [key, res] : m_resources) {
            if (res) {
                res->unload();
            }
        }
        m_resources.clear();
    }

private:
    /**
     * @brief Mapa hash que actúa como caché de recursos.
     * Clave: std::string (Nombre/ID).
     * Valor: std::shared_ptr<IResource> (Recurso polimórfico).
     */
    std::unordered_map<std::string, std::shared_ptr<IResource>> m_resources;
};