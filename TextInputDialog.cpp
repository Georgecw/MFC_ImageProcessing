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
    CDialogEx::OnInitDialog();  // 或者 CDialog::OnInitDialog()

    // 你的初始化代码
    m_slider_size.SetRange(30, 300);//设置滑动范围为1到20
    m_slider_size.SetTicFreq(20);//每1个单位画一刻度
    m_slider_size.SetPos(80);//设置滑块初始位置为10 

    m_silder_R.SetRange(1, 255);//设置滑动范围为1到20
    m_silder_R.SetTicFreq(10);//每1个单位画一刻度
    m_silder_R.SetPos(50);//设置滑块初始位置为10 

    m_silder_G.SetRange(1, 255);//设置滑动范围为1到20
    m_silder_G.SetTicFreq(10);//每1个单位画一刻度
    m_silder_G.SetPos(50);//设置滑块初始位置为10 

    m_silder_B.SetRange(1, 255);//设置滑动范围为1到20
    m_silder_B.SetTicFreq(10);//每1个单位画一刻度
    m_silder_B.SetPos(50);//设置滑块初始位置为10 

    return TRUE;  // 返回 TRUE，除非你将焦点设置到控件
}

BEGIN_MESSAGE_MAP(CTextInputDialog, CDialogEx)
    ON_BN_CLICKED(IDOK, &CTextInputDialog::OnBnClickedOk)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()




void CTextInputDialog::OnBnClickedOk()
{
    font_size = m_slider_size.GetPos();
    font_color = RGB(m_silder_R.GetPos(), m_silder_G.GetPos(), m_silder_B.GetPos());
    UpdateData(TRUE);  // 获取输入框中的文本
    CDialogEx::OnOK(); // 关闭对话框并返回IDOK
}

void CTextInputDialog::UpdateEditFont()
{
    // 获取 slider 控件的值
    CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_SIZE);
    int fontSize = pSlider->GetPos();

    pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_R);
    int font_R = pSlider->GetPos();

    pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_G);
    int font_G = pSlider->GetPos();

    pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_B);
    int font_B = pSlider->GetPos();

    // 删除旧的字体
    CFont m_edit_font;
    m_edit_font.DeleteObject();

    // 创建新的字体
    m_edit_font.CreatePointFont(fontSize, _T("Arial")); // 字体大小需要乘以 10，因为 CreatePointFont 以 1/10 点为单位

    // 获取编辑框控件指针
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TEXT);

    // 设置新的字体
    pEdit->SetFont(&m_edit_font);
}


void CTextInputDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // 确保是 slider 控件发出的消息
    if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SIZE)
    {
        UpdateEditFont();  // 更新字体大小
    }

    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
