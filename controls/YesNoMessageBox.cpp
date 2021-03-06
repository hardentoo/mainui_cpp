/*
YesNoMessageBox.h - simple generic yes/no message box
Copyright (C) 2017 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "extdll_menu.h"
#include "BaseMenu.h"
#include "Action.h"
#include "PicButton.h"
#include "YesNoMessageBox.h"
#include "Utils.h"

static void ToggleInactiveInternalCb( CMenuBaseItem *pSelf, void *pExtra );

CMenuYesNoMessageBox::CMenuYesNoMessageBox( bool alert )
{
	iFlags |= QMF_DIALOG;
	dlgMessage1.iFlags = QMF_INACTIVE|QMF_DROPSHADOW;
	dlgMessage1.eTextAlignment = QM_TOP;

	yes.iFlags = no.iFlags = QMF_DROPSHADOW;
	yes.eTextAlignment = no.eTextAlignment = QM_CENTER;
	yes.onActivated.pExtra = no.onActivated.pExtra = this;
	yes.bEnableTransitions = no.bEnableTransitions = false;

	SET_EVENT( yes, onActivated )
	{
		CMenuYesNoMessageBox *msgBox = (CMenuYesNoMessageBox*)pExtra;

		msgBox->Hide();
		msgBox->onPositive( msgBox );

	}
	END_EVENT( yes, onActivated )

	SET_EVENT( no, onActivated )
	{
		CMenuYesNoMessageBox *msgBox = (CMenuYesNoMessageBox*)pExtra;
		msgBox->Hide();
		msgBox->onNegative( msgBox );

	}
	END_EVENT( no, onActivated )

	m_bSetYes = m_bSetNo = false;
	m_bIsAlert = alert;

	szName = "CMenuYesNoMessageBox";
}

/*
==============
CMenuYesNoMessageBox::Init
==============
*/
void CMenuYesNoMessageBox::_Init( void )
{
	if( !m_bSetYes )
		SetPositiveButton( "Ok", PC_OK );

	if( !m_bSetNo )
		SetNegativeButton( "Cancel", PC_CANCEL );

	if( !(bool)onNegative )
		onNegative = CEventCallback::NoopCb;

	if( !(bool)onPositive )
		onPositive = CEventCallback::NoopCb;

	background.bForceColor = true;
	background.iColor = uiPromptBgColor;
	AddItem( background );
	AddItem( dlgMessage1 );
	AddItem( yes );

	// alert dialog has single OK button
	if( !m_bIsAlert )
		AddItem( no );
}

/*
==============
CMenuYesNoMessageBox::VidInit
==============
*/
void CMenuYesNoMessageBox::_VidInit( void )
{
	SetRect( DLG_X + 192, 256, 640, 256 );
	CalcPosition();
	CalcSizes();

	dlgMessage1.SetRect( 0, 24, 640, 256 - 24 );
	if( m_bIsAlert )
	{
		yes.SetRect( 298, 204, UI_BUTTONS_WIDTH / 2, UI_BUTTONS_HEIGHT );
	}
	else
	{
		yes.SetRect( 188, 204, UI_BUTTONS_WIDTH / 2, UI_BUTTONS_HEIGHT );
	}
	no.SetRect( 338, 204, UI_BUTTONS_WIDTH / 2, UI_BUTTONS_HEIGHT );

	dlgMessage1.SetCharSize( QM_DEFAULTFONT );

}

/*
==============
CMenuYesNoMessageBox::Draw
==============
*/
void CMenuYesNoMessageBox::Draw( void )
{
	UI_FillRect( 0,0, gpGlobals->scrWidth, gpGlobals->scrHeight, 0x40000000 );
	CMenuBaseWindow::Draw();
}

/*
==============
CMenuYesNoMessageBox::Key
==============
*/
const char *CMenuYesNoMessageBox::Key(int key, int down)
{
	if( UI::Key::IsEscape( key ) && down && m_bAllowEnterActivate )
	{
		Hide();
		onNegative( this );
		m_bAllowEnterActivate = true;

		return uiSoundNull;
	}
	else
	{
		return CMenuBaseWindow::Key( key, down );
	}
}

/*
==============
CMenuYesNoMessageBox::SetMessage
==============
*/
void CMenuYesNoMessageBox::SetMessage( const char *msg )
{
	dlgMessage1.szName = ( msg );
}

/*
==============
CMenuYesNoMessageBox::SetPositiveButton
==============
*/
void CMenuYesNoMessageBox::SetPositiveButton( const char *msg, EDefaultBtns buttonPic )
{
	m_bSetYes = true;
	yes.szName = msg;
	yes.SetPicture( buttonPic );
}

/*
==============
CMenuYesNoMessageBox::SetNegativeButton
==============
*/
void CMenuYesNoMessageBox::SetNegativeButton( const char *msg, EDefaultBtns buttonPic )
{
	m_bSetNo = true;
	no.szName = msg;
	no.SetPicture( buttonPic );
}

/*
==============
CMenuYesNoMessageBox::HighlightChoice
==============
*/
void CMenuYesNoMessageBox::HighlightChoice( EHighlight yesno )
{
	if( yesno == NO_HIGHLIGHT )
	{
		yes.bPulse = no.bPulse = false;
	}
	else
	{
		yes.bPulse = yesno == HIGHLIGHT_YES ? true : false;
		no.bPulse = yesno == HIGHLIGHT_NO ? true : false;
	}
}

CEventCallback CMenuYesNoMessageBox::MakeOpenEvent()
{
	return CEventCallback( OpenCb, this );
}


/*
==============
CMenuYesNoMessageBox::ToggleInactiveCb
==============
*/
void CMenuYesNoMessageBox::OpenCb( CMenuBaseItem *, void *pExtra )
{
	ToggleInactiveInternalCb( (CMenuBaseItem*)pExtra, NULL );
}

/*
==============
CMenuYesNoMessageBox::ToggleInactiveCb
==============
*/
static void ToggleInactiveInternalCb( CMenuBaseItem *pSelf, void * )
{
	pSelf->ToggleVisibility();
}


void CMenuYesNoMessageBox::Link( CMenuItemsHolder *holder )
{
	m_pParent = holder;
}
