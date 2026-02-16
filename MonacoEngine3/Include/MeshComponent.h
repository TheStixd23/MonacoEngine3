#pragma once

#include "Prerequisites.h"
#include "ECS/Component.h"
#include <vector>
#include <string>

class DeviceContext;


// =================================================================================
// COMPONENTE: MESH (Geometría)
// =================================================================================

/**
 * @class MeshComponent
 * @brief Componente ECS que almacena la información de geometría (malla) de un actor.
 *
 * Un MeshComponent contiene los buffers de CPU (vértices e índices) que describen la forma del objeto.
 * Este componente es pasivo (datos); el sistema de renderizado es quien lo lee para dibujar.
 */
class MeshComponent : public Component {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Constructor por defecto.
     * Inicializa el componente y lo registra como tipo MESH.
     */
    MeshComponent() :
        Component(ComponentType::MESH),
        m_numVertex(0),
        m_numIndex(0)
    {
    }


    /** * @brief Destructor virtual por defecto.
     */
    virtual
        ~MeshComponent() = default;


    // -----------------------------------------------------------------------------
    // CICLO DE VIDA (COMPONENT LIFE CYCLE)
    // -----------------------------------------------------------------------------

    /**
     * @brief Inicializa el componente.
     */
    void
        init() override
    {
    }


    /**
     * @brief Actualiza la malla (si es dinámica).
     * @param deltaTime Tiempo transcurrido desde la última actualización.
     */
    void
        update(float deltaTime) override
    {
    }


    /**
     * @brief Renderiza la malla.
     * @param deviceContext Contexto del dispositivo para operaciones gráficas.
     */
    void
        render(DeviceContext& deviceContext) override
    {
    }


    /**
     * @brief Libera los recursos asociados (Buffers de CPU/GPU).
     */
    void
        destroy() override
    {
        m_vertex.clear();
        m_index.clear();
        m_numVertex = 0;
        m_numIndex = 0;
    }


public:

    // -----------------------------------------------------------------------------
    // DATOS DE LA MALLA
    // -----------------------------------------------------------------------------

    /** * @brief Nombre identificador de la malla.
     */
    std::string m_name;


    /** * @brief Lista de vértices (Posición, Normales, UVs, Color).
     */
    std::vector<SimpleVertex> m_vertex;


    /** * @brief Lista de índices que definen la topología (Triángulos).
     */
    std::vector<unsigned int> m_index;


    /** * @brief Caché del número total de vértices.
     */
    int m_numVertex;


    /** * @brief Caché del número total de índices.
     */
    int m_numIndex;

};