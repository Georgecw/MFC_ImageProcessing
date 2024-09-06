// ParaSet.cpp: 实现文件
//

#include "pch.h"
#include "MFC_ImageProcessing.h"
#include "afxdialogex.h"
#include "ParaSet.h"


// ParaSet 对话框

IMPLEMENT_DYNAMIC(ParaSet, CDialogEx)

ParaSet::ParaSet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ParaSet, pParent)
	, para1(0)
{

}

ParaSet::~ParaSet()
{
}

void ParaSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, para1);
}


BEGIN_MESSAGE_MAP(ParaSet, CDialogEx)
END_MESSAGE_MAP()


// ParaSet 消息处理程序
