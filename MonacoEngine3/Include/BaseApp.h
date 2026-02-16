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
 * Esta función es necesaria para integrar la biblioteca Dear ImGui con la API de Win32.
 * Reenvía los eventos del sistema operativo (como clics del ratón, pulsaciones de teclas,
 * y cambios de foco) al contexto de ImGui para que la interfaz gráfica pueda procesarlos.
 *
 * @param hWnd Handle de la ventana que recibe el mensaje.
 * @param msg Identificador del mensaje (ej. WM_KEYDOWN, WM_LBUTTONDOWN).
 * @param wParam Información adicional del mensaje (dependiente del mensaje).
 * @param lParam Información adicional del mensaje (dependiente del mensaje).
 * @return LRESULT Resultado del procesamiento del mensaje. Generalmente devuelve true si ImGui manejó el evento.
 */
extern IMGUI_IMPL_API
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**
 * @class BaseApp
 * @brief Clase base que encapsula el núcleo de una aplicación gráfica con DirectX 11.
 *
 * Esta clase administra el ciclo de vida completo de la aplicación, desde la creación de la ventana
 * y la inicialización de los recursos gráficos, hasta el bucle principal de renderizado y la limpieza final.
 * Actúa como el punto de entrada para la lógica del motor, coordinando el dispositivo, el contexto,
 * la cadena de intercambio y las vistas necesarias para dibujar en pantalla.
 */
class BaseApp {
public:
    /**
     * @brief Constructor por defecto.
     */
    BaseApp() = default;

    /**
     * @brief Destructor virtual.
     *
     * Llama internamente al método @ref destroy() para asegurar que todos los recursos de DirectX
     * y las interfaces COM sean liberados correctamente al cerrar la aplicación.
     */
    ~BaseApp() { destroy(); }

    /**
     * @brief Lógica de pre-inicialización (Awake).
     *
     * Se ejecuta una sola vez al inicio, antes de crear la ventana o inicializar los gráficos.
     * Es el lugar ideal para configurar sistemas de registro (logging), cargar archivos de configuración,
     * o preparar estructuras de datos esenciales.
     *
     * @return HRESULT S_OK si la operación fue exitosa, o un código de error en caso contrario.
     */
    HRESULT awake();

    /**
     * @brief Inicia el bucle principal de la aplicación.
     *
     * Este método es el punto de entrada principal. Primero inicializa la ventana y los subsistemas gráficos
     * llamando a @ref init(). Si la inicialización es exitosa, entra en el bucle infinito de mensajes de Windows,
     * donde llama a @ref update() y @ref render() repetidamente en cada fotograma hasta que la aplicación se cierra.
     *
     * @param hInst Handle de la instancia de la aplicación (proporcionado por WinMain).
     * @param nCmdShow Comando para mostrar la ventana (minimizado, maximizado, normal, etc.).
     * @return int Código de salida de la aplicación al terminar el bucle.
     */
    int run(HINSTANCE hInst, int nCmdShow);

    /**
     * @brief Inicializa los subsistemas centrales de DirectX 11.
     *
     * Crea y configura los componentes fundamentales del pipeline gráfico:
     * - Dispositivo (Device) y Contexto (DeviceContext).
     * - Cadena de Intercambio (SwapChain).
     * - Vistas de Render Target (RTV) y Depth Stencil (DSV).
     * - Viewport.
     * - Shaders básicos y Buffers constantes.
     *
     * @return HRESULT S_OK si todos los recursos gráficos se inicializaron correctamente.
     */
    HRESULT init();

    /**
     * @brief Actualización lógica por fotograma (Update).
     *
     * Contiene la lógica del juego o simulación que debe ejecutarse en cada frame.
     * Aquí se actualizan transformaciones, física, entradas de usuario, animaciones, etc.
     *
     * @param deltaTime Tiempo transcurrido en segundos desde el último fotograma (delta time).
     */
    void update(float deltaTime);

    /**
     * @brief Rutina de renderizado por fotograma (Render).
     *
     * Ejecuta los comandos de dibujo para generar la imagen final.
     * 1. Limpia el Back Buffer y el Depth Buffer.
     * 2. Configura el pipeline (Shaders, Buffers, Texturas).
     * 3. Dibuja la geometría de la escena.
     * 4. Presenta el resultado en pantalla (SwapChain Present).
     */
    void render();

    /**
     * @brief Renderiza la interfaz gráfica de usuario (ImGui).
     *
     * Configura un nuevo frame de ImGui, define las ventanas y widgets de la UI,
     * y emite las llamadas de dibujo correspondientes para superponer la interfaz sobre la escena 3D.
     */
    void renderGUI();

    /**
     * @brief Libera todos los recursos de DirectX asignados.
     *
     * Se encarga de llamar al método Release() en todas las interfaces COM (Device, Context, Views, etc.)
     * y limpiar cualquier memoria dinámica asignada, evitando fugas de memoria.
     */
    void destroy();

private:
    /**
     * @brief Procedimiento de Ventana Principal (Window Procedure).
     *
     * Función estática callback que procesa los eventos enviados por el sistema operativo a la ventana
     * (como redimensionado, cierre, minimizado, entrada de teclado/ratón).
     * Reenvía los mensajes relevantes al gestor de ImGui.
     *
     * @param hWnd Handle de la ventana.
     * @param message Identificador del mensaje.
     * @param wParam Parámetro adicional del mensaje.
     * @param lParam Parámetro adicional del mensaje.
     * @return LRESULT Resultado del procesamiento.
     */
    static LRESULT CALLBACK
        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    /** @brief Envoltorio de la ventana principal de la aplicación. Maneja la creación y propiedades de la ventana de Win32. */
    Window                              m_window;

    /** @brief Interfaz del Dispositivo Direct3D 11. Se usa para la creación de recursos (texturas, buffers, shaders). */
    Device                              m_device;

    /** @brief Contexto Inmediato de Direct3D 11. Se usa para enviar comandos de renderizado a la GPU. */
    DeviceContext                       m_deviceContext;

    /** @brief Cadena de intercambio DXGI. Gestiona el doble buffer y la presentación de imágenes en pantalla. */
    SwapChain                           m_swapChain;

    /** @brief Textura que representa el Back Buffer (donde se dibuja la imagen actual). */
    Texture                             m_backBuffer;

    /** @brief Vista de Render Target (RTV) que permite al pipeline dibujar sobre la textura del Back Buffer. */
    RenderTargetView                    m_renderTargetView;

    /** @brief Textura utilizada como buffer de profundidad y estarcido (Depth-Stencil Buffer). */
    Texture                             m_depthStencil;

    /** @brief Vista del buffer de profundidad (DSV) para configurar el test de profundidad en el pipeline. */
    DepthStencilView                    m_depthStencilView;

    /** @brief Configuración del área de visualización (Viewport), que define dónde se proyecta la imagen en la ventana. */
    Viewport                            m_viewport;

    /** @brief Programa de Shader principal que encapsula Vertex Shader y Pixel Shader. */
    ShaderProgram                       m_shaderProgram;

    /** @brief Constant Buffer para datos estáticos que rara vez cambian (ej. Matriz de Vista en cámaras fijas). */
    Buffer                              m_cbNeverChanges;

    /** @brief Constant Buffer para datos que cambian al redimensionar la ventana (ej. Matriz de Proyección). */
    Buffer                              m_cbChangeOnResize;

    /** @brief Textura Albedo (color base) aplicada al objeto de demostración. */
    Texture                             m_EspadaAlbedo;

    /** @brief Textura de tipo Cubemap utilizada para renderizar el fondo del cielo (Skybox). */
    Texture                             m_skyboxTex;

    /** @brief Cámara principal de la escena, controla la vista y la proyección. */
    Camera                              m_camera;

    /** @brief Matriz de Vista almacenada en caché para evitar recálculos innecesarios. */
    XMMATRIX                            m_View;

    /** @brief Matriz de Proyección almacenada en caché. */
    XMMATRIX                            m_Projection;

    /** @brief Grafo de Escena (Scene Graph) para gestionar la jerarquía y transformaciones de las entidades. */
    SceneGraph                          m_sceneGraph;

    /** @brief Lista de actores (entidades) presentes en la escena. */
    std::vector<EU::TSharedPointer<Actor>> m_actors;

    /** @brief Puntero inteligente al actor principal de demostración (ej. una espada). */
    EU::TSharedPointer<Actor> m_Espada;

    /** @brief Puntero al recurso del modelo 3D cargado desde archivo. */
    Model3D* m_model;

    /** @brief Estructura de datos en CPU que refleja el contenido del Constant Buffer de redimensionado. */
    CBChangeOnResize                    cbChangesOnResize;

    /** @brief Estructura de datos en CPU que refleja el contenido del Constant Buffer estático. */
    CBNeverChanges                      cbNeverChanges;

    /** @brief Gestor de la interfaz gráfica ImGui, maneja el renderizado de menús y herramientas de depuración. */
    GUI                                 m_gui;
};