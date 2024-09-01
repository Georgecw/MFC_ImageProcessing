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

    // ���öԻ������
    SetWindowText(_T("Input Text"));

    // ��ȡ�Ի���Ŀͻ���
    CRect rect;
    GetClientRect(&rect);

    // ��̬�����༭��ؼ�
    m_editCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        CRect(10, 10, rect.Width() - 10, 30), this, 1001);

    // ����ȷ����ȡ����ť
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
    DDX_Text(pDX, 1001, m_strInputText);  // ���༭���е��ı����Ա������
}
