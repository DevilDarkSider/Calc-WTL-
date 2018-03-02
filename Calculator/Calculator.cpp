#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

CAppModule _Module;

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateChildWindows();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
	{
		 
		CenterWindow();
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		CComboBox myCombo = (CComboBox)(GetDlgItem(IDC_COMBO1));
		myCombo.AddString(L"+");
		myCombo.AddString(L"-");
		myCombo.AddString(L"*");
		myCombo.AddString(L"/");
		myCombo.AddString(L"%");
		myCombo.AddString(L"<<");
		myCombo.AddString(L">>");

		return TRUE;
	}

	LRESULT OnDestroy(UINT, WPARAM , LPARAM , BOOL& )
	{

		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}
	LRESULT OnOK(WORD , WORD wID, HWND , BOOL& )
	{
		/*CEdit ed = (CEdit)GetDlgItem(IDC_EDIT1);
		WCHAR buf[10];
		ed.GetWindowTextW(buf, 10);*/
		int x = GetDlgItemInt(IDC_EDIT1);
		int y = GetDlgItemInt(IDC_EDIT2);
		CComboBox myCombo = (CComboBox)GetDlgItem(IDC_COMBO1);
		SetDlgItemInt(IDC_EDIT3, Check_Inx(myCombo.GetCurSel(),x,y ));
		return 0;
	}

	LRESULT OnCancel(WORD , WORD wID, HWND , BOOL& )
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
	LRESULT OnEnChangeEdit1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	int Check_Inx(int index, int x,int y)
	{
		switch(index)
		{
		case 0:
			return x+y;
			break;
		case 1:
			return x-y;
			break;
		case 2:
			return x*y;
			break;
		case 3:
			return x/y;
			break;
		case 4:
			return x%y;
			break;
		case 5:
			return x<<y;
			break;
		default:
			return x>>y;
		}
	}
};

int Run(LPTSTR = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}

