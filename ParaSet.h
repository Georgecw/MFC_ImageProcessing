#pragma once
#include "afxdialogex.h"


// ParaSet 对话框

class ParaSet : public CDialogEx
{
	DECLARE_DYNAMIC(ParaSet)

public:
	ParaSet(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ParaSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ParaSet };
#endif

protected:
	int para1;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	 
	DECLARE_MESSAGE_MAP()
public:
	
	int GetPara1() const { return para1; }
};
