// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "ProcessingEngine.h"
#include "MyWorkerThread.h"

MyWorkerThread::MyWorkerThread( ProcessingEngine* handler_i, const std::string& parent_i, const std::string& filename_i,
    wxSemaphore& semaphore_i ) :
    wxThread( wxTHREAD_JOINABLE ),
    semaphore( semaphore_i ),
    handler( handler_i ),
    parent( parent_i ),
    filename( filename_i )
{
}

wxThread::ExitCode MyWorkerThread::Entry()
{
    handler->ProcessFile( parent, filename );
    semaphore.Post();
    return 0;
}
