#include "Stdafx.h"
#include "Mmsystem.h"
#include "Resource.h"
#include "GameFrameApp.h"
#include "GameFrameDlg.h"
#include "GlobalOption.h"
#include "GameFrameControl.h"
#include "ClientKernelSink.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameDlg, CSkinDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_GETMINMAXINFO()
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDC_CONTROL, OnBnClickedControl)
	ON_BN_CLICKED(IDC_SOUND, OnSound)
	ON_BN_CLICKED(IDC_OPTION_SET, OnOptionSet)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameDlg::CGameFrameDlg(CGameFrameView * pGameFrameView)  : CSkinDialog(IDD_GAME_FRAME), m_VorSplitter(VorSpliter)
{
	//界面状态
	m_bRectify=false;
	m_bHideList=false;
	m_rcNormalSize.SetRect(0,0,0,0);

	//设置变量
	m_bReset=false;
	m_bGameSceen=false;
	m_bInitDialog=false;

	//组件接口
	m_pKernelSink=NULL;
	m_pGameFrameControl=NULL;
	m_pGameFrameView=pGameFrameView;

	//配置变量
	m_bAllowSound=true;

	//创建钩子
	m_pKernelSink=new CClientKernelSink(*this);

	return;
}

//析构函数
CGameFrameDlg::~CGameFrameDlg()
{
	SafeDelete(m_pKernelSink);
	SafeDelete(m_pGameFrameControl);
	return;
}

//按钮消息
bool CGameFrameDlg::OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton)
{
	return true;
}

//拆分条消息
bool CGameFrameDlg::OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos)
{
	//设置变量
	m_bRectify=true;

	//调整界面
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return true;
}

//调整界面
void CGameFrameDlg::RectifyControl(int nWidth, int nHeight)
{

	/////////////////////////////////////////////////////////////////////////////
	CRect rcClient;
	GetWindowRect(&rcClient);
	int nWidth1=rcClient.Width();
	int nHeight1=rcClient.Height();
	//调整判断
	if ((nWidth1!=0)&&(nHeight1!=0))
	{
		//框架位置
		CRect rcFrame;
		rcFrame.SetRect(3,3,nWidth1-2,nHeight1-2);

		//窗口区域
		CRgn RegionWindow;
		RegionWindow.CreateRoundRectRgn(rcFrame.left,rcFrame.top,rcFrame.right,rcFrame.bottom,5,5);

		//设置区域
		SetWindowRgn(RegionWindow,TRUE);
	}
	///////////////////////////////////////////////////////////////////////////

	//状态判断
	if (m_bInitDialog==false) return;

	//变量定义
	CRect rcVorSplitter;
	const int nSplitterWidth=3;
	const int iXExc=GetXExcursionPos();
	const int iYExc=GetYExcursionPos();
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//获取位置
	m_VorSplitter.GetWindowRect(&rcVorSplitter);
	ScreenToClient(&rcVorSplitter);

	//调整拆分条
	if (m_bRectify==false)
	{
		//设置变量
		m_bRectify=true;

		//设置位置
		if (m_bHideList==false)
		{
			rcVorSplitter.left=nWidth-380+12;
			rcVorSplitter.right=rcVorSplitter.left+nSplitterWidth+127;
		}
		else
		{
			rcVorSplitter.right=nWidth;
			rcVorSplitter.left=nWidth-8;
		}
	}

	//设置拆分范围
	if (m_bHideList==false)
	{
		int nLessPos=nWidth-295,nMaxPos=nWidth-295;
		m_VorSplitter.SetSplitterRange(nLessPos,nMaxPos);
		if ((nWidth!=0)&&(nHeight!=0))
		{
			if (rcVorSplitter.left<nLessPos) 
			{
				rcVorSplitter.left=nLessPos;
				rcVorSplitter.right=rcVorSplitter.left+nSplitterWidth;
			}
			if (rcVorSplitter.right>nMaxPos)
			{
				rcVorSplitter.right=nMaxPos;
				rcVorSplitter.left=rcVorSplitter.right-nSplitterWidth;
			}
		}
	}

	if (m_VorSplitter.GetSplitterType()==HorSpliter) rcVorSplitter.SetRect(nWidth,0,nWidth,nHeight);

	//锁定屏幕
	LockWindowUpdate();

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(32);
	m_VorSplitter.ShowWindow(0);

	CImageHandle TitleLeft1(&m_SkinAttribute.m_FramLeft);
	if (m_SkinAttribute.m_FramLeft.IsNull()) return ;
	CImageHandle TitleMid1(&m_SkinAttribute.m_BottomM);
	if (m_SkinAttribute.m_BottomM.IsNull()) return ;
	CImageHandle TitleRight1(&m_SkinAttribute.m_FramRight);
	if (m_SkinAttribute.m_FramRight.IsNull()) return ;

	//if(true==g_GlobalOption.bNiuNiuGame)
	//{
	//DeferWindowPos(hDwp,m_pGameFrameView->GetSafeHwnd(),NULL,iXExc+TitleLeft1->GetWidth(),iYExc,nWidth-TitleRight1->GetWidth()-iXExc-TitleLeft1->GetWidth(),nHeight-iYExc-TitleMid1->GetHeight(),uFlags);
	////DeferWindowPos(hDwp,m_VorSplitter,NULL,rcVorSplitter.left,iYExc,rcVorSplitter.Width(),nHeight-iYExc-3,uFlags);
	//DeferWindowPos(hDwp,m_pGameFrameControl->GetSafeHwnd(),NULL,rcVorSplitter.right,iYExc,0,0,uFlags);
	//}else
	//{
	DeferWindowPos(hDwp,m_btOptionSet,NULL, nWidth-160,5,23,23,uFlags);
	//DeferWindowPos(hDwp,m_btSound,NULL, nWidth-160,0,22,22,uFlags);
	DeferWindowPos(hDwp,m_pGameFrameView->GetSafeHwnd(),NULL,iXExc+TitleLeft1->GetWidth()-1,iYExc,rcVorSplitter.right+50+22,nHeight-iYExc-TitleMid1->GetHeight(),uFlags);
	//DeferWindowPos(hDwp,m_pGameFrameView->GetSafeHwnd(),NULL,iXExc+TitleLeft1->GetWidth(),iYExc,nWidth - RightT->GetWidth()-10,nHeight-iYExc-TitleMid1->GetHeight(),uFlags);
	//DeferWindowPos(hDwp,m_VorSplitter,NULL,rcVorSplitter.left,iYExc,rcVorSplitter.Width(),nHeight-iYExc-3,uFlags);
	DeferWindowPos(hDwp,m_pGameFrameControl->GetSafeHwnd(),NULL,nWidth-215,iYExc,215,nHeight-iYExc-TitleMid1->GetHeight(),uFlags);
	//}
	EndDeferWindowPos(hDwp);

	//重画界面
	Invalidate(FALSE);
	UpdateWindow();

	//解除锁定
	UnlockWindowUpdate();

	//控制按钮
	CRect rcControl;
	//m_btControl.GetClientRect(&rcControl);
	//m_btControl.SetWindowPos(NULL,rcVorSplitter.left+(rcVorSplitter.Width()-rcControl.Width())/2+3,(nHeight-rcControl.Height())/2,0,0,uFlags|SWP_NOSIZE);
	m_btControl.ShowWindow(0);
	m_btOptionSet.ShowWindow(SW_SHOW);
	m_btSound.ShowWindow(0);
	//设置变量
	m_bRectify=false;

	return;
}

//网络消息
bool CGameFrameDlg::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{

	return false;
}

//用户进入
void __cdecl CGameFrameDlg::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//	AfxMessageBox("CGameFrameDlg::OnEventUserEnter");
	return;
}

//用户离开
void __cdecl CGameFrameDlg::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	return;
}

//用户积分
void __cdecl CGameFrameDlg::OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	return;
}

//用户状态
void __cdecl CGameFrameDlg::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	return;
}

//初始道具
void __cdecl CGameFrameDlg::OnEventInitProperty(tagPropertyInfo *pPropertyInfo, int nInfoCount)
{
	//获取对象
	CPropertyBar *pPropertyBar = CPropertyBar::GetInstance();
	ASSERT( pPropertyBar != NULL );
	if ( pPropertyBar == NULL ) return;

	//初始结构
	for ( int nIndex = 0; nIndex < nInfoCount; ++nIndex )
	{
		pPropertyBar->SetPropertyInfo((pPropertyInfo+nIndex)->nPropertyID, *(pPropertyInfo+nIndex));
	}

	return;
}

//初始鲜花
void __cdecl CGameFrameDlg::OnEventInitFlower(tagFlowerInfo *pFlowerInfo, int nInfoCount)
{
	//获取对象
	CPropertyBar *pPropertyBar = CPropertyBar::GetInstance();
	ASSERT( pPropertyBar != NULL );
	if ( pPropertyBar == NULL ) return;

	//初始结构
	for ( int nIndex = 0; nIndex < nInfoCount; ++nIndex )
	{
		pPropertyBar->SetFlowerInfo((pFlowerInfo+nIndex)->nFlowerID, *(pFlowerInfo+nIndex));
	}

	return;
}

//鲜花消息
void __cdecl CGameFrameDlg::OnEventFlower(const tagUserData * pSendUserData, const tagUserData * pRecvUserData, UINT uFlowerID, UINT uFlowerEffectID)
{
	//界面显示
	m_pGameFrameView->ShowFlower(SwitchViewChairID(pSendUserData->wChairID), SwitchViewChairID(pRecvUserData->wChairID), uFlowerID, 
		uFlowerEffectID, m_ClientKernelHelper->GetMeChairID()==pRecvUserData->wChairID);

	return;
}

//控制列表
bool CGameFrameDlg::ShowViewControl(bool bShowControl)
{
	if (bShowControl!=m_bHideList)
	{
		//设置变量
		m_bHideList=bShowControl;

		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//调整界面
		RectifyControl(rcClient.Width(),rcClient.Height());
	}

	return true;
}

//更新资源
bool CGameFrameDlg::UpdateSkinResource()
{
	HINSTANCE hInstanceMe=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btSound.SetButtonImage(m_bAllowSound?IDB_SOUND1:IDB_SOUND0, hInstanceMe, false);

	m_SkinAttribute.m_TitleLeft.SetLoadInfo(IDB_DLG_TL, hInstanceMe);
	m_SkinAttribute.m_TitleMid.SetLoadInfo(IDB_DLG_TM, hInstanceMe);
	m_SkinAttribute.m_TitleRight.SetLoadInfo(IDB_DLG_TR, hInstanceMe);

	m_SkinAttribute.m_FramLeft.SetLoadInfo(IDB_DLG_ML, hInstanceMe);
	m_SkinAttribute.m_FramRight.SetLoadInfo(IDB_DLG_MR, hInstanceMe);

	m_SkinAttribute.m_BottomL.SetLoadInfo(IDB_DLG_BL, hInstanceMe);
	m_SkinAttribute.m_BottomM.SetLoadInfo(IDB_DLG_BM, hInstanceMe);
	m_SkinAttribute.m_BottomR.SetLoadInfo(IDB_DLG_BR,hInstanceMe);

	m_SkinAttribute.m_ButtonClose.SetLoadInfo(IDB_DLG_CLOSE, hInstanceMe);
	m_SkinAttribute.m_ButtonMax.SetLoadInfo(IDB_DLG_MAX, hInstanceMe);
	m_SkinAttribute.m_ButtonMin.SetLoadInfo(IDB_DLG_MIN, hInstanceMe);
	m_SkinAttribute.m_ButtonRes.SetLoadInfo(IDB_DLG_RESTORE, hInstanceMe);
	m_SkinAttribute.m_crCaptionTXColor = RGB(222,221,201);

	m_btOptionSet.SetButtonImage(IDB_DLG_OPTIONSET, hInstanceMe,true);

	m_pGameFrameControl->m_UserListView.m_SkinHeadCtrl.m_SkinAttribute.m_ImageHeadLeft.SetLoadInfo(IDB_DLG_LISTHEAD_L, hInstanceMe);
	m_pGameFrameControl->m_UserListView.m_SkinHeadCtrl.m_SkinAttribute.m_ImageHeadRight.SetLoadInfo(IDB_DLG_LISTHEAD_R, hInstanceMe);
	m_pGameFrameControl->m_UserListView.m_SkinHeadCtrl.m_SkinAttribute.m_ImageHeadMid.SetLoadInfo(IDB_DLG_LISTHEAD_M, hInstanceMe);
	m_pGameFrameControl->m_UserListView.m_SkinHeadCtrl.m_SkinAttribute.m_crHeadText = RGB(252,247,189);
	m_pGameFrameControl->m_UserListView.m_SkinHeadCtrl.m_SkinAttribute.m_dwHeadHight = 16;

	//获取资源
	HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();

	tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;
	m_btControl.SetButtonImage(m_bHideList?GameFrameImage.uBtShowUserList:GameFrameImage.uBtHideUserList,hInstance,false);

	//控制按钮
	UINT uImageID=m_bAllowSound?GameFrameImage.uBtGameSound:GameFrameImage.uBtGameUnSound;
	m_pGameFrameControl->m_btSound.SetButtonImage(uImageID,hInstance,false);

	//控制按钮
	if (m_ClientKernelHelper.GetInterface()!=NULL) 
	{
		uImageID=m_ClientKernelHelper->IsAllowUserLookon()?GameFrameImage.uBtGameLookOn:GameFrameImage.uBtGameUnLookOn;
		m_pGameFrameControl->m_btLookOn.SetButtonImage(uImageID,hInstance,false);
	}
	if(true==g_GlobalOption.bddzGame||true==g_GlobalOption.bFishGame)
	{
		m_pGameFrameControl->m_ImageTitle1.SetLoadInfo(GameFrameImage.uUserListTL,hInstance);
		m_pGameFrameControl->m_ImageTitle2.SetLoadInfo(GameFrameImage.uUserListTM,hInstance);
		m_pGameFrameControl->m_ImageTitle3.SetLoadInfo(GameFrameImage.uUserListTR,hInstance);
	}else
	{
		m_pGameFrameControl->m_ImageTitle1.SetLoadInfo(IDB_DLG_RIGHT_T,hInstanceMe);
		m_pGameFrameControl->m_ImageTitle2.SetLoadInfo(IDB_DLG_RIGHT_M,hInstanceMe);
		m_pGameFrameControl->m_ImageTitle3.SetLoadInfo(IDB_DLG_RIGHT_B,hInstanceMe);
	}
	return true;
}

//发送函数
bool CGameFrameDlg::SendData(WORD wSubCmdID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendSocketData(MDM_GF_GAME,wSubCmdID);
}

//发送函数
bool CGameFrameDlg::SendData(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendSocketData(MDM_GF_GAME,wSubCmdID,pData,wDataSize);
}

//发送准备
bool CGameFrameDlg::SendUserReady(void * pBuffer, WORD wDataSize)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendUserReady(pBuffer,wDataSize);
}

//设置定时器
bool CGameFrameDlg::SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SetGameTimer(wChairID,nTimerID,nElapse);
}

//删除定时器
bool CGameFrameDlg::KillGameTimer(UINT nTimerID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->KillGameTimer(nTimerID);
}

//播放声音
bool CGameFrameDlg::PlayGameSound(LPCTSTR pszSoundName)
{
	if ((m_bAllowSound==true)&&(IsWindowVisible()))
	{
		::PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);
		return true;
	}

	return false;
}

//播放声音
bool CGameFrameDlg::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	if ((m_bAllowSound==true)&&(IsWindowVisible()))
	{
		//加载资源
		HRSRC hResour=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
		if (hResour==NULL) return false;
		HGLOBAL hGlobal=LoadResource(hInstance,hResour);
		if (hGlobal==NULL) return false;

		//播放声音
		LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
		::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

		//清理资源
		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}

	return true;
}

//插入图片
bool CGameFrameDlg::InsertImage(CBitmap * pBitmap)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertImage(pBitmap);
}

//插入名字
bool CGameFrameDlg::InsertUserName(LPCTSTR pszUserName)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertUserName(pszUserName);
}

//系统消息
bool CGameFrameDlg::InsertSystemString(LPCTSTR pszString)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertSystemString(pszString,MS_NORMAL);
}

//特色消息
bool CGameFrameDlg::InsertShtickString(LPCTSTR pszString, COLORREF crColor, bool bNewLine)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertShtickString(pszString,crColor,MS_NORMAL,bNewLine);
}

//普通消息
bool CGameFrameDlg::InsertGeneralString(LPCTSTR pszString, COLORREF crColor, bool bNewLine)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertGeneralString(pszString,crColor,MS_NORMAL,bNewLine);
}

//激活界面
void CGameFrameDlg::ActiveGameFrame()
{
	//获取窗口
	CWnd * pFocusWnd=GetFocus();
	CWnd * pMainWnd=AfxGetMainWnd();

	//激活判断
	if ((pFocusWnd==NULL)||(pMainWnd->IsChild(pFocusWnd)==FALSE)) pMainWnd->FlashWindow(TRUE);

	return;
}

//切换椅子
WORD CGameFrameDlg::SwitchViewChairID(WORD wChairID)
{
	//效验状态
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;

	//变量定义
	const tagUserData * pMeUserData=m_ClientKernelHelper->GetMeUserInfo();
	const tagServerAttribute * pServerAttribute=m_ClientKernelHelper->GetServerAttribute();

	//转换椅子
	WORD wViewChairID=(wChairID+pServerAttribute->wChairCount-pMeUserData->wChairID);
	switch (pServerAttribute->wChairCount)
	{
	case 2: { wViewChairID+=1; break; }
	case 3: { wViewChairID+=1; break; }
	case 4: { wViewChairID+=2; break; }
	case 5: { wViewChairID+=2; break; }
	case 6: { wViewChairID+=3; break; }
	case 7: { wViewChairID+=3; break; }
	case 8: { wViewChairID+=4; break; }
	}
	return wViewChairID%pServerAttribute->wChairCount;
}

//是否启用
bool CGameFrameDlg::IsEnableSound()
{
	return m_bAllowSound;
}

//启用声音
void CGameFrameDlg::EnableSound(bool bEnable)
{
	//获取资源
	HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();
	tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;

	//设置变量
	m_bAllowSound=bEnable;

	//控制按钮
	UINT uImageID=m_bAllowSound?GameFrameImage.uBtGameSound:GameFrameImage.uBtGameUnSound;
	m_pGameFrameControl->m_btSound.SetButtonImage(uImageID,hInstance,false);

	//读写注册
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("EnableSound"),m_bAllowSound);

	return;
}

//游戏状态
BYTE CGameFrameDlg::GetGameStatus()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return GS_FREE;
	return m_ClientKernelHelper->GetGameStatus();
}

//旁观模式
bool CGameFrameDlg::IsLookonMode()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return true;
	return m_ClientKernelHelper->IsLookonMode();
}

//允许旁观
bool CGameFrameDlg::IsAllowLookon()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return false;
	return m_ClientKernelHelper->IsAllowLookon();
}

//允许旁观
bool CGameFrameDlg::IsAllowUserLookon()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return false;
	return m_ClientKernelHelper->IsAllowUserLookon();
}

//旁观控制
bool CGameFrameDlg::AllowUserLookon(DWORD dwUserID, bool bAllowLookon)
{
	//状态效验
	if (m_ClientKernelHelper.GetInterface()==NULL) return false;

	//设置按钮
	if (dwUserID==0L)
	{
		//获取资源
		HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();
		tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;

		//控制按钮
		UINT uImageID=m_ClientKernelHelper->IsAllowUserLookon()?GameFrameImage.uBtGameLookOn:GameFrameImage.uBtGameUnLookOn;
		m_pGameFrameControl->m_btLookOn.SetButtonImage(uImageID,hInstance,false);
	}

	//发送消息
	return m_ClientKernelHelper->AllowUserLookon(dwUserID,bAllowLookon);
}

//游戏状态
void CGameFrameDlg::SetGameStatus(BYTE bGameStatus)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return;
	m_ClientKernelHelper->SetGameStatus(bGameStatus);
	return;
}

//自己位置
WORD CGameFrameDlg::GetMeChairID()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;
	return m_ClientKernelHelper->GetMeChairID();
}

//时间位置
WORD CGameFrameDlg::GetTimeChairID()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;
	return m_ClientKernelHelper->GetTimeChairID();
}

//加入游戏
VOID CGameFrameDlg::JoinInGameTable(WORD wChairID)
{
	//状态效验
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	if (m_ClientKernelHelper.GetInterface()==NULL) return;

	//获取信息
	const tagUserData * pUserData=m_ClientKernelHelper->GetMeUserInfo();
	if (pUserData==NULL) return;

	//变量定义
	IPC_JoinInGame JoinInGame;
	ZeroMemory(&JoinInGame,sizeof(JoinInGame));

	//设置变量
	JoinInGame.wChairID=wChairID;
	JoinInGame.wTableID=pUserData->wTableID;

	//发送消息
	m_ClientKernelHelper->SendProcessData(IPC_MAIN_CONCTROL,IPC_SUB_JOIN_IN_GAME,&JoinInGame,sizeof(JoinInGame));

	return;
}

//获取用户
const tagUserData * CGameFrameDlg::GetUserData(WORD wChairID)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return NULL;
	return m_ClientKernelHelper->GetUserInfo(wChairID);
}

//内核接口
void * CGameFrameDlg::GetClientKernel(const IID & Guid, DWORD dwQueryVer)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return NULL;
	return m_ClientKernelHelper->QueryInterface(Guid,dwQueryVer);
}

//控件绑定
void CGameFrameDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTROL, m_btControl);
	DDX_Control(pDX, IDC_VOR_SPLITER, m_VorSplitter);
	DDX_Control(pDX, IDC_SOUND, m_btSound);
	DDX_Control(pDX, IDC_OPTION_SET, m_btOptionSet);
}

//初始化函数
BOOL CGameFrameDlg::OnInitDialog()
{
	CString strAppName=AfxGetApp()->m_pszAppName;
	strAppName.MakeLower();
	if (-1!=strAppName.Find(TEXT("fish")))
	{
		g_GlobalOption.bFishGame=true;
		ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX,0,0);
	}
	__super::OnInitDialog();


	//设置属性
	ModifyStyle(WS_CAPTION|WS_BORDER,0,0);


	//读取配置
	m_bAllowSound=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("EnableSound"),TRUE)?true:false;

	//创建控制
	m_pGameFrameControl=new CGameFrameControl(this);
	if (m_pGameFrameControl==NULL) throw TEXT("游戏框架创建失败");

	//创建组件
	if (m_MessageProxyHelper.CreateInstance()==false) throw TEXT("信息组件创建失败");
	m_MessageProxyHelper->SetRichEditHwnd(&m_pGameFrameControl->m_ChatMessage);

	//设置界面
	m_VorSplitter.SetSplitterSink(this);

	//创建组件
	if (m_UserFaceResHelper.CreateInstance()==false) throw TEXT("头像组件加载失败");
	if (m_ClientKernelHelper.CreateInstance()==false) throw TEXT("游戏框架内核模块加载失败");

	//创建游戏视图
	CRect rcGameView(0,0,0,0);
	m_pGameFrameControl->Create(IDD_FRAME_CONTROL,this);
	if (m_pGameFrameControl->SetUserFaceRes(m_UserFaceResHelper.GetInterface())==false) throw TEXT("头像资源接口设置失败");
	m_pGameFrameView->Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcGameView,this,10);
	if (m_pGameFrameView->SetUserFaceRes(m_UserFaceResHelper.GetInterface())==false) throw TEXT("头像资源接口设置失败");
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
	if (pVideoServiceManager!=NULL && pVideoServiceManager->SetUserFaceRes(m_UserFaceResHelper.GetInterface())==false) throw TEXT("头像资源接口设置失败");

	//初始化内核
	LPCTSTR lpszCmdLine=AfxGetApp()->m_lpCmdLine;
	bool bSuccess=m_ClientKernelHelper->InitClientKernel(lpszCmdLine,m_pKernelSink);
	if (bSuccess==false) throw TEXT("游戏框架逻辑模块初始化失败");
	bSuccess=m_ClientKernelHelper->SetMessageProxy(m_MessageProxyHelper.GetInterface());
	if (bSuccess==false) throw TEXT("获取消息输出模块失败");

	//初始化游戏
	m_pGameFrameControl->SetClientKernel(m_ClientKernelHelper.GetInterface());
	if (InitGameFrame()==false) throw TEXT("游戏框架初始化失败");

		//设置颜色
	tagMessageOption MessageOption;
	memset(&MessageOption,0,sizeof(MessageOption));
	MessageOption.crName=g_GlobalOption.m_crMsgName;
	MessageOption.crSystemHead=RGB(191,163,19);/*g_GlobalOption.m_crMsgSysHead*/;
	MessageOption.crSystemString=RGB(191,163,19);/*g_GlobalOption.m_crMsgSysString*/;
	m_MessageProxyHelper->SetMessageOption(MessageOption);

	//加载资源
	UpdateSkinResource();

	//移动窗口
	INT nXScreen=GetSystemMetrics(SM_CXSCREEN);
	INT nYScreen=GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL,0,0,__min(nXScreen,1024),__min(nYScreen,720),SWP_NOZORDER|SWP_NOMOVE);

	//显示窗口
	m_bInitDialog=true;
	m_pGameFrameView->ShowWindow(SW_SHOW);
	m_pGameFrameControl->ShowWindow(SW_SHOW);

	//显示窗口
	if (true==g_GlobalOption.bFishGame)
	{
		MoveWindow(0,0,1270,624,FALSE);
		//m_btControl.ShowWindow(FALSE);
	}
	else
	{
		m_btControl.ShowWindow(TRUE);
		m_btControl.EnableWindow(TRUE);
		MoveWindow(0,0,1024,748,FALSE);
	}

	//显示窗口
	CenterWindow();
	ShowWindow(SW_SHOW);

	return TRUE;
}

//消息解释
BOOL CGameFrameDlg::PreTranslateMessage(MSG * pMsg)
{
	//过滤消息
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//确定消息
void CGameFrameDlg::OnOK()
{
	return;
}

//取消消息
void CGameFrameDlg::OnCancel()
{
	//退出提示
	if ((m_ClientKernelHelper.GetInterface()!=NULL)&&(m_ClientKernelHelper->GetExitFlags()==false))
	{
		WORD wChairID=GetMeChairID();
		const tagUserData * pUserData=GetUserData(wChairID);
		if (pUserData!=NULL)
		{
			int iRet=ShowInformation(TEXT("你正在游戏中，强行退出将被扣分，确实要强退吗？"),0,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
			if (iRet!=IDYES) return;
		}
	}

	//保存配置
	g_GlobalOption.SaveOptionParameter();

	//注销接口
	m_ClientKernelHelper->DestroyClientKernel();

	//销毁组件
	SafeDelete(m_pKernelSink);
	m_UserFaceResHelper.CloseInstance();
	m_ClientKernelHelper.CloseInstance();

	//关闭窗口
	DestroyWindow();

	return;
}

//控制按钮
VOID CGameFrameDlg::OnBnClickedControl()
{
	//设置变量
	m_bRectify=false;
	m_bHideList=!m_bHideList;

	//设置界面
	HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();
	tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;
	m_btControl.SetButtonImage(m_bHideList?GameFrameImage.uBtShowUserList:GameFrameImage.uBtHideUserList,hInstance,false);

	//设置界面
	m_VorSplitter.EnableSplitter(m_bHideList==false);

	//调整界面
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//位置信息
void CGameFrameDlg::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//设置位置
	if (true==g_GlobalOption.bFishGame)
	{
		lpMMI->ptMinTrackSize.x=1270;
		lpMMI->ptMinTrackSize.y=624;
	}
	else
	{
		lpMMI->ptMinTrackSize.x=1024;
		lpMMI->ptMinTrackSize.y=740;
	}

	return;
}

//位置改变
void CGameFrameDlg::OnWindowPosChanging(WINDOWPOS * lpwndpos)
{
	if (m_bInitDialog==false) lpwndpos->flags&=~SWP_SHOWWINDOW;
	__super::OnWindowPosChanging(lpwndpos);
}

//位置改变
void CGameFrameDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;

}

VOID CGameFrameDlg::OnSound()
{
	if (IsEnableSound()==true)
	{
		EnableSound(false);
	}
	else
	{
		EnableSound(true);
	}
	HINSTANCE hInstancleMe=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btSound.SetButtonImage(m_bAllowSound?IDB_SOUND1:IDB_SOUND0, hInstancleMe, false);
}

void CGameFrameDlg::OnOptionSet()
{

	OnGameOptionSet();
}


//////////////////////////////////////////////////////////////////////////
