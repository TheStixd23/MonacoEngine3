#pragma once
#include "Prerequisites.h"
#include "ECS/Component.h"

class DeviceContext;
class Entity;

/**
 * @class HierarchyComponent
 * @brief Componente que gestiona la jerarquía y relaciones parentales de una Entidad.
 *
 * Este componente permite construir un Grafo de Escena (Scene Graph). Convierte a la entidad
 * en un nodo que puede tener un padre y múltiples hijos.
 * Es fundamental para sistemas de transformación relativa (mover al padre mueve a los hijos)
 * y organización lógica de la escena.
 */
class HierarchyComponent : public Component {
public:
    /**
     * @brief Constructor por defecto.
     * Asigna el tipo de componente como HIERARCHY.
     */
    HierarchyComponent() : Component(ComponentType::HIERARCHY) {}

    /** @brief Destructor por defecto. */
    ~HierarchyComponent() = default;

    /** @brief Inicialización del componente (sin implementación específica). */
    void init() override {}

    /** @brief Actualización por frame (sin lógica por frame requerida). */
    void update(float) override {}

    /** @brief Renderizado (este componente es lógico, no visual). */
    void render(DeviceContext& deviceContext) override {}

    /**
     * @brief Limpia las relaciones jerárquicas.
     * Desvincula al padre y limpia la lista de hijos para evitar referencias colgantes al destruir la entidad.
     */
    void destroy() override {
        m_children.clear();
        m_parent = nullptr;
    }

    // --- API SceneGraph ---

    /**
     * @brief Asigna un padre a esta entidad.
     * @param parent Puntero a la entidad que actuará como padre.
     */
    void setParent(Entity* parent) {
        m_parent = parent;
    }

    /**
     * @brief Comprueba si esta entidad es una raíz (no tiene padre).
     * @return `true` si no tiene padre asignado, `false` en caso contrario.
     */
    bool isRoot() const {
        return m_parent == nullptr;
    }

    /**
     * @brief Comprueba si esta entidad tiene hijos asociados.
     * @return `true` si la lista de hijos no está vacía.
     */
    bool hasChildren() const {
        return !m_children.empty();
    }

    /**
     * @brief Añade una entidad como hija de esta.
     *
     * Realiza una comprobación para evitar añadir hijos duplicados o nulos.
     * @param child Puntero a la entidad hija a agregar.
     */
    void addChild(Entity* child) {
        if (!child) {
            return;
        }

        // Evitar duplicados
        if (std::find(m_children.begin(), m_children.end(), child) != m_children.end()) {
            return;
        }
        m_children.push_back(child);
    }

    /**
     * @brief Elimina una entidad específica de la lista de hijos.
     *
     * Utiliza el idioma erase-remove para eliminar eficientemente el puntero del vector.
     * @param child Puntero a la entidad hija a remover.
     */
    void removeChild(Entity* child) {
        if (!child) return;

        m_children.erase(
            std::remove(m_children.begin(), m_children.end(), child),
            m_children.end()
        );
    }

public:
    /** * @brief Puntero a la entidad padre.
     * `nullptr` indica que esta entidad está en la raíz del mundo.
     */
    Entity* m_parent = nullptr;

    /** * @brief Lista de punteros a las entidades hijas.
     * Define la estructura descendente del árbol de la escena.
     */
    std::vector<Entity*> m_children;
};