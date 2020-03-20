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

#include <memory>

#include "MyPictureModel.h"

class PictureFixerApp : public wxApp
{
public:
    virtual bool OnInit();
    bool isShuttingDown() const { return shuttingDown; }

    wxCriticalSection criticalSection;
    wxSemaphore allDoneSemaphore;

private:
    MyPictureModel* theModel;
    bool shuttingDown;
}; // class PictureFixerApp

DECLARE_APP( PictureFixerApp )

#endif // PICTUREFIXERAPP_H_
