#pragma once
#include "Layer.h"

namespace jug
{

class GameApp : public App
{
    JUG_CLASS(GameApp, NO_COPY, NO_MOVE)

public:
    explicit GameApp(const AppConfig& _config);
    ~GameApp() override = default;

    void Init() override;
    void Shutdown() override;
    void OnEvent(Event& _event) override;
    void Update(float _deltaTimeSec) override;

    // ===========================================
    //  Frame Time
    // ===========================================

    void                SetMaxFrameTime(float _sec);
    void                SetFixedFrameTime(float _sec);
    [[nodiscard]] float GetMaxFrameTimeSec() const;
    [[nodiscard]] float GetFixedFrameTimeSec() const;
    [[nodiscard]] float GetInterpolationAlpha() const;   // [0, 1)

    // ===========================================
    //  Layer
    // ===========================================

    Layer*                                  AttachLayer(Scoped<Layer>&& _pLayer, bool _bAttachOnTop = false);
    void                                    DetachLayer(Layer* _pLayer);
    [[nodiscard]] Span<const Scoped<Layer>> GetLayers() const;
    void                                    ClearLayers();

private:
    Scoped<Graphics> m_pGraphics = nullptr;

    float m_maxFrameTimeSec   = 1.f / 5.f;    // 5 fps
    float m_fixedFrameTimeSec = 1.f / 60.f;   // 60 fps
    float m_frameTimeAccSec   = 0.f;

    Vector<Scoped<Layer>> m_layers         = {};
    int                   m_layerIterDepth = 0;
};

[[nodiscard]] inline GameApp& GetGameApplication()
{
    return static_cast<GameApp&>(App::GetSingleton());
}

}   // namespace jug
