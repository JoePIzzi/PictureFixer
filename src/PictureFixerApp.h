/*
 * PictureFixerApp.h
 *
 *  Created on: Jan 1, 2020
 *      Author: joepi
 */

#ifndef PICTUREFIXERAPP_H_
#define PICTUREFIXERAPP_H_

#include <wx/app.h>
#include <wx/thread.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <memory>

#include "MyPictureModel.h"

class MainWindow;

class PictureFixerApp : public wxApp
{
public:
    virtual bool OnInit();
    wxFrame* getMainWindow();
    bool isShuttingDown() const { return shuttingDown; }
    void AddItem( wxThreadEvent& event );
    void postShutdown();
    void processingDone( wxThreadEvent& event );

    wxCriticalSection criticalSection;
    wxSemaphore allDoneSemaphore;

    DECLARE_EVENT_TABLE()

private:
    MyPictureModel* theModel;
    MainWindow* mainWindow;
    bool shuttingDown;
}; // class PictureFixerApp

DECLARE_APP( PictureFixerApp )

wxDECLARE_EVENT( wxEVT_ITEM_ADDED_EVENT, wxThreadEvent );
wxDECLARE_EVENT( wxEVT_THREAD_EXIT, wxThreadEvent );

#endif // PICTUREFIXERAPP_H_
