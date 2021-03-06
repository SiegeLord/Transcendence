//	DockScreen.h
//
//	Dock Screen classes
//	Copyright (c) 2014 by Kronosaur Productions, LLC. All Rights Reserved.

class CDockScreen;
class CPlayerShipController;
struct SDockFrame;

class IDockScreenDisplay
	{
	public:
		enum EResults
			{
			resultNone,

			//	HandleKeyDown

			resultHandled,
			resultShowPane,
			};

		struct SInitCtx
			{
			CPlayerShipController *pPlayer;
			CDockScreen *pDockScreen;
			CXMLElement *pDesc;
			AGScreen *pScreen;
			RECT rcRect;
			DWORD dwFirstID;
			const CVisualPalette *pVI;
			const SFontTable *pFontTable;

			CSpaceObject *pLocation;
			ICCItem *pData;
			};

		virtual ~IDockScreenDisplay (void) { }

		inline void DeleteCurrentItem (int iCount) { OnDeleteCurrentItem(iCount); }
		inline const CItem &GetCurrentItem (void) const { return OnGetCurrentItem(); }
		inline ICCItem *GetCurrentListEntry (void) const { return OnGetCurrentListEntry(); }
		inline bool GetDefaultBackgroundObj (CSpaceObject **retpObj) { return OnGetDefaultBackgroundObj(retpObj); }
		inline CItemListManipulator &GetItemListManipulator (void) { return OnGetItemListManipulator(); }
		inline int GetListCursor (void) { return OnGetListCursor(); }
		inline IListData *GetListData (void) { return OnGetListData(); }
		inline CSpaceObject *GetSource (void) { return OnGetSource(); }
		inline EResults HandleAction (DWORD dwTag, DWORD dwData) { return OnHandleAction(dwTag, dwData); }
		inline EResults HandleKeyDown (int iVirtKey) { return OnHandleKeyDown(iVirtKey); }
		ALERROR Init (SInitCtx &Ctx, CString *retsError);
		inline bool IsCurrentItemValid (void) const { return OnIsCurrentItemValid(); }
		inline EResults ResetList (CSpaceObject *pLocation) { return OnResetList(pLocation); }
		inline EResults SetListCursor (int iCursor) { return OnSetListCursor(iCursor); }
		inline EResults SetListFilter (const CItemCriteria &Filter) { return OnSetListFilter(Filter); }
		inline bool SelectNextItem (void) { return OnSelectNextItem(); }
		inline bool SelectPrevItem (void) { return OnSelectPrevItem(); }
		inline void ShowItem (void) { OnShowItem(); }
		inline void ShowPane (bool bNoListNavigation) { OnShowPane(bNoListNavigation); }

	protected:
		virtual void OnDeleteCurrentItem (int iCount) { }
		virtual const CItem &OnGetCurrentItem (void) const { return CItem::NullItem(); }
		virtual ICCItem *OnGetCurrentListEntry (void) const { return NULL; }
		virtual bool OnGetDefaultBackgroundObj (CSpaceObject **retpObj) { return false; }
		virtual CItemListManipulator &OnGetItemListManipulator (void) { return g_DummyItemListManipulator; }
		virtual int OnGetListCursor (void) { return -1; }
		virtual IListData *OnGetListData (void) { return NULL; }
		virtual CSpaceObject *OnGetSource (void) { return NULL; }
		virtual EResults OnHandleAction (DWORD dwTag, DWORD dwData) { return resultNone; }
		virtual EResults OnHandleKeyDown (int iVirtKey) { return resultNone; }
		virtual ALERROR OnInit (SInitCtx &Ctx, CString *retsError) { return NOERROR; }
		virtual bool OnIsCurrentItemValid (void) const { return false; }
		virtual EResults OnResetList (CSpaceObject *pLocation) { return resultNone; }
		virtual EResults OnSetListCursor (int iCursor) { return resultNone; }
		virtual EResults OnSetListFilter (const CItemCriteria &Filter) { return resultNone; }
		virtual bool OnSelectNextItem (void) { return false; }
		virtual bool OnSelectPrevItem (void) { return false; }
		virtual void OnShowItem (void) { }
		virtual void OnShowPane (bool bNoListNavigation);

		//	Helpers

		bool EvalBool (const CString &sCode, bool *retbResult, CString *retsError);
		CSpaceObject *EvalListSource (const CString &sString, CString *retsError);
		bool EvalString (const CString &sString, bool bPlain, ECodeChainEvents iEvent, CString *retsResult);

		CDockScreen *m_pDockScreen;
		CSpaceObject *m_pLocation;
		CPlayerShipController *m_pPlayer;
		ICCItem *m_pData;
	};

class CDockScreenActions
	{
	public:
		enum SpecialAttribs
			{
			specialAll,

			specialDefault,
			specialCancel,
			specialNextKey,
			specialPrevKey,
			};

		CDockScreenActions (void) : m_pData(NULL) { }
		~CDockScreenActions (void);

		ALERROR AddAction (const CString &sID, int iPos, const CString &sLabel, CExtension *pExtension, ICCItem *pCode, int *retiAction);
		void CleanUp (void);
		void CreateButtons (CGFrameArea *pFrame, CDesignType *pRoot, DWORD dwFirstTag, const RECT &rcFrame);
		void Execute (int iAction, CDockScreen *pScreen);
		void ExecuteExitScreen (bool bForceUndock = false);
		void ExecuteShowPane (const CString &sPane);
		bool FindByID (const CString &sID, int *retiAction = NULL);
		bool FindByID (ICCItem *pItem, int *retiAction = NULL);
		bool FindByKey (const CString &sKey, int *retiAction);
		bool FindSpecial (SpecialAttribs iSpecial, int *retiAction);
		inline CGButtonArea *GetButton (int iAction) const { return m_Actions[iAction].pButton; }
		inline int GetCount (void) const { return m_Actions.GetCount(); }
		inline const CString &GetKey (int iAction) const { return m_Actions[iAction].sKey; }
		inline const CString &GetLabel (int iAction) const { return m_Actions[iAction].sLabel; }
		int GetVisibleCount (void) const;
		ALERROR InitFromXML (CExtension *pExtension, CXMLElement *pActions, ICCItem *pData, CString *retsError);
		inline bool IsEnabled (int iAction) const { return m_Actions[iAction].bEnabled; }
		bool IsSpecial (int iAction, SpecialAttribs iSpecial);
		inline bool IsVisible (int iAction) const { return m_Actions[iAction].bVisible; }
		ALERROR RemoveAction (int iAction);
		inline void SetButton (int iAction, CGButtonArea *pButton) { m_Actions[iAction].pButton = pButton; }
		void SetEnabled (int iAction, bool bEnabled = true);
		void SetLabel (int iAction, const CString &sLabelDesc, const CString &sKey);
		void SetSpecial (int iAction, SpecialAttribs iSpecial, bool bEnabled = true);
		bool SetSpecial (CCodeChain &CC, int iAction, ICCItem *pSpecial, ICCItem **retpError);
		void SetVisible (int iAction, bool bVisible = true);

	private:
		struct SActionDesc
			{
			CString sID;
			CString sLabel;			//	Label for the action
			CString sKey;			//	Accelerator key
			CGButtonArea *pButton;	//	Pointer to button area

			CExtension *pExtension;	//	Source of the code

			CXMLElement *pCmd;		//	Special commands (e.g., <Exit/>
			CString sCode;			//	Code
			ICCItem *pCode;			//	Code (owned by us)

			bool bVisible;			//	Action is visible
			bool bEnabled;			//	Action is enabled

			bool bDefault;			//	This is the default action [Enter]
			bool bCancel;			//	This is the cancel action [Esc]
			bool bPrev;				//	This is the prev action [<-]
			bool bNext;				//	This is the next action [->]
			};

		void ExecuteCode (CDockScreen *pScreen, const CString &sID, CExtension *pExtension, ICCItem *pCode);
		void ParseLabelDesc (const CString &sLabelDesc, CString *retsLabel, CString *retsKey = NULL, TArray<SpecialAttribs> *retSpecial = NULL);
		void SetLabelDesc (SActionDesc *pAction, const CString &sLabelDesc, bool bOverrideSpecial = true);
		void SetSpecial (SActionDesc *pAction, SpecialAttribs iSpecial, bool bEnabled);

		TArray<SActionDesc> m_Actions;
		ICCItem *m_pData;			//	Data passed in to scrShowScreen (may be NULL)
	};

class CDockPane
	{
	public:
		CDockPane (void);
		~CDockPane (void);

		void CleanUp (AGScreen *pScreen = NULL);
		void ExecuteCancelAction (void);
		inline void ExecuteShowPane (const CString &sPane) { if (!m_bInShowPane) m_Actions.ExecuteShowPane(sPane); }
		inline CDockScreenActions &GetActions (void) { return m_Actions; }
		const CString &GetDescriptionString (void) const;
		int GetCounterValue (void) const;
		CString GetTextInputValue (void) const;
		bool HandleAction (DWORD dwTag, DWORD dwData);
		bool HandleChar (char chChar);
		bool HandleKeyDown (int iVirtKey);
		ALERROR InitPane (CDockScreen *pDockScreen, CXMLElement *pPaneDesc, const RECT &rcPaneRect);
		bool SetControlValue (const CString &sID, ICCItem *pValue);
		void SetCounterValue (int iValue);
		void SetDescription (const CString &sDesc);
		void SetTextInputValue (const CString &sValue);

	private:
		enum EControlTypes
			{
			controlNone,

			controlDesc,
			controlCounter,
			controlItemDisplay,
			controlTextInput,
			};

		struct SControl
			{
			SControl (void) :
					iType(controlNone),
					pArea(NULL),
					cyHeight(0),
					cyMinHeight(0),
					cyMaxHeight(0),
					bReplaceInput(false)
				{ }

			inline CGTextArea *AsTextArea (void) { return (CGTextArea *)pArea; }
			inline CGItemDisplayArea *AsItemDisplayArea (void) { return (CGItemDisplayArea *)pArea; }

			EControlTypes iType;
			CString sID;					//	Control ID

			AGArea *pArea;

			int cyHeight;					//	Computed height of control
			int cyMinHeight;				//	Minimum control height
			int cyMaxHeight;				//	Desired control height

			bool bReplaceInput;				//	Keeps track of counter state
			};

		void CreateControl (EControlTypes iType, const CString &sID);
		ALERROR CreateControls (CString *retsError);
		bool FindControl (const CString &sID, SControl **retpControl = NULL) const;
		CGTextArea *GetTextControlByType (EControlTypes iType) const;
		SControl *GetControlByType (EControlTypes iType) const;
		void RenderControls (void);
		ALERROR ReportError (const CString &sError);

		CDockScreen *m_pDockScreen;			//	Dock screen object
		CXMLElement *m_pPaneDesc;			//	XML describing pane

		RECT m_rcPane;						//	Pane region relative to screen
		CGFrameArea *m_pContainer;			//	Hold all pane areas

		TArray<SControl> m_Controls;
		CDockScreenActions m_Actions;

		CString m_sDesc;					//	Current description
		bool m_bInShowPane;					//	Keep track of re-entrancy
	};

class CDockScreen : public IScreenController
	{
	public:
		CDockScreen (void);
		virtual ~CDockScreen (void);

		void CleanUpScreen (void);
		bool EvalString (const CString &sString, ICCItem *pData = NULL, bool bPlain = false, ECodeChainEvents iEvent = eventNone, CString *retsResult = NULL);
		inline void ExecuteCancelAction (void) { m_CurrentPane.ExecuteCancelAction(); }
		inline CDockScreenActions &GetActions (void) { return m_CurrentPane.GetActions(); }
		inline ICCItem *GetData (void) { return m_pData; }
		inline CExtension *GetExtension (void) const { return m_pExtension; }
		inline CSpaceObject *GetLocation (void) { return m_pLocation; }
		CDesignType *GetResolvedRoot (CString *retsResolveScreen = NULL) const;
		inline AGScreen *GetScreen (void) const { return m_pScreen; }
		void HandleChar (char chChar);
		void HandleKeyDown (int iVirtKey);
		ALERROR InitScreen (HWND hWnd, 
							RECT &rcRect, 
							const SDockFrame &Frame,
							CExtension *pExtension,
							CXMLElement *pDesc, 
							const CString &sPane,
							ICCItem *pData,
							CString *retsPane,
							AGScreen **retpScreen);
		inline bool InOnInit (void) { return m_bInOnInit; }
		inline bool IsFirstOnInit (void) { return m_bFirstOnInit; }
		inline bool IsValid (void) { return (m_pScreen != NULL); }
		ALERROR ReportError (const CString &sError);
		inline void ResetFirstOnInit (void) { m_bFirstOnInit = true; }
		void ResetList (CSpaceObject *pLocation);
		void SetListFilter (const CItemCriteria &Filter);
		void Update (int iTick);

		//	Methods used by script code
		inline void DeleteCurrentItem (int iCount) { m_pDisplay->DeleteCurrentItem(iCount); }
		inline int GetCounter (void) { return m_CurrentPane.GetCounterValue(); }
		inline const CItem &GetCurrentItem (void) { return m_pDisplay->GetCurrentItem(); }
		ICCItem *GetCurrentListEntry (void);
		inline const CString &GetDescription (void) { return m_CurrentPane.GetDescriptionString(); }
		CG16bitImage *GetDisplayCanvas (const CString &sID);
		inline CItemListManipulator &GetItemListManipulator (void) { return m_pDisplay->GetItemListManipulator(); }
		inline int GetListCursor (void) { return m_pDisplay->GetListCursor(); }
		inline IListData *GetListData (void) { return m_pDisplay->GetListData(); }
		inline CString GetTextInput (void) { return m_CurrentPane.GetTextInputValue(); }
		inline bool IsCurrentItemValid (void) { return m_pDisplay->IsCurrentItemValid(); }
		void SelectNextItem (bool *retbMore = NULL);
		void SelectPrevItem (bool *retbMore = NULL);
		inline void SetDescription (const CString &sDesc) { m_CurrentPane.SetDescription(sDesc); }
		ALERROR SetDisplayText (const CString &sID, const CString &sText);
		inline bool SetControlValue (const CString &sID, ICCItem *pValue) { return m_CurrentPane.SetControlValue(sID, pValue); }
		inline void SetCounter (int iCount) { m_CurrentPane.SetCounterValue(iCount); }
		void SetListCursor (int iCursor);
		inline void SetTextInput (const CString &sText) { m_CurrentPane.SetTextInputValue(sText); }
		void ShowPane (const CString &sName);
		bool ShowScreen (const CString &sName, const CString &sPane);
		bool Translate (const CString &sTextID, ICCItem *pData, ICCItem **retpResult);
		//inline void Undock (void) { m_pPlayer->Undock(); }

		//	IScreenController virtuals
		virtual void Action (DWORD dwTag, DWORD dwData = 0);

	private:
		enum EControlTypes
			{
			ctrlText =						1,
			ctrlImage =						2,
			ctrlCanvas =					3,
			ctrlNeurohack =					100,
			};

		struct SDisplayControl
			{
			SDisplayControl (void) : pArea(NULL), pCode(NULL), bAnimate(false)
				{
				}

			~SDisplayControl (void)
				{
				CCodeChain &CC = g_pUniverse->GetCC();
				if (pCode)
					pCode->Discard(&CC);
				}

			EControlTypes iType;
			CString sID;
			AGArea *pArea;
			ICCItem *pCode;

			bool bAnimate;
			};

		ALERROR CreateBackgroundArea (CXMLElement *pDesc, AGScreen *pScreen, const RECT &rcRect, const RECT &rcInner);
		ALERROR CreateBackgroundImage (CXMLElement *pDesc, const RECT &rcRect, int xOffset);
		ALERROR CreateTitleArea (CXMLElement *pDesc, AGScreen *pScreen, const RECT &rcRect, const RECT &rcInner);
		bool EvalBool (const CString &sString);
		CString EvalInitialPane (void);
		CString EvalInitialPane (CSpaceObject *pSource, ICCItem *pData);
		SDisplayControl *FindDisplayControl (const CString &sID);
		ALERROR FireOnScreenInit (CSpaceObject *pSource, ICCItem *pData, CString *retsError);
		ALERROR InitCodeChain (CTranscendenceWnd *pTrans, CSpaceObject *pStation);
		ALERROR InitDisplay (CXMLElement *pDisplayDesc, AGScreen *pScreen, const RECT &rcScreen);
		ALERROR InitFonts (void);
		void ShowDisplay (bool bAnimateOnly = false);
		inline void ShowItem (void) { m_pDisplay->ShowItem(); }
		void UpdateCredits (void);

		void AddDisplayControl (CXMLElement *pDesc, 
								AGScreen *pScreen, 
								SDisplayControl *pParent, 
								const RECT &rcFrame, 
								SDisplayControl **retpDControl = NULL);
		void InitDisplayControlRect (CXMLElement *pDesc, const RECT &rcFrame, RECT *retrcRect);

		const SFontTable *m_pFonts;
		CPlayerShipController *m_pPlayer;
		CDesignType *m_pRoot;
		CString m_sScreen;
		CSpaceObject *m_pLocation;
		ICCItem *m_pData;
		CExtension *m_pExtension;
		CXMLElement *m_pDesc;
		AGScreen *m_pScreen;
		bool m_bFirstOnInit;
		bool m_bInOnInit;

		//	Title and header
		CG16bitImage *m_pBackgroundImage;
		CGTextArea *m_pCredits;
		CGTextArea *m_pCargoSpace;
		bool m_bFreeBackgroundImage;

		//	Display controls
		TArray<SDisplayControl> m_Controls;
		CXMLElement *m_pDisplayInitialize;
		bool m_bDisplayAnimate;

		IDockScreenDisplay *m_pDisplay;

		//	Panes
		CXMLElement *m_pPanes;
		RECT m_rcPane;
		CDockPane m_CurrentPane;

		//	Events
		ICCItem *m_pOnScreenUpdate;
	};

struct SDockFrame
	{
	SDockFrame (void) :
			pLocation(NULL),
			pRoot(NULL),
			pInitialData(NULL),
			pStoredData(NULL),
			pResolvedRoot(NULL)
		{ }

	CSpaceObject *pLocation;				//	Current location
	CDesignType *pRoot;						//	Either a screen or a type with screens
	CString sScreen;						//	Screen name (UNID or name)
	CString sPane;							//	Current pane
	ICCItem *pInitialData;					//	Data for the screen
	ICCItem *pStoredData;					//	Read-write data

	CDesignType *pResolvedRoot;
	CString sResolvedScreen;
	};

class CDockScreenStack
	{
	public:
		void DeleteAll (void);
		void DiscardOldFrame (SDockFrame &OldFrame);
		ICCItem *GetData (const CString &sAttrib);
		inline int GetCount (void) const { return m_Stack.GetCount(); }
		const SDockFrame &GetCurrent (void) const;
		inline bool IsEmpty (void) const { return (m_Stack.GetCount() == 0); }
		void Push (const SDockFrame &Frame);
		void Pop (void);
		void ResolveCurrent (const SDockFrame &ResolvedFrame);
		void SetCurrent (const SDockFrame &NewFrame, SDockFrame *retPrevFrame = NULL);
		void SetCurrentPane (const CString &sPane);
		void SetData (const CString &sAttrib, ICCItem *pData);

	private:
		TArray<SDockFrame> m_Stack;
	};

#include "DockScreenDisplayImpl.h"