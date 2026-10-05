#include "pch.h"
#include "Layer.h"

namespace jug
{

Layer::Layer(
    const StringView _name)
    : m_name(_name)
{
}

void Layer::OnAttach()
{
}

void Layer::OnDetach()
{
}

void Layer::OnFixedUpdate(
    float)
{
}

void Layer::OnUpdate(
    const float)
{
}

void Layer::OnRender()
{
}

void Layer::OnRenderGUI()
{
}

void Layer::OnEvent(
    Event&)
{
}

StringView Layer::GetName() const
{
    return m_name;
}

void Layer::SetEnable(
    const bool _bEnable)
{
    m_bEnable = _bEnable;
}

bool Layer::IsEnabled() const
{
    return m_bEnable;
}

}   // namespace jug
