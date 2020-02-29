#ifndef ProcessingEngine_H
#define ProcessingEngine_H

#include <string>
#include <vector>

#include <wx/types.h>
#include <wx/thread.h>

#include "BufferType.h"
#include "TagData.h"

class ProcessingEngine
{
public:
    typedef std::vector<wxThread*> ThreadArrayType;

    ProcessingEngine();
    ~ProcessingEngine();
    void ProcessDir( const std::string& parent_i, const std::string& directory );
    void ProcessFile( const std::string& parent_i, const std::string& fileName_i );

    typedef std::vector<TagData> TagListType;

private:
    void ProcessData( const BufferType& buffer_i, TagListType& tagList );

    static constexpr int maxThreads = 10;
    std::string defaultFileMask;
    wxSemaphore semaphore;
};

#endif
