// FileNewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "FileNewDlg.h"
#include "afxdialogex.h"


// CFileNewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFileNewDlg, CDialogEx)

CFileNewDlg::CFileNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILE_NEW, pParent)
	, m_nWidth(256)
	, m_nHeight(256)
	, m_nType(0)
{

}

CFileNewDlg::~CFileNewDlg()
{
}

void CFileNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDX_CBIndex(pDX, IDC_IMAGE_TYPE, m_nType);
}


BEGIN_MESSAGE_MAP(CFileNewDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileNewDlg �޽��� ó�����Դϴ�.
