#pragma once

#include "Prerequisites.h"
#include "IResource.h"
#include "MeshComponent.h"
#include "fbxsdk.h" // Asegúrate de que el path de inclusión sea correcto en tu proyecto


// =================================================================================
// ENUMERACIONES
// =================================================================================

/**
 * @enum ModelType
 * @brief Formatos de archivo de modelos 3D soportados por el cargador.
 */
enum ModelType {
    OBJ,    ///< Formato Wavefront OBJ (Geometría estática simple).
    FBX     ///< Formato Autodesk FBX (Soporta jerarquías, materiales, animaciones).
};


// =================================================================================
// RECURSO: MODELO 3D
// =================================================================================

/**
 * @class Model3D
 * @brief Recurso que representa un modelo tridimensional cargado desde disco.
 *
 * Esta clase gestiona la importación y almacenamiento de datos geométricos.
 * Actúa principalmente como un puente entre archivos externos (especialmente FBX)
 * y las estructuras internas del motor (@ref MeshComponent).
 *
 * Utiliza el **Autodesk FBX SDK** para parsear la escena, recorrer los nodos
 * y extraer vértices, índices y referencias a texturas.
 */
class Model3D : public IResource {

public:

    // -----------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR
    // -----------------------------------------------------------------------------

    /**
     * @brief Constructor del recurso Modelo 3D.
     *
     * Inicializa los punteros del SDK a null y establece el tipo de recurso.
     * Inmediatamente intenta cargar el archivo especificado en el nombre.
     *
     * @param name Nombre o ruta relativa del archivo del modelo.
     * @param modelType Enumerador que indica el formato del archivo (OBJ/FBX).
     */
    Model3D(const std::string& name, ModelType modelType) :
        IResource(name),
        m_modelType(modelType),
        lSdkManager(nullptr),
        lScene(nullptr)
    {
        SetType(ResourceType::Model3D);
        // Nota: Llamar a métodos virtuales (load) en el constructor puede ser riesgoso
        // en herencia profunda, pero funcional aquí si la clase no se deriva más.
        load(name);
    }


    /** * @brief Destructor por defecto.
     */
    ~Model3D() = default;


    // -----------------------------------------------------------------------------
    // IRESOURCE OVERRIDES (Ciclo de Vida)
    // -----------------------------------------------------------------------------

    /**
     * @brief Carga el archivo del modelo desde la ruta especificada.
     * Si el tipo es FBX, desencadena la inicialización del SDK y el proceso de importación.
     * @param path Ruta al archivo en disco.
     * @return `true` si la carga fue exitosa.
     */
    bool
        load(const std::string& path) override;


    /**
     * @brief Inicialización post-carga (Creación de buffers GPU).
     * @return `true` si la inicialización gráfica tuvo éxito.
     */
    bool
        init() override;


    /**
     * @brief Libera la memoria del modelo.
     * Destruye la escena FBX y el Manager del SDK para liberar recursos del sistema.
     */
    void
        unload() override;


    /**
     * @brief Calcula el tamaño aproximado del modelo en memoria.
     * Basado en la cantidad de vértices e índices almacenados.
     */
    size_t
        getSizeInBytes() const override;


    // -----------------------------------------------------------------------------
    // GETTERS (Accesores)
    // -----------------------------------------------------------------------------

    /**
     * @brief Obtiene la lista de mallas que componen el modelo.
     * Un solo archivo FBX puede contener múltiples sub-mallas.
     * @return Referencia constante al vector de componentes de malla.
     */
    const std::vector<MeshComponent>&
        GetMeshes() const
    {
        return m_meshes;
    }


    /**
     * @brief Obtiene los nombres de archivo de las texturas encontradas en el modelo.
     * @return Vector de strings con las rutas/nombres de las texturas.
     */
    std::vector<std::string>
        GetTextureFileNames() const
    {
        return textureFileNames;
    }


    // -----------------------------------------------------------------------------
    // FBX SDK INTERFACE (Lógica de Importación)
    // -----------------------------------------------------------------------------

    /**
     * @brief Inicializa el Gestor de FBX (FbxManager) y la configuración de IO.
     * Es el primer paso obligatorio antes de importar cualquier archivo FBX.
     * @return `true` si el manager se creó correctamente.
     */
    bool
        InitializeFBXManager();


    /**
     * @brief Carga y parsea un archivo FBX completo.
     *
     * 1. Crea el importador.
     * 2. Inicializa la escena.
     * 3. Importa el archivo a la escena.
     * 4. Convierte la geometría (triangulación).
     * 5. Inicia el recorrido del nodo raíz.
     *
     * @param filePath Ruta del archivo .fbx.
     * @return Vector de mallas extraídas.
     */
    std::vector<MeshComponent>
        LoadFBXModel(const std::string& filePath);


    /**
     * @brief Procesa recursivamente un nodo de la escena FBX.
     *
     * Recorre el árbol de la escena. Si el nodo contiene un atributo de tipo Malla (eMesh),
     * llama a @ref ProcessFBXMesh. Luego se llama a sí mismo para los hijos del nodo.
     * @param node Puntero al nodo FBX actual.
     */
    void
        ProcessFBXNode(FbxNode* node);


    /**
     * @brief Extrae la geometría de un nodo de malla FBX.
     *
     * Lee los puntos de control (vértices), las normales y las coordenadas UV.
     * Convierte los datos al formato @c SimpleVertex del motor y genera los índices.
     * @param node Nodo que contiene la malla a procesar.
     */
    void
        ProcessFBXMesh(FbxNode* node);


    /**
     * @brief Extrae información de materiales (Texturas).
     *
     * Busca propiedades de texturas (como Diffuse/Albedo) conectadas al material
     * y guarda los nombres de archivo en @c textureFileNames.
     * @param material Puntero al material FBX.
     */
    void
        ProcessFBXMaterials(FbxSurfaceMaterial* material);


private:

    // -----------------------------------------------------------------------------
    // DATOS INTERNOS (FBX SDK)
    // -----------------------------------------------------------------------------

    /** * @brief Puntero al gestor principal del SDK de FBX (Singleton de facto). */
    FbxManager* lSdkManager;


    /** * @brief Puntero a la escena que contiene el grafo de nodos importado. */
    FbxScene* lScene;


    /** * @brief Caché de nombres de texturas asociadas para carga diferida. */
    std::vector<std::string> textureFileNames;


public:

    // -----------------------------------------------------------------------------
    // DATOS PÚBLICOS
    // -----------------------------------------------------------------------------

    /** * @brief Tipo de formato del modelo. */
    ModelType m_modelType;


    /** * @brief Contenedor de todas las sub-mallas generadas. */
    std::vector<MeshComponent> m_meshes;

};