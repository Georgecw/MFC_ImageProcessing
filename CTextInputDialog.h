#pragma once
#include <afxwin.h>      // MFC核心和标准组件
#include <afxext.h>      // MFC扩展
#include <afxdialogex.h> // MFC对话框扩展类



class CTextInputDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CTextInputDialog)

public:
    CTextInputDialog(CWnd* pParent = nullptr);
    virtual ~CTextInputDialog();

    CString GetInputText() const { return m_strInputText; }
    int GetFontSize() const { return font_size; }
    COLORREF GetFontCol() const { return font_color; }

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_TEXT_INPUT_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
    int font_size = 100;
    COLORREF font_color = RGB(50, 50, 50);
    virtual BOOL OnInitDialog();
    void UpdateEditFont();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    
private:
    CString m_strInputText;  // 保存用户输入的文本

public:
    afx_msg void OnBnClickedOk();
    CSliderCtrl m_slider_size;
    CSliderCtrl m_silder_R;
    CSliderCtrl m_silder_G;
    CSliderCtrl m_silder_B;

};
