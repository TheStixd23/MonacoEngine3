#pragma once
#include "Prerequisites.h"

class Entity;
class DeviceContext;

/**
 * @class SceneGraph
 * @brief Gestor de la estructura jerárquica de la escena (Árbol de Escena).
 *
 * El SceneGraph es responsable de administrar las relaciones padre-hijo entre entidades
 * y de propagar las transformaciones geométricas (posición, rotación, escala) desde
 * los nodos padres a los hijos.
 *
 * También gestiona el ciclo de vida de actualización y renderizado de todas las entidades registradas.
 */
class SceneGraph {
public:
    /** @brief Constructor por defecto. */
    SceneGraph() = default;

    /** @brief Destructor por defecto. */
    ~SceneGraph() = default;

    /**
     * @brief Inicializa el grafo de escena.
     * Prepara las estructuras internas para comenzar a gestionar entidades.
     */
    void init();

    /**
     * @brief Registra una entidad en el grafo.
     *
     * Añade la entidad a la lista de gestión del grafo. Es necesario registrar una entidad
     * para que sea actualizada y renderizada por el motor.
     * @param e Puntero a la entidad a registrar.
     */
    void addEntity(Entity* e);

    /**
     * @brief Elimina una entidad del grafo.
     *
     * Deja de gestionar la entidad y, si tenía hijos, estos pueden quedar huérfanos o desvincularse
     * dependiendo de la implementación interna.
     * @param e Puntero a la entidad a remover.
     */
    void removeEntity(Entity* e);

    /**
     * @brief Verifica la ascendencia para prevenir ciclos.
     *
     * Comprueba si `possibleAncestor` es realmente un ancestro (padre, abuelo, etc.) de `node`.
     * Útil para evitar que un objeto sea padre de su propio abuelo (referencia circular).
     *
     * @param possibleAncestor La entidad que sospechamos es el ancestro.
     * @param node La entidad base desde la que buscamos hacia arriba.
     * @return `true` si existe relación de ascendencia, `false` en caso contrario.
     */
    bool isAncestor(Entity* possibleAncestor, Entity* node) const;

    /**
     * @brief Vincula una entidad hija a un padre.
     *
     * Establece la relación jerárquica. A partir de este momento, las transformaciones
     * del padre afectarán a la posición/rotación/escala local del hijo.
     *
     * @param child Entidad que será subordinada.
     * @param parent Entidad que actuará como contenedor/padre.
     * @return `true` si la operación fue exitosa (sin ciclos), `false` si falló.
     */
    bool attach(Entity* child, Entity* parent);

    /**
     * @brief Desvincula una entidad de su padre.
     *
     * La entidad `child` pasa a ser un nodo raíz en el grafo (su transformación se vuelve global).
     * @param child Entidad a desvincular.
     * @return `true` si se pudo desvincular, `false` si no tenía padre.
     */
    bool detach(Entity* child);

    /**
     * @brief Actualiza la lógica y las transformaciones de todas las entidades.
     *
     * Inicia el recorrido recursivo para recalcular las matrices de mundo (World Matrices)
     * asegurando que los padres se actualicen antes que sus hijos.
     *
     * @param deltaTime Tiempo transcurrido desde el último frame.
     * @param deviceContext Contexto del dispositivo (si es necesario para actualizaciones de recursos).
     */
    void update(float deltaTime, DeviceContext& deviceContext);

    /**
     * @brief Renderiza todas las entidades del grafo.
     * @param deviceContext Contexto gráfico para emitir comandos de dibujo.
     */
    void render(DeviceContext& deviceContext);

    /**
     * @brief Limpia el grafo y libera recursos.
     */
    void destroy();

private:
    /**
     * @brief Método interno recursivo para actualizar transformaciones.
     *
     * Multiplica la matriz local de la entidad por la matriz de mundo acumulada del padre
     * para obtener la posición real absoluta en la escena.
     *
     * @param node Entidad actual a actualizar.
     * @param parentWorld Matriz de transformación acumulada del padre.
     */
    void updateWorldRecursive(Entity* node, const XMMATRIX& parentWorld);

    /**
     * @brief Comprueba si una entidad es un nodo raíz (sin padre).
     * @param e Entidad a verificar.
     * @return `true` si es raíz.
     */
    bool isRoot(Entity* e) const;

    /**
     * @brief Verifica si una entidad ya está registrada en la lista interna.
     * @param e Entidad a buscar.
     */
    bool isRegistered(Entity* e) const;

private:
    // std::vector<EU::TSharedPointer<Entity>> m_entities; // Referencia antigua con Smart Pointers

public:
    /** @brief Lista plana de todas las entidades gestionadas por este grafo. */
    std::vector<Entity*> m_entities;
};