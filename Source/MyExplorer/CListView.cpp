#include "stdafx.h"
#include "CListView.h"
#include "CExplorer.h"
#include "CText.h"
#include <list>
#include <CommCtrl.h>

#include <shellapi.h>

#define MAX_NUMBER_OF_COLUMN 4

void CListView::AddItem(wchar_t *_text, LPARAM _lParam)
{
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iItem = m_nItem++;
	lvItem.iSubItem = 0;
	lvItem.pszText = _text;
	lvItem.lParam = _lParam;
	ListView_InsertItem(m_hListView, &lvItem);
}

void CListView::AddSubItem(wchar_t *_text, LPARAM _lParam)
{
	LVITEM lvItem;
	if (_lParam)
	{
		lvItem.mask = LVIF_TEXT | LVIF_PARAM;
		lvItem.lParam = _lParam;
	}
	else
	{
		lvItem.mask = LVIF_TEXT;
	}
	lvItem.iItem = m_nItem - 1;
	lvItem.iSubItem = m_nSubItem++;
	lvItem.pszText = _text;
	ListView_SetItem(m_hListView, &lvItem);

	if (m_nSubItem == MAX_NUMBER_OF_COLUMN)
	{
		m_nSubItem = 1;
	}
}

void CListView::AddColumn(_In_ WCHAR* _colName, _In_ __int32 _width)
{
	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_CENTER;
	lvCol.cx = _width;
	lvCol.pszText = _colName;
	ListView_InsertColumn(m_hListView, m_nColumn++, &lvCol);
	if (m_nColumn == MAX_NUMBER_OF_COLUMN)
	{
		m_nColumn = 0;
	}
}

wchar_t *CListView::GetPathSel()
{
	int nCurSelIndex = ListView_GetNextItem(m_hListView, -1, LVNI_FOCUSED);

	if (nCurSelIndex < 0) //người dùng nhấn double click vào nền listview
	{
		return NULL;
	}

	LVITEM lvItem;
	lvItem.mask = LVIF_PARAM;
	lvItem.iItem = nCurSelIndex;
	lvItem.iSubItem = 0;
	ListView_GetItem(m_hListView, &lvItem);

	return wcsdup((LPCWSTR)lvItem.lParam);
}

void CListView::InitFolderColumn()
{
	AddColumn(L"Tên", 172);
	AddColumn(L"Loại tập tin", 171);
	AddColumn(L"Kích thước", 171);
	AddColumn(L"Ngày chỉnh sửa", 171);
}

void CListView::InitDriveColumn()
{
	AddColumn(L"Tên", 172);
	AddColumn(L"Loại ổ đĩa", 171);
	AddColumn(L"Dung lượng trống", 171);
	AddColumn(L"Tổng dung lượng", 171);
}

void CListView::InitDesktopColumn()
{
	AddColumn(L"Tên", 172);
	AddColumn(L"Loại tập tin", 171);
	AddColumn(L"Kích thước", 171);
	AddColumn(L"Ngày chỉnh sửa", 171);
}

void CListView::LoadDesktop()
{
	/*DeleteAll();*/
	InitDesktopColumn();
	AddItem(L"My Computer", (LPARAM)L"MyComputer");
	AddSubItem(L"MyComputer");
}

void CListView::LoadMyComputer()
{
	//DeleteAll();
	InitDriveColumn();

	for (int i = 0; i < m_drives->Size(); ++i)
	{
		//Tên ổ đĩa (drive)
		AddItem(m_drives->operator[](i)->NAME(), (LPARAM)m_drives->operator[](i)->LABEL());

		//Type
		AddSubItem(m_drives->operator[](i)->TYPE());

		//Free Space
		CCapacity capacityFreeSpace = m_drives->operator[](i)->FreeSpace();
		wchar_t *lpwCapacityFreeSpace = new wchar_t[15];
		if (capacityFreeSpace.SIZE() != 0)
		{
			wcscpy(lpwCapacityFreeSpace, CText::NumberToWCS(capacityFreeSpace.SIZE(), 2));
			wcscat(lpwCapacityFreeSpace, L" ");
			wcscat(lpwCapacityFreeSpace, capacityFreeSpace.TYPE());
		}
		else
		{
			lpwCapacityFreeSpace[0] = 0;
		}
		AddSubItem(lpwCapacityFreeSpace);
		delete[]lpwCapacityFreeSpace;

		//Total Space
		CCapacity capacityTotalSpace = m_drives->operator[](i)->TotalSpace();
		wchar_t *lpwCapacityTotalSpace = new wchar_t[15];
		if (capacityTotalSpace.SIZE() != 0)
		{
			wcscpy(lpwCapacityTotalSpace, CText::NumberToWCS(capacityTotalSpace.SIZE(), 2));
			wcscat(lpwCapacityTotalSpace, L" ");
			wcscat(lpwCapacityTotalSpace, capacityTotalSpace.TYPE());
		}
		else
		{
			lpwCapacityTotalSpace[0] = 0;
		}
		AddSubItem(lpwCapacityTotalSpace);
		delete[]lpwCapacityTotalSpace;
	}
}

void CListView::LoadFileAndFolder(wchar_t *path)
{
	//DeleteAll();
	InitFolderColumn();
	std::list<CFile> listFolderAndFile = CExplorer::GetListDirectoryAndFile(path);

	for (CFile folderOrFile : listFolderAndFile)
	{
		//Name
		AddItem(folderOrFile.NAME(), (LPARAM)wcsdup(folderOrFile.PATH()));
		//Type
		AddSubItem(folderOrFile.TYPE());
		//Size
		CCapacity capacityFreeSpace = folderOrFile.CAPACITY();
		wchar_t *lpwCapacityFreeSpace = new wchar_t[15];
		if (capacityFreeSpace.SIZE() != 0)
		{
			wcscpy(lpwCapacityFreeSpace, CText::NumberToWCS(capacityFreeSpace.SIZE(), 2));
			wcscat(lpwCapacityFreeSpace, L" ");
			wcscat(lpwCapacityFreeSpace, capacityFreeSpace.TYPE());
		}
		else
		{
			lpwCapacityFreeSpace[0] = 0;
		}
		AddSubItem(lpwCapacityFreeSpace);
		delete[]lpwCapacityFreeSpace;
		//Modified Date
		AddSubItem(folderOrFile.DATEMODIFIED());
	}
}

void CListView::DeleteAll()
{
	m_nSubItem = 1;
	m_nItem = 0;

	for (__int8 i = 1; i <= MAX_NUMBER_OF_COLUMN; ++i) //xóa tất cả các cột trước đó
	{
		ListView_DeleteColumn(m_hListView, 0);
	}

	ListView_DeleteAllItems(m_hListView);
}

HWND CListView::GetHandle()
{
	return m_hListView;
}

void CListView::Init(CDrivesGroup *_drives, __int16 _width, __int16 _height, __int16 _x, __int16 _y, HWND hParentWnd, HINSTANCE hInst)
{
	m_nSubItem = 1;
	m_nItem = 0;
	m_nColumn = 0;
	m_drives = _drives;
	m_hListView = CreateWindowEx(0, WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | LVS_REPORT,
		_x, _y, _width, _height, hParentWnd, NULL, hInst, NULL);
}

void CListView::LoadChild(wchar_t *path)
{
	DeleteAll();
	std::list<CFile> listOfFile = CExplorer::GetListDirectoryAndFile(path);

	if (path == NULL)
	{
		return;
	}

	if (!wcscmp(path, L"Desktop"))
	{
		LoadDesktop();
	}
	else
	{
		if (!wcscmp(path, L"MyComputer"))
		{
			LoadMyComputer();
		}
		else
		{
			LoadFileAndFolder(path);
		}
	}
}

void CListView::LoadChild()
{
	wchar_t *path = GetPathSel();
	if (path == NULL) //người dùng nhấn double click vào nền listview
	{
		return;
	}

	if (!wcscmp(path, L"MyComputer"))
	{
		DeleteAll();
		LoadMyComputer();
	}

	WIN32_FIND_DATA fd;
	GetFileAttributesEx(path, GetFileExInfoStandard, &fd);

	//Nếu là thư mục
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		LoadChild(path);
	}
	else //Nếu là tập tin thì chạy nó
	{
		ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
	}
	delete[]path;
}

