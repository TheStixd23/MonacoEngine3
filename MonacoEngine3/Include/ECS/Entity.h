#pragma once
#include "Prerequisites.h"
#include "Component.h"
#include <vector> 

class DeviceContext;

/**
 * @class Entity
 * @brief Clase base abstracta que representa un objeto fundamental dentro del mundo del juego.
 *
 * En la arquitectura ECS (Entity-Component-System), la Entidad actúa principalmente como un
 * contenedor de identificadores y una colección de componentes (@ref Component).
 * Gestiona el ciclo de vida completo (Awake, Init, Update, Render, Destroy) y la propiedad
 * de los componentes adjuntos a ella.
 */
class Entity {
public:
    /**
     * @brief Constructor por defecto.
     */
    Entity() = default;

    /**
     * @brief Destructor virtual.
     * Garantiza que los destructores de las clases derivadas se invoquen correctamente al eliminar una entidad.
     */
    virtual ~Entity() = default;

    /**
     * @brief Fase de "Despertar" de la entidad.
     *
     * Se ejecuta inmediatamente después de la instanciación y antes de `init()`.
     * Ideal para inicializar variables internas o estados que no dependen de referencias externas.
     */
    virtual void awake() = 0;

    /**
     * @brief Inicializa la entidad con el contexto del dispositivo.
     *
     * Se llama después de que la entidad ha sido creada y despertada.
     * Aquí se deben cargar recursos gráficos o establecer referencias que dependan del DeviceContext.
     */
    virtual void init() = 0;

    /**
     * @brief Actualiza la lógica de la entidad y sus componentes.
     *
     * Se llama una vez por fotograma (frame).
     * @param deltaTime Tiempo transcurrido (en segundos) desde el último frame.
     * @param deviceContext Referencia al contexto del dispositivo para operaciones gráficas o de recursos.
     */
    virtual void update(float deltaTime, DeviceContext& deviceContext) = 0;

    /**
     * @brief Renderiza la entidad y sus componentes visuales.
     *
     * Se invoca durante la fase de dibujo del bucle principal.
     * @param deviceContext Contexto del dispositivo para emitir comandos de renderizado.
     */
    virtual void render(DeviceContext& deviceContext) = 0;

    /**
     * @brief Destruye la entidad y libera sus recursos.
     *
     * Debe encargarse de limpiar memoria, desconectar la entidad del grafo de escena y liberar punteros.
     */
    virtual void destroy() = 0;

    /**
     * @brief Agrega un nuevo componente a la entidad.
     *
     * El componente pasa a ser gestionado por esta entidad mediante un puntero compartido.
     *
     * @tparam T Tipo del componente a agregar. Debe heredar de la clase base @ref Component.
     * @param component Puntero compartido al componente que se va a adjuntar.
     */
    template <typename T>
    void addComponent(EU::TSharedPointer<T> component) {
        static_assert(std::is_base_of<Component, T>::value, "El tipo T debe derivar de Component");
        m_components.push_back(component.template dynamic_pointer_cast<Component>());
    }

    /**
     * @brief Busca y obtiene un componente específico asociado a esta entidad.
     *
     * Realiza una búsqueda lineal en la lista de componentes y devuelve el primero que coincida con el tipo solicitado.
     *
     * @warning Esta operación es O(N). Se recomienda cachear el resultado en `init()` en lugar de llamarlo cada frame.
     *
     * @tparam T Tipo del componente a obtener.
     * @return EU::TSharedPointer<T> Puntero compartido al componente si se encuentra; de lo contrario, un puntero vacío.
     */
    template<typename T>
    EU::TSharedPointer<T> getComponent() {
        for (auto& component : m_components) {
            EU::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
            if (specificComponent) {
                return specificComponent;
            }
        }
        return EU::TSharedPointer<T>();
    }

protected:
    /** * @brief Indica si la entidad está activa en la escena.
     * Si es `false`, la entidad generalmente se ignora en los ciclos de Update y Render.
     */
    bool m_isActive;

    /** * @brief Identificador único de la entidad.
     * Utilizado para búsquedas rápidas, serialización o depuración.
     */
    int m_id;

    /** * @brief Colección de componentes adjuntos a esta entidad.
     * Se utilizan punteros compartidos para asegurar la correcta gestión de la memoria.
     */
    std::vector<EU::TSharedPointer<Component>> m_components;
};