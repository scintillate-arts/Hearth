/* Copyright (c) 2020 Simular Games, LLC.
 * -------------------------------------------------------------------------------------------------
 *
 * MIT License
 * -------------------------------------------------------------------------------------------------
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * -------------------------------------------------------------------------------------------------
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once
#include <array>
#include <chrono>
#include <memory>
#include "event.hpp"
#include "version.hpp"
#include "window.hpp"
#include "graphics/cmdbuf.hpp"
#include "graphics/dscset.hpp"
#include "graphics/fence.hpp"
#include "graphics/frmbuf.hpp"
#include "graphics/gfxpip.hpp"
#include "graphics/rdrctx.hpp"
#include "graphics/rdrpss.hpp"
#include "graphics/resbuf.hpp"
#include "graphics/semphr.hpp"
#include "graphics/swpchn.hpp"
#include "graphics/txrimg.hpp"

namespace HAPI_NAMESPACE_NAME {

  /*!
   * \brief Describes a type that represents an application on a given, target platform.
   *
   * This is an abstraction of any given application, no matter what it is. It aims to provide
   * functionality for controlling and managing a given application, including it's main window and
   * other things.
   */
  class Application {
  public:
    //! \brief Describes the information needed to create an application.
    struct CreateInfo {
      //! \brief The name of the application being created.
      std::wstring_view appName;

      //! \brief The environment this application belongs to.
      Environment* appResidency;

      //! \brief The version of the application being created.
      Version appVersion;
    };

  private:
    /*!
     * \brief Describes the different phases of execution an application can be in.
     *
     * This is managerial, helps with managing the application and debugging the application.
     */
    enum ExecutionPhase : std::uint8_t {
      Initialization,
      EventPolling,
      Simulation,
      Updating,
      Rendering,
      Termination,
    };

    /*!
     * \brief Keeps track of the different states of execution the application can be in.
     *
     * Whenever the application changes phases, running-, or quitting states, it will be recorded by
     * the application in an instance of this struct.
     */
    struct ExecutionState {
      //! \brief Which phase of execution the application is in.
      ExecutionPhase phase;

      //! \brief Whether or not the application is in the "running" state.
      bool running : 1;

      //! \brief Whether or not the application is in the "quitting" state.
      bool quitting : 1;

    private:
      // Explicitly pads out this structure. Do not mess with this under any circumstance.
      char __mPadding__ : 7;
    };

    //! \brief Shortcut type definition for overly verbose standard chrono steady clock.
    using SteadyClock = std::chrono::steady_clock;

    //! \brief Shortcut type definition for overly verbose standard chrono steady clock time pont.
    using TimePoint = SteadyClock::time_point;

    //! \brief Shortcut type definition for overly verbose standard chrono nanoseconds.
    using Nanoseconds = std::chrono::nanoseconds;

    /*!
     * \brief Keeps track of various timing related information for the application.
     */
    struct Timing {
      //! \brief Collects the number of passing nanoseconds per frame.
      Nanoseconds accumulator;

      //! \brief The amount of nanoseconds the last frame took.
      Nanoseconds lastFrameDelta;

      //! \brief The start of the application.
      TimePoint start;

      //! \brief The end of the application.
      TimePoint end;

      //! \brief The number of frames that have elapsed.
      std::uint64_t framesElapsed;
    };

  public:
    /*!
     * \brief     Explicitly defined constructor, constructs this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     *
     * Makes sure this object is added to the environment.
     *
     * This constructor is made non-explicit to allow aggregate construction via the CreateInfo
     * structure.
     */
    Application(const CreateInfo& createInfo);

    /*!
     * \brief  Explicitly defined destructor, prepares this object for destruction.
     *
     * Makes sure this object is removed from the environment.
     */
   ~Application() noexcept;

  private:
    // Not allowed.
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

  public:
    /*!
     * \brief     Sets the application quitting state, to the value given.
     * \param[in] shouldQuit Whether or not the application should quit.
     */
    void quit(bool shouldQuit) noexcept;

    /*!
     * \brief Sets the application into the running state.
     *
     * This function initializes the application, sets it into the running state, and begins
     * executing application frames until it is set to quit and exits the internal application
     * frame loop, at which point it terminates.
     */
    void run() noexcept;

    /*!
     * \brief  Gets the application execution phase.
     * \return The value of the execution phase from the execution state.
     */
    ExecutionPhase phase() const noexcept;

    /*!
     * \brief  Checks if the application is still running.
     * \return The value of the running sub-state from the execution state.
     */
    bool running() const noexcept;

    /*!
     * \brief  Checks if the application is quitting.
     * \return The value of the quitting sub-state from the execution state.
     */
    bool quitting() const noexcept;

  private:
    //! \brief Initializes the application window.
    void initializeWindow();

    //! \brief Initializes the render context.
    void initializeRenderContext();

    //! \brief Initialize the swapchain.
    void initializeSwapChain();

    //! \brief Initializes the renderpass.
    void initializeRenderPass();

    //! \brief Initializes the framebuffers.
    void initializeFrameBuffers();

    //! \brief Initializes the vertex buffer.
    void initializeVertexBuffer();

    //! \brief Initializes the index buffer.
    void initializeIndexBuffer();

    //! \brief Initializes the uniform buffer.
    void initializeUniformBuffer();

    //! \brief Initializes the texture image.
    void initializeTextureImage();

    //! \brief Initializes the descriptor pool.
    void initializeDescriptorPool();

    //! \brief Initializes the descriptor set layout.
    void initializeDescriptorSetLayout();

    //! \brief Initializes the descriptor set.
    void initializeDescriptorSet();

    //! \brief Initializes the pipeline layout.
    void initializePipelineLayout();

    //! \brief Initializes the graphics pipeline.
    void initializeGraphicsPipeline();

    //! \brief Initializes the command pool.
    void initializeCommandPool();

    //! \brief Initializes the command buffer.
    void initializeCommandBuffer();

    //! \brief Initializes this application.
    void initialize();

    //! \brief Terminates this application.
    void terminate() noexcept;

    //! \brief Executes a frame of operations within the application.
    void executeFrame() noexcept;

    //! \brief Rebuilds the swapchain and framebuffers.
    void rebuildSwapChainAndFrameBuffers();

    /*!
     * \brief     Receives an emitted event and processes it.
     * \param[in] evnt The event that was emitted.
     * \return
     */
    void onEvent(Event& evnt);

    /*!
     * \brief     Called when the window closes, via callback.
     * \param[in] evnt The event that was passed.
     */
    void onWindowClose(WindowCloseEvent& evnt);

    /*!
     * \brief     Called when the window gains or loses focus, via callback.
     * \param[in] evnt The event that was passed.
     */
    void onWindowFocus(WindowFocusEvent& evnt);

    /*!
     * \brief     Called when the window's framebuffer is resized, via callback.
     * \param[in] evnt The event that was passed.
     */
    void onFrameBufferResize(FrameBufferResizeEvent& evnt);

    /*!
     * \brief     Called when the window is maximized or restored, via callback.
     * \param[in] evnt The event that was passed.
     */
    void onWindowMaximize(WindowMaximizeEvent& evnt);

    /*!
     * \brief     Called when the window is minimized or restored, via callback.
     * \param[in] evnt The event that was passed.
     */
    void onWindowMinimize(WindowMinimizeEvent& evnt);

    /*!
     * \brief     Called whenever the window is dragged around, via callback.
     * \param[in] evnt The event that was passed.
     */
    void onWindowMoving(WindowMovingEvent& evnt);

    /*!
     * \brief     Called whenever the window is resized, via callback.
     * \param[in] evnt The event that was passed.
     */
    void onWindowResize(WindowResizeEvent& evnt);

    /*!
     * \brief     Called whenever the window is repositioned, via callback.
     * \param[in] evnt The event that was passed.
     */
    void onWindowReposition(WindowRepositionEvent& evnt);

  private:
    //! \brief The timing information for this application.
    Timing mTiming;

    //! \brief The information this application was created with.
    CreateInfo mCreateInfo;

    //! \brief Describes the executing state of the application.
    ExecutionState mExecutionState;

    //! \brief The main window for this application.
    Window* mMainWindow;

    //! \brief The framebuffers for this application.
    std::array<std::unique_ptr<gfx::FrameBuffer>, 3> mFrameBuffers;

    //! \brief The render context for this application.
    std::unique_ptr<gfx::RenderContext> mRenderContext;

    //! \brief The swapchain for this application.
    std::unique_ptr<gfx::SwapChain> mSwapChain;

    //! \brief The renderpass for this application.
    std::unique_ptr<gfx::RenderPass> mRenderPass;

    //! \brief The resource buffer we will be using for our vertices.
    std::unique_ptr<gfx::ResourceBuffer> mVertexBuffer;

    //! \brief The resource buffer we will be using for our indices.
    std::unique_ptr<gfx::ResourceBuffer> mIndexBuffer;

    //! \brief The resource buffer we will be using for uniforms.
    std::unique_ptr<gfx::ResourceBuffer> mUniformBuffer;

    //! \brief The texture image we will be displaying to the screen.
    std::unique_ptr<gfx::TextureImage> mTextureImage;

    //! \brief The descriptor pool we will be getting our descriptors from.
    std::unique_ptr<gfx::DescriptorPool> mDescriptorPool;

    //! \brief The descriptor set layout we will be using.
    std::unique_ptr<gfx::DescriptorSetLayout> mDescriptorLayout;

    //! \brief The descriptor set that we will be using to update our uniform buffer.
    std::unique_ptr<gfx::DescriptorSet> mUniformDescriptorSet;

    //! \brief The layout for the graphics pipeline.
    std::unique_ptr<gfx::PipelineLayout> mPipelineLayout;

    //! \brief The graphics pipeline used for this application.
    std::unique_ptr<gfx::Pipeline> mGraphicsPipeline;

    //! \brief The command pool we will use to create command buffers.
    std::unique_ptr<gfx::CommandPool> mCommandPool;

    //! \brief The command buffer used to record commands.
    std::unique_ptr<gfx::CommandBuffer> mCommandBuffer;

    //! \brief Whether or not the window minimized.
    bool mWindowMinimized;
  };

}
