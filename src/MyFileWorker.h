#ifndef MyFileWorker_H
#define MyFileWorker_H

#include <wx/thread.h>

#include <string>

class ProcessingEngine;
class wxDataViewItem;

class MyFileWorker : public wxThread
{
public:
    MyFileWorker( ProcessingEngine* handler_i, 
                  std::string parent_i, 
                  std::string filename_i,
                  wxSemaphore& semaphore_i );

    virtual ExitCode Entry() override;

private:
    wxSemaphore& semaphore;
    ProcessingEngine* handler;
    const std::string parent;
    const std::string filename;
};


#endif // !MyFileWorker_H
