#pragma once
#include "Prerequisites.h"
#include "Entity.h"
#include "Buffer.h"
#include "Texture.h"
#include "Transform.h"
#include "SamplerState.h"
#include "ShaderProgram.h"

// Pre-declaraciones para evitar dependencias circulares y acelerar compilación
class Device;
class DeviceContext;
class MeshComponent;

/**
 * @class Actor
 * @brief Representa una entidad gráfica renderizable en la escena.
 *
 * La clase Actor hereda de @ref Entity y actúa como el objeto principal para elementos visibles
 * en el juego. Agrupa la geometría (Mallas), la apariencia (Texturas/Shaders) y los estados
 * necesarios para el pipeline de renderizado (Buffers, Samplers).
 *
 * Además de renderizarse a sí mismo, maneja la lógica para proyectar sombras (Shadow Mapping).
 */
class Actor : public Entity {
public:
    /**
     * @brief Constructor por defecto.
     * Crea un actor vacío sin recursos inicializados.
     */
    Actor() = default;

    /**
     * @brief Constructor de inicialización.
     * @param device Referencia al dispositivo gráfico (Device) para inicializar recursos internos.
     */
    Actor(Device& device);

    /**
     * @brief Destructor virtual.
     * Garantiza la correcta limpieza de recursos en clases derivadas.
     */
    virtual ~Actor() = default;

    /**
     * @brief Método de despertar.
     * Se llama inmediatamente después de la construcción del objeto, antes de cualquier actualización.
     */
    void awake() override {}

    /**
     * @brief Inicializa el actor.
     * Configura estados iniciales o recursos que requieren que el objeto esté completamente construido.
     */
    void init() override {}

    /**
     * @brief Actualiza la lógica del actor en cada fotograma.
     *
     * Se encarga de actualizar transformaciones, animaciones o lógica de juego antes del renderizado.
     *
     * @param deltaTime Tiempo transcurrido (en segundos) desde el último frame.
     * @param deviceContext Contexto del dispositivo para actualizar subrecursos o buffers.
     */
    void update(float deltaTime, DeviceContext& deviceContext) override;

    /**
     * @brief Renderiza el actor en la escena principal.
     *
     * Configura el Input Layout, Vertex/Index Buffers, Shaders y Samplers, y emite
     * las llamadas de dibujo (Draw Calls) correspondientes.
     *
     * @param deviceContext Contexto del dispositivo para enviar comandos a la GPU.
     */
    void render(DeviceContext& deviceContext) override;

    /**
     * @brief Libera los recursos de memoria y GPU asociados al actor.
     * Debe llamarse antes de eliminar el objeto para evitar fugas de memoria en VRAM.
     */
    void destroy();

    /**
     * @brief Asigna la geometría al actor.
     *
     * Toma una lista de componentes de malla y genera los Vertex Buffers e Index Buffers
     * necesarios en la GPU.
     *
     * @param device Dispositivo gráfico encargado de crear los buffers.
     * @param meshes Vector de componentes de malla (@ref MeshComponent) que conforman el objeto.
     */
    void setMesh(Device& device, std::vector<MeshComponent> meshes);

    /**
     * @brief Obtiene el nombre identificador del actor.
     * @return Cadena de texto con el nombre actual.
     */
    std::string getName() { return m_name; }

    /**
     * @brief Establece un nuevo nombre para el actor.
     * @param name Nuevo nombre a asignar.
     */
    void setName(const std::string& name) { m_name = name; }

    /**
     * @brief Asigna las texturas materiales al actor.
     * @param textures Vector de texturas (@ref Texture) a utilizar en el shader.
     */
    void setTextures(std::vector<Texture> textures) { m_textures = textures; }

    /**
     * @brief Habilita o deshabilita la proyección de sombras para este actor.
     * @param v `true` para proyectar sombras, `false` para ignorarlas.
     */
    void setCastShadow(bool v) { castShadow = v; }

    /**
     * @brief Consulta si el actor está configurado para proyectar sombras.
     * @return `true` si proyecta sombras, `false` en caso contrario.
     */
    bool canCastShadow() const { return castShadow; }

    /**
     * @brief Renderiza el actor en el mapa de sombras (Shadow Map).
     *
     * Utiliza un shader simplificado (generalmente solo de vértices) para registrar
     * la profundidad del objeto desde la perspectiva de la luz.
     *
     * @param deviceContext Contexto del dispositivo para las operaciones de dibujo.
     */
    void renderShadow(DeviceContext& deviceContext);

private:
    /** @brief Lista de componentes de malla que definen la forma del actor. */
    std::vector<MeshComponent> m_meshes;

    /** @brief Lista de texturas aplicadas al modelo. */
    std::vector<Texture> m_textures;

    /** @brief Buffers de vértices en GPU para cada malla. */
    std::vector<Buffer> m_vertexBuffers;

    /** @brief Buffers de índices en GPU para cada malla. */
    std::vector<Buffer> m_indexBuffers;

    // Recursos de Estado (Comentados o activos según implementación)
    // BlendState m_blendstate;    ///< Estado de mezcla de colores.
    // Rasterizer m_rasterizer;    ///< Estado de rasterización (Cull mode, Fill mode).

    /** @brief Estado de muestreo para las texturas (Filtros, Address Mode). */
    SamplerState m_sampler;

    /** @brief Estructura de datos para el Constant Buffer de transformaciones (World, View, Projection). */
    CBChangesEveryFrame m_model;

    /** @brief Buffer constante en GPU que almacena la estructura @c m_model. */
    Buffer m_modelBuffer;

    // --- Recursos para Sombras ---

    /** @brief Programa de shader específico para el pase de sombras. */
    ShaderProgram m_shaderShadow;

    /** @brief Buffer constante auxiliar para el shader de sombras. */
    Buffer m_shaderBuffer;

    // BlendState m_shadowBlendState;            ///< Blend state para sombras.
    // DepthStencilState m_shadowDepthStencilState; ///< Depth state para sombras.

    /** @brief Estructura de datos para el Constant Buffer durante el pase de sombras. */
    CBChangesEveryFrame m_cbShadow;

    /** @brief Posición de la luz, necesaria para cálculos de iluminación/sombra. */
    XMFLOAT4 m_LightPos;

    /** @brief Nombre del actor para identificación y depuración. Por defecto "Actor". */
    std::string m_name = "Actor";

    /** @brief Bandera que determina si el objeto debe renderizarse en el Shadow Map. */
    bool castShadow = true;
};