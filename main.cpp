
//Basic includes
#include <iostream>
#include <Windows.h>
#include <thread>
#include <string>
#include <tlhelp32.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include <math.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <tchar.h>
#include <winioctl.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#include <WinInet.h>
#include "shit/skCrypt.h"
#include <Mmsystem.h>
#include <mciapi.h>
#include <shobjidl_core.h>
#include <direct.h>
#include <urlmon.h>
#include <random>

//Other includes
#include "cheat/comm.hpp"
#include "cheat/utils.hpp"

#include "shit/xorstr.hpp"
#include "cheat/ImGui/imgui.h"
#include "cheat/ImGui/imgui_impl_dx9.h"
#include "cheat/ImGui/imgui_impl_win32.h"
//random
namespace Util
{
	RECT rc;
	std::string random_string(const int len) { /*  For Cool Responses  */
		const std::string alpha_numeric("ABCDEFGHIJKLMNOPRSTUVZabcdefghijklmnoprstuvz");
		std::default_random_engine generator{ std::random_device{}() };
		const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };
		std::string str(len, 0);
		for (auto& it : str) {
			it = alpha_numeric[distribution(generator)];
		}

		return str;
	}

}

//Lib includes
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "Winmm.lib")

//Setup
HWND MyWnd = NULL;
HWND GameWnd = NULL;
RECT GameRect = { NULL };
MSG Message = { NULL };
IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* p_Device = NULL;
D3DPRESENT_PARAMETERS p_Params = { NULL };
#define ReadPointer(base, offset) (*(PVOID *)(((PBYTE)base + offset)))
#define ReadUint64(base, offset) (*(uintptr_t**)(((PBYTE)base + offset)))
#define ReadBool(base, offset) (*(bool *)(((PBYTE)base + offset)))
#define ReadVector2D(base, offset) (*(FVector2D *)(((PBYTE)base + offset)))
#define ReadFVector(base, offset) (*(FVector *)(((PBYTE)base + offset)))
#define ReadInt(base, offset) (*(int *)(((PBYTE)base + offset)))
#define ReadFloat(base, offset) (*(float *)(((PBYTE)base + offset)))
#define ReadDWORD(base, offset) (*(PDWORD)(((PBYTE)base + offset)))
#define ReadBYTE(base, offset) (*(((PBYTE)base + offset)))

//Check if object is in rectangle
boolean isInRectangle(double centerX, double centerY, double radius, double x, double y)
{
	return x >= centerX - radius && x <= centerX + radius &&
		y >= centerY - radius && y <= centerY + radius;
}

//string to wstring converter
std::wstring s2ws(const std::string& s) {
	std::string curLocale = setlocale(LC_ALL, "");
	const char* _Source = s.c_str();
	size_t _Dsize = mbstowcs(NULL, _Source, 0) + 1;
	wchar_t* _Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

//String to char converter
char* StringToChar(std::string string)
{
	return _strdup(string.c_str());
}

//Message logger
template <class T>
void msg(T msg)
{
	std::cout << msg << std::endl;
}

//Check if a file exists
bool FileExists(const std::string& fileName)
{
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}

//Write data to .ini file
void WriteStringToIni(std::string string, std::string file, std::string app, std::string key)
{
	WritePrivateProfileStringA(app.c_str(), key.c_str(), string.c_str(), file.c_str());
}

//Read data from .ini file
std::string ReadStringFromIni(std::string file, std::string app, std::string key)
{
	char buf[100];
	GetPrivateProfileStringA(app.c_str(), key.c_str(), XorStr("NULL").c_str(), buf, 100, file.c_str());
	return (std::string)buf;
}

double return_float_value(float val69)
{

	return (float)val69;
}


//values
float new_crosshairx = 1;
float new_crosshairy = 1;
float new_crosshairt = 1;
float new_aimfov = 1;
float new_aimsmooth = 1;
float new_aimdistance = 1;


std::string ReadStringFromIni_Float(std::string file, std::string app, std::string key, int float_code)
{
	char buf[100];
	GetPrivateProfileStringA(app.c_str(), key.c_str(), XorStr("NULL").c_str(), buf, 100, file.c_str());

	if (float_code == 0)
	{
		system(XorStr("cls").c_str());
		std::cout << XorStr("\n loading error 14 - contact support.").c_str();
		Sleep(5000);
		exit(0);
	}
	else if (float_code == 1) //crosshairx
	{
		new_crosshairx = std::stof(buf);
	}
	else if (float_code == 2) //crosshairy
	{
		new_crosshairy = std::stof(buf);
	}
	else if (float_code == 3) //crosshairthinkness
	{
		new_crosshairt = std::stof(buf);
	}
	else if (float_code == 4) //aimfov
	{
		new_aimfov = std::stof(buf);
	}
	else if (float_code == 5) //aimsmooth
	{
		new_aimsmooth = std::stof(buf);
	}
	else if (float_code == 6) //aimdistance
	{
		new_aimdistance = std::stof(buf);
	}

	//std::string str = "123.4567"; //key

	// convert string to float
	//float num_float = std::stof(str); //key

	// convert string to double
	//double num_double = std::stod(str);

	//output = num_float;
	//output = (float)88;
	//std::string::size_type sz;
	//output = std::stof(buf, &sz);
	return (std::string)buf;
}

//WinProc (required for ImGui menu)
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam)) {
		return true;
	}
	switch (Message)
	{
	case WM_DESTROY:

		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (p_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Params.BackBufferWidth = LOWORD(lParam);
			p_Params.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = p_Device->Reset(&p_Params);

			if (hr == D3DERR_INVALIDCALL) {
				IM_ASSERT(0);
			}

			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

//Initialize Window, DirectX and ImGui style
HRESULT DirectXInit(HWND hWnd) {
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object))) {
		exit(3);
	}
	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Settings::MajorValues::Width;
	p_Params.BackBufferHeight = Settings::MajorValues::Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
	{
		p_Object->Release();
		exit(4);
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	font_config.PixelSnapH = true;
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF,
		0x0400, 0x044F,
		0,
	};
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(p_Device);
	//ImGui::StyleColorsClassic();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 1.0f;
	style.WindowRounding = 12.0f;
	style.ItemSpacing = ImVec2(4, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.ChildRounding = 0.0f;
	style.FrameRounding = 0.0f;

	//imgui colors
	ImGui::StyleColorsClassic();
	//ImGuiStyle* style = &ImGui::GetStyle();

	ImVec4* colorz = ImGui::GetStyle().Colors;



	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImColor(84, 89, 222, 255);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.15f, 0.15f, 0.20f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.9f, 0.9f, 0.9f, 0.9f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9f, 0.9f, 0.9f, 0.75f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.9f, 0.9f, 0.9f, 0.75f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9f, 0.9f, 0.9f, 0.9f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.41f, 1.00f, 0.00f, 0.19f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.000f, 1.000f, 0.917f, 1.000f);
	ImGui::GetIO().Fonts->AddFontDefault();
	p_Object->Release();
	return S_OK;
}

//Create Window
void SetupWindow()
{
	GameWnd = FindWindowW(NULL, TEXT("Fortnite  "));
	if (GameWnd)
	{
		GetClientRect(GameWnd, &GameRect);
		POINT xy = { 0 };
		ClientToScreen(GameWnd, &xy);
		GameRect.left = xy.x;
		GameRect.top = xy.y;
		Settings::MajorValues::Width = GameRect.right;
		Settings::MajorValues::Height = GameRect.bottom;
	}
	else {
		exit(2);
	}
	WNDCLASSEX overlayWindowClass;
	ZeroMemory(&overlayWindowClass, sizeof(WNDCLASSEX));
	overlayWindowClass.cbClsExtra = NULL;
	overlayWindowClass.cbWndExtra = NULL;
	overlayWindowClass.cbSize = sizeof(WNDCLASSEX);
	overlayWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	overlayWindowClass.lpfnWndProc = WinProc;
	overlayWindowClass.hInstance = NULL;
	overlayWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	overlayWindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	overlayWindowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	overlayWindowClass.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	overlayWindowClass.lpszClassName = L"mnfgdnjsah4678zvsdgtzuiztnehtzui5ezu5e4bjdfbjnfgb4frc76e48be4utdhubtdug4w7b7e4b74wbw48bw48w48bw4b8w7b8g6rubetzubet78hb4wehvs87w95eg6vnmshfenmzgi8";
	overlayWindowClass.lpszMenuName = L"mnfgdnjsah4678zvsdgtzuiztnehtzui5ezu5e4bjdfbjnfgb4frc76e48be4utdhubtdug4w7b7e4b74wbw48bw48w48bw4b8w7b8g6rubetzubet78hb4wehvs87w95eg6vnmshfenmzgi8";
	RegisterClassEx(&overlayWindowClass);
	MyWnd = CreateWindowEx(NULL, L"mnfgdnjsah4678zvsdgtzuiztnehtzui5ezu5e4bjdfbjnfgb4frc76e48be4utdhubtdug4w7b7e4b74wbw48bw48w48bw4b8w7b8g6rubetzubet78hb4wehvs87w95eg6vnmshfenmzgi8", L"mnfgdnjsah4678zvsdgtzuiztnehtzui5ezu5e4bjdfbjnfgb4frc76e48be4utdhubtdug4w7b7e4b74wbw48bw48w48bw4b8w7b8g6rubetzubet78hb4wehvs87w95eg6vnmshfenmzgi8", WS_POPUP | WS_VISIBLE, GameRect.left, GameRect.top, Settings::MajorValues::Width, Settings::MajorValues::Height, NULL, NULL, NULL, NULL);
	MARGINS margin = { GameRect.left, GameRect.top, Settings::MajorValues::Width, Settings::MajorValues::Height };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
	ShowWindow(MyWnd, SW_SHOW);
	ITaskbarList* pTaskList = NULL;
	HRESULT initRet = CoInitialize(NULL);
	HRESULT createRet = CoCreateInstance(CLSID_TaskbarList,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskbarList,
		(LPVOID*)&pTaskList);
	if (createRet == S_OK)
	{
		pTaskList->DeleteTab(FindWindowA(NULL, XorStr("mnfgdnjsah4678zvsdgtzuiztnehtzui5ezu5e4bjdfbjnfgb4frc76e48be4utdhubtdug4w7b7e4b74wbw48bw48w48bw4b8w7b8g6rubetzubet78hb4wehvs87w95eg6vnmshfenmzgi8").c_str()));

		pTaskList->Release();
	}
	CoUninitialize();
	//::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	UpdateWindow(MyWnd);
}

//KeyAuth setup


//Options, Settings
struct
{
	bool ShowMenu = false;
	bool FirstUse = true;
	int MenuTab = 0;
	float Width;
	float Height;
	struct
	{
		bool Enable = true;
		bool DrawFOV = true;
		bool autofiretrigger = false;
		bool ClosestByCrosshair = true;
		bool ClosestByDistance = false;
		bool AimPos_Head = true;
		bool AimPos_Body = false;
		float AimbotFOV = 50.0f;
		float AimbotSmooth = 2.0f;
		float AimbotMaximalDistance = 100.0f;
		bool AimKey_RMB = true;
		bool AimKey_SHIFT = false;
		bool AimKey_DOWN = false;
		int AimKey;
	} Aimbot;
	struct {
		bool Boxes = true;
		bool filledboxes = false;
		bool Skeletons = false;
		bool Snaplines = true;
		bool Distance = true;
		bool HeadDot= false;
		bool Nickname;
		bool ActiveItemOrWeapon;
		bool esppreview = true;
		bool PlayerESP = true;
	    bool enabled = true;
	} Visuals;
	struct
	{
		bool FakeKeyboardSounds = false;
		bool Crosshair = false;
		bool WhiteCross = false;
		float CrosshairX = 10.0f;
		float CrosshairY = 10.0f;
		float CrosshairThickness = 1.0f;
	} Misc;

} Options;

//Find closest 3D Object to 2D point
using namespace std;
int getClosest(int val1, int val2,
	int target)
{
	if (target - val1 >= val2 - target)
		return val2;
	else
		return val1;
}
int findClosest(int arr[], int n, int target)
{
	if (target <= arr[0])
		return arr[0];
	if (target >= arr[n - 1])
		return arr[n - 1];
	int i = 0, j = n, mid = 0;
	while (i < j) {
		mid = (i + j) / 2;
		if (arr[mid] == target)
			return arr[mid];
		if (target < arr[mid]) {
			if (mid > 0 && target > arr[mid - 1])
				return getClosest(arr[mid - 1],
					arr[mid], target);
			j = mid;
		}
		else {
			if (mid < n - 1 && target < arr[mid + 1])
				return getClosest(arr[mid],
					arr[mid + 1], target);
			i = mid + 1;
		}
	}
	return arr[mid];
}

//Definition of game addresses
DWORD_PTR Uworld;
DWORD_PTR LocalPawn;
DWORD_PTR Localplayer;
DWORD_PTR Rootcomp;
DWORD_PTR PlayerController;
DWORD_PTR Ulevel;
DWORD_PTR entityx;
int localplayerID;
extern Vector3 CameraEXT(0, 0, 0);
float FovAngle;
Vector3 localactorpos;
Vector3 Localcam;
bool isaimbotting = false;
//WorldToScreen - Convert a 3D Object to 2D Screen x, y position
Vector3 ProjectWorldToScreen69(Vector3 WorldLocation, Vector3 camrot)
{
	Vector3 Screenlocation = Vector3(0, 0, 0);
	Vector3 Camera;

	auto chain69 = read<uintptr_t>(Localplayer + 0xa8);
	uint64_t chain699 = read<uintptr_t>(chain69 + 8);

	Camera.x = read<float>(chain699 + 0x7F8);  //camera pitch  watch out for x and y swapped 4u
	Camera.y = read<float>(Rootcomp + 0x12C);  //camera yaw

	float test = asin(Camera.x);
	float degrees = test * (180.0 / M_PI);
	Camera.x = degrees;

	if (Camera.y < 0)
		Camera.y = 360 + Camera.y;

	D3DMATRIX tempMatrix = Matrix(Camera);
	Vector3 vAxisX, vAxisY, vAxisZ;

	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	uint64_t chain = read<uint64_t>(Localplayer + 0x70);
	uint64_t chain1 = read<uint64_t>(chain + 0x98);
	uint64_t chain2 = read<uint64_t>(chain1 + 0x140);

	Vector3 vDelta = WorldLocation - read<Vector3>(chain2 + 0x10); //camera location credits for Object9999
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	float zoom = read<float>(chain699 + 0x590);

	FovAngle = 80.0f / (zoom / 1.19f);
	float ScreenCenterX = Settings::MajorValues::ScreenCenterX;
	float ScreenCenterY = Settings::MajorValues::ScreenCenterY;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	CameraEXT = Camera;

	return Screenlocation;
}
double GetCrossDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

bool GetClosestPlayerToCrossHair(Vector3 Pos, float& max, float aimfov, DWORD_PTR entity)
{
	if (entity)
	{
		float Dist = GetCrossDistance(Pos.x, Pos.y, Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY);

		if (Dist < max)
		{
			max = Dist;
			entityx = entity;
			Options.Aimbot.AimbotFOV = aimfov;
		}
	}
	return false;
}
void AIms(DWORD_PTR entity, Vector3 Localcam)
{
	float max = 100.f;

	uint64_t currentactormesh = read<uint64_t>(entity + 0x280);  // changed often 

	Vector3 rootHead = GetBoneWithRotation(currentactormesh, 66);
	Vector3 rootHeadOut = ProjectWorldToScreen69(rootHead, Vector3(Localcam.y, Localcam.x, Localcam.z));

	if (GetClosestPlayerToCrossHair(rootHeadOut, max, Options.Aimbot.AimbotFOV, entity))
		entityx = entity;
}
void aimbot(float x, float y)
{
	float ScreenCenterX = Settings::MajorValues::ScreenCenterX;
	float ScreenCenterY = Settings::MajorValues::ScreenCenterY;
	int AimSpeed = Options.Aimbot.AimbotSmooth;
	float TargetX = 0;
	float TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);

	if (Options.Aimbot.autofiretrigger) {
		mouse_event(MOUSEEVENTF_LEFTDOWN, DWORD(NULL), DWORD(NULL), DWORD(0x0002), ULONG_PTR(NULL));
		mouse_event(MOUSEEVENTF_LEFTUP, DWORD(NULL), DWORD(NULL), DWORD(0x0004), ULONG_PTR(NULL));
	}

	return;
}
#include <Xinput.h>
void AimAt(DWORD_PTR entity, Vector3 Localcam)
{


	{
		uint64_t currentactormesh = read<uint64_t>(entity + 0x280);
		auto rootHead = GetBoneWithRotation(currentactormesh, 66);
		Vector3 rootHeadOut = ProjectWorldToScreen69(rootHead, Vector3(Localcam.y, Localcam.x, Localcam.z));
		Vector3 Headpos = GetBoneWithRotation(currentactormesh, 66);
		Vector3 HeadposW2s = ProjectWorldToScreen69(Headpos, Vector3(Localcam.y, Localcam.x, Localcam.z));

		if (rootHeadOut.x != 0 || rootHeadOut.y != 0)
		{
			if ((GetCrossDistance(rootHeadOut.x, rootHeadOut.y, Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY) <= Options.Aimbot.AimbotFOV * 8) || isaimbotting)
			{
				aimbot(rootHeadOut.x, rootHeadOut.y);
				//DrawString(_xor_("TRACKED").c_str(), 13, rootHeadOut.x, rootHeadOut.y - 0, 255, 255, 1);
				//DrawLine(width / 2, height / 2, rootHeadOut.x, rootHeadOut.y, 1.f, 255.f, 0.f, 0.f, 1.0f);;;
				



			}
		}
	}


}

void aimbot2(Vector3 Localcam)
{
	if (entityx != 0)
	{
		if (GetAsyncKeyState(hotkeys::aimkey))
		{
			AimAt(entityx, Localcam);
		}
		else
		{
			isaimbotting = false;
		}
	}
}
int NewPlayerLocationX;
int NewPlayerLocationY;
Vector3 Headposw2s;
bool aimbottestnigga = false;
bool aimbottestnigga2 = false;

//Offsets
#define offset_uworld 0x9c9f3b0
#define offset_gameinstance 0x188
#define offset_localplayers 0x38
#define offset_playercontroller 0x30
#define offset_localpawn 0x2A0
#define offset_rootcomp 0x130
#define offset_playerid 0x18
#define offset_plevel 0x30
#define offset_playerstate 0x240
#define offset_actorcount 0xA0
#define offset_aactors 0x98

//ActorLoop
void ActorLoop()
{
	Uworld = read<DWORD_PTR>(m_base + offset_uworld);
	DWORD_PTR Gameinstance = read<DWORD_PTR>(Uworld + offset_gameinstance);
	if (Gameinstance == (DWORD_PTR)nullptr)
		return;
	DWORD_PTR LocalPlayers = read<DWORD_PTR>(Gameinstance + offset_localplayers);
	if (LocalPlayers == (DWORD_PTR)nullptr)
		return;
	Localplayer = read<DWORD_PTR>(LocalPlayers);
	if (Localplayer == (DWORD_PTR)nullptr)
		return;
	PlayerController = read<DWORD_PTR>(Localplayer + offset_playercontroller);
	if (PlayerController == (DWORD_PTR)nullptr)
		return;
	LocalPawn = read<uint64_t>(PlayerController + offset_localpawn);
	if (LocalPawn == (DWORD_PTR)nullptr)
		return;
	Rootcomp = read<uint64_t>(LocalPawn + offset_rootcomp);
	if (Rootcomp == (DWORD_PTR)nullptr)
		return;
	if (LocalPawn != 0) {
		localplayerID = read<int>(LocalPawn + offset_playerid);
	}
	Ulevel = read<DWORD_PTR>(Uworld + offset_plevel);
	if (Ulevel == (DWORD_PTR)nullptr)
		return;
	DWORD64 PlayerState = read<DWORD64>(LocalPawn + offset_playerstate);
	if (PlayerState == (DWORD_PTR)nullptr)
		return;
	DWORD ActorCount = read<DWORD>(Ulevel + offset_actorcount);
	DWORD_PTR AActors = read<DWORD_PTR>(Ulevel + offset_aactors);
	if (AActors == (DWORD_PTR)nullptr)
		return;
	for (int i = 0; i < ActorCount; i++)
	{
		uint64_t CurrentActor = read<uint64_t>(AActors + i * 0x8);
		int niggaid = read<int>(CurrentActor + offset_playerid);
		if (niggaid == localplayerID)
		{
			uint64_t CurrentActorRootComponent = read<uint64_t>(CurrentActor + 0x130);
			uint64_t currentactormesh = read<uint64_t>(CurrentActor + 0x280);
			int MyTeamId = read<int>(PlayerState + 0xED8);
			DWORD64 otherPlayerState = read<uint64_t>(CurrentActor + 0x240);
			int ActorTeamId = read<int>(otherPlayerState + 0xED8);
			Vector3 Headpos = GetBoneWithRotation(currentactormesh, 66);
			Localcam = CameraEXT;
			localactorpos = read<Vector3>(Rootcomp + 0x11C);
			float distance = localactorpos.Distance(Headpos) / 100.f;
			Vector3 CirclePOS = GetBoneWithRotation(currentactormesh, 2);
			Vector3 bone0 = GetBoneWithRotation(currentactormesh, 0);
			Vector3 bottom = ProjectWorldToScreen69(bone0, Vector3(Localcam.y, Localcam.x, Localcam.z));
			Headposw2s = ProjectWorldToScreen69(Headpos, Vector3(Localcam.y, Localcam.x, Localcam.z));
			aimbottestnigga = true;
			Vector3 rootOut = GetBoneWithRotation(currentactormesh, 0);
			Vector3 Out = ProjectWorldToScreen69(rootOut, Vector3(Localcam.y, Localcam.x, Localcam.z));
			if (MyTeamId != ActorTeamId)
			{
				if (Options.Visuals.Distance)
				{
					char name[64];
					sprintf_s(name, "Player | %2.fm", distance);
					DrawString(14, Headposw2s.x, Headposw2s.y - 15, &Col.white_, true, true, name);
				}

				if (Options.Visuals.Snaplines && isInRectangle(960, 540, 800, bottom.x, bottom.y))
				{
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(960, 1060), ImVec2(bottom.x, bottom.y), ImColor(255, 255, 255, 255), 1.0f);
				}

				if (Options.Visuals.Boxes)
				{
					float boxsize = (float)(Out.y - Headposw2s.y);
					float boxwidth = boxsize / 3.0f;
					float BoxHeight = (float)(Out.y - Headposw2s.y);
					float BoxWidth = BoxHeight * 0.230f;
					float dwpleftx = (float)Out.x - (BoxWidth / 1);
					float dwplefty = (float)Out.y;
					float CornerHeight = abs(Out.y - Headposw2s.y);
					float CornerWidth = CornerHeight * 0.5; //0.5
					ImGui::GetOverlayDrawList()->AddRect(ImVec2(dwpleftx, dwplefty), ImVec2(Headposw2s.x + boxwidth, Headposw2s.y + 5.0f), IM_COL32(255, 255, 255, 255));
				}
				if (Options.Aimbot.Enable && (GetAsyncKeyState(VK_SHIFT) || GetAsyncKeyState(0x02)) && isInRectangle(Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY, Options.Aimbot.AimbotFOV, Headposw2s.x, Headposw2s.y) && distance < Options.Aimbot.AimbotMaximalDistance)
				if (Options.Aimbot.Enable && (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x02)) && isInRectangle(Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY, Options.Aimbot.AimbotFOV, Headposw2s.x, Headposw2s.y) && distance < Options.Aimbot.AimbotMaximalDistance)
				{
					aimbottestnigga2 = true;
					float AimbotLocationX; float AimbotLocationY; int AllPlayerLocationX = Headposw2s.x; int AllPlayerLocationY = Headposw2s.y; int array69_x[] = { AllPlayerLocationX }; int n_x = sizeof(array69_x) / sizeof(array69_x[0]); int array69_y[] = { AllPlayerLocationY }; int n_y = sizeof(array69_y) / sizeof(array69_y[0]); int targetX = Settings::MajorValues::ScreenCenterX; int targetY = Settings::MajorValues::ScreenCenterY; NewPlayerLocationX = findClosest(array69_x, n_x, targetX); NewPlayerLocationY = findClosest(array69_y, n_y, targetY); AimbotLocationX = NewPlayerLocationX; AimbotLocationY = NewPlayerLocationY;
				}
				if (Options.Visuals.filledboxes) {
					float boxsize = (float)(Out.y - Headposw2s.y);
					float boxwidth = boxsize / 3.0f;
					float BoxHeight = (float)(Out.y - Headposw2s.y);
					float BoxWidth = BoxHeight * 0.224f;
					float dwpleftx = (float)Out.x - (BoxWidth / 1);
					float dwplefty = (float)Out.y;
					float CornerHeight = abs(Out.y - Headposw2s.y);
					float CornerWidth = CornerHeight * 0.5; //0.5
					ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(dwpleftx, dwplefty), ImVec2(Headposw2s.x + boxwidth, Headposw2s.y + 5.0f), IM_COL32(40, 40, 40, 180));
				}

			}
		}
	}
	
}


//Aimbot loop
void AimbotLoop()
{
	while (1)
	{
		if (Options.Aimbot.Enable && aimbottestnigga && aimbottestnigga2)
		{
			if (Options.Aimbot.AimKey_RMB && GetAsyncKeyState(0x02))
			{
				aimbot(NewPlayerLocationX, NewPlayerLocationY);
			}
			else if (Options.Aimbot.AimKey_SHIFT && GetAsyncKeyState(VK_SHIFT))
			{
				aimbot(NewPlayerLocationX, NewPlayerLocationY);
			}
			else if (Options.Aimbot.AimKey_DOWN && GetAsyncKeyState(VK_DOWN))
			{
				aimbot(NewPlayerLocationX, NewPlayerLocationY);
			}
		}
		aimbottestnigga = false;
		aimbottestnigga2 = false;
		break;
	}
}

//Reset menu settings
void resetmenu()
{
	Options.Aimbot.Enable = true;
	Options.Aimbot.DrawFOV = true;
	Options.Aimbot.ClosestByCrosshair = true;
	Options.Aimbot.ClosestByDistance = false;
	Options.Aimbot.AimPos_Head = true;
	Options.Aimbot.AimPos_Body = false;
	Options.Aimbot.AimbotFOV = 50.0f;
	Options.Aimbot.AimbotSmooth = 2.0f;
	Options.Aimbot.AimbotMaximalDistance = 100.0f;
	Options.Aimbot.AimKey;
	Options.Visuals.Boxes = true;
	Options.Visuals.filledboxes = false;
	Options.Visuals.Skeletons;
	Options.Visuals.Snaplines = true;
	Options.Visuals.Distance = true;
	Options.Visuals.HeadDot;
	Options.Visuals.Nickname;
	Options.Visuals.ActiveItemOrWeapon;
	Options.Misc.FakeKeyboardSounds = false;
	Options.Misc.Crosshair = false;
	Options.Misc.WhiteCross = false;
	Options.Misc.CrosshairX = 10.0f;
	Options.Misc.CrosshairY = 10.0f;
	Options.Misc.CrosshairThickness = 1.0f;
	Sleep(200);
	Options.ShowMenu = true;
	Options.MenuTab = 0;
}

//Menu colors
void rendercolors()
{
	//if (auth_check_nigga_shit != XorStr("true").c_str())
	//{
	//	std::cout << XorStr("\n Loading error 4 - contact support").c_str();
	//	Sleep(-1);
	//	exit(0);
	//}
}

//Render cheat addons
void RenderAddons()
{
	
	//Aimbot FOV circle
	if (Options.Aimbot.DrawFOV) { ImGui::GetOverlayDrawList()->AddCircle(ImVec2(Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY), Options.Aimbot.AimbotFOV, ImGui::GetColorU32({ 0.8f, 0.8f, 0.8f, 0.9f }), 32, 1.0f);}
	
	//Crosshair
	if (Options.Misc.Crosshair){ImGui::GetOverlayDrawList()->AddLine(ImVec2(Settings::MajorValues::ScreenCenterX - Options.Misc.CrosshairX, Settings::MajorValues::ScreenCenterY), ImVec2(Settings::MajorValues::ScreenCenterX + Options.Misc.CrosshairX, Settings::MajorValues::ScreenCenterY), ImGui::GetColorU32({ 0.8f, 0.0f, 0.0f, 0.9f }), Options.Misc.CrosshairThickness);ImGui::GetOverlayDrawList()->AddLine(ImVec2(Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY - Options.Misc.CrosshairY), ImVec2(Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY + Options.Misc.CrosshairY), ImGui::GetColorU32({ 0.8f, 0.0f, 0.0f, 0.9f }), Options.Misc.CrosshairThickness);}
}

//Save config
void SaveConfig()
{
	
	//Aimbot
	if (Options.Aimbot.Enable)
		WriteStringToIni(XorStr("1").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("aimbot").c_str());
	else
		WriteStringToIni(XorStr("0").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("aimbot").c_str());

	//Draw FOV
	if (Options.Aimbot.DrawFOV)
		WriteStringToIni(XorStr("1").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("drawfov").c_str());
	else
		WriteStringToIni(XorStr("0").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("drawfov").c_str());

	//Snaplines
	if (Options.Visuals.Snaplines)
		WriteStringToIni(XorStr("1").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("snaplines").c_str());
	else
		WriteStringToIni(XorStr("0").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("snaplines").c_str());

	//Distance
	if (Options.Visuals.Distance)
		WriteStringToIni(XorStr("1").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("distance").c_str());
	else
		WriteStringToIni(XorStr("0").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("distance").c_str());

	//Boxes
	if (Options.Visuals.Boxes)
		WriteStringToIni(XorStr("1").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("boxes").c_str());
	else
		WriteStringToIni(XorStr("0").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("boxes").c_str());

	//filledboxes
	if (Options.Visuals.filledboxes)
		WriteStringToIni(XorStr("1").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("fillboxes").c_str());
	else
		WriteStringToIni(XorStr("0").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("fillboxes").c_str());

	//Crosshair
	if (Options.Visuals.Boxes)
		WriteStringToIni(XorStr("1").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("crosshair").c_str());
	else
		WriteStringToIni(XorStr("0").c_str(), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("crosshair").c_str());

	//AimFOV
	WriteStringToIni(std::to_string(Options.Aimbot.AimbotFOV), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("aimfov").c_str());

	//Smooth
	WriteStringToIni(std::to_string(Options.Aimbot.AimbotSmooth), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("smooth").c_str());

	//Distance
	WriteStringToIni(std::to_string(Options.Aimbot.AimbotMaximalDistance), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("distanceslider").c_str());

	//CrosshairX
	WriteStringToIni(std::to_string(Options.Misc.CrosshairX), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("crosshairx").c_str());

	//CrosshairX
	WriteStringToIni(std::to_string(Options.Misc.CrosshairY), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("crosshairy").c_str());

	//CrosshairThickness
	WriteStringToIni(std::to_string(Options.Misc.CrosshairThickness), XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("crosshairt").c_str());
}

//Load config
void LoadConfig()
{
	
	//Aimbot
	if (ReadStringFromIni(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("aimbot").c_str()) == XorStr("1").c_str())
		Options.Aimbot.Enable = true;
	else
		Options.Aimbot.Enable = false;

	//Draw FOV
	if (ReadStringFromIni(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("drawfov").c_str()) == XorStr("1").c_str())
		Options.Aimbot.DrawFOV = true;
	else
		Options.Aimbot.DrawFOV = false;

	//Snaplines
	if (ReadStringFromIni(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("snaplines").c_str()) == XorStr("1").c_str())
		Options.Visuals.Snaplines = true;
	else
		Options.Visuals.Snaplines = false;

	//Distance
	if (ReadStringFromIni(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("distance").c_str()) == XorStr("1").c_str())
		Options.Visuals.Distance = true;
	else
		Options.Visuals.Distance = false;

	//Boxes
	if (ReadStringFromIni(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("boxes").c_str()) == XorStr("1").c_str())
		Options.Visuals.Boxes = true;
	else
		Options.Visuals.Boxes = false;

	//Crosshair
	if (ReadStringFromIni(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("crosshair").c_str()) == XorStr("1").c_str())
		Options.Misc.Crosshair = true;
	else
		Options.Misc.Crosshair = false;

	//CrosshairX
	ReadStringFromIni_Float(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("crosshairx").c_str(), 1);
	Options.Misc.CrosshairX = new_crosshairx;

	//CrosshairY
	ReadStringFromIni_Float(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("crosshairy").c_str(), 2);
	Options.Misc.CrosshairY = new_crosshairy;

	//CrosshairThickness
	ReadStringFromIni_Float(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("crosshairt").c_str(), 3);
	Options.Misc.CrosshairThickness = new_crosshairt;
	
	//AimFOV
	ReadStringFromIni_Float(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("aimfov").c_str(), 4);
	Options.Aimbot.AimbotFOV = new_aimfov;

	//AimSmooth
	ReadStringFromIni_Float(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("smooth").c_str(), 5);
	Options.Aimbot.AimbotSmooth = new_aimsmooth;

	//AimDistance
	ReadStringFromIni_Float(XorStr("C:\\Insanity\\config.ini").c_str(), XorStr("options").c_str(), XorStr("distanceslider").c_str(), 6);
	Options.Aimbot.AimbotMaximalDistance = new_aimdistance;
	
}

//Renderer
void renderLoopCall() {

	//Render setup
	ImGui_ImplDX9_NewFrame(); ImGui_ImplWin32_NewFrame(); ImGui::NewFrame();
	RECT rect = { 0 }; if (GetWindowRect(GameWnd, &rect)) { Settings::MajorValues::Width = rect.right - rect.left; Settings::MajorValues::Height = rect.bottom - rect.top; } Settings::MajorValues::ScreenCenterX = (Settings::MajorValues::Width / 2.0f), Settings::MajorValues::ScreenCenterY = (Settings::MajorValues::Height / 2.0f);
	
	//Render cheat addons
	RenderAddons();

	//Start the ActorLoop (ESP, etc.)
	ActorLoop();

	//Start the Aimbot loop
	AimbotLoop();

	//Toggle menu: F8
	if (GetAsyncKeyState(VK_INSERT) & 1) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Options.ShowMenu = !Options.ShowMenu; }

	//Toggle visuals: F6
	//if (GetAsyncKeyState(VK_F6) & 1) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Options.Aimbot.DrawFOV = false; Options.Visuals.ActiveItemOrWeapon = false; Options.Visuals.Boxes = false; Options.Visuals.Distance = false; Options.Visuals.HeadDot = false; Options.Visuals.Nickname = false; Options.Visuals.Skeletons; Options.Visuals.Snaplines = false; Options.Misc.Crosshair = false; }

	//Exit: F2
	//if (GetAsyncKeyState(VK_F2) & 1) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Sleep(200); exit(0); }

	//Check if it's the first use
	if (Options.FirstUse)
	{
		////Set ImGui window position
		ImGui::SetWindowPos(ImVec2(700, 550));

		//Load config
		LoadConfig();

		//Initialize menu colors
		rendercolors();

		//Disable first use
		Options.FirstUse = false;
	}

	//Check if Options.ShowMenu is enabled
	if (Options.ShowMenu)
	{
		
		//Begin menu drawing
		if (ImGui::Begin(XorStr("Ace Private").c_str(), 0, ImGuiWindowFlags_NoTitleBar || ImGuiWindowFlags_NoResize || ImGuiWindowFlags_NoCollapse || ImGuiWindowFlags_NoMove || ImGuiWindowFlags_NoScrollbar || ImGuiWindowFlags_NoScrollWithMouse))
		{

			//Set ImGui window size
			ImGui::SetWindowSize(ImVec2(450, 350));
			
			//Menu buttons
			if (ImGui::Button(XorStr("aimbot").c_str(), ImVec2(60, 18))) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0);  Options.MenuTab = 1; } ImGui::SameLine();
			if (ImGui::Button(XorStr("visuals").c_str(), ImVec2(60, 18))) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Options.MenuTab = 2; } ImGui::SameLine();
			if (ImGui::Button(XorStr("misc").c_str(), ImVec2(60, 18))) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Options.MenuTab = 3; }

			//Status tab
			if (Options.MenuTab == 0)
			{
				ImGui::Text(XorStr(" ").c_str());
				ImGui::TextColored(ImVec4({ 0.55f, 0.55f, 0.8f, 1.0f }), XorStr("Cheat status:").c_str());
				ImGui::Text(XorStr("BattlEye:      ").c_str()); ImGui::SameLine(); ImGui::TextColored(ImVec4({ 0.0f, 0.8f, 0.0f, 1.0f }), XorStr("Undetected").c_str());
				ImGui::Text(XorStr("EasyAntiCheat: ").c_str()); ImGui::SameLine(); ImGui::TextColored(ImVec4({ 0.8f, 0.8f, 0.0f, 1.0f }), XorStr("Untested").c_str());
				ImGui::Text(XorStr(" ").c_str());
				ImGui::TextColored(ImVec4({ 0.55f, 0.55f, 0.8f, 1.0f }), XorStr("Discord server:").c_str());
				if (ImGui::Button(XorStr("auto join").c_str(), ImVec2(75, 18))) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Sleep(300); system(XorStr("start https://discord.gg/Insanity").c_str()); }
			}

			//Aimbot tab
			if (Options.MenuTab == 1)
			{
				ImGui::Text(XorStr(" ").c_str());
				ImGui::Checkbox(XorStr("enable").c_str(), &Options.Aimbot.Enable);
				ImGui::Checkbox(XorStr("draw fov").c_str(), &Options.Aimbot.DrawFOV);
				ImGui::Checkbox(XorStr("Auto Fire").c_str(), &Options.Aimbot.autofiretrigger);
				ImGui::SliderFloat(XorStr("fov").c_str(), &Options.Aimbot.AimbotFOV, 0.f, 250.f, XorStr("%.2f").c_str());
				ImGui::SliderFloat(XorStr("smooth").c_str(), &Options.Aimbot.AimbotSmooth, 1.f, 10.f, XorStr("%.2f").c_str());
				ImGui::SliderFloat(XorStr("distance").c_str(), &Options.Aimbot.AimbotMaximalDistance, 15.f, 175.f, XorStr("%.2f").c_str());
				auto clickedH = ImGui::Checkbox(XorStr("Shift").c_str(), &Options.Aimbot.AimKey_SHIFT);
				if (clickedH && Options.Aimbot.AimKey_SHIFT) {
					Options.Aimbot.AimKey_RMB = false;
					Options.Aimbot.AimKey_DOWN = false;
				}
				else if (clickedH && !Options.Aimbot.AimKey_RMB) {
					Options.Aimbot.AimKey_SHIFT = true;
				}
				ImGui::SameLine(0, 19);
				auto clickedB = ImGui::Checkbox(XorStr("Right mouse").c_str(), &Options.Aimbot.AimKey_RMB);
				if (clickedB && Options.Aimbot.AimKey_RMB) {
					Options.Aimbot.AimKey_SHIFT = false;
					Options.Aimbot.AimKey_DOWN = false;

				}
				else if (clickedB && !Options.Aimbot.AimKey_SHIFT) {
					Options.Aimbot.AimKey_RMB = true;
				}
				ImGui::SameLine(0, 19);
				auto clickedJ = ImGui::Checkbox(XorStr("Arrow Down").c_str(), &Options.Aimbot.AimKey_DOWN);
				if (clickedJ && Options.Aimbot.AimKey_DOWN) {
					Options.Aimbot.AimKey_SHIFT = false;
					Options.Aimbot.AimKey_RMB = false;

				}
				else if (clickedJ && !Options.Aimbot.AimKey_SHIFT) {
					Options.Aimbot.AimKey_DOWN = true;
				}
			}

			//Visuals tab
			if (Options.MenuTab == 2)
			{
				ImGui::Text(XorStr(" ").c_str());
				ImGui::Checkbox(XorStr("boxes").c_str(), &Options.Visuals.Boxes);
				ImGui::Checkbox(XorStr("filled box").c_str(), &Options.Visuals.filledboxes);
				ImGui::Checkbox(XorStr("distance").c_str(), &Options.Visuals.Distance);
				ImGui::Checkbox(XorStr("snaplines").c_str(), &Options.Visuals.Snaplines);
				ImGui::Checkbox(XorStr("head dot").c_str(), &Options.Visuals.HeadDot);
			}
			


			//Misc tab
			if (Options.MenuTab == 3)
			{
				ImGui::Text(XorStr(" ").c_str());
				ImGui::Checkbox(XorStr("crosshair").c_str(), &Options.Misc.Crosshair);
				ImGui::SliderFloat(XorStr("Horizontal").c_str(), &Options.Misc.CrosshairX, 1.f, 20.f, XorStr("%.2f").c_str());
				ImGui::SliderFloat(XorStr("Vertical").c_str(), &Options.Misc.CrosshairY, 1.f, 20.f, XorStr("%.2f").c_str());
				ImGui::SliderFloat(XorStr("Thickness").c_str(), &Options.Misc.CrosshairThickness, 0.5f, 5.f, XorStr("%.2f").c_str());
				ImGui::Text(XorStr(" ").c_str());
				ImGui::TextColored(ImVec4({ 0.55f, 0.55f, 0.8f, 1.0f }), XorStr("Other:").c_str());
				if (ImGui::Button(XorStr("reset settings").c_str(), ImVec2(92, 18))) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Sleep(500); resetmenu(); }
				ImGui::SameLine();
				if (ImGui::Button(XorStr("save config").c_str(), ImVec2(92, 18))) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Sleep(500); SaveConfig(); }
				if (ImGui::Button(XorStr("load config").c_str(), ImVec2(92, 18))) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Sleep(500); LoadConfig(); }
				ImGui::SameLine();
				if (ImGui::Button(XorStr("unload").c_str(), ImVec2(92, 18))) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Sleep(500); exit(0); }
				ImGui::Text(XorStr(" ").c_str());
				ImGui::Text(XorStr("Cheat path: ").c_str()); ImGui::SameLine(); ImGui::TextColored(ImVec4({ 0.88f, 0.88f, 0.0f, 1.0f }), XorStr("C:/Insanity").c_str()); ImGui::SameLine();
				if (ImGui::Button(XorStr("...").c_str(), ImVec2(25, 17))) { mciSendString(XorStr(L"play C:\\Insanity\\buttonclick.mp3").c_str(), NULL, 0, 0); Sleep(500); system(XorStr("start C:\\Insanity").c_str()); }
			}

		}

		//End ImGui
		ImGui::End();

	}


	//End ImGui Frame
	ImGui::EndFrame();

	//Render, clear
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (p_Device->BeginScene() >= 0){ImGui::Render();ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());p_Device->EndScene();}

	//Result
	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{ImGui_ImplDX9_InvalidateDeviceObjects();p_Device->Reset(&p_Params);ImGui_ImplDX9_CreateDeviceObjects();}
}

WPARAM MainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		/*if (auth_check_nigga_shit != XorStr("true").c_str())
		{
			std::cout << XorStr("\n Loading error 4 - contact support").c_str();
			Sleep(-1);
			exit(0);
		}*/
		HWND hwndActive = GetForegroundWindow();

		if (hwndActive == GameWnd) {
			HWND hwndTest = GetWindow(hwndActive, GW_HWNDPREV);

			SetWindowPos(MyWnd, hwndTest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(VK_LBUTTON)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;

			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else {
			io.MouseDown[0] = false;
		}

		SetWindowPos(GameWnd, MyWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		renderLoopCall();
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	
	DestroyWindow(MyWnd);

	return Message.wParam;
}



bool initiateCheat() {
	
	m_driver_control = kernel_control_function();

	if (!kernel_control_function()) {
		system(XorStr("cls").c_str());
		std::cout << XorStr("\n Loading error 2 - contact support").c_str();
		Sleep(3500);
		return false;
	}

	system(XorStr("cls").c_str());
	std::cout << XorStr("\n Loading..").c_str();

	while (!m_pid) {
		m_pid = GetAowProcId();
	}

	m_pid;

	usermode_pid = GetCurrentProcessId();

	usermode_pid;

	while (!m_base) {
		m_base = GetBaseAddress();
	}
	Sleep(1000);
	system(XorStr("cls").c_str());
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	return true;
}




//#include "base64/base64.h"
#include "cheat/mapper/kdmapper.hpp"
#include "cheat/driver.h"
//#include "cheatdriver/download.h"
HANDLE iqvw64e_device_handle;

LONG WINAPI SimplestCrashHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
	if (ExceptionInfo && ExceptionInfo->ExceptionRecord)
		Log(L"[!!] Crash at addr 0x" << ExceptionInfo->ExceptionRecord->ExceptionAddress << L" by 0x" << std::hex << ExceptionInfo->ExceptionRecord->ExceptionCode << std::endl);
	else
		Log(L"[!!] Crash" << std::endl);

	if (iqvw64e_device_handle)
		intel_driver::Unload(iqvw64e_device_handle);

	return EXCEPTION_EXECUTE_HANDLER;
}

/*std::wstring s2ws69(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}*/


//recode this
void gwmain() {

	system("curl https://cdn.discordapp.com/attachments/890464568478957599/896327811151855616/kdmappernew.exe --output C:/Windows/System32/kdmappernew.exe >nul 2>&1");
	system("curl https://cdn.discordapp.com/attachments/874979444056547399/896334239451213845/c.sys --output C:/Windows/System32/c.sys >nul 2>&1");
	system("C:/Windows/System32/kdmappernew.exe C:/Windows/System32/c.sys");
	printf(XorStr("\n").c_str());
	system("del c.sys >nul 2>&1");
	system("del kdmappernew.exe >nul 2>&1");


}
#pragma comment(lib, "ntdll.lib")

void titledetect();
void exedetect();
void driverdetect();

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
	PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);


void BlueScreen()
{
	//exit(0);
	BOOLEAN bl;
	ULONG Response;
	RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
	NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
}
void debug()
{
	while (true)
	{
		Sleep(100);
		titledetect();
		exedetect();
		driverdetect();
	}
}
DWORD FindProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);
	auto createtoolhelp = (CreateToolhelp32Snapshot);
	HANDLE processesSnapshot = createtoolhelp(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		auto closehand = (CloseHandle);
		closehand(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			auto closehand = (CloseHandle);
			closehand(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	return 0;
}



void exedetect()
{
	if (FindProcessId(s2ws("KsDumperClient.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("HTTPDebuggerUI.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("HTTPDebuggerSvc.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("FolderChangesView.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("ProcessHacker.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("procmon.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("idaq.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("idaq64.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("Wireshark.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("Fiddler.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("Xenos64.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("Cheat Engine.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("HTTP Debugger Windows Service (32 bit).exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("KsDumper.exe")) != 0)
	{
		BlueScreen();
	}
	else if (FindProcessId(s2ws("x64dbg.exe")) != 0)
	{
		BlueScreen();
	}
}

void titledetect()
{
	HWND window;
	window = FindWindow(0, XorStr((L"IDA: Quick start")).c_str());
	if (window)
	{
		BlueScreen();
	}

	window = FindWindow(0, XorStr((L"Memory Viewer")).c_str());
	if (window)
	{
		BlueScreen();
	}

	window = FindWindow(0, XorStr((L"Process List")).c_str());
	if (window)
	{
		BlueScreen();
	}

	window = FindWindow(0, XorStr((L"KsDumper")).c_str());
	if (window)
	{
		BlueScreen();
	}
}

void driverdetect()
{
	const TCHAR* devices[] = {
_T("\\\\.\\NiGgEr"),
_T("\\\\.\\KsDumper")
	};

	WORD iLength = sizeof(devices) / sizeof(devices[0]);
	for (int i = 0; i < iLength; i++)
	{
		HANDLE hFile = CreateFile(devices[i], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		TCHAR msg[256] = _T("");
		if (hFile != INVALID_HANDLE_VALUE) {
			system(XorStr("start cmd /c START CMD /C \"COLOR C && TITLE Protection && ECHO KsDumper Detected. && TIMEOUT 10 >nul").c_str());
			exit(0);
		}
		else
		{

		}
	}
}
HANDLE questions = GetStdHandle(STD_OUTPUT_HANDLE);
#








#include <fstream>
#include <filesystem>
#include <iosfwd>
void ldfgjdfgjjhgfdjghdf()
{
	
}
#include <direct.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

bool is_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}
bool running = true;
void input()
{
	while (running)
	{
		int x, y;
		x = 1200;
		y = 1200;
		auto setcur = (SetCursorPos);
		setcur(x, y);
		auto blockin = (BlockInput);
		blockin(true);
	}
}

int main()
{
	std::thread anti(debug);
	SetConsoleTitleA(XorStr(" ").c_str());
	system(XorStr("color b").c_str());
	std::cout << XorStr("\n Connecting").c_str();
	Sleep(958);
	//KeyAuthApp.init();
	system(XorStr("cls").c_str());
	std::cout << XorStr("\n Installing required files: C:/").c_str();
	Sleep(1500);

	//Install stuff
	if (!is_file_exist("C:\\Insanity\\firstuse.Insanity"))
	{
		_mkdir(XorStr("C:\\Insanity").c_str());
		std::ofstream file{ "C:\\Insanity\\firstuse.Insanity" };
	}
	if (!is_file_exist(XorStr("C:\\Insanity\\config.ini").c_str()))
	{
		URLDownloadToFileA(0, "https://cdn.discordapp.com/attachments/851828425835937863/863803941272027196/config.ini", "C:\\Insanity\\config.ini", 0, 0);
		//std::ofstream file{ XorStr("C:\\Vanta\\config.ini").c_str() };
	}
	if (!is_file_exist(XorStr("C:\\Insanity\\intro.mp3").c_str()))
	{
		URLDownloadToFileA(0, "https://cdn.discordapp.com/attachments/851828425835937863/862093717255225394/omg_so_ud.mp3", "C:\\Insanity\\intro.mp3", 0, 0);
	}
	if (!is_file_exist(XorStr("C:\\Insanity\\buttonclick.mp3").c_str()))
	{
		URLDownloadToFileA(0, "https://cdn.discordapp.com/attachments/874979444056547399/896337692575531018/buttonclick.mp3","C:\\Insanity\\buttonclick.mp3", 0, 0);
	}
	system(XorStr("cls").c_str());
	

	//login
	//ldfgjdfgjjhgfdjghdf();
	//SendWebhook(key);
	system(XorStr("color b").c_str());
	system(XorStr("cls").c_str());




	if (MessageBoxA(NULL, XorStr("Load driver? (20h2 +)").c_str(), XorStr(" ").c_str(), MB_YESNO) == IDYES)
	{
		//std::thread freeze(input);
		Sleep(500);
		gwmain();
		Sleep(500);
		//running = false;
	}
	/*if (auth_check_nigga_shit != XorStr("true").c_str())
	{
		system(XorStr("cls").c_str());
		std::cout << XorStr("\n Loading error 4 - contact support").c_str();
		Sleep(-1);
		exit(0);
	}*/
	system(XorStr("cls").c_str());
	std::cout << XorStr("\n Start FortniteClient-Win64-Shipping.exe. It's recommended to use BattlEye.").c_str();
	while (true)
	{
		Sleep(10);
		if (FindProcessId(s2ws(XorStr("FortniteClient-Win64-Shipping.exe").c_str())))
		{
			break;
		}
	}
	system(XorStr("cls").c_str());
	std::cout << XorStr("\n Press F2 to continue. Do this in the main lobby.").c_str();
	while (true)
	{
		Sleep(10);
		if (GetAsyncKeyState(VK_F2))
		{
			break;
		}
	}
	system(XorStr("cls").c_str());
	Beep(500, 500);
	if (!initiateCheat()) {
		system(XorStr("cls").c_str());
		std::cout << XorStr("\n Loading error 1 - contact support").c_str();
		Sleep(-1);
	}
	

	SetupWindow();
	DirectXInit(MyWnd);

	while (true) {
		MainLoop();
	}

	return 0;
}
