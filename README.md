# 🛠️ MonacoEngine3 — Motor Gráfico DirectX 11

## 📋 Descripción

**MonacoEngine3** es un motor gráfico avanzado desarrollado en C++ y **DirectX 11**. A diferencia de sus versiones anteriores, esta iteración presenta una arquitectura robusta basada en componentes (**ECS**), un grafo de escena jerárquico (**Scene Graph**) y un editor visual integrado (**GUI**) para la manipulación de escenas en tiempo real.

El motor está diseñado con un enfoque modular, encapsulando la API de DirectX en clases de alto nivel para facilitar la gestión de recursos, el renderizado y la lógica de juego.

## ✨ Características Principales

### 🖥️ Motor Gráfico (DirectX 11)

* **Pipeline Modular:** Abstracción completa de la API mediante clases wrapper (`Device`, `DeviceContext`, `SwapChain`, `RenderTargetView`, `DepthStencilView`).
* **Shaders HLSL:** Sistema de gestión de `ShaderProgram` para compilación y enlace de Vertex y Pixel Shaders.
* **Buffers Eficientes:** Manejo automatizado de Vertex Buffers, Index Buffers y Constant Buffers (`Buffer`).
* **Texturizado:** Soporte para texturas 2D y Cubemaps para Skyboxes.

### 🧱 Arquitectura de Entidades (ECS & Scene Graph)

* **Entity-Component-System:** Estructura flexible donde los `Actor` son contenedores de componentes (`Transform`, `MeshComponent`, `HierarchyComponent`).
* **Grafo de Escena (Scene Graph):** Sistema jerárquico completo que permite relaciones Padre-Hijo. Las transformaciones (posición, rotación, escala) se propagan correctamente a través de la jerarquía.
* **Transformaciones:** Cálculos matriciales precisos (SRT) con orden de rotación controlado para evitar Gimbal Lock visual.

### 📦 Gestión de Recursos (Assets)

* **Resource Manager Centralizado:** Singleton (`ResourceManager`) que implementa el patrón **Flyweight** para evitar la duplicación de datos en memoria.
* **Carga de Modelos 3D:** Integración con **Autodesk FBX SDK** para importar modelos complejos (`.fbx`, `.obj`) incluyendo mallas y materiales.
* **Ciclo de Vida:** Estados de carga gestionados (`Unloaded`, `Loading`, `Loaded`, `Failed`).

### 🎨 Editor Integrado (GUI)

* **Interfaz ImGui Personalizada:** Estilo visual "Monaco Dark" profesional.
* **Inspector de Propiedades:** Visualización y edición en tiempo real de componentes (Transform, Tag, Layer).
* **Outliner (Jerarquía):** Vista de árbol para seleccionar y organizar actores en la escena.
* **Gizmos 3D:** Integración de **ImGuizmo** para manipular objetos (Traslación, Rotación, Escala) directamente en el Viewport.
* **Cámara de Editor:** Cámara tipo "LookAt" y FPS con movimiento libre.

## 🏛️ Arquitectura del Software

El motor sigue un flujo de ejecución gestionado por la clase `BaseApp`, que controla el bucle principal de Windows:

1. **Awake/Init:** Inicialización de ventana, dispositivo DX11, ImGui y carga de recursos iniciales.
2. **Update:**
* Procesamiento de input (Win32/ImGui).
* Actualización lógica del `SceneGraph` (matrices de mundo).
* Lógica de componentes (`update`).


3. **Render:**
* Limpieza de buffers.
* Renderizado de la escena 3D (Shaders, Mallas).
* Renderizado de la GUI (Capas de ImGui sobre la escena).
* Presentación (SwapChain).


4. **Destroy:** Liberación segura de memoria y punteros COM (`SAFE_RELEASE`).

## 📚 Dependencias

* **DirectX 11 SDK:** API gráfica principal.
* **DirectX Math (xnamath/DirectXMath):** Operaciones vectoriales y matriciales.
* **Dear ImGui:** Biblioteca para la interfaz gráfica de usuario.
* **ImGuizmo:** Extensiones de ImGui para manipuladores 3D.
* **Autodesk FBX SDK:** Para la importación de archivos de modelos 3D.

## 🎮 Controles (Cámara de Editor)

* **W, A, S, D:** Movimiento de la cámara (Adelante, Izquierda, Atrás, Derecha).
* **Mouse:** Rotación de la vista (Yaw/Pitch).
* **Gizmos:**
* `W`: Modo Traslación.
* `E`: Modo Rotación.
* `R`: Modo Escala.
