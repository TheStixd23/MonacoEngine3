#pragma once
#include "Prerequisites.h"

#include "imgui.h"
#include <imgui_internal.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ImGuizmo.h"

class Viewport;
class Window;
class Device;
class DeviceContext;
class Actor;

/**
 * @class GUI
 * @brief Clase encargada de gestionar la Interfaz Gráfica de Usuario (UI) del editor.
 *
 * Esta clase centraliza la inicialización, actualización y renderizado de Dear ImGui.
 * Provee herramientas para la manipulación de la escena, como el Inspector, el Outliner,
 * la barra de herramientas y los Gizmos para transformar objetos.
 */
class GUI {
public:
    /**
     * @brief Constructor por defecto.
     */
    GUI() = default;

    /**
     * @brief Destructor por defecto.
     */
    ~GUI() = default;

    /**
     * @brief Método de inicialización temprana (Awake).
     * Se ejecuta antes de la inicialización gráfica.
     */
    void awake();

    /**
     * @brief Inicializa el contexto de ImGui y los backends gráficos.
     *
     * Configura los estilos, carga las fuentes y establece la conexión con la ventana (Win32)
     * y el dispositivo gráfico (DirectX 11).
     *
     * @param window Referencia a la ventana de la aplicación.
     * @param device Referencia al dispositivo gráfico para crear texturas de fuentes.
     * @param deviceContext Contexto del dispositivo para comandos de dibujo.
     */
    void init(Window& window, Device& device, DeviceContext& deviceContext);

    /**
     * @brief Actualiza la lógica de la interfaz por fotograma.
     *
     * Inicia un nuevo frame de ImGui, procesa la entrada del usuario y dibuja las ventanas
     * lógicas (Inspector, Toolbar, etc.) antes de la renderización final.
     *
     * @param viewport Referencia a la cámara/viewport actual.
     * @param window Referencia a la ventana principal.
     */
    void update(Viewport& viewport, Window& window);

    /**
     * @brief Renderiza la interfaz gráfica.
     *
     * Envía los datos de dibujo generados por ImGui al pipeline de renderizado de DirectX.
     * Debe llamarse al final del bucle de renderizado.
     */
    void render();

    /**
     * @brief Libera los recursos de ImGui.
     * Cierra el contexto y limpia los backends al cerrar la aplicación.
     */
    void destroy();

    /**
     * @brief Dibuja la barra de menú principal (Main Menu Bar).
     * Contiene menús como File, Edit, Tools, etc.
     */
    void ToolBar();

    /**
     * @brief Gestiona el popup modal para cerrar la aplicación.
     * Muestra una ventana de confirmación antes de salir.
     */
    void closeApp();

    /**
     * @brief Inicializa los datos para los tooltips (información sobre herramientas).
     */
    void toolTipData();

    /**
     * @brief Aplica un estilo visual personalizado tipo "Apple Liquid".
     *
     * Configura los colores y el redondeo de las ventanas para simular un aspecto de cristal.
     *
     * @param opacity Opacidad de las ventanas (0.0f a 1.0f).
     * @param accent Color de acento para elementos seleccionados (ImVec4).
     */
    void appleLiquidStyle(float opacity /*0..1f*/, ImVec4 accent /*=#0A84FF*/);

    /**
     * @brief Dibuja un control personalizado para vectores de 3 componentes (X, Y, Z).
     *
     * Muestra tres campos numéricos con botones de colores para restablecer valores individuales.
     * Útil para editar Transformaciones (Posición, Rotación, Escala).
     *
     * @param label Etiqueta del control.
     * @param values Puntero al array de 3 flotantes a modificar.
     * @param resetValues Valor al que se restablece si se presiona el botón del eje (por defecto 0.0f).
     * @param columnWidth Ancho de la columna de la etiqueta.
     */
    void vec3Control(const std::string& label,
        float* values,
        float resetValues = 0.0f,
        float columnWidth = 100.0f);

    /**
     * @brief Dibuja el panel Inspector general para un Actor.
     *
     * Muestra propiedades básicas como nombre, tags y layers.
     * @param actor Puntero compartido al actor seleccionado.
     */
    void inspectorGeneral(EU::TSharedPointer<Actor> actor);

    /**
     * @brief Dibuja los componentes del Actor en el Inspector.
     *
     * Itera sobre los componentes (como Transform) y muestra sus controles específicos.
     * @param actor Puntero compartido al actor.
     */
    void inspectorContainer(EU::TSharedPointer<Actor> actor);

    /**
     * @brief Dibuja el panel de Jerarquía (Outliner).
     *
     * Muestra una lista de árbol con todos los actores de la escena para su selección.
     * @param actors Vector con todos los actores activos en la escena.
     */
    void outliner(const std::vector<EU::TSharedPointer<Actor>>& actors);

    /**
     * @brief Gestiona y dibuja el Gizmo de transformación (ImGuizmo).
     *
     * Permite manipular (Mover, Rotar, Escalar) el actor seleccionado directamente en la vista 3D.
     *
     * @param view Matriz de vista de la cámara actual.
     * @param projection Matriz de proyección de la cámara actual.
     * @param actor Actor seleccionado sobre el cual aplicar la transformación.
     */
    void editTransform(const XMMATRIX& view, const XMMATRIX& projection, EU::TSharedPointer<Actor> actor);

    /**
     * @brief Dibuja una barra de herramientas flotante para controlar el Gizmo.
     * Permite cambiar entre operaciones (Traslación, Rotación, Escala) y modos (Local/Mundial).
     */
    void drawGizmoToolbar();

    /**
     * @brief Función auxiliar para convertir una matriz XMMATRIX de DirectX a un array de float[16].
     *
     * Necesaria porque ImGuizmo espera un puntero `float*` crudo, no un tipo SIMD de DirectX.
     *
     * @param mat Matriz de origen (DirectX).
     * @param dest Puntero al array de destino (debe tener espacio para 16 floats).
     */
    void ToFloatArray(const XMMATRIX& mat, float* dest) {
        XMFLOAT4X4 temp;
        XMStoreFloat4x4(&temp, mat);
        memcpy(dest, &temp, sizeof(float) * 16);
    }

private:
    bool checkboxValue = true;      ///< Variable de prueba para UI.
    bool checkboxValue2 = false;    ///< Variable de prueba para UI.
    std::vector<const char*> m_objectsNames; ///< Cache de nombres de objetos para UI.
    std::vector<const char*> m_tooltips;     ///< Textos de ayuda.

    bool show_exit_popup = false;   ///< Controla la visibilidad del popup de confirmación de salida.

public:
    /** * @brief Índice del actor actualmente seleccionado en el Outliner.
     * -1 indica que no hay ninguna selección.
     */
    int selectedActorIndex = -1;
};