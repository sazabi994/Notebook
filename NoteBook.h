
// NoteBook.h : NoteBook Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CNoteBookApp:
// �йش����ʵ�֣������ NoteBook.cpp
//

class CNoteBookApp : public CWinApp
{
public:
	CNoteBookApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNoteBookApp theApp;
