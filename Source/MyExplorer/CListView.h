#include "CDrivesGroup.h"

#ifndef CLISTVIEW
#define CLISTVIEW

class CListView
{
private:
	__int8 m_nSubItem;
	__int16 m_nItem;
	__int8 m_nColumn;
	HWND m_hListView;
	CDrivesGroup *m_drives;

	void AddItem(wchar_t *_text, LPARAM _lParam = NULL);
	void AddSubItem(wchar_t *_text, LPARAM _lParam = NULL);
	void AddColumn(WCHAR* _colName, __int32 _width);

	wchar_t *GetPathSel();

	void InitDesktopColumn();
	void InitFolderColumn();
	void InitDriveColumn();

	void LoadDesktop();
	void LoadMyComputer();
	void LoadFileAndFolder(wchar_t *path);
	void DeleteAll();
public:
	HWND GetHandle();
	void Init(CDrivesGroup *_drives,__int16 _width, __int16 _height, __int16 _x, __int16 _y, HWND hParentWnd, HINSTANCE hInst);
	void LoadChild(wchar_t *path);
	void LoadChild(); //load childe with curent path (path selected)
};

#endif