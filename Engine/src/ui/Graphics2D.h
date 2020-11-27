#pragma once

#include "core/Pointer.h"
#include "graphics/Graphics.h"
#include "graphics/buffer/RenderTarget.h"

#include <dxgi1_6.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <d2d1_3.h>
#include <dwrite.h>
#include <d3d11on12.h>
#include <DirectXMath.h>

#include <wrl.h>

namespace Ghurund::UI {
    using namespace DirectX;
    using Microsoft::WRL::ComPtr;

    enum class UIState {
        RECORDING, IDLE
    };

    class Graphics2D:public Pointer {
    private:
        ComPtr<ID2D1DeviceContext2> deviceContext;
        ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;
        ComPtr<ID3D11On12Device> m_d3d11On12Device;
        ComPtr<IDWriteFactory> m_dwriteFactory;
        ComPtr<ID2D1Factory3> m_d2dFactory;
        ComPtr<ID2D1Device2> m_d2dDevice;
        UIState state = UIState::IDLE;
        RenderTarget* currentTarget = nullptr;

    public:
        ~Graphics2D() {
            uninit();
        }

        inline ID2D1Device2* getDevice() {
            return m_d2dDevice.Get();
        };

        __declspec(property(get = getDevice)) ID2D1Device2* Device;

        inline ID2D1DeviceContext2* getDeviceContext() {
            return deviceContext.Get();
        };

        __declspec(property(get = getDeviceContext)) ID2D1DeviceContext2* DeviceContext;

        inline ID3D11On12Device* getDevice11() {
            return m_d3d11On12Device.Get();
        };

        __declspec(property(get = getDevice11)) ID3D11On12Device* Device11;

        inline ID2D1Factory3* getFactory() {
            return m_d2dFactory.Get();
        };

        __declspec(property(get = getFactory)) ID2D1Factory3* Factory;

        inline IDWriteFactory* getDWriteFactory() {
            return m_dwriteFactory.Get();
        };

        __declspec(property(get = getDWriteFactory)) IDWriteFactory* DWriteFactory;

        inline UIState getState() const {
            return state;
        }

        __declspec(property(get = getState)) UIState State;

        Status init(Graphics& graphics);

        void uninit();

        Status beginPaint(RenderTarget& target);

        Status endPaint();
    };
}