#pragma once
#include "framework.h"
#include <memory>


class Clicker
{
public:
	enum MenuId {
		EXIT = 0x1234,
		ENABLE,
		DISABLE,
		ABOUT
	};

	static Clicker* Instance() {
		if (m_instance == nullptr) {
			m_instance = new Clicker;
		}
		return m_instance;
	}
	void Init(HWND hwnd);
	void Show();
private:
	bool m_initialized = false;
	bool m_enabled = false;
	HWND m_hwnd = NULL;
	HMENU m_hMenu = NULL;
	HHOOK m_hhook = NULL;
	static Clicker* m_instance;
	LPWSTR m_clickWave;

	Clicker() {}
	~Clicker() {}

	void EnableClickFeedback();
	void DisableClickFeedback();
	static LRESULT CALLBACK LowLevelMouseProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam);
	void InitContextMenu();
	void InitClickSound();
	void InitMouseHook();
};


