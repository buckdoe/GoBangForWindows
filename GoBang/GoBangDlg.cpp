// GoBangDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GoBang.h"
#include "GoBangDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_HAND MAKEINTRESOURCE(32649)
#define HOT_SPOT_SIZE (10)
#define GO_SIZE (15)
#define MARK_SIZE (4)


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGoBangDlg �Ի���




CGoBangDlg::CGoBangDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGoBangDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(m_stGridPointInfo, 0, sizeof(m_stGridPointInfo));
	m_bIsWhite = FALSE;
	m_bIsBoardClicked = FALSE;
	m_bIsGameOver = FALSE;
	m_bIsLastComputer = FALSE;
}


void CGoBangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGoBangDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_RESTART, &CGoBangDlg::OnBnClickedBtnRestart)
END_MESSAGE_MAP()


// CGoBangDlg ��Ϣ�������

BOOL CGoBangDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	int i, j;

	for (i = 0; i < LINE_NUM; i++)
	{
		for (j = 0; j < LINE_NUM; j++)
		{
			m_stGridPointInfo[i][j].eColorType = COLOR_BUTT;
			m_stGridPointInfo[i][j].IsHasGoBang = FALSE;
			m_stGridPointInfo[i][j].ptGridPoint.x = -1;
			m_stGridPointInfo[i][j].ptGridPoint.y = -1;
		}
	}

	m_TupleScoreTable[TUPLE_STATE_BLANK] = 7;
	m_TupleScoreTable[TUPLE_STATE_B] = 35;
	m_TupleScoreTable[TUPLE_STATE_BB] = 800;
	m_TupleScoreTable[TUPLE_STATE_BBB] = 15000;

	m_TupleScoreTable[TUPLE_STATE_BBBB] = 800000;

	/*
	m_TupleScoreTable[TUPLE_STATE_W] = 15;
	m_TupleScoreTable[TUPLE_STATE_WW] = 400;
	m_TupleScoreTable[TUPLE_STATE_WWW] = 1800;

	m_TupleScoreTable[TUPLE_STATE_WWWW] = 100000;
	*/

	m_TupleScoreTable[TUPLE_STATE_W] = 35;
	m_TupleScoreTable[TUPLE_STATE_WW] = 800;
	m_TupleScoreTable[TUPLE_STATE_WWW] = 15000;

	m_TupleScoreTable[TUPLE_STATE_WWWW] = 800000;

	m_TupleScoreTable[TUPLE_STATE_BX_WX] = 0;

	memset(m_TupleInfo, 0, sizeof(m_TupleInfo));
	memset(&m_ptClickGridPoint, 0, sizeof(POINT));
	memset(&m_ptComputerPoint, 0, sizeof(POINT));
	memset(&m_ptLastComputerPoint, 0, sizeof(POINT));

	GetDlgItem(IDC_BTN_RESTART)->ShowWindow(SW_HIDE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGoBangDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGoBangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{			
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		CPen pen, redPen;
		CPen *pOldPen = NULL;

		pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		pOldPen = dc.SelectObject(&pen);
		dc.SelectStockObject(NULL_BRUSH);

		CRect rect;
		GetClientRect(&rect);

		// ������ӳߴ�
		int nInterval = (rect.Width() > rect.Height() ? rect.Height(): rect.Width() 
				- 2 * MARGIN_SIZE) / GRID_NUM;

		// ���ĵ�
		POINT pt = rect.CenterPoint();

		int nRadius = GRID_NUM * nInterval / 2;
			
		int i, j, nLeftX, nLeftY, nRightX, nRightY;

		nLeftX = pt.x - GRID_NUM * nInterval / 2;
		nLeftY = pt.y - GRID_NUM * nInterval / 2;
		nRightX = pt.x + GRID_NUM * nInterval / 2;
		nRightY = pt.y + GRID_NUM * nInterval / 2;

		// ��15��ˮƽ��
		for (i = 0; i < LINE_NUM; i++)
		{
			dc.MoveTo(nLeftX, nLeftY + i * nInterval);
			dc.LineTo(nRightX, nLeftY + i * nInterval);

			// ÿˮƽ�����ϸ����y������ͬ
			for (j = 0; j < LINE_NUM; j++)
			{
				m_stGridPointInfo[i][j].ptGridPoint.y = nLeftY + i * nInterval;
			}
		}

		// ��15����ֱ��
		for (j = 0; j < LINE_NUM; j++)
		{
			dc.MoveTo(nLeftX + j * nInterval, nLeftY);
			dc.LineTo(nLeftX + j * nInterval, nRightY);

			// ÿ����ֱ���ϸ����x������ͬ
			for (i = 0; i < LINE_NUM; i++)
			{
				m_stGridPointInfo[i][j].ptGridPoint.x = nLeftX + j * nInterval;
			}
		}

		for (i = 0; i < LINE_NUM; i++)
		{
			for (j = 0; j < LINE_NUM; j++)
			{
				if (TRUE == m_stGridPointInfo[i][j].IsHasGoBang)
				{
					if (COLOR_WHITE == m_stGridPointInfo[i][j].eColorType)
					{
						dc.SelectStockObject(WHITE_BRUSH);
					}
					else
					{
						dc.SelectStockObject(BLACK_BRUSH);
					}
					
					nLeftX = m_stGridPointInfo[i][j].ptGridPoint.x - GO_SIZE;
					nLeftY = m_stGridPointInfo[i][j].ptGridPoint.y - GO_SIZE;

					nRightX = m_stGridPointInfo[i][j].ptGridPoint.x + GO_SIZE;
					nRightY = m_stGridPointInfo[i][j].ptGridPoint.y + GO_SIZE;

					dc.Ellipse(nLeftX, nLeftY, nRightX, nRightY);

					if (m_stGridPointInfo[i][j].ptGridPoint.x == m_ptComputerPoint.x
						&& m_stGridPointInfo[i][j].ptGridPoint.y == m_ptComputerPoint.y)
					{
						/* ��+�� */
						redPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
						pOldPen = dc.SelectObject(&redPen);

						dc.MoveTo(m_ptComputerPoint.x, m_ptComputerPoint.y);
						dc.LineTo(m_ptComputerPoint.x - MARK_SIZE, m_ptComputerPoint.y);
						dc.MoveTo(m_ptComputerPoint.x, m_ptComputerPoint.y);
						dc.LineTo(m_ptComputerPoint.x + MARK_SIZE, m_ptComputerPoint.y);
						dc.MoveTo(m_ptComputerPoint.x, m_ptComputerPoint.y);
						dc.LineTo(m_ptComputerPoint.x, m_ptComputerPoint.y - MARK_SIZE);
						dc.MoveTo(m_ptComputerPoint.x, m_ptComputerPoint.y);
						dc.LineTo(m_ptComputerPoint.x, m_ptComputerPoint.y + MARK_SIZE);
						dc.SelectObject(&pen);
					}
				}
			}
		}

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CGoBangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGoBangDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	int i, j;
	CClientDC dc(this);

	for (i = 0; i < LINE_NUM; i++)
	{
		for (j = 0; j < LINE_NUM; j++)
		{
			if (abs(point.x - m_stGridPointInfo[i][j].ptGridPoint.x) < HOT_SPOT_SIZE 
				&& abs(point.y - m_stGridPointInfo[i][j].ptGridPoint.y) < HOT_SPOT_SIZE)
			{
				SetCursor(LoadCursor(NULL,ID_HAND));
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CGoBangDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int i, j, k, t;

	if (true == m_bIsGameOver)
	{
		return;
	}

	for (i = 0; i < LINE_NUM; i++)
	{
		for (j = 0; j < LINE_NUM; j++)
		{
			if (abs(point.x - m_stGridPointInfo[i][j].ptGridPoint.x) < HOT_SPOT_SIZE 
				&& abs(point.y - m_stGridPointInfo[i][j].ptGridPoint.y) < HOT_SPOT_SIZE)
			{
				if (m_stGridPointInfo[i][j].IsHasGoBang)
				{
					return;
				}

				m_ptClickGridPoint = m_stGridPointInfo[i][j].ptGridPoint;
				m_stGridPointInfo[i][j].IsHasGoBang = TRUE;
				m_bIsBoardClicked = TRUE;

				if (m_bIsWhite)
				{
					m_stGridPointInfo[i][j].eColorType = COLOR_WHITE;
					TRACE(_T("Human: (%d, %d, white)\n"), i, j);
				}
				else
				{
					m_stGridPointInfo[i][j].eColorType = COLOR_BLACK;
					TRACE(_T("Human: (%d, %d, black)\n"), i, j);
				}

				DrawHumanGoBang(); //������
				m_bIsLastComputer = false;
				m_bIsWhite = !m_bIsWhite;
				m_bIsBoardClicked = FALSE;

				UpdateTupleInfo();

				if (TRUE == IsGameOver())
				{
					if (IDYES == MessageBox(_T("Game over ! Human Win ! \nOnce again ?"), NULL, MB_YESNO))
					{
						RestartGame();
					}
					else
					{
						GetDlgItem(IDC_BTN_RESTART)->ShowWindow(SW_SHOW);
					}
	
					return;
				}

				GetBestPoint();
				DrawComputerGoBang(); //��������
				m_bIsLastComputer = true;

				/* ��������״̬ */
				for (k = 0;k < LINE_NUM; k++)
				{
					for (t = 0; t < LINE_NUM; t++)
					{
						if (m_stGridPointInfo[k][t].ptGridPoint.x == m_ptComputerPoint.x
							&& m_stGridPointInfo[k][t].ptGridPoint.y == m_ptComputerPoint.y)
						{
							m_stGridPointInfo[k][t].IsHasGoBang = TRUE;

							if (m_bIsWhite)
							{
								m_stGridPointInfo[k][t].eColorType = COLOR_WHITE;
								TRACE(_T("Computer: (%d, %d, white)\n"), k, t);
							}
							else
							{
								m_stGridPointInfo[k][t].eColorType = COLOR_BLACK;
								TRACE(_T("Computer: (%d, %d, black)\n"), k, t);
							}
						}
					}
				}

				m_bIsWhite = !m_bIsWhite;

				UpdateTupleInfo();

				if (TRUE == IsGameOver())
				{
					if (IDYES == MessageBox(_T("Game over ! Computer Win ! \nOnce again ?"), NULL, MB_YESNO))
					{
						RestartGame();
					}
					else
					{
						GetDlgItem(IDC_BTN_RESTART)->ShowWindow(SW_SHOW);
					}
					
					return;
				}
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CGoBangDlg::DrawHumanGoBang()
{
	int nLeftX, nLeftY, nRightX, nRightY;

	CClientDC dc(this); // ���ڻ��Ƶ��豸������
	CPen pen;
	CPen *pOldPen = NULL;

	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	pOldPen = dc.SelectObject(&pen);

	if (m_bIsWhite)
	{
		dc.SelectStockObject(WHITE_BRUSH);
	}
	else
	{
		dc.SelectStockObject(BLACK_BRUSH);
	}

	nLeftX = m_ptClickGridPoint.x - GO_SIZE;
	nLeftY = m_ptClickGridPoint.y - GO_SIZE;

	nRightX = m_ptClickGridPoint.x + GO_SIZE;
	nRightY = m_ptClickGridPoint.y + GO_SIZE;

	dc.Ellipse(nLeftX, nLeftY, nRightX, nRightY);
}

void CGoBangDlg::DrawComputerGoBang()
{
	int nLeftX, nLeftY, nRightX, nRightY;

	CClientDC dc(this); // ���ڻ��Ƶ��豸������
	CPen pen, redPen;
	CPen *pOldPen = NULL;

	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	pOldPen = dc.SelectObject(&pen);

	if (m_bIsWhite)
	{
		dc.SelectStockObject(WHITE_BRUSH);
	}
	else
	{
		dc.SelectStockObject(BLACK_BRUSH);
	}

	nLeftX = m_ptComputerPoint.x - GO_SIZE;
	nLeftY = m_ptComputerPoint.y - GO_SIZE;

	nRightX = m_ptComputerPoint.x + GO_SIZE;
	nRightY = m_ptComputerPoint.y + GO_SIZE;

	dc.Ellipse(nLeftX, nLeftY, nRightX, nRightY);

	/* ��+�� */
	dc.SelectObject(pOldPen);
	redPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pOldPen = dc.SelectObject(&redPen);

	dc.MoveTo(m_ptComputerPoint.x, m_ptComputerPoint.y);
	dc.LineTo(m_ptComputerPoint.x - MARK_SIZE, m_ptComputerPoint.y);
	dc.MoveTo(m_ptComputerPoint.x, m_ptComputerPoint.y);
	dc.LineTo(m_ptComputerPoint.x + MARK_SIZE, m_ptComputerPoint.y);
	dc.MoveTo(m_ptComputerPoint.x, m_ptComputerPoint.y);
	dc.LineTo(m_ptComputerPoint.x, m_ptComputerPoint.y - MARK_SIZE);
	dc.MoveTo(m_ptComputerPoint.x, m_ptComputerPoint.y);
	dc.LineTo(m_ptComputerPoint.x, m_ptComputerPoint.y + MARK_SIZE);

	/* �����һ����+�� */
	if (m_ptLastComputerPoint.x && m_ptLastComputerPoint.y)
	{
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		pOldPen = dc.SelectObject(&pen);

		if (m_bIsWhite)
		{
			dc.SelectStockObject(WHITE_BRUSH);
		}
		else
		{
			dc.SelectStockObject(BLACK_BRUSH);
		}

		nLeftX = m_ptLastComputerPoint.x - GO_SIZE;
		nLeftY = m_ptLastComputerPoint.y - GO_SIZE;

		nRightX = m_ptLastComputerPoint.x + GO_SIZE;
		nRightY = m_ptLastComputerPoint.y + GO_SIZE;

		dc.Ellipse(nLeftX, nLeftY, nRightX, nRightY);
	}

	m_ptLastComputerPoint = m_ptComputerPoint;
}

BOOL CGoBangDlg::IsGameOver()
{
	int i;
	
	for (i = 0; i < TUPLE_NUM; i++)
	{
		if (m_TupleInfo[i].eColor[0] != COLOR_BUTT
			&& m_TupleInfo[i].eColor[0] == m_TupleInfo[i].eColor[1]
			&& m_TupleInfo[i].eColor[0] == m_TupleInfo[i].eColor[2]
			&& m_TupleInfo[i].eColor[0] == m_TupleInfo[i].eColor[3]
			&& m_TupleInfo[i].eColor[0] == m_TupleInfo[i].eColor[4]
			)
		{
			m_bIsGameOver = true;
			return TRUE;
		}
	}
	
	return FALSE;
}

int CGoBangDlg::GetTupleScore(int nWhiteCnt, int nBlackCnt)
{
	if (5 == nWhiteCnt || 5 == nBlackCnt)
	{
		return -1;
	}

	if (nWhiteCnt < 0 ||  nWhiteCnt > 4
		|| nBlackCnt < 0 || nBlackCnt > 4)
	{
		MessageBox(_T("��������"), MB_OK);
		return -1;
	}

	if (nWhiteCnt == 0 && nBlackCnt == 0)
	{
		return m_TupleScoreTable[TUPLE_STATE_BLANK];
	}
	else if (nWhiteCnt == 0 && nBlackCnt == 1)
	{
		return m_TupleScoreTable[TUPLE_STATE_B];
	}
	else if (nWhiteCnt == 0 && nBlackCnt == 2)
	{
		return m_TupleScoreTable[TUPLE_STATE_BB];
	}
	else if (nWhiteCnt == 0 && nBlackCnt == 3)
	{
		return m_TupleScoreTable[TUPLE_STATE_BBB];
	}
	else if (nWhiteCnt == 0 && nBlackCnt == 4)
	{
		return m_TupleScoreTable[TUPLE_STATE_BBBB];
	}
	else if (nWhiteCnt == 1 && nBlackCnt == 0)
	{
		return m_TupleScoreTable[TUPLE_STATE_W];
	}
	else if (nWhiteCnt == 2 && nBlackCnt == 0)
	{
		return m_TupleScoreTable[TUPLE_STATE_WW];
	}
	else if (nWhiteCnt == 3 && nBlackCnt == 0)
	{
		return m_TupleScoreTable[TUPLE_STATE_WWW];
	}
	else if (nWhiteCnt == 4 && nBlackCnt == 0)
	{
		return m_TupleScoreTable[TUPLE_STATE_WWWW];
	}
	else 
	{
		return m_TupleScoreTable[TUPLE_STATE_BX_WX];
	}
}

void CGoBangDlg::UpdateTupleInfo()
{
	int i, j, k, t, nWhiteCount, nBlackCount, nIndex;
	
	nIndex = 0;
	nWhiteCount = 0;
	nBlackCount = 0;

	/* ͳ�Ƹ�����Ԫ����Ϣ */
	for (i = 0; i < LINE_NUM; i++)
	{
		for (j = 0; j < LINE_NUM - 4; j++)
		{
			for (k = 0; k < 5; k++)
			{
				m_TupleInfo[nIndex].stPoints[k] = m_stGridPointInfo[i][j + k].ptGridPoint;
				m_TupleInfo[nIndex].eColor[k] = m_stGridPointInfo[i][j + k].eColorType;

				switch (m_stGridPointInfo[i][j + k].eColorType)
				{
				case COLOR_WHITE:
						nWhiteCount++;
						break;

				case COLOR_BLACK:
						nBlackCount++;
						break;
				default:
					break;
				}
			}	

			m_TupleInfo[nIndex].nScore = GetTupleScore(nWhiteCount, nBlackCount);
			nIndex++; //165
			nWhiteCount = 0;
			nBlackCount = 0;
		}
	}

	/* ͳ�Ƹ�����Ԫ����Ϣ */
	for (j = 0; j < LINE_NUM; j++)
	{
		for (i = 0; i < LINE_NUM - 4; i++)
		{	
			for (k = 0; k < 5; k++)
			{
				m_TupleInfo[nIndex].stPoints[k] = m_stGridPointInfo[i + k][j].ptGridPoint;
				m_TupleInfo[nIndex].eColor[k] = m_stGridPointInfo[i + k][j].eColorType;
				
				switch (m_stGridPointInfo[i + k][j].eColorType)
				{
				case COLOR_WHITE:
						nWhiteCount++;
						break;

				case COLOR_BLACK:
						nBlackCount++;
						break;
				default:
					break;
				}
			}

			m_TupleInfo[nIndex].nScore = GetTupleScore(nWhiteCount, nBlackCount);
			nIndex++; //330
			nWhiteCount = 0;
			nBlackCount = 0;
		}
	}

	/* ͳ�����ϵ�������Ԫ����Ϣ */
	/* ͳ�����ϰ벿����Ԫ����Ϣ�����Խ��ߣ�*/
	for (i = 4; i < LINE_NUM; i++)
	{
		t = 0;
		for (j = 0; j <= (i - 4); j++)
		{
			for (k = 0; k < 5; k++)
			{
				m_TupleInfo[nIndex].stPoints[k] = m_stGridPointInfo[i - k - t][j + k].ptGridPoint;
				m_TupleInfo[nIndex].eColor[k] = m_stGridPointInfo[i - k - t][j + k].eColorType;

				switch (m_stGridPointInfo[i - k - t][j + k].eColorType)
				{
				case COLOR_WHITE:
						nWhiteCount++;
						break;

				case COLOR_BLACK:
						nBlackCount++;
						break;
				default:
					break;
				}
			}
			t++;

			m_TupleInfo[nIndex].nScore = GetTupleScore(nWhiteCount, nBlackCount);
			nIndex++; //396
			nWhiteCount = 0;
			nBlackCount = 0;
		}
	}

	/* ͳ�����°벿����Ԫ����Ϣ */
	for (i = 1; i < LINE_NUM - 4; i++)
	{
		t = 0;
		for (j = LINE_NUM - 1; j >= i + 4; j--)
		{
			for (k = 0; k < 5; k++)
			{
				m_TupleInfo[nIndex].stPoints[k] = m_stGridPointInfo[i + k + t][j - k].ptGridPoint;
				m_TupleInfo[nIndex].eColor[k] = m_stGridPointInfo[i + k + t][j - k].eColorType;

				switch (m_stGridPointInfo[i + k + t][j - k].eColorType)
				{
				case COLOR_WHITE:
						nWhiteCount++;
						break;

				case COLOR_BLACK:
						nBlackCount++;
						break;
				default:
					break;
				}
			}	
			t++;

			m_TupleInfo[nIndex].nScore = GetTupleScore(nWhiteCount, nBlackCount);
			nIndex++; //451
			nWhiteCount = 0;
			nBlackCount = 0;	
		}
	}

	/* ͳ�����µ�������Ԫ����Ϣ */
	/* ͳ�����°벿��Ԫ����Ϣ�����Խ���) */
	for (i = 0; i < LINE_NUM - 4; i++)
	{
		t = 0;
		for (j = 0; j < LINE_NUM - i - 4; j++)
		{
			for (k = 0; k < 5; k++)
			{
				m_TupleInfo[nIndex].stPoints[k] = m_stGridPointInfo[i + k + t][j + k].ptGridPoint;
				m_TupleInfo[nIndex].eColor[k] = m_stGridPointInfo[i + k + t][j + k].eColorType;

				switch (m_stGridPointInfo[i + k + t][j + k].eColorType)
				{
				case COLOR_WHITE:
						nWhiteCount++;
						break;

				case COLOR_BLACK:
						nBlackCount++;
						break;
				default:
					break;
				}
			}
			t++;

			m_TupleInfo[nIndex].nScore = GetTupleScore(nWhiteCount, nBlackCount);
			nIndex++; //517
			nWhiteCount = 0;
			nBlackCount = 0;
		}
	}

	/* ͳ�����ϰ벿��Ԫ����Ϣ */
	for (i = 4; i < LINE_NUM - 1; i++)
	{
		t = 0;
		for (j = LINE_NUM - 1; j >= LINE_NUM - i + 3; j--)
		{
			for (k = 0; k < 5; k++)
			{
				m_TupleInfo[nIndex].stPoints[k] = m_stGridPointInfo[i - k - t][j - k].ptGridPoint;
				m_TupleInfo[nIndex].eColor[k] = m_stGridPointInfo[i - k - t][j - k].eColorType;

				switch (m_stGridPointInfo[i - k - t][j - k].eColorType)
				{
				case COLOR_WHITE:
						nWhiteCount++;
						break;

				case COLOR_BLACK:
						nBlackCount++;
						break;
				default:
					break;
				}
			}
			t++;

			m_TupleInfo[nIndex].nScore = GetTupleScore(nWhiteCount, nBlackCount);
			nIndex++; //572
			nWhiteCount = 0;
			nBlackCount = 0;
		}
	}
}

void CGoBangDlg::GetBestPoint()
{
	int i, j, k, t;
	POINT ptBestPoint;
	int nBestScore = 0;

	/* ����ÿ���հ�λ������Ԫ���ֵ�ܺ� */
	for (i = 0; i < LINE_NUM; i++)
	{
		for (j = 0; j < LINE_NUM; j++)
		{
			m_stGridPointInfo[i][j].nScore = 0;

			if (!m_stGridPointInfo[i][j].IsHasGoBang)
			{
				for (k = 0; k < TUPLE_NUM; k++)
				{
					for (t = 0; t < 5; t++)
					{
						if (m_stGridPointInfo[i][j].ptGridPoint.x == m_TupleInfo[k].stPoints[t].x 
							&& m_stGridPointInfo[i][j].ptGridPoint.y == m_TupleInfo[k].stPoints[t].y)
						{
							m_stGridPointInfo[i][j].nScore += m_TupleInfo[k].nScore;
							break;
						}
					}
				}

				if (nBestScore < m_stGridPointInfo[i][j].nScore)
				{
					nBestScore = m_stGridPointInfo[i][j].nScore;
					ptBestPoint = m_stGridPointInfo[i][j].ptGridPoint;
				}
			}
		}
	}

	m_ptComputerPoint = ptBestPoint;
}

void CGoBangDlg::RestartGame()
{
	int i, j;

	memset(&m_ptComputerPoint, 0, sizeof(POINT));
	memset(&m_ptClickGridPoint, 0, sizeof(POINT));
	memset(&m_ptLastComputerPoint, 0, sizeof(POINT));

	m_bIsWhite = false;
	m_bIsBoardClicked = false;
	m_bIsLastComputer = false;
	m_bIsGameOver = false;

	memset(m_TupleInfo, 0, sizeof(m_TupleInfo));
	memset(m_stGridPointInfo, 0, sizeof(m_stGridPointInfo));

	for (i = 0; i < LINE_NUM; i++)
	{
		for (j = 0; j < LINE_NUM; j++)
		{
			m_stGridPointInfo[i][j].eColorType = COLOR_BUTT;
			m_stGridPointInfo[i][j].IsHasGoBang = FALSE;
			m_stGridPointInfo[i][j].ptGridPoint.x = -1;
			m_stGridPointInfo[i][j].ptGridPoint.y = -1;
		}
	}

	CRect rect;
	GetClientRect(&rect);
	InvalidateRect(&rect, 1);

	GetDlgItem(IDC_BTN_RESTART)->ShowWindow(SW_HIDE);
}

void CGoBangDlg::OnBnClickedBtnRestart()
{
	RestartGame();
}
