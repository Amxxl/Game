//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0601
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>

#if defined(NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_5.h>
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <fstream>

#include <stdio.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#pragma warning(disable: 4100) // Disable unreferenced parameters warning.
#pragma warning(disable: 4458) // Disable same named class members warning.

// DirectXTK headers
//#include "Audio.h"
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
//#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
//#include "XboxDDSTextureLoader.h"

#include <chrono>
#include <sstream>

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) : result(hr) {}

        virtual char const* what() const override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }

    // Helper to get device from device context.
    inline ID3D11Device* GetDevice(ID3D11DeviceContext* deviceContext)
    {
        ID3D11Device* device = nullptr;
        deviceContext->GetDevice(&device);
        return device;
    }

    class ExecutionTimer
    {
        public:
            ExecutionTimer()
            {
                begin = std::chrono::high_resolution_clock::now();
            }

            ~ExecutionTimer()
            {
                end = std::chrono::high_resolution_clock::now();
                duration = end - begin;

                float ms = duration.count() * 1000.0f;

                std::wstringstream ss(L"");
                ss  << L"Execution Time: " << ms << " ms. ";

                OutputDebugString(ss.str().c_str());
            }
        private:
            std::chrono::time_point<std::chrono::steady_clock> begin, end;
            std::chrono::duration<float> duration;
    };
}