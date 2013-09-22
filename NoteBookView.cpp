
// NoteBookView.cpp : CNoteBookView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CHAR()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CNoteBookView 构造/析构

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
	// TODO: 在此处添加构造代码
	m_iMouseSelected=0;
	m_ptCaret=CPoint(0,0);
	m_ptWriteCaret=CPoint(0,0);

}

CNoteBookView::~CNoteBookView()
{
}

BOOL CNoteBookView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CNoteBookView 绘制

void CNoteBookView::OnDraw(CDC* /*pDC*/)
{
	CNoteBookDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CNoteBookView 打印

BOOL CNoteBookView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CNoteBookView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CNoteBookView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CNoteBookView 诊断

#ifdef _DEBUG
void CNoteBookView::AssertValid() const
{
	CView::AssertValid();
}

void CNoteBookView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNoteBookDoc* CNoteBookView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNoteBookDoc)));
	return (CNoteBookDoc*)m_pDocument;
}
#endif //_DEBUG


// CNoteBookView 消息处理程序





void CNoteBookView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC *dc;
	dc=GetDC();  //获取DC句柄
	CString str;
	
	CSize sz;
	CPoint pt;
	TEXTMETRIC tm;   //保存字体信息
	dc->GetTextMetrics(&tm);   //获取DC中的字体信息
	CString strChar;
	int IsLeft=0;  //左键标志位；

	//当文档出图随光标第一次输入时
	if(m_ptCaret==m_ptWriteCaret)
	{
		//回车的情况
		if(0x0d==nChar)
		{
			m_ptCaret.y+=tm.tmHeight;
			m_ptCaret.x=0;
			//以行为单位保存输入的字符串
			m_strAarray.Add(m_strLine);
			m_strLine.Empty();    //清空字体数组；
			m_IsCaretEnter=0;
		}
		else if(0x08==nChar)
			//退格键的实现
		{
			COLORREF clr=dc->SetTextColor(dc->GetBkColor());//将输出颜色设置为背景色
			dc->TextOut(0,m_ptCaret.y,m_strLine); //已背景色输出文字
			m_strLine=m_strLine.Left(m_strLine.GetLength()-1);//减少一个文字后付给str
			dc->SetTextColor(clr);                            //将字体颜色设置为原有颜色
			dc->TextOutA(0,m_ptCaret.y,m_strLine);//再次输出；
		}
		/*//左键
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
			m_strLine+=(char)nChar;   //将字体自加到字符串中
		//if(IsLeft==0)
		//{
			//将光标随着字符输入的移动而移动
			sz=dc->GetTextExtent(m_strLine);  //获取字符串的整体形状信息
			pt.x=0+sz.cx;
			pt.y=m_ptCaret.y;
		//}
		SetCaretPos(pt);                  //将插入符设置在字符串的末尾
		//IsLeft=0;

		//输出文字
		dc->TextOut(0,m_ptCaret.y,m_strLine);
		//收集坐标，给插入符的最后位置
		m_ptCaret=pt;
		//保存输入文字的最后位置	
		m_ptWriteCaret=pt;
		m_ptOrgCaret=pt;//将本次输入的位置保留
	}
	//在已输入的文字中间添加
	else
	{
		
		//获取插入符所在行及所在行信息
		int iRow=m_ptCaret.y/tm.tmHeight;//获取光标所在的是第几行
		CString strLine;	         
		strLine=m_strAarray.GetAt(iRow);//获取本行的字符串
		CSize sz=dc->GetTextExtent(strLine); //获取本行字符串的矩形距离
        CString strLeft;
		CString strRight;

		//在插入位置继续输入
		if(m_ptOrgCaret==m_ptCaret)
		{
			//插入回车的情况
			if(0x0d==nChar)
			{
				strLeft=m_strLeft;  //获取原有左边的字符
				strRight=m_strRight;//获取原有右边的字符
				//strLeft+=(char)nChar;//增加新的文字

				COLORREF clr=dc->SetTextColor(dc->GetBkColor());//将输出颜色设置为背景色
				int Row=m_strAarray.GetSize();//获取元素的个数
				CPoint ptPoint=CPoint(0,0);

				//将原有输出全部擦除
				for(int i=0;i<Row;i++)
				{
					dc->TextOutA(ptPoint.x,ptPoint.y,m_strAarray.GetAt(i));
					ptPoint.y+=tm.tmHeight;
				}

				dc->SetTextColor(clr);//换回原有颜色；

				//以行为单位保存输入的字符串
				m_strAarray.SetAtGrow(iRow,m_strLeft);//把插入符左边的文字重新付给第iRow行
				CString strAppend;
				strAppend=m_strAarray.GetAt(Row-1);//先把左后一行的字符串暂时附到外面
				//从倒数第二个开始向前每一个元素都想后移动一位
				for(int i=Row-2;i>iRow;i--)
					m_strAarray.SetAtGrow(i,m_strAarray.GetAt(i+1));
				m_strAarray.SetAtGrow(iRow+1,strRight);//插入符右边的字符串移动到下一个元素中；
				m_strAarray.Add(strAppend);//把最后一个元素增加到末尾
				
				//输出全部集合类；
				ptPoint=CPoint(0,0);
				for(int i=0;i<=Row;i++)
				{
					CString strLook;
					strLook=m_strAarray.GetAt(i);
					dc->TextOutA(0,ptPoint.y,m_strAarray.GetAt(i));
					ptPoint.y+=tm.tmHeight;//输出完一行后插入符位置下移一行；
				}

				m_ptWriteCaret.y=ptPoint.y;
				m_ptWriteCaret.x=(dc->GetTextExtent(m_strAarray.GetAt(Row))).cx;
				//设置插入符的显示位置
				ptPoint.y=(iRow+1)*tm.tmHeight;
				ptPoint.x=0;
				SetCaretPos(ptPoint);

				//将strLeft、strRight恢复原值；
				strLeft.Empty();
				strLeft=m_strLeft;
				strRight.Empty();
				strRight=m_strRight;
				m_IsCaretEnter=1;

				//设置文字输出的位置
			}
			else if(0x08==nChar)
				//退格键的实现
			{
				COLORREF clr=dc->SetTextColor(dc->GetBkColor());//将输出颜色设置为背景色
				dc->TextOut(0,m_ptCaret.y,m_strLine); //已背景色输出文字
				m_strLine=m_strLine.Left(m_strLine.GetLength()-1);//减少一个文字后付给str
				dc->SetTextColor(clr);                            //将字体颜色设置为原有颜色
				dc->TextOutA(0,m_ptCaret.y,m_strLine);//再次输出；
			}
			else
			{
				strLeft=m_strLeft;  //获取原有左边的字符
				strRight=m_strRight;//获取原有右边的字符
				strLeft+=(char)nChar;//增加新的文字
				m_strLeft=strLeft;   //把左边新的字符付给全局变量
			}

		}
		//在新位置插入
		else
		{
			
			int iNum=m_ptCaret.x/tm.tmAveCharWidth;//获取插入符位于本行第几个字之间
			strLeft=strLine.Left(iNum);//获取操作符左边的文字
			strRight=strLine.Right(strLine.GetLength()-iNum);
			strLeft+=(char)nChar;//获取新输入的字符；
		}

        if(m_IsCaretEnter==0)
		{
			//擦除原有输出；
			COLORREF clr=dc->SetTextColor(dc->GetBkColor());//获取窗口背景色
			dc->TextOut(0,m_ptCaret.y,strLine);//已背景色输出全部文字
			dc->SetTextColor(clr);//重新设置字体颜色
		

			//设置新的光标位置
			CSize szStr1=dc->GetTextExtent(strLeft);//获取输出左边字及新输入的文字宽度
			pt.x=szStr1.cx;//将值付给光标
			pt.y=m_ptCaret.y;
			SetCaretPos(pt);//设置光标；

			//将字符串内容合并	
			strLine.Empty();
			strLine=strLeft+strRight;//将正行文字组合在一起；
			dc->TextOutA(0,pt.y,strLine);//输出文字；

			m_strLeft=strLeft;
			m_strRight=strRight;

			m_ptCaret=pt;
			m_ptOrgCaret=m_ptCaret;//将本次输入的位置保留
			m_strAarray.SetAtGrow(iRow,strLine);//将新的字符串覆盖原字符串
			
			m_strLine=strLine;
		}
	}
	
	CView::OnChar(nChar, nRepCnt, nFlags);
}


int CNoteBookView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    
	// TODO:  在此添加您专用的创建代码
	SetClassLong(m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_IBEAM));
	CDC *dc;
	dc=GetDC();
	TEXTMETRIC tm;
	GetTextMetrics(*dc,&tm);
	if(m_iMouseSelected==0)
	{
		//创建插入符
		CPoint point;
		point.x=0;
		point.y=0;
		CreateSolidCaret(2,tm.tmHeight);
		SetCaretPos(point);
		ShowCaret();
		m_iMouseSelected=1;
		//传递光标位置
		m_ptCaret=point;
	}

	return 0;
}


void CNoteBookView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC *dc;
	dc=GetDC();
	TEXTMETRIC tm;
	dc->GetTextMetrics(&tm);//获取字体信息
	CPoint pt;//保存插入符坐标
	int iHeight;
	CPoint ptPoint;
	CString strLine;
	int iNum;
	if(m_IsCaretEnter==0)
	{
		//把没有输入回车的最后一行或唯一一行字符输入到集合类中
		m_strAarray.Add(m_strLine);
		//清空字符串
		m_strLine.Empty();
	}
	else
		m_IsCaretEnter=0;

	//判断是否输入了文字；
	if(m_ptWriteCaret.x==0&&m_ptWriteCaret.y==0)
		pt=CPoint(0,0);
	//已输入文字
	else
	{
		
		//光标位置超过了输入字符的情况
		if(point.y>(m_ptWriteCaret.y+tm.tmHeight))
			pt=m_ptWriteCaret;//将字符创放在输入文字的末尾；

		//插入符纵坐标在输入纵坐标之内
		else 
		{
			iHeight=point.y/tm.tmHeight;//判断插入符在第几行；
			strLine=m_strAarray.GetAt(iHeight);//从集合类中获取改行字符串；
			CSize sz=dc->GetTextExtent(strLine);//获取改行字符串的长度
			iNum=point.x/tm.tmAveCharWidth;     //获取改行字符串的第几个位置插入
			//在每行已输入字里
			if(point.x<sz.cx)
			{
				//点击首行的情况
				if(iHeight==0)
				{
					//首行首列（第一个字母）
					if(iNum==0)
						ptPoint.x=0;
					//首行其他列时
					else
						ptPoint.x=(dc->GetTextExtent(strLine.Left(iNum))).cx;//插入符的横坐标；
					ptPoint.y=0;
					pt=ptPoint;
				}
				//非首行时
				else
				{
					//首列的情况
					if(iNum==0)
						ptPoint.x=0;
					else
					{
						ptPoint.x=(dc->GetTextExtent(strLine.Left(iNum))).cx;//插入符的横坐标；
					}
					ptPoint.y=iHeight*tm.tmHeight;
					pt=ptPoint;
				}
			}
			//在每行横坐标已输入文字外
			else
			{
				ptPoint.y=iHeight*tm.tmHeight;
				ptPoint.x=sz.cx;
				pt=ptPoint;
			}
			CString strLeft;//光标左边的字符
			strLeft=strLine.Left(iNum);
			dc->TextOutA(0,pt.y,strLeft);//输出光标左边的字符
			CString strRight;
			strRight=strLine.Right(strLine.GetLength()-iNum);//获取光标右边的字符；
			dc->TextOut(dc->GetTextExtent(strLine.Left(iNum)).cx,pt.y,strRight);
		}
		//保存插入符的实时位置
		m_ptCaret=pt;	
		SetCaretPos(pt);
		
	}
	


	CView::OnLButtonDown(nFlags, point);
}
