#include "CDrivesGroup.h"

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#ifndef CTREEVIEW
#define CTREEVIEW

class CTreeView
{
private:
	HWND m_hTreeView;
	CDrivesGroup *m_drives;

	void InitBasicParent();
	bool IsLoaded(HTREEITEM hCheckSel);
	TVITEMEX GetItemFromHTreeItem(HTREEITEM hTreeItem);
	void LoadChild(HTREEITEM hParent);
public:
	wchar_t *GetPath(HTREEITEM hTreeItem);
	HWND GetHandle();
	void Init(CDrivesGroup *_drives, __int16 _width, __int16 _height, __int16 _x, __int16 _y, HWND hParentWnd, HINSTANCE hInst);
	void InitChild(HTREEITEM hPrev, HTREEITEM hCurSel);
};

#endif