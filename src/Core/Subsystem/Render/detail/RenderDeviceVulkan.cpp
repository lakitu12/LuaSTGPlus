/**
 * @file
 * @date 2022/3/9
 * @author 9chu
 * 这个文件是 LuaSTGPlus 项目的一部分，请在项目所定义之授权许可范围内合规使用。
 */
#include "RenderDeviceVulkan.hpp"

#include <vector>
#include <SDL_syswm.h>

#if VULKAN_SUPPORTED == 1
#include <EngineFactoryVk.h>
#endif

using namespace std;
using namespace lstg;
using namespace lstg::Subsystem::Render::detail;
using namespace Diligent;

#if VULKAN_SUPPORTED == 1

RenderDeviceVulkan::RenderDeviceVulkan(WindowSystem* window)
{
    auto windowSize = window->GetRenderSize();

    // 获取 SDL 原生窗口
    assert(window);
    SDL_SysWMinfo systemWindowInfo;
    SDL_VERSION(&systemWindowInfo.version);
    if (SDL_FALSE == ::SDL_GetWindowWMInfo(window->GetNativeHandle(), &systemWindowInfo))
        LSTG_THROW(RenderDeviceInitializeFailedException, "SDL_GetWindowWMInfo fail, SDL_GetError: {}", SDL_GetError());

    // 创建上下文窗口
    NativeWindow nativeWindow;
#if defined(LSTG_PLATFORM_WIN32)
    nativeWindow = Win32NativeWindow {systemWindowInfo.info.win.window};
#elif defined(LSTG_PLATFORM_MACOS)
    // 创建 MetalView
    m_stView = make_unique<OSX::MetalView>(systemWindowInfo.info.cocoa.window, window->GetFeatures() & WindowFeatures::HighDPISupport);
    nativeWindow = MacOSNativeWindow {m_stView->GetView()};
#else
    #error "Unsupported platform"
#endif

    // 获取 Factory
    auto* factory = GetEngineFactoryVk();
    assert(factory);

    // 创建引擎
    SwapChainDesc swapChainDesc;
    swapChainDesc.Width = std::get<0>(windowSize);
    swapChainDesc.Height = std::get<1>(windowSize);
#if LSTG_PLATFORM_MACOS
    // We need at least 3 buffers in Metal to avoid massive
    // performance degradation in full screen mode.
    // https://github.com/KhronosGroup/MoltenVK/issues/808
    swapChainDesc.BufferCount = 3;
#endif
    EngineVkCreateInfo engineCreateInfo;
#ifdef LSTG_DEVELOPMENT
    engineCreateInfo.SetValidationLevel(VALIDATION_LEVEL_1);
#endif
    factory->CreateDeviceAndContextsVk(engineCreateInfo, &m_pRenderDevice, &m_pRenderContext);
    if (!m_pRenderDevice)
        LSTG_THROW(RenderDeviceInitializeFailedException, "Unable to initialize Vulkan render device");
    if (!m_pSwapChain)
        factory->CreateSwapChainVk(m_pRenderDevice, m_pRenderContext, swapChainDesc, nativeWindow, &m_pSwapChain);
    if (!m_pSwapChain)
        LSTG_THROW(RenderDeviceInitializeFailedException, "Unable to initialize swap chain");
}

#endif
