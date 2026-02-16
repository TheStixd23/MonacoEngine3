#pragma once

#include "Prerequisites.h"
#include "EngineUtilities/Vectors/Vector3.h"

// =================================================================================
// CLASE: CAMERA
// =================================================================================

/**
 * @class Camera
 * @brief Cámara en primera persona o de tipo "LookAt" para entornos 3D.
 *
 * Esta clase encapsula la lógica matemática para generar las matrices de Vista (View)
 * y Proyección (Projection) necesarias para el pipeline de renderizado.
 *
 * Utiliza un sistema de coordenadas de Mano Izquierda (Left-Handed o LH), estándar
 * en DirectX. Gestiona la posición, orientación (vectores base) y el frustum.
 */
class Camera {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /** * @brief Constructor por defecto.
     */
    Camera();


    /** * @brief Destructor por defecto.
     */
    ~Camera() = default;


    // -----------------------------------------------------------------------------
    // CONFIGURACIÓN DE LA LENTE (PROJECTION)
    // -----------------------------------------------------------------------------

    /**
     * @brief Configura la matriz de proyección en perspectiva (Left-Handed).
     *
     * Define el volumen de visualización (frustum) de la cámara.
     * @param fovYRadians Campo de visión vertical (Field of View) en radianes.
     * @param aspectRatio Relación de aspecto (ancho / alto) de la ventana.
     * @param nearPlane Distancia al plano de corte cercano (Near Z).
     * @param farPlane Distancia al plano de corte lejano (Far Z).
     * @note Debe llamarse al inicializar la ventana y en cada evento `OnResize`.
     */
    void
        setLens(float fovYRadians, float aspectRatio, float nearPlane, float farPlane);


    // -----------------------------------------------------------------------------
    // CONTROL DE TRANSFORMACIÓN (WORLD SPACE)
    // -----------------------------------------------------------------------------

    /**
     * @brief Establece la posición de la cámara usando coordenadas individuales.
     * @param x Coordenada X.
     * @param y Coordenada Y.
     * @param z Coordenada Z.
     */
    void
        setPosition(float x, float y, float z);


    /**
     * @brief Establece la posición de la cámara usando un vector.
     * @param pos Vector con la nueva posición.
     */
    void
        setPosition(const EU::Vector3& pos);


    /**
     * @brief Obtiene la posición actual de la cámara en el espacio de mundo.
     * @return Copia del vector de posición.
     */
    [[nodiscard]]
    EU::Vector3
        getPosition() const
    {
        return m_position;
    }


    /**
     * @brief Reorienta la cámara para mirar hacia un objetivo específico.
     *
     * Recalcula los vectores base (Right, Up, Forward) para que la cámara apunte
     * desde su posición actual hacia el `target`.
     * @param pos Nueva posición de la cámara.
     * @param target Punto objetivo al que mirar.
     * @param up Vector "Arriba" del mundo (por defecto 0,1,0).
     */
    void
        lookAt(const EU::Vector3& pos, const EU::Vector3& target, const EU::Vector3& up);


    // -----------------------------------------------------------------------------
    // SISTEMA DE MOVIMIENTO (FPS STYLE)
    // -----------------------------------------------------------------------------

    /**
     * @brief Desplaza la cámara en la dirección de su vector "Forward".
     * @param d Distancia a mover. Positivo avanza, negativo retrocede.
     */
    void
        walk(float d);


    /**
     * @brief Desplaza la cámara en la dirección de su vector "Right".
     * @param d Distancia a mover. Positivo derecha, negativo izquierda.
     */
    void
        strafe(float d);


    /**
     * @brief Rota la cámara sobre el eje Y global (Giro horizontal).
     * Útil para cámaras tipo FPS donde el "suelo" siempre está abajo.
     * @param radians Ángulo de rotación en radianes.
     */
    void
        yaw(float radians);


    /**
     * @brief Rota la cámara sobre su eje "Right" local (Inclinación vertical).
     * Controla el ángulo de mirada hacia arriba o abajo.
     * @param radians Ángulo de rotación en radianes.
     */
    void
        pitch(float radians);


    // -----------------------------------------------------------------------------
    // PIPELINE GRÁFICO (MATRICES)
    // -----------------------------------------------------------------------------

    /**
     * @brief Reconstruye la matriz de Vista (View Matrix).
     *
     * Debe llamarse una vez por frame si la cámara se ha movido o rotado.
     * Orthonormaliza los vectores base y regenera la matriz World->View.
     */
    void
        updateViewMatrix();


    /**
     * @brief Obtiene la Matriz de Vista (World -> View).
     * @return Matriz XMMATRIX lista para shaders.
     */
    [[nodiscard]]
    XMMATRIX
        getView() const
    {
        return XMLoadFloat4x4(&m_view);
    }


    /**
     * @brief Obtiene la Matriz de Proyección (View -> Clip/Screen).
     * @return Matriz XMMATRIX lista para shaders.
     */
    [[nodiscard]]
    XMMATRIX
        getProj() const
    {
        return XMLoadFloat4x4(&m_proj);
    }


    /**
     * @brief Obtiene una versión de la Matriz de Vista sin traslación.
     *
     * Ideal para renderizar Skyboxes, donde la geometría del cielo debe parecer
     * infinitamente lejana y moverse con la rotación pero no con el desplazamiento.
     */
    [[nodiscard]]
    XMMATRIX
        GetViewNoTranslation() const
    {
        XMMATRIX v = getView();
        // Anular la fila de traslación (fila 4 en memoria, r[3] en registros SIMD)
        v.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
        return v;
    }


    // -----------------------------------------------------------------------------
    // GETTERS: VECTORES BASE
    // -----------------------------------------------------------------------------

    [[nodiscard]] EU::Vector3 GetRight()   const { return m_right; }
    [[nodiscard]] EU::Vector3 GetUp()      const { return m_up; }
    [[nodiscard]] EU::Vector3 GetForward() const { return m_forward; }


    // -----------------------------------------------------------------------------
    // GETTERS: PARÁMETROS DE PROYECCIÓN
    // -----------------------------------------------------------------------------

    [[nodiscard]] float getFovY()   const { return m_fovY; }
    [[nodiscard]] float getAspect() const { return m_aspectRatio; }
    [[nodiscard]] float getNearZ()  const { return m_nearPlane; }
    [[nodiscard]] float getFarZ()   const { return m_farPlane; }


    // -----------------------------------------------------------------------------
    // UTILIDADES
    // -----------------------------------------------------------------------------

    /**
     * @brief Conversor auxiliar de vector SIMD (DirectX) a Vector3 (Motor).
     */
    inline EU::Vector3
        FromXM(FXMVECTOR v)
    {
        XMFLOAT3 t;
        XMStoreFloat3(&t, v);
        return EU::Vector3(t.x, t.y, t.z);
    }


private:

    // -----------------------------------------------------------------------------
    // DATOS MIEMBRO PRIVADOS
    // -----------------------------------------------------------------------------

    /** @brief Posición de la cámara en el espacio de mundo. */
    EU::Vector3 m_position;

    // --- Base Ortonormal (Espacio de Mundo) ---

    EU::Vector3 m_right{ 1.0f, 0.0f, 0.0f }; /**< Eje X local. */
    EU::Vector3 m_up{ 0.0f, 1.0f, 0.0f }; /**< Eje Y local. */
    EU::Vector3 m_forward{ 0.0f, 0.0f, 1.0f }; /**< Eje Z local (Mirada). */

    // --- Matrices Cacheadas ---

    XMFLOAT4X4 m_view{};  ///< Matriz de Vista almacenada.
    XMFLOAT4X4 m_proj{};  ///< Matriz de Proyección almacenada.

    // --- Parámetros de la Proyección ---

    float m_fovY{ XM_PIDIV4 };  ///< FOV Vertical (45 grados por defecto).
    float m_aspectRatio{ 1.0f };       ///< Relación de aspecto.
    float m_nearPlane{ 0.01f };      ///< Plano cercano.
    float m_farPlane{ 1000.0f };    ///< Plano lejano.

    // --- Estado ---

    /** @brief Bandera para indicar si la matriz de vista necesita recalculo. */
    bool m_viewDirty = true;
};