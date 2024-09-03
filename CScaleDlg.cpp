// CScaleDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFC_ImageProcessing.h"
#include "afxdialogex.h"
#include "CScaleDlg.h"

double m_Scale_Height = 1.0;
double m_Scale_Width = 1.0;

// CScaleDlg 对话框

IMPLEMENT_DYNAMIC(CScaleDlg, CDialogEx)

CScaleDlg::CScaleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCALE, pParent)
	, m_Scale_Height_Temp(0)
	, m_Scale_Width_Temp(0)
{

}

CScaleDlg::~CScaleDlg()
{
}

void CScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HEIGHT_EDIT, m_Scale_Height_Temp);
	DDX_Text(pDX, IDC_WIDTH_EDIT, m_Scale_Width_Temp);
}


BEGIN_MESSAGE_MAP(CScaleDlg, CDialogEx)
	ON_BN_CLICKED(IDOK2, &CScaleDlg::OnClickedIdok2)
END_MESSAGE_MAP()


// CScaleDlg 消息处理程序



void CScaleDlg::OnClickedIdok2()
{
	UpdateData(TRUE);
	m_Scale_Height = m_Scale_Height_Temp;
	m_Scale_Width = m_Scale_Width_Temp;
	m_nModalResult = IDOK2;
	EndDialog(IDOK2);  // 关闭对话框并返回 IDOK2
	// TODO: 在此添加控件通知处理程序代码
}
