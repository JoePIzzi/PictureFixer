#ifndef MyDirWorker_H
#define MyDirWorker_H


#include <wx/thread.h>

#include <string>
#include <memory>

class ProcessingEngine;

class MyDirWorker :
    public wxThread
{
public:
    MyDirWorker( ProcessingEngine& handler_i, std::string parent_i, std::string filename_i );
    ~MyDirWorker();

    virtual wxThread::ExitCode Entry() override;

private:
    ProcessingEngine& handler;
    const std::string parent;
    const std::string filename;
};


#endif // !myDirWorker_H
