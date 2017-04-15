// GoBangDlg.h : 头文件
//

#pragma once

#define LINE_NUM      (15)
#define GRID_NUM      (LINE_NUM - 1)
#define MARGIN_SIZE   (50)
#define TUPLE_NUM     ((LINE_NUM - 5 + 1) * LINE_NUM * 2 \
						+ 2 * ((1 + LINE_NUM - 5 + 1) * (LINE_NUM - 5 + 1) / 2 \
						+ (1 + LINE_NUM - 1 - 5 + 1) * (LINE_NUM - 1 - 5 + 1) / 2))						

typedef enum tagCOLOR_TYPE
{
	COLOR_WHITE = 0,
	COLOR_BLACK,
	COLOR_BUTT
} COLOR_TYPE;

typedef struct tagGRID_POINT_INFO
{
	BOOL IsHasGoBang;
	COLOR_TYPE eColorType;
	POINT ptGridPoint;
	int nScore;
} GRID_POINT_INFO;

typedef enum tagTUPLE_STATE
{
	TUPLE_STATE_BLANK,
	TUPLE_STATE_B,
	TUPLE_STATE_BB,
	TUPLE_STATE_BBB,
	TUPLE_STATE_BBBB,
	TUPLE_STATE_W,
	TUPLE_STATE_WW,
	TUPLE_STATE_WWW,
	TUPLE_STATE_WWWW,
	TUPLE_STATE_BX_WX,
	TUPLE_STATE_BUTT
}TUPLE_STATE;

typedef struct tagTUPLE_INFO
{
	int nScore;
	POINT stPoints[5];
	COLOR_TYPE eColor[5];
} TUPLE_INFO;


// CGoBangDlg 对话框
class CGoBangDlg : public CDialog
{
// 构造
public:
	CGoBangDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GOBANG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	GRID_POINT_INFO m_stGridPointInfo[LINE_NUM][LINE_NUM];
	POINT m_ptClickGridPoint;
	POINT m_ptComputerPoint;
	POINT m_ptLastComputerPoint;
	bool m_bIsWhite;
	bool m_bIsBoardClicked;
	bool m_bIsLastComputer;
	bool m_bIsGameOver;
	TUPLE_INFO m_TupleInfo[TUPLE_NUM];
	int m_TupleScoreTable[TUPLE_STATE_BUTT];

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	void DrawHumanGoBang();
	void DrawComputerGoBang();
	BOOL IsGameOver(); 
	int GetTupleScore(int nWhiteCnt, int nBlackCnt);
	void UpdateTupleInfo();
	void GetBestPoint();
	void RestartGame();
public:
	afx_msg void OnBnClickedBtnRestart();
};
