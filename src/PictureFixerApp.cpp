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

wxDEFINE_EVENT( wxEVT_ITEM_ADDED_EVENT, wxThreadEvent );
wxDEFINE_EVENT( wxEVT_THREAD_EXIT, wxThreadEvent );

BEGIN_EVENT_TABLE( PictureFixerApp, wxApp )
    EVT_THREAD( wxEVT_ITEM_ADDED_EVENT, PictureFixerApp::AddItem )
    EVT_THREAD( wxEVT_THREAD_EXIT, PictureFixerApp::processingDone )
END_EVENT_TABLE()

bool PictureFixerApp::OnInit()
{
    if ( !wxApp::OnInit() )
    {
        return false;
    }

    theModel = new MyPictureModel();
    mainWindow = new MainWindow( theModel );

    mainWindow->Show( true );

    return true;
}

wxFrame* PictureFixerApp::getMainWindow()
{
    return mainWindow;
}

void PictureFixerApp::AddItem( wxThreadEvent& event )
{
    mainWindow->AddItem( event );
}

void PictureFixerApp::postShutdown()
{
    shuttingDown = true;
}

void PictureFixerApp::processingDone( wxThreadEvent& )
{
    mainWindow->processingDone();
}
