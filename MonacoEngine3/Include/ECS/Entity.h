#pragma once
#include "Prerequisites.h"
#include "Component.h"
#include <vector> // Asegúrate de incluir vector si no está en Prerequisites

class DeviceContext;

/**
 * @class Entity
 * @brief Clase base abstracta que representa un objeto dentro del mundo del juego.
 * * La entidad funciona como un contenedor de componentes. Gestiona el ciclo de vida
 * (awake, init, update, render, destroy) y la propiedad de sus componentes adjuntos.
 */
class Entity {
public:
    /**
     * @brief Constructor por defecto.
     */
    Entity() = default;

    /**
     * @brief Destructor virtual.
     * Garantiza que los destructores de las clases derivadas se llamen correctamente.
     */
    virtual ~Entity() = default;

    /**
     * @brief Fase de "Despertar" de la entidad.
     * * Se ejecuta antes de init(). Ideal para inicializar variables internas o estados
     * que no dependen de referencias externas o de otros componentes.
     */
    virtual void 
    awake() = 0;

    /**
     * @brief Inicializa la entidad con un contexto de dispositivo.
     * * Se debe llamar después de que la entidad haya sido creada y despertada (awake).
     * Aquí se deben cargar recursos o establecer referencias dependientes del contexto.
     * * @param deviceContext El contexto del dispositivo necesario para la inicialización gráfica/lógica.
     */
    virtual void 
    init() = 0; // NOTA: Tu doc original decía @return bool, pero la firma es void. Lo he corregido.

    /**
     * @brief Actualiza la lógica de la entidad y sus componentes.
     * * Se llama una vez por frame.
     * * @param deltaTime El tiempo transcurrido (en segundos) desde el último frame.
     * @param deviceContext Referencia al contexto del dispositivo actual.
     */
    virtual void 
    update(float deltaTime, DeviceContext& deviceContext) = 0;

    /**
     * @brief Renderiza la entidad y sus componentes visuales.
     * * Se llama en la fase de dibujado del loop principal.
     * * @param deviceContext Contexto del dispositivo para realizar las operaciones gráficas.
     */
    virtual void 
    render(DeviceContext& deviceContext) = 0;

    /**
     * @brief Destruye la entidad y libera recursos.
     * * Debe encargarse de limpiar la memoria, liberar punteros y desconectar la entidad del mundo.
     */
    virtual void 
    destroy() = 0;

    /**
     * @brief Agrega un nuevo componente a la entidad.
     * * El componente pasa a ser gestionado por esta entidad.
     * * @tparam T Tipo del componente, debe derivar de la clase base Component.
     * @param component Puntero compartido al componente que se va a agregar.
     */
    template <typename T> void 
    addComponent(EU::TSharedPointer<T> component) {
        static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
        m_components.push_back(component.template dynamic_pointer_cast<Component>());
    }

    /**
     * @brief Busca y obtiene un componente específico asociado a esta entidad.
     * * Realiza una búsqueda lineal en la lista de componentes.
     * * @warning Esta operación es O(N). Evita llamarla en cada frame dentro de un bucle crítico.
     * * @tparam T Tipo del componente a obtener.
     * @return EU::TSharedPointer<T> Puntero compartido al componente si se encuentra, o un puntero vacío si no existe.
     */
    template<typename T>
    EU::TSharedPointer<T> 
    getComponent() {
        for (auto& component : m_components) {
            EU::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
            if (specificComponent) {
                return specificComponent;
            }
        }
        return EU::TSharedPointer<T>();
    }

private:
    // Sección privada reservada para implementación interna si fuera necesaria.

protected:
    /** * @brief Indica si la entidad está activa en la escena.
     * Si es false, generalmente la entidad no debería actualizarse ni renderizarse.
     */
    bool m_isActive;

    /** * @brief Identificador único de la entidad.
     * Útil para búsquedas rápidas o serialización.
     */
    int m_id;

    /** * @brief Lista de componentes adjuntos a esta entidad.
     * Almacenados como punteros compartidos para gestión automática de memoria.
     */
    std::vector<EU::TSharedPointer<Component>> m_components;
};
