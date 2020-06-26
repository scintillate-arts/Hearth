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
#include <iostream>
#include <stdexcept>
#include <thread>
#include <hearth/application.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "eventbus.hpp"

namespace HAPI_NAMESPACE_NAME {

  // Temporary.
  struct Vertex {
    glm::fvec2 position;
    glm::fvec3 color;
  };

  // Temporary.
  struct UniformBufferObject {
    alignas(16) glm::fmat4 model;
    alignas(16) glm::fmat4 view;
    alignas(16) glm::fmat4 proj;
  };

  Application::Application(const CreateInfo& createInfo)
    : mTiming()
    , mCreateInfo(createInfo)
    , mExecutionState()
  {
    // First things first, make sure the environment isn't null.
    if (createInfo.appResidency == nullptr)
      throw std::runtime_error("Expected non-null application residency.");

    // Track this application.
    createInfo.appResidency->mRunningApps.push_back(this);
    EventBus::registerHandler(this, &Application::onEvent, 0);
  }

  Application::~Application() noexcept {
    // Untrack application.
    auto& apps = mCreateInfo.appResidency->mRunningApps;
    auto  itr  = std::find(apps.begin(), apps.end(), this);
    if (itr != apps.end())
      apps.erase(itr);
  }

  void Application::quit(bool shouldQuit) noexcept {
    mExecutionState.quitting = shouldQuit;
  }

  void Application::run() noexcept {
    // Try to initialize application.
    try {
      initialize();
    } catch (const std::runtime_error& err) {
      std::cerr << err.what() << std::endl;
      return;
    }

    // Show window.
    mMainWindow->show();

    // Prepare frame execution.
    mExecutionState.running = true;
    mTiming.start           = SteadyClock::now();

    // Execute frames.
    while (!mExecutionState.quitting)
      executeFrame();

    // We are finished.
    mExecutionState.running = false;
    mTiming.end             = SteadyClock::now();

    // Terminate.
    terminate();
  }

  Application::ExecutionPhase Application::phase() const noexcept {
    return mExecutionState.phase;
  }

  bool Application::running() const noexcept {
    return mExecutionState.running;
  }

  bool Application::quitting() const noexcept {
    return mExecutionState.quitting;
  }

  void Application::initializeWindow() {
    // Provide window create info.
    const Window::CreateInfo wndCreateInfo {
      .wndName       = mCreateInfo.appName,
      .wndResidency  = mCreateInfo.appResidency,
      .wndPosition   = glm::ivec2{ 640, 360 },
      .wndSize       = glm::uvec2{ 1280, 720 },
      .opacity       = 1.0f,
      .fullscreen    = false,
      .decorated     = true,
      .userResizable = false,
      .transparent   = false,
      .visible       = false
    };

    // Create Window.
    mMainWindow = Window::create(wndCreateInfo);
    if (mMainWindow == nullptr)
      throw std::runtime_error("Failed to create application window.");
  }

  void Application::initializeRenderContext() {
    // Provide render context create info.
    const gfx::RenderContext::CreateInfo rdrctxCreateInfo {
      .appName        = "Hearthfire",
      .surface        = mMainWindow,
      .appVersion     = Version::v1_0_0
    };

    // Create RenderContext.
    mRenderContext = std::make_unique<gfx::RenderContext>(rdrctxCreateInfo);
  }

  void Application::initializeSwapChain() {
    // Provide swapchain create info.
    const gfx::SwapChain::CreateInfo swpchnCreateInfo {
      .surfacePair     = std::pair{ mMainWindow, mRenderContext->surface() },
      .physicalDevice  = mRenderContext->physicalDevice(),
      .logicalDevice   = mRenderContext->logicalDevice(),
      .imageResolution = mMainWindow->size(),
      .imageFormat     = gfx::Format::B8G8R8A8unorm,
      .bufferStrategy  = gfx::BufferStrategy::DoubleBuffer,
      .vsyncEnabled    = false
    };

    // Create swapchain.
    mSwapChain = std::make_unique<gfx::SwapChain>(swpchnCreateInfo);
  }

  void Application::initializeRenderPass() {
    // Provide attachment descriptions.
    const gfx::AttachmentDescription colorAttachment {
      .format         = mSwapChain->imageFormat(),
      .loadOp         = gfx::AttachmentLoadOp::Clear,
      .storeOp        = gfx::AttachmentStoreOp::Store,
      .stencilLoadOp  = gfx::AttachmentLoadOp::DontCare,
      .stencilStoreOp = gfx::AttachmentStoreOp::DontCare,
      .initialLayout  = gfx::ImageLayout::Undefined,
      .finalLayout    = gfx::ImageLayout::PresentSrc
    };

    // Provide attachment references.
    const gfx::AttachmentReference colorAttachmentRef {
      .index  = 0,
      .layout = gfx::ImageLayout::ColorAttachmentOptimal
    };

    // Provide subpass description.
    const gfx::SubpassDescription subpass {
      .inputAttachmentRefs       = std::vector<gfx::AttachmentReference>{ },
      .colorAttachmentRefs       = std::vector{ colorAttachmentRef },
      .resolveAttachmentRef      = {},
      .depthStencilAttachmentRef = {},
      .pipelineBindPoint         = gfx::PipelineBindPoint::Graphics
    };

    // Provide renderpass create info.
    const gfx::RenderPass::CreateInfo rdrpssCreateInfo {
      .attachments   = std::vector{ &colorAttachment },
      .subpasses     = std::vector{ &subpass },
      .logicalDevice = mRenderContext->logicalDevice()
    };

    // Create renderpass.
    mRenderPass = std::make_unique<gfx::RenderPass>(rdrpssCreateInfo);
  }

  void Application::initializeFrameBuffers() {
    // Create framebuffers.
    const auto& imageViews      = mSwapChain->imageViews();
    const auto  imageResolution = mSwapChain->imageResolution();
    const auto  bufferStrategy  = mSwapChain->bufferStrategy();
    for (std::size_t index = 0; index < static_cast<std::size_t>(bufferStrategy); index++) {
      // Provide framebuffer create info.
      const gfx::FrameBuffer::CreateInfo frmbufCreateInfo {
        .attachments   = std::vector{ imageViews[index] },
        .resolution    = imageResolution,
        .logicalDevice = mRenderContext->logicalDevice(),
        .renderPass    = mRenderPass->handle()
      };

      // Create framebuffer.
      mFrameBuffers[index] = std::make_unique<gfx::FrameBuffer>(frmbufCreateInfo);
    }
  }

  void Application::initializeVertexBuffer() {
    // Provide vertices to render.
    const std::array<Vertex, 4> vertices {
      Vertex{ { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
      Vertex{ {  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
      Vertex{ {  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } },
      Vertex{ { -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } }
    };

    // Provide resource buffer create info.
    const gfx::ResourceBuffer::CreateInfo vertbuffCreateInfo {
      .physicalDevice = mRenderContext->physicalDevice(),
      .logicalDevice  = mRenderContext->logicalDevice(),
      .bufferSize     = sizeof(vertices[0]) * vertices.size(),
      .initialData    = vertices.data(),
      .bufferUsage    = gfx::ResourceBuffer::UsageVertexBufferBit
    };

    // Create resource buffer.
    mVertexBuffer = std::make_unique<gfx::ResourceBuffer>(vertbuffCreateInfo);
  }

  void Application::initializeIndexBuffer() {
    // Provide indices to render.
    const std::array<std::uint32_t, 6> indices {
      0, 1, 2, 2, 3, 0
    };

    // Provide index buffer create info.
    const gfx::ResourceBuffer::CreateInfo indbufCreateInfo {
      .physicalDevice = mRenderContext->physicalDevice(),
      .logicalDevice  = mRenderContext->logicalDevice(),
      .bufferSize     = sizeof(indices[0]) * indices.size(),
      .initialData    = indices.data(),
      .bufferUsage    = gfx::ResourceBuffer::UsageIndexBufferBit
    };

    // Create resource buffer.
    mIndexBuffer = std::make_unique<gfx::ResourceBuffer>(indbufCreateInfo);
  }

  void Application::initializeUniformBuffer() {
    // The data to store.
    UniformBufferObject object;

    // Provide uniform buffer create info.
    const gfx::ResourceBuffer::CreateInfo ufmbufCreateInfo {
      .physicalDevice = mRenderContext->physicalDevice(),
      .logicalDevice  = mRenderContext->logicalDevice(),
      .bufferSize     = sizeof(UniformBufferObject),
      .initialData    = &object,
      .bufferUsage    = gfx::ResourceBuffer::UsageUniformBufferBit |
                        gfx::ResourceBuffer::UsageTransferDstBit
    };

    // Create uniform buffer.
    mUniformBuffer = std::make_unique<gfx::ResourceBuffer>(ufmbufCreateInfo);
  }

  void Application::initializeDescriptorPool() {
    // Provide descriptor set size information.
    const gfx::DescriptorPool::SizeInfo descSizeInfo {
      .descriptorCount = 1,
      .descriptorType  = gfx::DescriptorType::UniformBuffer
    };

    // Provide descriptor pool create info.
    const gfx::DescriptorPool::CreateInfo dscpllCreateInfo {
      .sizeInformations = std::vector{ &descSizeInfo },
      .logicalDevice    = mRenderContext->logicalDevice(),
      .maxSets          = 1
    };

    // Create descriptor pool.
    mDescriptorPool = std::make_unique<gfx::DescriptorPool>(dscpllCreateInfo);
  }

  void Application::initializeDescriptorSetLayout() {
    // Provide descriptor binding.
    const gfx::DescriptorSetLayout::Binding descriptorBinding {
      .binding         = 0,
      .descriptorCount = 1,
      .stages          = gfx::ShaderStageVertexBit,
      .descriptorType  = gfx::DescriptorType::UniformBuffer
    };

    // Provide descriptor set layout create info.
    const gfx::DescriptorSetLayout::CreateInfo dslCreatInfo {
      .bindings      = std::vector{ &descriptorBinding },
      .logicalDevice = mRenderContext->logicalDevice()
    };

    // Create descriptor set layout.
    mDescriptorLayout = std::make_unique<gfx::DescriptorSetLayout>(dslCreatInfo);
  }

  void Application::initializeDescriptorSet() {
    // Provide descriptor set buffer info.
    const gfx::DescriptorSet::BufferInfo dscBufferInfo {
      .buffer       = mUniformBuffer.get(),
      .bufferOffset = 0,
      .bufferSize   = sizeof(UniformBufferObject),
      .binding      = 0
    };

    // Provide descriptor set create info.
    const gfx::DescriptorSet::CreateInfo dscsetCreateInfo {
      .bufferInfos      = std::vector{ &dscBufferInfo },
      .descriptorPool   = mDescriptorPool.get(),
      .descriptorLayout = mDescriptorLayout.get(),
      .logicalDevice    = mRenderContext->logicalDevice(),
      .descriptorType   = gfx::DescriptorType::UniformBuffer
    };

    // Create descriptor set.
    mUniformDescriptorSet = std::make_unique<gfx::DescriptorSet>(dscsetCreateInfo);
  }

  void Application::initializePipelineLayout() {
    // Prefetch descriptor layout pointer to make compiler happy.
    const auto* descriptorLayout = mDescriptorLayout.get();

    // Provide pipeline layout create info.
    const gfx::PipelineLayout::CreateInfo piplytCreateInfo {
      .descriptorLayouts = std::vector{ descriptorLayout },
      .logicalDevice     = mRenderContext->logicalDevice()
    };

    // Create pipeline layout.
    mPipelineLayout = std::make_unique<gfx::PipelineLayout>(piplytCreateInfo);
  }

  void Application::initializeGraphicsPipeline() {
    // Provide binding description.
    const gfx::BindingDescription bindingDesc {
      .binding = 0,
      .stride  = sizeof(Vertex)
    };

    // Provide position attribute description.
    const gfx::AttributeDescription posAttrDesc {
      .location = 0,
      .binding  = 0,
      .format   = gfx::Format::R32G32sfloat,
      .offset   = offsetof(Vertex, position)
    };

    // Provide color attribute description.
    const gfx::AttributeDescription colorAttrDesc {
      .location = 1,
      .binding  = 0,
      .format   = gfx::Format::R32G32B32sfloat,
      .offset   = offsetof(Vertex, color)
    };

    // Provide color blend attachment.
    const gfx::ColorBlendAttachment colorBlendAttachment {
      .srcColorFactor = gfx::BlendFactor::Zero,
      .dstColorFactor = gfx::BlendFactor::Zero,
      .colorOp        = gfx::BlendOp::Add,
      .srcAlphaFactor = gfx::BlendFactor::Zero,
      .dstAlphaFactor = gfx::BlendFactor::Zero,
      .alphaOp        = gfx::BlendOp::Add,
      .colorWriteMask = gfx::ColorComponentRBit |
                        gfx::ColorComponentGBit |
                        gfx::ColorComponentBBit |
                        gfx::ColorComponentABit,
      .blendEnabled   = false
    };

    // Provide color blend state.
    const gfx::ColorBlendState colorBlendState {
      .attachments    = std::vector{ &colorBlendAttachment },
      .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f },
      .logicOp        = gfx::LogicOp::Copy,
      .logicOpEnabled = false
    };

    // Provide graphics pipeline create info.
    const gfx::Pipeline::CreateInfo gfxpipCreateInfo {
      .vertexBindings   = std::vector{ &bindingDesc },
      .vertexAttributes = std::vector{ &posAttrDesc, &colorAttrDesc },
      .colorBlending    = &colorBlendState,
      .layout           = mPipelineLayout.get(),
      .base             = nullptr,
      .logicalDevice    = mRenderContext->logicalDevice(),
      .renderPass       = mRenderPass->handle(),
      .subpass          = 0,
      .lineWidth        = 1.0f,
      .topology         = gfx::TopologyType::TriangleList,
      .polygonMode      = gfx::PolygonMode::Fill,
      .cullMode         = gfx::FaceCullMode::Back,
      .frontFace        = gfx::FrontFace::CounterClockwise
    };

    // Create graphics pipeline.
    mGraphicsPipeline = std::make_unique<gfx::Pipeline>(gfxpipCreateInfo);
  }

  void Application::initializeCommandPool() {
    // Provide command pool create info.
    const gfx::CommandPool::CreateInfo cmdpllCreateInfo {
      .logicalDevice = mRenderContext->logicalDevice(),
      .queueIndex    = mRenderContext->graphicsQueueIndex()
    };

    // Create command pool.
    mCommandPool = std::make_unique<gfx::CommandPool>(cmdpllCreateInfo);
  }

  void Application::initializeCommandBuffer() {
    // Provide command buffer create info.
    const gfx::CommandBuffer::CreateInfo cmdbufCreateInfo {
      .commandPool   = mCommandPool.get(),
      .logicalDevice = mRenderContext->logicalDevice()
    };

    // Create command buffers and semaphores.
    mCommandBuffer = std::make_unique<gfx::CommandBuffer>(cmdbufCreateInfo);
  }

  void Application::initialize() {
    initializeWindow();
    initializeRenderContext();
    initializeSwapChain();
    initializeRenderPass();
    initializeFrameBuffers();
    initializeVertexBuffer();
    initializeIndexBuffer();
    initializeUniformBuffer();
    initializeDescriptorPool();
    initializeDescriptorSetLayout();
    initializeDescriptorSet();
    initializePipelineLayout();
    initializeGraphicsPipeline();
    initializeCommandPool();
    initializeCommandBuffer();
  }

  void Application::terminate() noexcept {
    mCommandBuffer.reset();
    mCommandPool.reset();
    mGraphicsPipeline.reset();
    mPipelineLayout.reset();
    mUniformDescriptorSet.reset();
    mDescriptorLayout.reset();
    mDescriptorPool.reset();
    mUniformBuffer.reset();
    mIndexBuffer.reset();
    mVertexBuffer.reset();
    for (auto& framebuffer : mFrameBuffers)
      framebuffer.reset();

    mRenderPass.reset();
    mSwapChain.reset();
    mRenderContext.reset();
  }

  void Application::executeFrame() noexcept {
    // Get start of frame.
    const auto frameStart = SteadyClock::now();

    // Poll events.
    mCreateInfo.appResidency->pollEvents();

    // Get our delta time.
          using fsec         = std::chrono::duration<float>;
    const auto  delta        = std::chrono::duration_cast<fsec>(mTiming.lastFrameDelta);
    const auto  deltaElapsed = std::chrono::duration_cast<fsec>(frameStart - mTiming.start).count();

    // Perform updates.
    // Update(delta);
    // Render(delta);

    UniformBufferObject ubo;
    {
      const auto extent = mSwapChain->imageResolution();
      ubo.model = glm::rotate(glm::fmat4(1.0f), deltaElapsed * glm::radians(90.0f), glm::fvec3(0.0f, 0.0f, 1.0f));
      ubo.view  = glm::lookAt(glm::fvec3(2.0f, 2.0f, 2.0f), glm::fvec3(0.0f, 0.0f, 0.0f), glm::fvec3(0.0f, 0.0f, 1.0f));
      ubo.proj  = glm::perspective(glm::radians(45.0f), static_cast<float>(extent.x) / static_cast<float>(extent.y), 0.1f, 10.0f);
    }

    // Provide renderpass begin info.
    const gfx::BeginRenderPassInfo brpi {
      .renderPass       = mRenderPass.get(),
      .frameBuffer      = mFrameBuffers[mTiming.framesElapsed % 2].get(),
      .renderAreaExtent = mSwapChain->imageResolution()
    };

    // Get swapchain size.
    const auto resolution = mSwapChain->imageResolution();

    // Provide viewport.
    const gfx::Viewport viewport {
      .origin   = glm::ivec2{ 0,             resolution.y },
      .extent   = glm::ivec2{ resolution.x, -resolution.y },
      .minDepth = 0.0f,
      .maxDepth = 1.0f,
    };

    // Provide scissor.
    const gfx::Scissor scissor {
      .offset = glm::ivec2{ 0, 0 },
      .extent = resolution
    };

    mCommandBuffer->begin();
    mCommandBuffer->updateBuffer(mUniformBuffer.get(), 0, &ubo, sizeof(UniformBufferObject));
    mCommandBuffer->beginRenderPass(brpi);
    mCommandBuffer->bindPipeline(mGraphicsPipeline.get(), gfx::PipelineBindPoint::Graphics);
    mCommandBuffer->updateViewport(viewport);
    mCommandBuffer->updateScissor(scissor);
    mCommandBuffer->bindVertexBuffer(mVertexBuffer.get());
    mCommandBuffer->bindIndexBuffer(mIndexBuffer.get());
    mCommandBuffer->bindDescriptorSet(mUniformDescriptorSet.get(), mPipelineLayout.get());
    mCommandBuffer->drawIndexed(6, 0, 0);
    mCommandBuffer->endRenderPass();
    mCommandBuffer->end();
    mCommandBuffer->submit(mRenderContext->graphicsQueue());
    mSwapChain->present(mRenderContext->presentQueue());

    // Temporarily simulate frame execution, sleep half a frame time.
    std::this_thread::sleep_for(std::chrono::milliseconds(6));

    // Finish off frame and increment frame count.
    const auto frameEnd = SteadyClock::now();
    mTiming.lastFrameDelta = frameEnd - frameStart;
    mTiming.framesElapsed++;
  }

  void Application::onEvent(Event& evnt) noexcept {
    switch (evnt.type()) {
    case EventType::WindowClose:
      onWindowClose(static_cast<WindowCloseEvent&>(evnt));
      break;
    default:
      // Re-emit to component systems.
      break;
    }
  }

  void Application::onWindowClose(WindowCloseEvent& evnt) noexcept {
    quit(evnt.origin() == mMainWindow);
    evnt.consume();
  }

}
