#include "stdafx.h"
#include "SkpColl.h"
#include "RubyUtils.h"
#include "Coll.h"
#include "D:/Ruby/lib/ruby/1.8/i386-mswin32/ruby.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSkpCollApp

BEGIN_MESSAGE_MAP(CSkpCollApp, CWinApp)
END_MESSAGE_MAP()


// CSkpCollApp construction

CSkpCollApp::CSkpCollApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


CSkpCollApp theApp;


BOOL CSkpCollApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

//void Init_SkpCol()
//{
//    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
//    Coll::InitColl();
//}
