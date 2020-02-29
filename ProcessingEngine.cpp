// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#endif

#include "ProcessingEngine.h"
#include "JPEG_Tag.h"
#include "TagData.h"
#include "MyWorkerThread.h"
#include "PictureFixerApp.h"
#include "MainWindow.h"

#include <algorithm>
#include <utility>

ProcessingEngine::ProcessingEngine() :
    defaultFileMask( "*.jpg" ),
    semaphore( maxThreads, maxThreads )
{
}

ProcessingEngine::~ProcessingEngine()
{
}

void ProcessingEngine::ProcessDir( const std::string& parent_i, const std::string& directory_i )
{
    if ( wxDir::Exists( directory_i ) )
    {
        wxDir dir( directory_i );
        if ( dir.IsOpened() )
        {
            wxThreadEvent* myEvent = new wxThreadEvent( EVT_ADD_DIRECTORY_EVENT );
            myEvent->SetPayload( &std::make_pair( parent_i, directory_i ) );
            wxGetApp().QueueEvent( myEvent );
            //wxYield();
            if ( dir.HasSubDirs() )
            {
                wxString dirName;
                dir.GetFirst( &dirName, wxEmptyString, wxDIR_DIRS );
                do
                {
                    if ( dirName == ".xvpics" )
                    {
                        continue;
                    }
                    dirName = directory_i + wxFileName::GetPathSeparator() + dirName;
                    ProcessDir( directory_i, dirName );
                } while ( dir.GetNext( &dirName ) );
            }

            if ( dir.HasFiles() )
            {
                wxString fileName;
                dir.GetFirst( &fileName, defaultFileMask );
                do
                {
                    fileName = directory_i + wxFileName::GetPathSeparator() + fileName;
                    MyWorkerThread* newThread = new MyWorkerThread( this, parent_i, fileName, semaphore );
                    semaphore.Wait();
                    newThread->Run();
                } while ( dir.GetNext( &fileName ) );
            }
        }
    }
}

void ProcessingEngine::ProcessFile( const std::string& parent_i, const std::string& fileName_i )
{
    if ( wxFile::Exists( fileName_i ) )
    {
        wxFile file( fileName_i );
        if ( file.IsOpened() )
        {
            wxThreadEvent* myFileEvent = new wxThreadEvent( EVT_ADD_FILE_EVENT );
            myFileEvent->SetPayload( &std::make_pair( parent_i, fileName_i ) );
            wxGetApp().QueueEvent( myFileEvent );
            //wxYield();
            wxFileOffset length = file.Length();
            BufferType buffer( length );
            wxFileOffset read_bytes = file.Read( buffer.data(), length+1 );
            wxASSERT( read_bytes == length );
            wxASSERT( file.Eof() );
            std::vector<TagData> tagList;
            ProcessData( buffer, tagList );
            wxThreadEvent* myDataEvent = new wxThreadEvent( EVT_ADD_DATA_EVENT );
            myDataEvent->SetPayload( &std::make_pair( fileName_i, tagList ) );
            wxGetApp().QueueEvent( myDataEvent );
            //wxYield();
        }
    }
}

void ProcessingEngine::ProcessData( const BufferType& buffer_i, TagListType& tagList )
{
    const auto end = buffer_i.end();
    const auto beginning = buffer_i.cbegin();
    auto currentPosition = buffer_i.begin();
    while ( currentPosition != end )
    {
        {
            wxCriticalSectionLocker locker( wxGetApp().criticalSection );
            if ( wxGetApp().isShuttingDown() )
            {
                // Aplication is shutting down. Just exit.
                break;
            }
        }

        bool isLittleEndian = false;
        currentPosition = std::find( currentPosition, end, JPEG_Tag::JPEG_Tag_ID );

        if ( ( currentPosition  == end ) || ( currentPosition + 1 == end ) )
        {
            break;
        }
        int position = currentPosition - beginning;
        wxByte identifier = *( ++currentPosition );
        if ( ( identifier == 0 ) || ( identifier == 0xFF ) )
        {
            continue;
        }
        TagData newTag( position, identifier );
        if ( newTag.getTag().hasLength() )
        {
            wxByte byte0;
            wxByte byte1;
            if ( isLittleEndian )
            {
                byte1 = *( ++currentPosition );
                byte0 = *( ++currentPosition );
            }
            else
            {
                byte0 = *( ++currentPosition );
                byte1 = *( ++currentPosition );
            }
            int length = byte0 * 256 + byte1;
            newTag.setLength( length );
            if ( length <= 2 )
            {
                break;
            }
            length -= 2;
            // safely advance the iterator
            while ( ( currentPosition != end ) && ( length >= 0 ) )
            {
                ++currentPosition;
                --length;
            }
            if ( currentPosition == end )
            {
                newTag.setBad();
            }
        }
        tagList.push_back( newTag );
    }
}
