#pragma once

class SandboxApp : public jug::GameApp
{
    JUG_CLASS(SandboxApp, NO_COPY, NO_MOVE)
    using Super = jug::GameApp;

public:
    explicit SandboxApp(const jug::AppConfig& _config);
    ~SandboxApp() override = default;

    void Init() override;
};
