#ifndef MyWorkerThread_H
#define MyWorkerThread_H

#include <wx/thread.h>

#include <string>

class ProcessingEngine;
class wxDataViewItem;

class MyWorkerThread : public wxThread
{
public:
    MyWorkerThread( ProcessingEngine* handler_i, const std::string& parent_i, const std::string& filename_i,
        wxSemaphore& semaphore_i );

    virtual ExitCode Entry() override;

private:
    wxSemaphore& semaphore;
    ProcessingEngine* handler;
    const std::string& parent;
    const std::string filename;
};


#endif // !MyWorkerThread_H
