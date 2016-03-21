#pragma once

#include "header.h"

#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}
const int GEOMETRY_COUNT = 2;

ID2D1Factory*           g_pD2DFactory = NULL; // Direct2D factory
ID2D1HwndRenderTarget*  g_pRenderTarget = NULL; // Render target
ID2D1SolidColorBrush*   g_pBlackBrush = NULL; // Outline brush
ID2D1RadialGradientBrush* g_pRadialGradientBrush = NULL; // Radial gradient brush

														 // 2 circle to build up a geometry group. this is the outline of the progress bar
D2D1_ELLIPSE g_Ellipse0 = D2D1::Ellipse(D2D1::Point2F(500, 50), 7, 7);
D2D1_ELLIPSE g_Ellipse1 = D2D1::Ellipse(D2D1::Point2F(500, 50), 10, 10);

D2D1_ELLIPSE g_Ellipse[GEOMETRY_COUNT] =
{
	g_Ellipse0,
	g_Ellipse1,
};

ID2D1EllipseGeometry* g_pEllipseArray[GEOMETRY_COUNT] = { NULL };
ID2D1GeometryGroup* g_pGeometryGroup = NULL;

VOID CreateD2DResource(HWND hWnd) //생성
{
	if (!g_pRenderTarget)
	{
		HRESULT hr;

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
		if (FAILED(hr))
		{
			MessageBox(hWnd, TEXT("Create D2D factory failed!"), TEXT("Error"), 0);
			return;
		}

		// Obtain the size of the drawing area
		RECT rc;
		GetClientRect(hWnd, &rc);

		// Create a Direct2D render target
		hr = g_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hWnd,
				D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
				),
			&g_pRenderTarget
			);
		if (FAILED(hr))
		{
			MessageBox(hWnd, TEXT("Create render target failed!"), TEXT("Error"), 0);
			return;
		}

		// Create the outline brush(black)
		hr = g_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black), //테두리
			&g_pBlackBrush
			);
		if (FAILED(hr))
		{
			MessageBox(hWnd, TEXT("Create outline brush(black) failed!"), TEXT("Error"), 0);
			return;
		}

		// Define gradient stops
		D2D1_GRADIENT_STOP gradientStops[2];
		gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::LimeGreen);
		gradientStops[0].position = 1.f;
		gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::White);
		gradientStops[1].position = 1.f;

		// Create gradient stops collection
		ID2D1GradientStopCollection* pGradientStops = NULL;
		hr = g_pRenderTarget->CreateGradientStopCollection(
			gradientStops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pGradientStops
			);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("Create gradient stops collection failed!"), TEXT("Error"), 0);
		}

		// Create a linear gradient brush to fill in the ellipse
		hr = g_pRenderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(370, 370),
				D2D1::Point2F(0, 0),	350, 350), //이것은 늘려줘야 한다.
			pGradientStops,
			&g_pRadialGradientBrush
			);

		if (FAILED(hr))
		{
			MessageBox(hWnd, TEXT("Create linear gradient brush failed!"), TEXT("Error"), 0);
			return;
		}

		// Create the 2 ellipse.
		for (int i = 0; i < GEOMETRY_COUNT; ++i)
		{
			hr = g_pD2DFactory->CreateEllipseGeometry(g_Ellipse[i], &g_pEllipseArray[i]);
			if (FAILED(hr))
			{
				MessageBox(hWnd, TEXT("Create Ellipse Geometry failed!"), TEXT("Error"), 0);
				return;
			}
		}

		// Create the geometry group, the 2 circles make up a group.
		hr = g_pD2DFactory->CreateGeometryGroup(
			D2D1_FILL_MODE_ALTERNATE,
			(ID2D1Geometry**)&g_pEllipseArray,
			ARRAYSIZE(g_pEllipseArray),
			&g_pGeometryGroup
			);
	}
}

VOID Render(HWND hwnd)
{
	// total angle to rotate
	static float totalAngle = 0.0f;

	// Get last time
	static DWORD lastTime = timeGetTime();

	// Get current time
	DWORD currentTime = timeGetTime();

	// Calculate time elapsed in current frame.
	float timeDelta = (float)(currentTime - lastTime) * 0.1;

	// Increase the totalAngle by the time elapsed in current frame.
	totalAngle += timeDelta;

	CreateD2DResource(hwnd);

	g_pRenderTarget->BeginDraw();

	// Clear background color to White
	g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	// Draw geometry group
	g_pRenderTarget->DrawGeometry(g_pGeometryGroup, g_pBlackBrush);

	// Roatate the gradient brush based on the total elapsed time
	D2D1_MATRIX_3X2_F rotMatrix = D2D1::Matrix3x2F::Rotation(totalAngle, D2D1::Point2F(500, 50));
	g_pRadialGradientBrush->SetTransform(rotMatrix);

	// Fill geometry group with the transformed brush
	g_pRenderTarget->FillGeometry(g_pGeometryGroup, g_pRadialGradientBrush);

	HRESULT hr = g_pRenderTarget->EndDraw();
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Draw failed!"), TEXT("Error"), 0);
		return;
	}

	// Update last time to current time for next loop
	lastTime = currentTime;
}

VOID Cleanup()
{
	SAFE_RELEASE(g_pRenderTarget);
	SAFE_RELEASE(g_pBlackBrush);
	SAFE_RELEASE(g_pGeometryGroup);
	SAFE_RELEASE(g_pRadialGradientBrush);

	for (int i = 0; i < GEOMETRY_COUNT; ++i)
	{
		SAFE_RELEASE(g_pEllipseArray[i]);
		g_pEllipseArray[i] = NULL;
	}

	SAFE_RELEASE(g_pD2DFactory);
}
