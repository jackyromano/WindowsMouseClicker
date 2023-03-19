#include "Clicker.h"
#include "Mmsystem.h"
#include "resource.h"

#pragma comment(lib, "Winmm.lib")

Clicker* Clicker::m_instance = nullptr;

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void Clicker::Init(HWND hwnd)
{
	m_hwnd = hwnd;

	InitContextMenu();
	InitClickSound();
	InitMouseHook();

	m_initialized = true;
}

void Clicker::Show()
{
	if (!m_initialized) return;

	POINT curPoint;
	GetCursorPos(&curPoint);
	int clicked = TrackPopupMenu(m_hMenu, TPM_RETURNCMD, curPoint.x, curPoint.y, 0, m_hwnd, NULL);
	if (clicked != 0) {
		switch (clicked) {
		case Clicker::EXIT:
			PostQuitMessage(0);
			break;
		case Clicker::ENABLE:
			EnableClickFeedback();
			break;
		case Clicker::DISABLE:
			DisableClickFeedback();
			break;
		case Clicker::ABOUT:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, About);
			break;
		}
	}
}

void Clicker::EnableClickFeedback()
{
	m_enabled = true;
}

void Clicker::DisableClickFeedback()
{
	m_enabled = false;
}

LRESULT CALLBACK Clicker::LowLevelMouseProc(_In_ int    nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if (Clicker::Instance()->m_enabled) {
		HMODULE hMod;
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)Clicker::LowLevelMouseProc, &hMod);

		switch (wParam) {
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			PlaySound(Clicker::Instance()->m_clickWave, NULL, SND_MEMORY | SND_ASYNC | SND_SYSTEM);
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void Clicker::InitContextMenu()
{
	m_hMenu = CreatePopupMenu();
	AppendMenu(m_hMenu, MF_STRING, Clicker::EXIT, L"Exit");
	AppendMenu(m_hMenu, MF_STRING, Clicker::ENABLE, L"Enable");
	AppendMenu(m_hMenu, MF_STRING, Clicker::DISABLE, L"Disable");
	AppendMenu(m_hMenu, MF_SEPARATOR, -1, L"");
	AppendMenu(m_hMenu, MF_STRING, Clicker::ABOUT, L"About");
}

void Clicker::InitClickSound()
{
	HANDLE hResInfo = FindResource(GetModuleHandle(NULL), L"IDI_CLICK", L"WAVE");
	if (hResInfo == NULL) {
		MessageBox(NULL, L"Failed to find click resource", L"ERROR", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
		return;
	}

	HANDLE hRes = LoadResource(NULL, (HRSRC)hResInfo);
	if (hRes == NULL) {
		MessageBox(NULL, L"Failed to load click resource", L"ERROR", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
		return;
	}

	m_clickWave = (LPWSTR)LockResource(hRes);

	if (m_clickWave == NULL) {
		MessageBox(NULL, L"Failed to lock resource", L"ERROR", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
	}
}

void Clicker::InitMouseHook()
{
	m_hhook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
	if (m_hhook == NULL) {
		MessageBox(NULL, L"Failed to install Mouse hook", L"ERROR", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
		return;
	}
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
