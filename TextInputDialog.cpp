#include "pch.h"
#include "MFC_ImageProcessing.h"
#include "CTextInputDialog.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CTextInputDialog, CDialogEx)

CTextInputDialog::CTextInputDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_TEXT_INPUT_DIALOG, pParent)
{
    
}

CTextInputDialog::~CTextInputDialog()
{
}

void CTextInputDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_TEXT, m_strInputText);

    DDX_Control(pDX, IDC_SLIDER_SIZE, m_slider_size);
    DDX_Control(pDX, IDC_SLIDER_R, m_silder_R);
    DDX_Control(pDX, IDC_SLIDER_G, m_silder_G);
    DDX_Control(pDX, IDC_SLIDER_B, m_silder_B);
}

BOOL CTextInputDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();  // ���� CDialog::OnInitDialog()

    // ��ĳ�ʼ������
    m_slider_size.SetRange(30, 300);//���û�����ΧΪ1��20
    m_slider_size.SetTicFreq(20);//ÿ1����λ��һ�̶�
    m_slider_size.SetPos(80);//���û����ʼλ��Ϊ10 

    m_silder_R.SetRange(1, 255);//���û�����ΧΪ1��20
    m_silder_R.SetTicFreq(10);//ÿ1����λ��һ�̶�
    m_silder_R.SetPos(50);//���û����ʼλ��Ϊ10 

    m_silder_G.SetRange(1, 255);//���û�����ΧΪ1��20
    m_silder_G.SetTicFreq(10);//ÿ1����λ��һ�̶�
    m_silder_G.SetPos(50);//���û����ʼλ��Ϊ10 

    m_silder_B.SetRange(1, 255);//���û�����ΧΪ1��20
    m_silder_B.SetTicFreq(10);//ÿ1����λ��һ�̶�
    m_silder_B.SetPos(50);//���û����ʼλ��Ϊ10 

    return TRUE;  // ���� TRUE�������㽫�������õ��ؼ�
}

BEGIN_MESSAGE_MAP(CTextInputDialog, CDialogEx)
    ON_BN_CLICKED(IDOK, &CTextInputDialog::OnBnClickedOk)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()




void CTextInputDialog::OnBnClickedOk()
{
    font_size = m_slider_size.GetPos();
    font_color = RGB(m_silder_R.GetPos(), m_silder_G.GetPos(), m_silder_B.GetPos());
    UpdateData(TRUE);  // ��ȡ������е��ı�
    CDialogEx::OnOK(); // �رնԻ��򲢷���IDOK
}

void CTextInputDialog::UpdateEditFont()
{
    // ��ȡ slider �ؼ���ֵ
    CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SIZE);
    int fontSize = pSlider->GetPos();

    pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_R);
    int font_R = pSlider->GetPos();

    pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_G);
    int font_G = pSlider->GetPos();

    pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_B);
    int font_B = pSlider->GetPos();

    // ɾ���ɵ�����
    CFont m_edit_font;
    m_edit_font.DeleteObject();

    // �����µ�����
    m_edit_font.CreatePointFont(fontSize, _T("Arial")); // �����С��Ҫ���� 10����Ϊ CreatePointFont �� 1/10 ��Ϊ��λ

    // ��ȡ�༭��ؼ�ָ��
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TEXT);

    // �����µ�����
    pEdit->SetFont(&m_edit_font);
}


void CTextInputDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // ȷ���� slider �ؼ���������Ϣ
    if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SIZE)
    {
        UpdateEditFont();  // ���������С
    }

    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
