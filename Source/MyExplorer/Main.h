#pragma once

#include "resource.h"
#include "CDrivesGroup.h"
#include "CListView.h"
#include "CTreeView.h"

CDrivesGroup *drives = new CDrivesGroup();
CListView listView;
CTreeView treeView;

void OnPaint(HWND hWnd);
void OnDestroy(HWND hWnd);
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);

