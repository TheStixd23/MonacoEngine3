#pragma once
#include "Prerequisites.h"
#include <vector>

// Forward declarations para reducir dependencias
class Entity;
class DeviceContext;

/**
 * @class SceneGraph
 * @brief Sistema encargado de gestionar la jerarquía y el ciclo de vida de las entidades activas.
 * * Este gestor actúa como el contenedor principal del mundo. Se encarga de propagar las
 * transformaciones (World Matrices) de padres a hijos y de despachar los eventos de
 * actualización (Update) y renderizado (Render) a todas las entidades registradas.
 */
class SceneGraph {
public:

    /**
     * @brief Constructor por defecto.
     */
    SceneGraph() = default;

    /**
     * @brief Destructor por defecto.
     */
    ~SceneGraph() = default;

    /**
     * @brief Inicializa los recursos internos del grafo de escena.
     */
    void
        init();

    /**
     * @brief Registra una nueva entidad en el grafo para su gestión.
     * @param e Puntero a la entidad a registrar.
     */
    void
        addEntity(Entity* e);

    /**
     * @brief Elimina una entidad del grafo y detiene su gestión.
     * @param e Puntero a la entidad a remover.
     */
    void
        removeEntity(Entity* e);

    /**
     * @brief Verifica si una entidad es ancestro (padre, abuelo, etc.) de otra.
     * Útil para evitar ciclos en la jerarquía (ej: que el padre sea hijo de su propio hijo).
     * @param possibleAncestor La entidad que sospechamos es el ancestro.
     * @param node La entidad descendiente.
     * @return true si existe relación de ancestro.
     */
    bool
        isAncestor(Entity* possibleAncestor, Entity* node) const;

    /**
     * @brief Vincula una entidad hija a un padre en la jerarquía.
     * @param child Entidad que será subordinada.
     * @param parent Entidad que actuará como padre.
     * @return true si la operación fue exitosa (sin ciclos ni errores).
     */
    bool
        attach(Entity* child, Entity* parent);

    /**
     * @brief Desvincula una entidad de su padre, dejándola en la raíz del grafo.
     * @param child Entidad a desvincular.
     * @return true si la entidad tenía padre y fue desvinculada.
     */
    bool
        detach(Entity* child);

    /**
     * @brief Actualiza la lógica de todas las entidades y recalcula las matrices de mundo.
     * @param deltaTime Tiempo transcurrido desde el último frame.
     * @param deviceContext Contexto del dispositivo (si es necesario para la lógica).
     */
    void
        update(float deltaTime, DeviceContext& deviceContext);

    /**
     * @brief Renderiza todas las entidades visibles gestionadas por el grafo.
     * @param deviceContext Contexto gráfico necesario para dibujar.
     */
    void
        render(DeviceContext& deviceContext);

    /**
     * @brief Destruye el grafo, liberando todas las entidades y recursos asociados.
     */
    void
        destroy();

private:

    /**
     * @brief Método interno para recorrer y actualizar las matrices de transformación recursivamente.
     * Multiplica la matriz local de la entidad por la matriz acumulada del padre.
     * @param node Entidad actual a procesar.
     * @param parentWorld Matriz de transformación del padre (World Matrix).
     */
    void
        updateWorldRecursive(Entity* node, const XMMATRIX& parentWorld);

    /**
     * @brief Comprueba si una entidad está en la raíz (sin padre).
     */
    bool
        isRoot(Entity* e) const;

    /**
     * @brief Verifica si una entidad ya está registrada en la lista interna.
     */
    bool
        isRegistered(Entity* e) const;

public:

    /**
     * @brief Lista lineal de todas las entidades gestionadas.
     * @note Se mantiene pública por requerimientos de acceso rápido, pero debe manipularse con cuidado.
     */
    std::vector<Entity*> m_entities;
};