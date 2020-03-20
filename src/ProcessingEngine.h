#ifndef ProcessingEngine_H
#define ProcessingEngine_H

#include <string>
#include <vector>
#include <memory>

#include <wx/types.h>
#include <wx/thread.h>

#include "TagData.h"

class MyPictureModel;

class ProcessingEngine
{
public:
    typedef std::vector<wxThread*> ThreadArrayType;
    typedef std::vector<wxByte> BufferType;

    ProcessingEngine( MyPictureModel* model_i );
    ~ProcessingEngine();
    void StartProcessing( std::string directory );
    void ProcessDir( wxThread* owningThread_i, std::string parent_i, std::string directory );
    void ProcessFile( std::string parent_i, std::string fileName_i );

    typedef std::vector<TagData> TagListType;

private:
    void ProcessData( const BufferType& buffer_i, TagListType& tagList );

    static constexpr int maxThreads = 10;
    std::string defaultFileMask;
    wxSemaphore semaphore;
    wxEvtHandler* handler;
    MyPictureModel* model;
};

#endif
