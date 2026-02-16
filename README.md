# 🛠️ MonacoEngine3 — Motor Gráfico DirectX 11 (Framework ECS)

## 📋 Resumen

`MonacoEngine3` es una evolución integral hacia un motor gráfico profesional basado en **DirectX 11** y C++. A diferencia de sus versiones iniciales, esta iteración abandona la lógica centralizada para implementar una arquitectura modular basada en **Entidades y Componentes (ECS)** y un **Grafo de Escena (Scene Graph)** jerárquico.

El motor permite la carga de modelos complejos mediante el **FBX SDK**, gestión automatizada de recursos a través de un **ResourceManager** y una interfaz de edición en tiempo real impulsada por **Dear ImGui** y **ImGuizmo**.

## 📌 Índice

  - [Resumen](#-resumen)
  - [Objetivos del motor](#-objetivos-del-motor)
  - [Arquitectura del Sistema (ECS & Scene Graph)](#-arquitectura-del-sistema-ecs--scene-graph)
      - [Componentes principales](#componentes-principales)
      - [Jerarquía y Propagación](#jerarquía-y-propagación)
  - [Pipeline Gráfico y Recursos](#-pipeline-gráfico-y-recursos)
  - [Flujo de Ejecución (Ciclo de Vida)](#-flujo-de-ejecución-ciclo-de-vida)
  - [Herramientas del Editor (GUI)](#-herramientas-del-editor-gui)
  - [Diagrama UML](#-diagrama-uml)

-----

## 🎯 Objetivos del motor

| Objetivo | Descripción |
|---|---|
| **Arquitectura ECS** | Separación de datos y lógica mediante Entidades (`Actor`) y Componentes (`Transform`, `Mesh`, etc.). |
| **Scene Graph** | Implementación de jerarquías Padre-Hijo con propagación de matrices de transformación (SRT). |
| **Carga de Activos** | Importación de modelos `.fbx` y `.obj` utilizando el SDK de Autodesk FBX. |
| **Editor Visual** | Interfaz de usuario para manipulación de transformaciones, inspección de variables y jerarquía. |
| **Gestión Flyweight** | Uso de un `ResourceManager` para evitar la duplicación de texturas y mallas en memoria. |

-----

## 🏗 Arquitectura del Sistema (ECS & Scene Graph)

### Componentes principales

| Clase | Responsabilidad | Relación |
|---|---|---|
| **`BaseApp`** | Orquestador principal. Maneja el bucle de mensajes y los subsistemas DX11. | Posee `SceneGraph`, `GUI` y `Camera`. |
| **`Actor`** | Especialización de `Entity`. Es el contenedor de objetos en el mundo. | Hereda de `Entity`, contiene `Components`. |
| **`Transform`** | Gestiona Posición, Rotación y Escala. Calcula la matriz de mundo local. | Componente obligatorio por `Actor`. |
| **`HierarchyComponent`** | Almacena punteros a padres e hijos dentro del grafo. | Define la estructura del árbol en el `SceneGraph`. |
| **`MeshComponent`** | Contenedor de geometría (vértices e índices) cargados desde disco. | Alimenta a la clase `Buffer` para crear VBO/IBO. |
| **`SceneGraph`** | Sistema que recorre la jerarquía y actualiza las matrices globales. | Realiza el `updateWorldRecursive`. |

### Jerarquía y Propagación

1. El `SceneGraph` inicia el recorrido desde los nodos raíz (entidades sin padre).
2. Cada nodo calcula su matriz local: $S \times R \times T$.
3. La matriz global se obtiene multiplicando: $MatrizLocal \times MatrizPadre$.
4. Este flujo asegura que al mover un "Padre", todos sus "Hijos" se desplacen de forma relativa automáticamente.

-----

## 📷 Pipeline Gráfico y Recursos

| Recurso | Clase Wrapper | Descripción |
|---|---|---|
| **Dispositivo** | `Device` / `DeviceContext` | Abstracción de la creación de recursos y envío de comandos a la GPU. |
| **Buffers** | `Buffer` | Clase genérica para manejar Vertex, Index y Constant Buffers. |
| **Texturas** | `Texture` | Carga de imágenes (DDS, PNG, JPG) y creación de Render Targets. |
| **Shaders** | `ShaderProgram` | Compilación y vinculación de Vertex Shader, Pixel Shader e Input Layout. |
| **Recursos** | `ResourceManager` | Caché global que garantiza que un modelo o textura se cargue solo una vez. |

-----

## 🚀 Flujo de Ejecución (Ciclo de Vida)

1. **`awake()`**: Configuración inicial de logs y parámetros de ventana.
2. **`init()`**: 
   - Inicialización de DirectX 11 y el contexto de ImGui.
   - Carga de modelos mediante `ResourceManager` e instanciación de `Actors`.
   - Vinculación de jerarquías en el `SceneGraph`.
3. **`update()`**:
   - Procesamiento de Input de usuario y cámara FPS.
   - `SceneGraph::update()`: Recálculo recursivo de matrices de mundo.
   - Actualización de lógica de componentes.
4. **`render()`**:
   - Limpieza de Buffers (RTV/DSV).
   - `SceneGraph::render()`: Dibujo de entidades registradas.
   - `renderGUI()`: Dibujo de paneles de ImGui y Gizmos.
   - `SwapChain::present()`: Intercambio de buffers de imagen.

-----

## 🎨 Herramientas del Editor (GUI)

El motor integra un editor profesional para facilitar el desarrollo:
* **Inspector Panel:** Permite modificar valores numéricos de `Transform` y propiedades del `Actor` en tiempo real.
* **Hierarchy Panel (Outliner):** Vista de árbol para organizar y seleccionar objetos de la escena.
* **Gizmos (ImGuizmo):** Manipuladores visuales para Traslación (**W**), Rotación (**E**) y Escala (**R**).
* **Tema Visual:** Estilo personalizado "Monaco Dark" con bordes redondeados y paleta de grises profesionales.

-----

## 📊 Diagrama UML

Este diagrama representa la estructura de clases actual y cómo interactúan los componentes ECS con el sistema de renderizado.

```mermaid
classDiagram
    class BaseApp {
        -Window m_window
        -Device m_device
        -SceneGraph m_sceneGraph
        -GUI m_gui
        +init()
        +update()
        +render()
    }

    class Entity {
        <<Abstract>>
        #vector~Component*~ m_components
        +addComponent()
        +getComponent()
    }

    class Actor {
        -string m_name
        +renderShadow()
    }

    class Component {
        <<Interface>>
        +init()*
        +update()*
        +render()*
    }

    class Transform {
        +Vector3 position
        +Vector3 rotation
        +Vector3 scale
        +Matrix matrix
    }

    class MeshComponent {
        +vector~Vertex~ m_vertex
        +vector~uint~ m_index
    }

    class SceneGraph {
        -vector~Entity*~ m_entities
        +attach(child, parent)
        +updateWorldRecursive()
    }

    BaseApp --> SceneGraph
    BaseApp --> GUI
    SceneGraph --> Entity
    Entity <|-- Actor
    Entity "1" *-- "many" Component
    Component <|-- Transform
    Component <|-- MeshComponent
    Component <|-- HierarchyComponent
    Actor --> ResourceManager : requests assets
