#pragma once
#include "Prerequisites.h"
#include "EngineUtilities/Vectors/Vector3.h"
#include "Component.h"

/**
 * @class Transform
 * @brief Componente que gestiona la posición, rotación y escala de una entidad en el espacio 3D.
 *
 * El componente Transform es el núcleo de la manipulación espacial. Almacena los vectores
 * de transformación local y es responsable de calcular la **Matriz de Mundo (World Matrix)**
 * que se envía a la GPU para renderizar el objeto en la posición correcta.
 */
class Transform : public Component {
public:
    /**
     * @brief Constructor por defecto.
     * Inicializa un Transform con posición (0,0,0), rotación (0,0,0) y escala predeterminada.
     */
    Transform() : position(),
        rotation(),
        scale(),
        matrix(),
        Component(ComponentType::TRANSFORM) {
    }

    /**
     * @brief Inicializa el componente.
     * Establece la escala inicial en (1,1,1) y resetea la matriz a la identidad.
     */
    void init() override {
        scale.one();
        matrix = XMMatrixIdentity();
    }

    /**
     * @brief Calcula y actualiza la matriz de transformación.
     *
     * Construye la matriz de mundo combinando las transformaciones en el orden estándar:
     * **Escala (S) -> Rotación (R) -> Traslación (T)**.
     *
     * @note Se realiza la rotación eje por eje (X * Y * Z) para tener un control explícito
     * sobre el orden de Euler y minimizar problemas de Gimbal Lock visuales.
     *
     * @param deltaTime Tiempo transcurrido (no utilizado directamente en el cálculo de la matriz estática).
     */
    void update(float deltaTime) override {
        // 1. Escala
        XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

        // 2. Rotación (Manual por ejes para compatibilidad con ImGuizmo)
        // Usamos X * Y * Z (o el orden que prefieras, pero separado) para evitar
        // el comportamiento predeterminado de RollPitchYaw que puede causar gimbal lock visual.
        XMMATRIX rotX = XMMatrixRotationX(rotation.x);
        XMMATRIX rotY = XMMatrixRotationY(rotation.y);
        XMMATRIX rotZ = XMMatrixRotationZ(rotation.z);
        XMMATRIX rotationMatrix = rotX * rotY * rotZ;

        // 3. Traslación
        XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);

        // Componer: Scale -> Rotation -> Translation
        matrix = scaleMatrix * rotationMatrix * translationMatrix;
    }

    /** @brief Método de renderizado (sin uso en Transform, solo datos lógicos). */
    void render(DeviceContext& deviceContext) override {}

    /** @brief Método de destrucción para limpieza de recursos. */
    void destroy() override {}

    /**
     * @brief Obtiene la posición local.
     * @return Referencia constante al vector de posición.
     */
    const EU::Vector3& getPosition() const { return position; }

    /**
     * @brief Establece la posición local.
     * @param newPos Nuevo vector de coordenadas (X, Y, Z).
     */
    void setPosition(const EU::Vector3& newPos) { position = newPos; }

    /**
     * @brief Obtiene la rotación local actual.
     * @return Vector con los ángulos de Euler en **Radianes**.
     */
    const EU::Vector3& getRotation() const { return rotation; }

    /**
     * @brief Establece la rotación local.
     * @param newRot Nuevo vector de rotación (X, Y, Z) en **Radianes**.
     */
    void setRotation(const EU::Vector3& newRot) { rotation = newRot; }

    /**
     * @brief Obtiene la escala local.
     * @return Vector de escala.
     */
    const EU::Vector3& getScale() const { return scale; }

    /**
     * @brief Establece la escala local.
     * @param newScale Nuevo vector de escala (habitualmente 1,1,1 es el tamaño original).
     */
    void setScale(const EU::Vector3& newScale) { scale = newScale; }

    /**
     * @brief Asigna todos los valores de transformación simultáneamente.
     * Útil para reiniciar o copiar estados completos de transformación.
     * * @param newPos Nueva posición.
     * @param newRot Nueva rotación (en Radianes).
     * @param newSca Nueva escala.
     */
    void setTransform(const EU::Vector3& newPos,
        const EU::Vector3& newRot,
        const EU::Vector3& newSca) {
        position = newPos;
        rotation = newRot;
        scale = newSca;
    }

    /**
     * @brief Desplaza la entidad relativo a su posición actual.
     * @param translation Vector de desplazamiento a sumar a la posición actual.
     */
    void translate(const EU::Vector3& translation);

private:
    /** @brief Posición en el espacio 3D local. */
    EU::Vector3 position;

    /** @brief Rotación en ángulos de Euler, almacenada en **Radianes**. */
    EU::Vector3 rotation;

    /** @brief Escala del objeto en los ejes X, Y, Z. */
    EU::Vector3 scale;

public:
    /** * @brief Matriz de transformación compuesta (World Matrix).
     * Calculada automáticamente en update() combinando S*R*T.
     * Esta es la matriz que se envía al Vertex Shader.
     */
    XMMATRIX matrix;
};