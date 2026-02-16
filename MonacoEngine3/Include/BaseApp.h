/**
 * @file BaseApp.h
 * @brief Core application framework for the Pandora Core Engine.
 *
 * Defines the BaseApp class responsible for the application lifecycle,
 * window management, and DirectX 11 rendering loop initialization.
 *
 * @author Gael Jimenez
 * @copyright Obsidian Node Studio
 * @version 1.0
 */

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
 * @brief External handler for ImGui Win32 messages.
 *
 * Forwards Windows messages (mouse, keyboard, etc.) to the ImGui context
 * for UI interaction handling.
 *
 * @param hWnd Handle to the window.
 * @param msg The message ID.
 * @param wParam Additional message info.
 * @param lParam Additional message info.
 * @return LRESULT Message processing result.
 */
extern IMGUI_IMPL_API
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**
 * @class BaseApp
 * @brief Abstract base class for D3D11 applications.
 *
 * Manages the main infinite loop, OS message pumping, and initialization
 * of core graphics resources (Device, Context, SwapChain).
 */
class
	BaseApp {
public:
	/**
	 * @brief Default constructor.
	 */
	BaseApp() = default;

	/**
	 * @brief Virtual destructor. Calls destroy() to cleanup resources.
	 */
	~BaseApp() { destroy(); }

	/**
	 * @brief Pre-initialization logic.
	 *
	 * Called before window or graphics initialization. Useful for
	 * configuration or logging setup.
	 *
	 * @return HRESULT S_OK on success.
	 */
	HRESULT
		awake();

	/**
	 * @brief Starts the application main loop.
	 *
	 * Initializes the window and systems, then enters the message loop
	 * calling update() and render() every frame.
	 *
	 * @param hInst Application instance handle.
	 * @param nCmdShow Window show command (minimized, maximized, etc.).
	 * @return int Application exit code.
	 */
	int
		run(HINSTANCE hInst, int nCmdShow);

	/**
	 * @brief Initializes core DirectX 11 subsystems.
	 *
	 * Creates Device, SwapChain, RenderTargetView, DepthStencilView,
	 * and Shaders/Buffers required for the base pipeline.
	 *
	 * @return HRESULT S_OK on success, error code on failure.
	 */
	HRESULT
		init();

	/**
	 * @brief Per-frame logic update.
	 *
	 * @param deltaTime Time elapsed since the last frame (seconds).
	 */
	void
		update(float deltaTime);

	/**
	 * @brief Per-frame rendering routine.
	 *
	 * Clears buffers, draws the scene, and presents the back buffer.
	 */
	void
		render();

	/**
	 * @brief Renders the graphical user interface (ImGui).
	 */
	void 
		renderGUI();

	/**
	 * @brief Releases all allocated DirectX resources.
	 */
	void
		destroy();

private:
	/**
	 * @brief Main Window Procedure.
	 *
	 * Handles Win32 events (resize, close, input) and forwards
	 * relevant messages to ImGui.
	 */
	static LRESULT CALLBACK
		WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	/** @brief Main application window wrapper. */
	Window                              m_window;
	/** @brief Direct3D 11 Device interface. */
	Device								m_device;
	/** @brief Direct3D 11 Immediate Context. */
	DeviceContext						m_deviceContext;
	/** @brief DXGI SwapChain for buffer presentation. */
	SwapChain                           m_swapChain;
	/** @brief Texture resource for the back buffer. */
	Texture                             m_backBuffer;
	/** @brief Render Target View for the back buffer. */
	RenderTargetView                    m_renderTargetView;
	/** @brief Texture resource for depth testing. */
	Texture                             m_depthStencil;
	/** @brief Depth Stencil View for the depth texture. */
	DepthStencilView                    m_depthStencilView;
	/** @brief Rasterizer viewport configuration. */
	Viewport                            m_viewport;
	/** @brief Core Shader Program (Vertex & Pixel). */
	ShaderProgram						m_shaderProgram;

	/** @brief Constant Buffer for static data. */
	Buffer								m_cbNeverChanges;
	/** @brief Constant Buffer for resize-dependent data. */
	Buffer								m_cbChangeOnResize;

	/** @brief Albedo texture for the demo object. */
	Texture 							m_PrintStreamAlbedo;
	/** @brief Cubemap texture for the skybox. */
	Texture                             m_skyboxTex;

	/** @brief Main camera for the scene. */
	Camera								m_camera;

	/** @brief Cached View Matrix. */
	XMMATRIX                            m_View;
	/** @brief Cached Projection Matrix. */
	XMMATRIX                            m_Projection;

	/** @brief Scene Graph for managing hierarchy. */
	SceneGraph                          m_sceneGraph;
	
	/** @brief List of actors in the scene. */
	std::vector<EU::TSharedPointer<Actor>> m_actors;
	/** @brief Pointer to the main demo actor. */
	EU::TSharedPointer<Actor> m_PrintStream;

	/** @brief Pointer to the loaded 3D model. */
	Model3D* m_model;

	/** @brief CPU structure for resize constant buffer. */
	CBChangeOnResize					cbChangesOnResize;
	/** @brief CPU structure for static constant buffer. */
	CBNeverChanges						cbNeverChanges;

	/** @brief ImGui manager. */
	GUI									m_gui;
};
