/*
 * PictureFixerApp.cpp
 *
 *  Created on: Jan 1, 2020
 *      Author: joepi
 */


#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "PictureFixerApp.h"
#include "MainWindow.h"
#include "ProcessingEngine.h"

IMPLEMENT_APP( PictureFixerApp )

bool PictureFixerApp::OnInit()
{
    MainWindow* mainWindow = new MainWindow( theEngine );

    mainWindow->Show( true );

    return true;
}
