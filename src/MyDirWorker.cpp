// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "ProcessingEngine.h"
#include "MyDirWorker.h"
#include "PictureFixerApp.h"

MyDirWorker::MyDirWorker( ProcessingEngine& handler_i, std::string parent_i, std::string filename_i ) :
    handler( handler_i ),
    parent( parent_i ),
    filename( filename_i )
{
}

MyDirWorker::~MyDirWorker()
{
    wxThreadEvent terminateEvent( wxEVT_THREAD, wxEVT_THREAD_EXIT );
    wxGetApp().QueueEvent( terminateEvent.Clone() );
}

wxThread::ExitCode MyDirWorker::Entry()
{
    handler.ProcessDir( this, parent, filename );
    return 0;
}
