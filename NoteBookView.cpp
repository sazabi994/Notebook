
// NoteBookView.cpp : CNoteBookView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "NoteBook.h"
#endif

#include "NoteBookDoc.h"
#include "NoteBookView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNoteBookView

IMPLEMENT_DYNCREATE(CNoteBookView, CView)

BEGIN_MESSAGE_MAP(CNoteBookView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CHAR()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CNoteBookView ����/����

CNoteBookView::CNoteBookView()
	: m_iMouseSelected(0)
	, m_ptCaret(0)
	, m_strLine(_T(""))
	, m_ptWriteCaret(0)
	, m_ptOrgCaret(0)
	, m_strLeft(_T(""))
	, m_strRight(_T(""))
	, m_IsCaretEnter(0)
{
	// TODO: �ڴ˴���ӹ������
	m_iMouseSelected=0;
	m_ptCaret=CPoint(0,0);
	m_ptWriteCaret=CPoint(0,0);

}

CNoteBookView::~CNoteBookView()
{
}

BOOL CNoteBookView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CNoteBookView ����

void CNoteBookView::OnDraw(CDC* /*pDC*/)
{
	CNoteBookDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CNoteBookView ��ӡ

BOOL CNoteBookView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CNoteBookView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CNoteBookView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CNoteBookView ���

#ifdef _DEBUG
void CNoteBookView::AssertValid() const
{
	CView::AssertValid();
}

void CNoteBookView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNoteBookDoc* CNoteBookView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNoteBookDoc)));
	return (CNoteBookDoc*)m_pDocument;
}
#endif //_DEBUG


// CNoteBookView ��Ϣ�������





void CNoteBookView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDC *dc;
	dc=GetDC();  //��ȡDC���
	CString str;
	
	CSize sz;
	CPoint pt;
	TEXTMETRIC tm;   //����������Ϣ
	dc->GetTextMetrics(&tm);   //��ȡDC�е�������Ϣ
	CString strChar;
	int IsLeft=0;  //�����־λ��

	//���ĵ���ͼ�����һ������ʱ
	if(m_ptCaret==m_ptWriteCaret)
	{
		//�س������
		if(0x0d==nChar)
		{
			m_ptCaret.y+=tm.tmHeight;
			m_ptCaret.x=0;
			//����Ϊ��λ����������ַ���
			m_strAarray.Add(m_strLine);
			m_strLine.Empty();    //����������飻
			m_IsCaretEnter=0;
		}
		else if(0x08==nChar)
			//�˸����ʵ��
		{
			COLORREF clr=dc->SetTextColor(dc->GetBkColor());//�������ɫ����Ϊ����ɫ
			dc->TextOut(0,m_ptCaret.y,m_strLine); //�ѱ���ɫ�������
			m_strLine=m_strLine.Left(m_strLine.GetLength()-1);//����һ�����ֺ󸶸�str
			dc->SetTextColor(clr);                            //��������ɫ����Ϊԭ����ɫ
			dc->TextOutA(0,m_ptCaret.y,m_strLine);//�ٴ������
		}
		/*//���
		else if(0x1B==nChar)
		{
			CString strR;
			strR=m_strLine.Right(1);
			CSize str=dc->GetTextExtent(strR);
			pt.x=(dc->GetTextExtent(m_strLine)).cx-str.cx;
			pt.y=m_ptCaret.y;
			IsLeft=1;
		}*/
		else
			m_strLine+=(char)nChar;   //�������Լӵ��ַ�����
		//if(IsLeft==0)
		//{
			//����������ַ�������ƶ����ƶ�
			sz=dc->GetTextExtent(m_strLine);  //��ȡ�ַ�����������״��Ϣ
			pt.x=0+sz.cx;
			pt.y=m_ptCaret.y;
		//}
		SetCaretPos(pt);                  //��������������ַ�����ĩβ
		//IsLeft=0;

		//�������
		dc->TextOut(0,m_ptCaret.y,m_strLine);
		//�ռ����꣬������������λ��
		m_ptCaret=pt;
		//�����������ֵ����λ��	
		m_ptWriteCaret=pt;
		m_ptOrgCaret=pt;//�����������λ�ñ���
	}
	//��������������м����
	else
	{
		
		//��ȡ����������м���������Ϣ
		int iRow=m_ptCaret.y/tm.tmHeight;//��ȡ������ڵ��ǵڼ���
		CString strLine;	         
		strLine=m_strAarray.GetAt(iRow);//��ȡ���е��ַ���
		CSize sz=dc->GetTextExtent(strLine); //��ȡ�����ַ����ľ��ξ���
        CString strLeft;
		CString strRight;

		//�ڲ���λ�ü�������
		if(m_ptOrgCaret==m_ptCaret)
		{
			//����س������
			if(0x0d==nChar)
			{
				strLeft=m_strLeft;  //��ȡԭ����ߵ��ַ�
				strRight=m_strRight;//��ȡԭ���ұߵ��ַ�
				//strLeft+=(char)nChar;//�����µ�����

				COLORREF clr=dc->SetTextColor(dc->GetBkColor());//�������ɫ����Ϊ����ɫ
				int Row=m_strAarray.GetSize();//��ȡԪ�صĸ���
				CPoint ptPoint=CPoint(0,0);

				//��ԭ�����ȫ������
				for(int i=0;i<Row;i++)
				{
					dc->TextOutA(ptPoint.x,ptPoint.y,m_strAarray.GetAt(i));
					ptPoint.y+=tm.tmHeight;
				}

				dc->SetTextColor(clr);//����ԭ����ɫ��

				//����Ϊ��λ����������ַ���
				m_strAarray.SetAtGrow(iRow,m_strLeft);//�Ѳ������ߵ��������¸�����iRow��
				CString strAppend;
				strAppend=m_strAarray.GetAt(Row-1);//�Ȱ����һ�е��ַ�����ʱ��������
				//�ӵ����ڶ�����ʼ��ǰÿһ��Ԫ�ض�����ƶ�һλ
				for(int i=Row-2;i>iRow;i--)
					m_strAarray.SetAtGrow(i,m_strAarray.GetAt(i+1));
				m_strAarray.SetAtGrow(iRow+1,strRight);//������ұߵ��ַ����ƶ�����һ��Ԫ���У�
				m_strAarray.Add(strAppend);//�����һ��Ԫ�����ӵ�ĩβ
				
				//���ȫ�������ࣻ
				ptPoint=CPoint(0,0);
				for(int i=0;i<=Row;i++)
				{
					CString strLook;
					strLook=m_strAarray.GetAt(i);
					dc->TextOutA(0,ptPoint.y,m_strAarray.GetAt(i));
					ptPoint.y+=tm.tmHeight;//�����һ�к�����λ������һ�У�
				}

				m_ptWriteCaret.y=ptPoint.y;
				m_ptWriteCaret.x=(dc->GetTextExtent(m_strAarray.GetAt(Row))).cx;
				//���ò��������ʾλ��
				ptPoint.y=(iRow+1)*tm.tmHeight;
				ptPoint.x=0;
				SetCaretPos(ptPoint);

				//��strLeft��strRight�ָ�ԭֵ��
				strLeft.Empty();
				strLeft=m_strLeft;
				strRight.Empty();
				strRight=m_strRight;
				m_IsCaretEnter=1;

				//�������������λ��
			}
			else if(0x08==nChar)
				//�˸����ʵ��
			{
				COLORREF clr=dc->SetTextColor(dc->GetBkColor());//�������ɫ����Ϊ����ɫ
				dc->TextOut(0,m_ptCaret.y,m_strLine); //�ѱ���ɫ�������
				m_strLine=m_strLine.Left(m_strLine.GetLength()-1);//����һ�����ֺ󸶸�str
				dc->SetTextColor(clr);                            //��������ɫ����Ϊԭ����ɫ
				dc->TextOutA(0,m_ptCaret.y,m_strLine);//�ٴ������
			}
			else
			{
				strLeft=m_strLeft;  //��ȡԭ����ߵ��ַ�
				strRight=m_strRight;//��ȡԭ���ұߵ��ַ�
				strLeft+=(char)nChar;//�����µ�����
				m_strLeft=strLeft;   //������µ��ַ�����ȫ�ֱ���
			}

		}
		//����λ�ò���
		else
		{
			
			int iNum=m_ptCaret.x/tm.tmAveCharWidth;//��ȡ�����λ�ڱ��еڼ�����֮��
			strLeft=strLine.Left(iNum);//��ȡ��������ߵ�����
			strRight=strLine.Right(strLine.GetLength()-iNum);
			strLeft+=(char)nChar;//��ȡ��������ַ���
		}

        if(m_IsCaretEnter==0)
		{
			//����ԭ�������
			COLORREF clr=dc->SetTextColor(dc->GetBkColor());//��ȡ���ڱ���ɫ
			dc->TextOut(0,m_ptCaret.y,strLine);//�ѱ���ɫ���ȫ������
			dc->SetTextColor(clr);//��������������ɫ
		

			//�����µĹ��λ��
			CSize szStr1=dc->GetTextExtent(strLeft);//��ȡ�������ּ�����������ֿ��
			pt.x=szStr1.cx;//��ֵ�������
			pt.y=m_ptCaret.y;
			SetCaretPos(pt);//���ù�ꣻ

			//���ַ������ݺϲ�	
			strLine.Empty();
			strLine=strLeft+strRight;//���������������һ��
			dc->TextOutA(0,pt.y,strLine);//������֣�

			m_strLeft=strLeft;
			m_strRight=strRight;

			m_ptCaret=pt;
			m_ptOrgCaret=m_ptCaret;//�����������λ�ñ���
			m_strAarray.SetAtGrow(iRow,strLine);//���µ��ַ�������ԭ�ַ���
			
			m_strLine=strLine;
		}
	}
	
	CView::OnChar(nChar, nRepCnt, nFlags);
}


int CNoteBookView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    
	// TODO:  �ڴ������ר�õĴ�������
	SetClassLong(m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_IBEAM));
	CDC *dc;
	dc=GetDC();
	TEXTMETRIC tm;
	GetTextMetrics(*dc,&tm);
	if(m_iMouseSelected==0)
	{
		//���������
		CPoint point;
		point.x=0;
		point.y=0;
		CreateSolidCaret(2,tm.tmHeight);
		SetCaretPos(point);
		ShowCaret();
		m_iMouseSelected=1;
		//���ݹ��λ��
		m_ptCaret=point;
	}

	return 0;
}


void CNoteBookView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDC *dc;
	dc=GetDC();
	TEXTMETRIC tm;
	dc->GetTextMetrics(&tm);//��ȡ������Ϣ
	CPoint pt;//������������
	int iHeight;
	CPoint ptPoint;
	CString strLine;
	int iNum;
	if(m_IsCaretEnter==0)
	{
		//��û������س������һ�л�Ψһһ���ַ����뵽��������
		m_strAarray.Add(m_strLine);
		//����ַ���
		m_strLine.Empty();
	}
	else
		m_IsCaretEnter=0;

	//�ж��Ƿ����������֣�
	if(m_ptWriteCaret.x==0&&m_ptWriteCaret.y==0)
		pt=CPoint(0,0);
	//����������
	else
	{
		
		//���λ�ó����������ַ������
		if(point.y>(m_ptWriteCaret.y+tm.tmHeight))
			pt=m_ptWriteCaret;//���ַ��������������ֵ�ĩβ��

		//�����������������������֮��
		else 
		{
			iHeight=point.y/tm.tmHeight;//�жϲ�����ڵڼ��У�
			strLine=m_strAarray.GetAt(iHeight);//�Ӽ������л�ȡ�����ַ�����
			CSize sz=dc->GetTextExtent(strLine);//��ȡ�����ַ����ĳ���
			iNum=point.x/tm.tmAveCharWidth;     //��ȡ�����ַ����ĵڼ���λ�ò���
			//��ÿ������������
			if(point.x<sz.cx)
			{
				//������е����
				if(iHeight==0)
				{
					//�������У���һ����ĸ��
					if(iNum==0)
						ptPoint.x=0;
					//����������ʱ
					else
						ptPoint.x=(dc->GetTextExtent(strLine.Left(iNum))).cx;//������ĺ����ꣻ
					ptPoint.y=0;
					pt=ptPoint;
				}
				//������ʱ
				else
				{
					//���е����
					if(iNum==0)
						ptPoint.x=0;
					else
					{
						ptPoint.x=(dc->GetTextExtent(strLine.Left(iNum))).cx;//������ĺ����ꣻ
					}
					ptPoint.y=iHeight*tm.tmHeight;
					pt=ptPoint;
				}
			}
			//��ÿ�к�����������������
			else
			{
				ptPoint.y=iHeight*tm.tmHeight;
				ptPoint.x=sz.cx;
				pt=ptPoint;
			}
			CString strLeft;//�����ߵ��ַ�
			strLeft=strLine.Left(iNum);
			dc->TextOutA(0,pt.y,strLeft);//��������ߵ��ַ�
			CString strRight;
			strRight=strLine.Right(strLine.GetLength()-iNum);//��ȡ����ұߵ��ַ���
			dc->TextOut(dc->GetTextExtent(strLine.Left(iNum)).cx,pt.y,strRight);
		}
		//����������ʵʱλ��
		m_ptCaret=pt;	
		SetCaretPos(pt);
		
	}
	


	CView::OnLButtonDown(nFlags, point);
}
