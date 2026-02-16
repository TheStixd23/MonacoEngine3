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
#include "EngineUtilities/Utilities/Camera.h"


// =================================================================================
// EXTERNAL HANDLERS (ImGui)
// =================================================================================

/**
 * @brief Manejador externo de mensajes de Win32 para ImGui.
 * Reenvía los eventos del sistema operativo al contexto de ImGui.
 */
extern IMGUI_IMPL_API LRESULT
ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// =================================================================================
// CLASE PRINCIPAL: BASE APP
// =================================================================================

/**
 * @class BaseApp
 * @brief Clase base que encapsula el núcleo de una aplicación gráfica con DirectX 11.
 *
 * Administra el ciclo de vida completo: creación de ventana, inicialización de recursos,
 * bucle de renderizado y limpieza. Actúa como el punto de entrada para la lógica del motor.
 */
class BaseApp {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Constructor por defecto.
     */
    BaseApp() = default;


    /**
     * @brief Destructor virtual.
     * Llama a @ref destroy() para asegurar la liberación de recursos COM.
     */
    ~BaseApp()
    {
        destroy();
    }


    // -----------------------------------------------------------------------------
    // CICLO DE VIDA DE LA APLICACIÓN (App Lifecycle)
    // -----------------------------------------------------------------------------

    /**
     * @brief Lógica de pre-inicialización (Awake).
     * Se ejecuta una sola vez antes de crear la ventana. Ideal para logs o configs.
     * @return HRESULT S_OK si la operación fue exitosa.
     */
    HRESULT
        awake();


    /**
     * @brief Inicializa los subsistemas centrales de DirectX 11.
     * Crea Device, Context, SwapChain, RTV, DSV, Viewport y Shaders básicos.
     * @return HRESULT S_OK si todos los recursos gráficos se inicializaron correctamente.
     */
    HRESULT
        init();


    /**
     * @brief Inicia el bucle principal de la aplicación.
     * Contiene el bucle infinito de mensajes de Windows (Update/Render loop).
     * @param hInst Handle de la instancia de la aplicación.
     * @param nCmdShow Comando para mostrar la ventana.
     * @return int Código de salida de la aplicación.
     */
    int
        run(HINSTANCE hInst, int nCmdShow);


    /**
     * @brief Actualización lógica por fotograma (Update).
     * @param deltaTime Tiempo transcurrido en segundos desde el último fotograma.
     */
    void
        update(float deltaTime);


    /**
     * @brief Rutina de renderizado por fotograma (Render).
     * Limpia buffers, configura el pipeline, dibuja la geometría y presenta el frame.
     */
    void
        render();


    /**
     * @brief Renderiza la interfaz gráfica de usuario (ImGui).
     * Dibuja la UI sobre la escena 3D.
     */
    void
        renderGUI();


    /**
     * @brief Libera todos los recursos de DirectX asignados.
     * Libera interfaces COM y memoria dinámica.
     */
    void
        destroy();


private:

    // -----------------------------------------------------------------------------
    // PROCEDIMIENTOS INTERNOS (Win32)
    // -----------------------------------------------------------------------------

    /**
     * @brief Procedimiento de Ventana Principal (Window Procedure).
     * Callback estático que procesa eventos del SO y los reenvía a ImGui.
     */
    static LRESULT CALLBACK
        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:

    // -----------------------------------------------------------------------------
    // SISTEMAS CORE (Ventana & API Gráfica)
    // -----------------------------------------------------------------------------

    /** @brief Envoltorio de la ventana principal de Win32. */
    Window          m_window;

    /** @brief Interfaz del Dispositivo Direct3D 11 (Creación de recursos). */
    Device          m_device;

    /** @brief Contexto Inmediato de Direct3D 11 (Comandos de dibujo). */
    DeviceContext   m_deviceContext;

    /** @brief Cadena de intercambio (Double Buffering). */
    SwapChain       m_swapChain;

    /** @brief Configuración del área de visualización. */
    Viewport        m_viewport;


    // -----------------------------------------------------------------------------
    // RENDER TARGETS & VISTAS
    // -----------------------------------------------------------------------------

    /** @brief Textura del Back Buffer. */
    Texture             m_backBuffer;

    /** @brief Vista de Render Target (RTV) para el Back Buffer. */
    RenderTargetView    m_renderTargetView;

    /** @brief Textura de Profundidad/Estarcido. */
    Texture             m_depthStencil;

    /** @brief Vista de Profundidad (DSV). */
    DepthStencilView    m_depthStencilView;


    // -----------------------------------------------------------------------------
    // RECURSOS DEL PIPELINE (Shaders & Buffers)
    // -----------------------------------------------------------------------------

    /** @brief Programa de Shader principal (Vertex + Pixel). */
    ShaderProgram       m_shaderProgram;

    /** @brief Constant Buffer estático (Rara vez cambia). */
    Buffer              m_cbNeverChanges;

    /** @brief Constant Buffer dinámico (Cambia al redimensionar). */
    Buffer              m_cbChangeOnResize;


    // -----------------------------------------------------------------------------
    // DATOS DE LA ESCENA (Assets & Lógica)
    // -----------------------------------------------------------------------------

    /** @brief Textura Albedo para demo. */
    Texture             m_EspadaAlbedo;

    /** @brief Textura Cubemap para el Skybox. */
    Texture             m_skyboxTex;

    /** @brief Recurso de modelo 3D cargado. */
    Model3D* m_model;

    /** @brief Cámara principal. */
    Camera              m_camera;

    /** @brief Grafo de Escena para jerarquías. */
    SceneGraph          m_sceneGraph;

    /** @brief Lista de actores en la escena. */
    std::vector<EU::TSharedPointer<Actor>> m_actors;

    /** @brief Actor principal de demostración. */
    EU::TSharedPointer<Actor>              m_Espada;


    // -----------------------------------------------------------------------------
    // DATOS DE REFLEXIÓN (CPU Mirrors for Constant Buffers)
    // -----------------------------------------------------------------------------

    /** @brief Caché de la Matriz de Vista. */
    XMMATRIX            m_View;

    /** @brief Caché de la Matriz de Proyección. */
    XMMATRIX            m_Projection;

    /** @brief Estructura CPU para buffer de resize. */
    CBChangeOnResize    cbChangesOnResize;

    /** @brief Estructura CPU para buffer estático. */
    CBNeverChanges      cbNeverChanges;


    // -----------------------------------------------------------------------------
    // INTERFAZ DE USUARIO & TOOLS
    // -----------------------------------------------------------------------------

    /** @brief Gestor de ImGui. */
    GUI                 m_gui;

};