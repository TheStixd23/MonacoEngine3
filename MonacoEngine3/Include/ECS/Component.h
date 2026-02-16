#pragma once
#include "Prerequisites.h"

class DeviceContext;

/**
 * @class Component
 * @brief Clase base abstracta que define la interfaz para todos los componentes de una entidad.
 *
 * En la arquitectura ECS (Entity-Component-System) o similar, esta clase actúa como el contrato
 * que deben cumplir todos los componentes adjuntos a un @ref Actor o Entidad.
 * Proporciona el ciclo de vida básico: inicialización, actualización, renderizado y destrucción.
 */
class Component {
public:
    /**
     * @brief Constructor por defecto.
     * Inicializa un componente sin tipo específico.
     */
    Component() = default;

    /**
     * @brief Constructor de inicialización con tipo.
     * @param type Enumerador @ref ComponentType que identifica la naturaleza del componente (ej. Mesh, Transform).
     */
    Component(const ComponentType type) : m_type(type) {}

    /**
     * @brief Destructor virtual.
     * Necesario para asegurar que los destructores de las clases derivadas se invoquen correctamente.
     */
    virtual ~Component() = default;

    /**
     * @brief Inicializa el componente.
     * * Método virtual puro que debe ser implementado por las clases derivadas para configurar
     * estados iniciales o recursos dependientes después de la construcción.
     */
    virtual void init() = 0;

    /**
     * @brief Actualiza la lógica del componente.
     * * Se llama una vez por fotograma. Implementa el comportamiento dinámico del componente.
     * @param deltaTime Tiempo transcurrido (en segundos) desde el último frame.
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Renderiza el componente.
     * * Se llama durante la fase de dibujo. Permite al componente enviar comandos al pipeline gráfico.
     * @param deviceContext Contexto del dispositivo gráfico para realizar operaciones de dibujo.
     */
    virtual void render(DeviceContext& deviceContext) = 0;

    /**
     * @brief Libera los recursos del componente.
     * * Método virtual puro para limpiar memoria, buffers o referencias antes de la destrucción del objeto.
     */
    virtual void destroy() = 0;

    /**
     * @brief Obtiene el identificador de tipo del componente.
     * @return Valor del enumerado @ref ComponentType que corresponde a este componente.
     */
    ComponentType getType() const { return m_type; }

protected:
    /** @brief Identificador del tipo de componente, usado para casting seguro y lógica específica. */
    ComponentType m_type;
};