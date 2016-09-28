#include "stdafx.h"
#include "CTreeView.h"
#include "CExplorer.h"
#include <list>
#include <CommCtrl.h>


void CTreeView::InitBasicParent()
{
	TV_INSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_TEXT | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	//Desktop
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = TVI_ROOT;
	tvInsert.item.pszText = _T("Desktop");
	tvInsert.item.lParam = (LPARAM)_T("Desktop");
	HTREEITEM hDesktop = TreeView_InsertItem(m_hTreeView, &tvInsert);
	//My Computer
	tvInsert.hParent = hDesktop;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.pszText = _T("My Computer");
	tvInsert.item.lParam = (LPARAM)_T("MyComputer");
	HTREEITEM hMyComputer = TreeView_InsertItem(m_hTreeView, &tvInsert);
	//Load Drives
	for (__int8 i = 0; i < m_drives->Size(); ++i)
	{
		tvInsert.hParent = hMyComputer;
		tvInsert.item.pszText = m_drives->operator[](i)->NAME();
		tvInsert.item.lParam = (LPARAM)m_drives->operator[](i)->LABEL();
		HTREEITEM hDrive = TreeView_InsertItem(m_hTreeView, &tvInsert);
		//Preload, đánh dấu là chưa load
		tvInsert.hParent = hDrive;
		tvInsert.item.pszText = L"PreLoad";
		tvInsert.item.lParam = (LPARAM)L"PreLoad";
		TreeView_InsertItem(m_hTreeView, &tvInsert);
	}
	//Mặc định cho My Computer expand và select nó
	TreeView_Expand(m_hTreeView, hMyComputer, TVE_EXPAND); //hiển thị các nhánh con của MyComputer
	TreeView_SelectItem(m_hTreeView, hMyComputer); //chọn vào MyComputer
}

wchar_t *CTreeView::GetPath(HTREEITEM hTreeItem)
{
	wchar_t *curPath;
	TVITEMEX tvItem = GetItemFromHTreeItem(hTreeItem);
	curPath = wcsdup((LPCWSTR)tvItem.lParam);
	return curPath;
}

HWND CTreeView::GetHandle()
{
	return m_hTreeView;
}

void CTreeView::Init(CDrivesGroup *_drives,__int16 _width, __int16 _height, __int16 _x, __int16 _y, HWND hParentWnd, HINSTANCE hInst)
{
	m_drives = _drives;
	m_hTreeView = CreateWindowEx(0, WC_TREEVIEW, L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_TABSTOP |
		TVS_HASLINES | TVS_HASBUTTONS, _x, _y, _width, _height, hParentWnd, NULL, hInst, NULL);
	InitBasicParent();
}

TVITEMEX CTreeView::GetItemFromHTreeItem(HTREEITEM hTreeItem)
{
	TVITEMEX tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = hTreeItem;
	TreeView_GetItem(m_hTreeView, &tvItem);
	return tvItem;
}

bool CTreeView::IsLoaded(HTREEITEM hCheckSel)
{
	//Đảm bảo không phải là Root hay là con của Root (MyComputer)
	//Do Root và MyComputer đã nạp rồi
	HTREEITEM nodeRoot = TreeView_GetRoot(m_hTreeView);
	if (hCheckSel == nodeRoot || hCheckSel == TreeView_GetChild(m_hTreeView, nodeRoot))
	{
		return true;
	}

	HTREEITEM hCurSelChild = TreeView_GetChild(m_hTreeView, hCheckSel);

	if (hCurSelChild == NULL) //folder này đã vào, nhưng trong đó không có folder con
	{
		return true;
	}

	TVITEMEX tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = hCurSelChild;
	TreeView_GetItem(m_hTreeView, &tvItem);
	if (wcscmp((LPCWSTR)tvItem.lParam, L"PreLoad") != 0)
	{
		return true;
	}

	return false;
}

void CTreeView::LoadChild(HTREEITEM hParent)
{
	TVITEMEX tvItem = GetItemFromHTreeItem(hParent);
	
	wchar_t *lpwCurPath = GetPath(hParent);
	std::list<CFile> listOfFolder = CExplorer::GetListDirectory(lpwCurPath);
	delete[]lpwCurPath;

	TV_INSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_TEXT | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvInsert.hInsertAfter = TVI_LAST;

	for (CFile file : listOfFolder)
	{
		tvInsert.hParent = hParent;
		tvInsert.item.pszText = _wcsdup(file.NAME());
		tvInsert.item.lParam = (LPARAM)_wcsdup(file.PATH());
		HTREEITEM hDrive = TreeView_InsertItem(m_hTreeView, &tvInsert);

		//Preload, đánh dấu là chưa load
		tvInsert.hParent = hDrive;
		tvInsert.item.pszText = L"PreLoad";
		tvInsert.item.lParam = (LPARAM)L"PreLoad";
		TreeView_InsertItem(m_hTreeView, &tvInsert);
	}
	TreeView_Expand(m_hTreeView, hParent, TVE_EXPAND);
	TreeView_SelectItem(m_hTreeView, hParent);
}

void CTreeView::InitChild(HTREEITEM hPrev, HTREEITEM hCurSel)
{
	if (IsLoaded(hCurSel)) //đã load rồi thì chỉ cần expand ra là đủ
	{
		return;
	}

	HTREEITEM hCurSelChild = TreeView_GetChild(m_hTreeView, hCurSel);
	TreeView_DeleteItem(m_hTreeView, hCurSelChild); //xóa Preload child

	LoadChild(hCurSel);
}
