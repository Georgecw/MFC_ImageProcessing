#include "pch.h"
#include "MFC_ImageProcessing.h"
#include "CTextInputDialog.h"

BEGIN_MESSAGE_MAP(CTextInputDialog, CDialogEx)
END_MESSAGE_MAP()

CTextInputDialog::CTextInputDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(), m_strInputText(_T(""))
{
}

BOOL CTextInputDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置对话框标题
    SetWindowText(_T("Input Text"));

    // 获取对话框的客户区
    CRect rect;
    GetClientRect(&rect);

    // 动态创建编辑框控件
    m_editCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        CRect(10, 10, rect.Width() - 10, 30), this, 1001);

    // 创建确定和取消按钮
    CButton* pButtonOK = new CButton();
    pButtonOK->Create(_T("OK"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        CRect(rect.Width() / 2 - 50, rect.Height() - 40, rect.Width() / 2, rect.Height() - 10),
        this, IDOK);
    CButton* pButtonCancel = new CButton();
    pButtonCancel->Create(_T("Cancel"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(rect.Width() / 2 + 10, rect.Height() - 40, rect.Width() / 2 + 60, rect.Height() - 10),
        this, IDCANCEL);

    return TRUE;
}

void CTextInputDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, 1001, m_strInputText);  // 将编辑框中的文本与成员变量绑定
}
