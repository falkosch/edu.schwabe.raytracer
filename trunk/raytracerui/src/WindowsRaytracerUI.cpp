#include "WindowsRaytracerUI.h"
#include "stdafx.h"

#include <windowsx.h>

namespace raytracerui
{

	const Float WindowsRaytracerUI::MOUSE_SENSITIVITY = .02f;

	WNDCLASSEX WindowsRaytracerUI::windowClass;

	WindowsRaytracerUI::WindowsRaytracerUI(
		Raytracer & raytracerIn,
		const RaytraceParameters & parametersIn,
		const ASizeT & fastPreviewSizeIn)
		:
		RaytracerUI(raytracerIn, parametersIn, fastPreviewSizeIn),
		previousMousePosition(),
		activeDrag(DragTypes::None),
		hWnd()
	{
		InitWindow();
	}

	WindowsRaytracerUI::~WindowsRaytracerUI()
	{
		DestroyWindow(hWnd);
	}

	void WindowsRaytracerUI::InitWindowClass()
	{
		ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = static_cast<UINT>(CS_OWNDC | CS_HREDRAW | CS_VREDRAW);
		windowClass.lpfnWndProc = DelegatingWndProc;
		windowClass.hInstance = GetModuleHandle(nullptr);
		windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		windowClass.lpszClassName = TEXT("WindowsRaytracerUIWndClass");
		windowClass.hIconSm = windowClass.hIcon;
		RegisterClassEx(&windowClass);
	}

	void WindowsRaytracerUI::InitWindow()
	{
		InitWindowClass();
		hWnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			windowClass.lpszClassName,
			TEXT("WindowsRaytracerUI"),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			528,
			551,
			nullptr,
			nullptr,
			windowClass.hInstance,
			this
		);
		ShowWindow(hWnd, SW_SHOWDEFAULT);
	}

	LRESULT WindowsRaytracerUI::run()
	{
		BOOL bRet;
		MSG msg;
		LRESULT returnCode = Zero<LRESULT>();
		while (Zero<BOOL>() != (bRet = GetMessage(&msg, hWnd, Zero<UINT>(), Zero<UINT>()))) {
			if (bRet == NegativeOne<BOOL>()) {
				std::cerr << "GetMessage failure " << bRet << std::endl;
				break;
			}
			else {
				TranslateMessage(&msg);
				returnCode = DispatchMessage(&msg);
			}
		}
		return returnCode;
	}

	void WindowsRaytracerUI::delegateReshape()
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		reshape(Int2(rect.right, rect.bottom) - Int2(rect.left, rect.top));
	}

	void WindowsRaytracerUI::triggerRaytracing(const bool fastPreview)
	{
		RaytracerUI::triggerRaytracing(fastPreview);
		InvalidateRect(hWnd, nullptr, false);
	}

	void WindowsRaytracerUI::display()
	{
		raytracer->requestUpdate();
		if (output) {
			//output->saveAsBMP("output.bmp");

			const BITMAPINFO bitmapInfo = output->getBITMAPINFO();
			const BITMAP bitmap = output->getBITMAP();

			PAINTSTRUCT psPaint;
			ZeroMemory(&psPaint, sizeof(PAINTSTRUCT));

			const HDC hdc = BeginPaint(hWnd, &psPaint);
			StretchDIBits(
				hdc,
				Zero<int>(),
				Zero<int>(),
				x(screenSize),
				y(screenSize),
				Zero<int>(),
				Zero<int>(),
				bitmap.bmWidth,
				bitmap.bmHeight,
				bitmap.bmBits,
				&bitmapInfo,
				DIB_RGB_COLORS,
				SRCCOPY
			);
			EndPaint(hWnd, &psPaint);
		}
	}

	void WindowsRaytracerUI::keyPressed(const WPARAM wParam)
	{
		switch (wParam) {

		case 'Q':
			parameters.cullingOrientation = ((parameters.cullingOrientation + Two<Int>()) % 3) - One<Int>();
			std::cout << "Culling orientation: " << parameters.cullingOrientation << std::endl;
			break;

		case 'W':
			std::cout << "Saving raytraced image ...";
			output->saveAsBMP("raytraced.bmp");
			std::cout << " raytraced.bmp" << std::endl;
			break;

		case 'E':
			showMapIndex = (showMapIndex + One<ASizeT>()) % static_cast<ASizeT>(3);
			break;

		case 'R':
			parameters.camera->resetView();
			break;

		case 'T':
			disableFastPreview = !disableFastPreview;
			std::cout << "Fast preview disabled: " << disableFastPreview << std::endl;
			break;

		case 'A':
			parameters.samplingFactor *= Half<Float>();
			std::cout << "Sampling-factor: " << parameters.samplingFactor << std::endl;
			break;

		case 'S':
			parameters.samplingFactor += One<Float>();
			std::cout << "Sampling-factor: " << parameters.samplingFactor << std::endl;
			break;

		case 'D':
			parameters.maxTraceDepth -= select(parameters.maxTraceDepth > Zero<ASizeT>(), One<ASizeT>(), Zero<ASizeT>());
			std::cout << "Max trace-depth: " << parameters.maxTraceDepth << std::endl;
			break;

		case 'F':
			parameters.maxTraceDepth += One<ASizeT>();
			std::cout << "Max trace-depth: " << parameters.maxTraceDepth << std::endl;
			break;

		case 'G':
			parameters.rayPacketSize -= select(parameters.rayPacketSize > Zero<ASizeT>(), One<ASizeT>(), Zero<ASizeT>());
			std::cout << "Ray packet size: " << parameters.rayPacketSize << std::endl;
			break;

		case 'H':
			parameters.rayPacketSize += One<ASizeT>();
			std::cout << "Ray packet size: " << parameters.rayPacketSize << std::endl;
			break;

		case 'J':
			parameters.supersamplingFactor -= select(parameters.supersamplingFactor > Zero<ASizeT>(), One<ASizeT>(), Zero<ASizeT>());
			std::cout << "Supersampling factor: " << parameters.supersamplingFactor << std::endl;
			break;

		case 'K':
			parameters.supersamplingFactor += One<ASizeT>();
			std::cout << "Supersampling factor: " << parameters.supersamplingFactor << std::endl;
			break;

		case VK_SPACE:
			break;

		case VK_RETURN:
			triggerRaytracing(false);

		default:
			return;
		}

		triggerRaytracing(true);
	}

	void WindowsRaytracerUI::mousePressed(const MouseButtons button, const ButtonStates state, const Int2 & position)
	{
		previousMousePosition = position;

		switch (button) {

		case MouseButtons::Left:
			if (state == ButtonStates::Down) {
				if (GetKeyState(VK_CONTROL) & 0x8000)
					activeDrag = DragTypes::ShiftXY;
				else if (GetKeyState(VK_SHIFT) & 0x8000)
					activeDrag = DragTypes::ShiftZ;
				else if (GetKeyState(VK_MENU) & 0x8000) // ALT key
					activeDrag = DragTypes::Scale;
				else
					activeDrag = DragTypes::Rotate;
			}
			else if (state == ButtonStates::Up) {
				activeDrag = DragTypes::None;
			}
			break;

		case MouseButtons::Right:
			if (state == ButtonStates::Down) {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
					activeDrag = DragTypes::Object;
				}
				else {
					activeDrag = DragTypes::Light;
				}
			}
			else if (state == ButtonStates::Up) {
				activeDrag = DragTypes::None;
			}
			break;

		default:
			break;
		}
	}

	void WindowsRaytracerUI::mouseDragged(const Int2 & position)
	{
		const Float2 delta = convert<Float2>(position - previousMousePosition);
		previousMousePosition = position;

		switch (activeDrag) {

		case DragTypes::Rotate:
			parameters.camera->rotate(Float3(x(delta), y(delta)));
			triggerRaytracing(true);
			break;

		case DragTypes::ShiftXY:
			parameters.camera->translate(Float3(-x(delta), y(delta)) * MOUSE_SENSITIVITY);
			triggerRaytracing(true);
			break;

		case DragTypes::ShiftZ:
			parameters.camera->translate(Float3(Zero<Float>(), Zero<Float>(), x(delta) - y(delta)) * MOUSE_SENSITIVITY);
			triggerRaytracing(true);
			break;

		case DragTypes::Scale:
			parameters.camera->scale(Float3((x(delta) + y(delta)) * MOUSE_SENSITIVITY) + Float3(One<Float3::ValueType>()));
			triggerRaytracing(true);
			break;

		case DragTypes::Light:
		{
			Scene::LightsCollection & lights = parameters.sceneShader->getLights();
			lights[lights.size() - One<ASizeT>()]->position += Float4(Zero<Float>(), Zero<Float>(), x(delta) - y(delta)) * MOUSE_SENSITIVITY;
		}
		triggerRaytracing(true);
		break;

		case DragTypes::Object:
		{
			Scene*const scene = static_cast<Scene*const>(parameters.sceneShader);
			Scene::SceneList &sceneObjects = scene->getSceneObjects();
			SceneObject &lastSceneObject = *sceneObjects[sceneObjects.size() - One<ASizeT>()];
			lastSceneObject.translate(Float3(Zero<Float>(), Zero<Float>(), x(delta) - y(delta)) * MOUSE_SENSITIVITY);
		}
		triggerRaytracing(true);
		break;

		case DragTypes::None:
		default:
			break;
		}
	}

	LRESULT WindowsRaytracerUI::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {

		case WM_CLOSE:
			PostQuitMessage(Zero<int>());
			break;

		case WM_ERASEBKGND:
			return Zero<LRESULT>();

		case WM_PAINT:
			display();
			return Zero<LRESULT>();

		case WM_SIZE:
			delegateReshape();
			return Zero<LRESULT>();

		case WM_LBUTTONDOWN:
			mousePressed(MouseButtons::Left, ButtonStates::Down, getXY(lParam));
			return Zero<LRESULT>();

		case WM_LBUTTONUP:
			mousePressed(MouseButtons::Left, ButtonStates::Up, getXY(lParam));
			return Zero<LRESULT>();

		case WM_RBUTTONDOWN:
			mousePressed(MouseButtons::Right, ButtonStates::Down, getXY(lParam));
			return Zero<LRESULT>();

		case WM_RBUTTONUP:
			mousePressed(MouseButtons::Right, ButtonStates::Up, getXY(lParam));
			return Zero<LRESULT>();

		case WM_MOUSEMOVE:
			mouseDragged(getXY(lParam));
			return Zero<LRESULT>();

		case WM_KEYUP:
			keyPressed(wParam);
			return Zero<LRESULT>();

		default:
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT CALLBACK WindowsRaytracerUI::DelegatingWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WindowsRaytracerUI* targetWindow;

		if (msg == WM_CREATE) {
			LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			targetWindow = reinterpret_cast<WindowsRaytracerUI*>(createStruct->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(targetWindow));
		}
		else {
			targetWindow = reinterpret_cast<WindowsRaytracerUI*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		if (targetWindow) {
			return targetWindow->WndProc(msg, wParam, lParam);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	const Int2 WindowsRaytracerUI::getXY(LPARAM lParam)
	{
		return Int2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}

}
