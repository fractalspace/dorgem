// HyperLink.cpp : implementation file
//
// HyperLink static control. Will open the default browser with the given URL
// when the user clicks on the link.
//
// Copyright (C) 1997 - 1999 Chris Maunder
// All rights reserved. May not be sold for profit.
//
// Thanks to Pål K. Tønder for auto-size and window caption changes.
//
// "GotoURL" function by Stuart Patterson
// As seen in the August, 1997 Windows Developer's Journal.
// Copyright 1997 by Miller Freeman, Inc. All rights reserved.
// Modified by Chris Maunder to use TCHARs instead of chars.
//
// "Default hand cursor" from Paul DiLascia's Jan 1998 MSJ article.
//
// 2/29/00 -- P. Shaffer standard font mod.
//
// Adjusted SetLinkCursor() by Frank Fesevur, to fit my needs

#include "StdAfx.h"
#include "Dorgem.h"
#include "HyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOLTIP_ID 1

/////////////////////////////////////////////////////////////////////////////
// CHyperLink

CHyperLink::CHyperLink()
{
    m_hLinkCursor       = NULL;                 // No cursor as yet
    m_crLinkColour      = RGB(  0,   0, 238);   // Blue
    m_crVisitedColour   = RGB( 85,  26, 139);   // Purple
    m_crHoverColour     = RGB(255,   0,   0);   // Red
    m_bOverControl      = FALSE;                // Cursor not yet over control
    m_bVisited          = FALSE;                // Hasn't been visited yet.
    m_nUnderline        = ulHover;              // Underline the link?
    m_bAdjustToFit      = TRUE;                 // Resize the window to fit the text?
    m_strURL.Empty();
    m_nTimerID          = 100;
}

CHyperLink::~CHyperLink()
{
    m_UnderlineFont.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink overrides

BOOL CHyperLink::DestroyWindow()
{
    KillTimer(m_nTimerID);

    return CStatic::DestroyWindow();
}

BOOL CHyperLink::PreTranslateMessage(MSG* pMsg)
{
    m_ToolTip.RelayEvent(pMsg);
    return CStatic::PreTranslateMessage(pMsg);
}

void CHyperLink::PreSubclassWindow()
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

    // Set the URL as the window text
    if (m_strURL.IsEmpty())
        GetWindowText(m_strURL);

    // Check that the window text isn't empty. If it is, set it as the URL.
    CString strWndText;
    GetWindowText(strWndText);
    if (strWndText.IsEmpty())
    {
        ASSERT(!m_strURL.IsEmpty());    // Window and URL both NULL. DUH!
        SetWindowText(m_strURL);
    }

    CFont* pFont = GetFont();
    if (!pFont)
    {
        HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        if (hFont == NULL)
            hFont = (HFONT) GetStockObject(ANSI_VAR_FONT);
        if (hFont)
            pFont = CFont::FromHandle(hFont);
    }
    ASSERT(pFont->GetSafeHandle());

    // Create the underline font
    LOGFONT lf;
    pFont->GetLogFont(&lf);
    m_StdFont.CreateFontIndirect(&lf);
    lf.lfUnderline = (BYTE) TRUE;
    m_UnderlineFont.CreateFontIndirect(&lf);

    PositionWindow();        // Adjust size of window to fit URL if necessary
    SetLinkCursor();         // Try and load up a "hand" cursor
    SetUnderline();

    // Create the tooltip
    CRect rect;
    GetClientRect(rect);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this, m_strURL, rect, TOOLTIP_ID);

    CStatic::PreSubclassWindow();
}

BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
    //{{AFX_MSG_MAP(CHyperLink)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

void CHyperLink::OnClicked()
{
    m_bOverControl = FALSE;
    if (ShellExecute(NULL, NULL, m_strURL, NULL, NULL, SW_SHOWNORMAL) < (HINSTANCE)32)
    {
        m_bVisited = FALSE;
        MsgBoxEx(GetApp()->GetSystemError(GetLastError()));
    }
    else
        SetVisited();
}

HBRUSH CHyperLink::CtlColor(CDC* pDC, UINT nCtlColor)
{
    ASSERT(nCtlColor == CTLCOLOR_STATIC);

    if (m_bOverControl)
        pDC->SetTextColor(m_crHoverColour);
    else
        if (m_bVisited)
            pDC->SetTextColor(m_crVisitedColour);
        else
            pDC->SetTextColor(m_crLinkColour);

    // transparent text.
    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CHyperLink::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bOverControl)        // Cursor has just moved over control
    {
        m_bOverControl = TRUE;

        if (m_nUnderline == ulHover)
            SetFont(&m_UnderlineFont);
        Invalidate();

        SetTimer(m_nTimerID, 100, NULL);
    }
    CStatic::OnMouseMove(nFlags, point);
}

void CHyperLink::OnTimer(UINT nIDEvent)
{
    CPoint p(GetMessagePos());
    ScreenToClient(&p);

    CRect rect;
    GetClientRect(rect);
    if (!rect.PtInRect(p))
    {
        m_bOverControl = FALSE;
        KillTimer(m_nTimerID);

        if (m_nUnderline != ulAlways)
            SetFont(&m_StdFont);
        rect.bottom+=10;
        InvalidateRect(rect);
    }

    CStatic::OnTimer(nIDEvent);
}

BOOL CHyperLink::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
    if (m_hLinkCursor)
    {
        ::SetCursor(m_hLinkCursor);
        return TRUE;
    }
    return FALSE;
}

BOOL CHyperLink::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(rect);
    pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink operations

void CHyperLink::SetURL(CString strURL)
{
    m_strURL = strURL;

    if (::IsWindow(GetSafeHwnd()))
        PositionWindow();
}

CString CHyperLink::GetURL() const
{
    return m_strURL;
}

void CHyperLink::SetTooltip(UINT uID)
{
    CString strText;
    strText.LoadString(uID);
    SetTooltip(strText);
}

void CHyperLink::SetTooltip(LPCSTR szText)
{
    m_ToolTip.UpdateTipText(szText, this, TOOLTIP_ID);
}

CString CHyperLink::GetTooltip()
{
    CString strText;
    m_ToolTip.GetText(strText, this, TOOLTIP_ID);
    return(strText);
}

void CHyperLink::SetColours(COLORREF crLinkColour, COLORREF crVisitedColour,
                            COLORREF crHoverColour /* = -1 */)
{
    m_crLinkColour    = crLinkColour;
    m_crVisitedColour = crVisitedColour;

    if (crHoverColour == -1)
        m_crHoverColour = ::GetSysColor(COLOR_HIGHLIGHT);
    else
        m_crHoverColour = crHoverColour;

    if (::IsWindow(m_hWnd))
        Invalidate();
}

COLORREF CHyperLink::GetLinkColour() const
{
    return m_crLinkColour;
}

COLORREF CHyperLink::GetVisitedColour() const
{
    return m_crVisitedColour;
}

COLORREF CHyperLink::GetHoverColour() const
{
    return m_crHoverColour;
}

void CHyperLink::SetVisited(BOOL bVisited /* = TRUE */)
{
    m_bVisited = bVisited;

    if (::IsWindow(GetSafeHwnd()))
        Invalidate();
}

BOOL CHyperLink::GetVisited() const
{
    return m_bVisited;
}

//////////////////////////////////////////////////////////////////////////
// Set the hand-cursor. First try to load the Windows hand cursor. If
// this fails (Win95/NT4), load the cursor from our own resources.

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)   // Taken from WINUSER.H
#endif // IDC_HAND

void CHyperLink::SetLinkCursor(HCURSOR hCursor)
{
    // First try to load Windows Hand Cursor
    if (hCursor == NULL)
        hCursor = GetApp()->LoadStandardCursor(IDC_HAND);

    // No cursor handle - load our own
    if (hCursor == NULL)
        hCursor = GetApp()->LoadCursor(IDC_LINK_CURSOR);

    ASSERT(hCursor != NULL);
    m_hLinkCursor = hCursor;
}

HCURSOR CHyperLink::GetLinkCursor() const
{
    return m_hLinkCursor;
}

void CHyperLink::SetUnderline(int nUnderline /*=ulHover*/)
{
    if (m_nUnderline == nUnderline)
        return;

    if (::IsWindow(GetSafeHwnd()))
    {
        if (nUnderline == ulAlways)
            SetFont(&m_UnderlineFont);
        else
            SetFont(&m_StdFont);

        Invalidate();
    }

    m_nUnderline = nUnderline;
}

int CHyperLink::GetUnderline() const
{
    return m_nUnderline;
}

void CHyperLink::SetAutoSize(BOOL bAutoSize /* = TRUE */)
{
    m_bAdjustToFit = bAutoSize;

    if (::IsWindow(GetSafeHwnd()))
        PositionWindow();
}

BOOL CHyperLink::GetAutoSize() const
{
    return m_bAdjustToFit;
}


// Move and resize the window so that the window is the same size
// as the hyperlink text. This stops the hyperlink cursor being active
// when it is not directly over the text. If the text is left justified
// then the window is merely shrunk, but if it is centred or right
// justified then the window will have to be moved as well.
//
// Suggested by Pål K. Tønder

void CHyperLink::PositionWindow()
{
    if (!::IsWindow(GetSafeHwnd()) || !m_bAdjustToFit)
        return;

    // Get the current window position
    CRect WndRect, ClientRect;
    GetWindowRect(WndRect);
    GetClientRect(ClientRect);

    ClientToScreen(ClientRect);

    CWnd* pParent = GetParent();
    if (pParent)
    {
        pParent->ScreenToClient(WndRect);
        pParent->ScreenToClient(ClientRect);
    }

    // Get the size of the window text
    CString strWndText;
    GetWindowText(strWndText);

    CDC* pDC = GetDC();
    CFont* pOldFont = pDC->SelectObject(&m_UnderlineFont);
    CSize Extent = pDC->GetTextExtent(strWndText);
    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);

    // Adjust for window borders
    Extent.cx += WndRect.Width() - ClientRect.Width();
    Extent.cy += WndRect.Height() - ClientRect.Height();

    // Get the text justification via the window style
    DWORD dwStyle = GetStyle();

    // Recalc the window size and position based on the text justification
    if (dwStyle & SS_CENTERIMAGE)
        WndRect.DeflateRect(0, (WndRect.Height() - Extent.cy)/2);
    else
        WndRect.bottom = WndRect.top + Extent.cy;

    if (dwStyle & SS_CENTER)
        WndRect.DeflateRect((WndRect.Width() - Extent.cx)/2, 0);
    else if (dwStyle & SS_RIGHT)
        WndRect.left  = WndRect.right - Extent.cx;
    else // SS_LEFT = 0, so we can't test for it explicitly
        WndRect.right = WndRect.left + Extent.cx;

    // Move the window
    SetWindowPos(NULL, WndRect.left, WndRect.top, WndRect.Width(), WndRect.Height(), SWP_NOZORDER);
}
