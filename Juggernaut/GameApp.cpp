#include "pch.h"
#include "GameApp.h"

#include <ranges>

namespace jug
{

GameApp::GameApp(
    const AppConfig& _config)
    : App(_config)
{
}

void GameApp::Init()
{
    Flags<eGraphicsOption> flags = kNoneFlags;
#ifdef JUG_DEBUG
    flags |= eGraphicsOption::Debug;
#endif
    m_pGraphics = MakeScoped<Graphics>(flags);
}

void GameApp::Shutdown()
{
    ClearLayers();
    m_pGraphics.Reset();
}

void GameApp::Update(
    const float _deltaTimeSec)
{
    const float frameTimeSec = Min(_deltaTimeSec, m_maxFrameTimeSec);
    m_frameTimeAccSec += frameTimeSec;

    ++m_layerIterDepth;

    while (m_frameTimeAccSec >= m_fixedFrameTimeSec)
    {
        for (Scoped<Layer>& pLayer: m_layers)
        {
            if (pLayer->IsEnabled())
            {
                pLayer->OnFixedUpdate(m_fixedFrameTimeSec);
            }
        }
        m_frameTimeAccSec -= m_fixedFrameTimeSec;
    }

    for (Scoped<Layer>& pLayer: m_layers)
    {
        if (pLayer->IsEnabled())
        {
            pLayer->OnUpdate(frameTimeSec);
        }
    }

    for (Scoped<Layer>& pLayer: m_layers)
    {
        if (pLayer->IsEnabled())
        {
            pLayer->OnRender();
        }
    }

    for (Scoped<Layer>& pLayer: m_layers)
    {
        if (pLayer->IsEnabled())
        {
            pLayer->OnRenderGUI();
        }
    }

    --m_layerIterDepth;

    m_pGraphics->Frame();
}

void GameApp::OnEvent(
    Event& _event)
{
    ++m_layerIterDepth;

    // 이벤트의 처리는 최상위 -> 최하위
    for (Scoped<Layer>& pLayer: m_layers | std::views::reverse)
    {
        if (_event.IsHandled())
        {
            break;
        }

        if (pLayer->IsEnabled())
        {
            pLayer->OnEvent(_event);
        }
    }

    --m_layerIterDepth;
}

void GameApp::SetMaxFrameTime(
    const float _sec)
{
    JUG_ASSERT(_sec > 0.f, "Max frame time must be greater than 0.\n");
    m_maxFrameTimeSec = _sec;
}

void GameApp::SetFixedFrameTime(
    const float _sec)
{
    JUG_ASSERT(_sec > 0.f, "Fixed frame time must be greater than 0.\n");
    m_fixedFrameTimeSec = _sec;
}

float GameApp::GetMaxFrameTimeSec() const
{
    return m_maxFrameTimeSec;
}

float GameApp::GetFixedFrameTimeSec() const
{
    return m_fixedFrameTimeSec;
}

float GameApp::GetInterpolationAlpha() const
{
    return m_frameTimeAccSec / m_fixedFrameTimeSec;
}

Layer* GameApp::AttachLayer(
    Scoped<Layer>&& _pLayer,
    const bool      _bAttachOnTop)
{
    JUG_ASSERT(_pLayer, "Layer is null.\n");
    JUG_ASSERT(m_layerIterDepth == 0, "Cannot attach layer during layer iteration. Use ExecuteOnMainThread.\n");

    const auto insertIt = _bAttachOnTop ? m_layers.end() : m_layers.begin();
    const auto it       = m_layers.insert(insertIt, std::move(_pLayer));
    (*it)->OnAttach();
    return it->GetPtr();
}

void GameApp::DetachLayer(
    Layer* _pLayer)
{
    JUG_ASSERT(_pLayer, "Layer is null.\n");
    JUG_ASSERT(m_layerIterDepth == 0, "Cannot detach layer during layer iteration. Use ExecuteOnMainThread.\n");

    const auto it = std::ranges::find_if(m_layers, [_pLayer](const Scoped<Layer>& pLayer) { return pLayer.GetPtr() == _pLayer; });
    JUG_ASSERT(it != m_layers.end(), "Layer not found.\n");

    _pLayer->OnDetach();
    m_layers.erase(it);
}

Span<const Scoped<Layer>> GameApp::GetLayers() const
{
    return m_layers;
}

void GameApp::ClearLayers()
{
    JUG_ASSERT(m_layerIterDepth == 0, "Cannot clear layers during layer iteration. Use ExecuteOnMainThread.\n");

    for (Scoped<Layer>& pLayer: m_layers)
    {
        pLayer->OnDetach();
    }

    m_layers.clear();
}

}   // namespace jug
