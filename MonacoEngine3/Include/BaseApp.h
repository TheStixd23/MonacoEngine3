#pragma once
#include "Prerequisites.h"
#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "Texture.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Viewport.h"
#include "ShaderProgram.h"
#include "MeshComponent.h"
#include "Buffer.h"
#include "SamplerState.h"
#include "Model3D.h"
#include "ECS/Actor.h"
#include "GUI/GUI.h"
#include "SceneGraph/SceneGraph.h"
#include "EngineUtilities\Utilities\Camera.h"

/**
 * @brief Manejador externo de mensajes de Win32 para ImGui.
 *
 * Reenvía los eventos de Windows (ratón, teclado, foco) al contexto de ImGui
 * para permitir la interacción con la interfaz de usuario.
 *
 * @param hWnd Handle de la ventana.
 * @param msg Identificador del mensaje.
 * @param wParam Información adicional del mensaje (Word Param).
 * @param lParam Información adicional del mensaje (Long Param).
 * @return LRESULT Resultado del procesamiento del mensaje.
 */
extern IMGUI_IMPL_API
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**
 * @class BaseApp
 * @brief Clase base abstracta para aplicaciones DirectX 11.
 *
 * Encapsula el ciclo de vida de la aplicación. Gestiona el bucle principal (Main Loop),
 * el bombeo de mensajes del sistema operativo y la inicialización de los recursos
 * gráficos centrales (Dispositivo, Contexto, SwapChain, Vistas).
 */
class BaseApp {
public:
    /**
     * @brief Constructor por defecto.
     */
    BaseApp() = default;

    /**
     * @brief Destructor virtual.
     * Llama internamente a @ref destroy() para asegurar la limpieza de recursos.
     */
    ~BaseApp() { destroy(); }

    /**
     * @brief Lógica de pre-inicialización (Awake).
     *
     * Se ejecuta antes de crear la ventana o inicializar los gráficos.
     * Útil para configurar sistemas de log, cargar configuraciones o preparar datos.
     *
     * @return HRESULT S_OK si la operación fue exitosa.
     */
    HRESULT awake();

    /**
     * @brief Inicia el bucle principal de la aplicación.
     *
     * Inicializa la ventana y los subsistemas gráficos, y luego entra en el bucle
     * infinito de mensajes, llamando a @ref update() y @ref render() en cada frame.
     *
     * @param hInst Handle de la instancia de la aplicación.
     * @param nCmdShow Comando de visualización de la ventana (minimizado, maximizado, etc.).
     * @return int Código de salida de la aplicación.
     */
    int run(HINSTANCE hInst, int nCmdShow);

    /**
     * @brief Inicializa los subsistemas centrales de DirectX 11.
     *
     * Crea el Device, DeviceContext, SwapChain, RenderTargetView y DepthStencilView.
     * Configura el Viewport y carga los Shaders y Buffers básicos del pipeline.
     *
     * @return HRESULT S_OK si la inicialización gráfica es correcta.
     */
    HRESULT init();

    /**
     * @brief Actualización lógica por fotograma (Update).
     *
     * @param deltaTime Tiempo transcurrido en segundos desde el último frame.
     */
    void update(float deltaTime);

    /**
     * @brief Rutina de renderizado por fotograma (Render).
     *
     * Limpia los buffers (Color y Profundidad), dibuja la escena y presenta
     * el resultado en pantalla (Swap Chain Present).
     */
    void render();

    /**
     * @brief Renderiza la interfaz gráfica de usuario (ImGui).
     *
     * Configura el frame de ImGui y emite las llamadas de dibujo de la UI sobre la escena 3D.
     */
    void renderGUI();

    /**
     * @brief Libera todos los recursos de DirectX asignados.
     *
     * Se encarga de llamar a Release() en todas las interfaces COM y limpiar la memoria.
     */
    void destroy();

private:
    /**
     * @brief Procedimiento de Ventana Principal (Window Procedure).
     *
     * Procesa eventos de Win32 (redimensionado, cierre, entrada) y reenvía
     * los necesarios a ImGui o al motor.
     */
    static LRESULT CALLBACK
        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    /** @brief Envoltorio de la ventana principal de la aplicación. */
    Window                              m_window;

    /** @brief Interfaz del Dispositivo Direct3D 11 (Creación de recursos). */
    Device                              m_device;

    /** @brief Contexto Inmediato de Direct3D 11 (Comandos de renderizado). */
    DeviceContext                       m_deviceContext;

    /** @brief Cadena de intercambio DXGI para la presentación de imágenes. */
    SwapChain                           m_swapChain;

    /** @brief Textura que representa el Back Buffer. */
    Texture                             m_backBuffer;

    /** @brief Vista de Render Target para dibujar en el Back Buffer. */
    RenderTargetView                    m_renderTargetView;

    /** @brief Textura para el buffer de profundidad y estarcido (Depth-Stencil). */
    Texture                             m_depthStencil;

    /** @brief Vista del buffer de profundidad. */
    DepthStencilView                    m_depthStencilView;

    /** @brief Configuración del área de visualización (Viewport). */
    Viewport                            m_viewport;

    /** @brief Programa de Shader principal (Vertex & Pixel Shaders). */
    ShaderProgram                       m_shaderProgram;

    /** @brief Constant Buffer para datos estáticos (ej. Matriz de Vista si no cambia). */
    Buffer                              m_cbNeverChanges;

    /** @brief Constant Buffer para datos que cambian al redimensionar (ej. Proyección). */
    Buffer                              m_cbChangeOnResize;

    /** @brief Textura Albedo para el objeto de demostración. */
    Texture                             m_EspadaAlbedo;

    /** @brief Textura Cubemap para el cielo (Skybox). */
    Texture                             m_skyboxTex;

    /** @brief Cámara principal de la escena. */
    Camera                              m_camera;

    /** @brief Matriz de Vista almacenada (caché). */
    XMMATRIX                            m_View;

    /** @brief Matriz de Proyección almacenada (caché). */
    XMMATRIX                            m_Projection;

    /** @brief Grafo de Escena para gestionar la jerarquía de entidades. */
    SceneGraph                          m_sceneGraph;

    /** @brief Lista de actores/entidades en la escena. */
    std::vector<EU::TSharedPointer<Actor>> m_actors;

    /** @brief Puntero al actor principal de demostración. */
    EU::TSharedPointer<Actor> m_Espada;

    /** @brief Puntero al recurso del modelo 3D cargado. */
    Model3D* m_model;

    /** @brief Estructura en CPU para actualizar el buffer de redimensionado. */
    CBChangeOnResize                    cbChangesOnResize;

    /** @brief Estructura en CPU para actualizar el buffer estático. */
    CBNeverChanges                      cbNeverChanges;

    /** @brief Gestor de la interfaz ImGui. */
    GUI                                 m_gui;
};