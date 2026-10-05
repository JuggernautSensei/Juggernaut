#pragma once

namespace jug
{

class Layer
{
    JUG_CLASS(Layer, NO_COPY, NO_MOVE)

public:
    explicit Layer(StringView _name);
    virtual ~Layer() = default;

    virtual void OnAttach();
    virtual void OnDetach();

    virtual void OnFixedUpdate(float _fixedDeltaTimeSec);
    virtual void OnUpdate(float _deltaTimeSec);

    virtual void OnRender();
    virtual void OnRenderGUI();

    virtual void OnEvent(Event& _event);

    [[nodiscard]] StringView GetName() const;

    void               SetEnable(bool _bEnable);
    [[nodiscard]] bool IsEnabled() const;

private:
    String m_name    = {};
    bool   m_bEnable = true;
};

}   // namespace jug
