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
#include "MyDirWorker.h"
#include "MyFileWorker.h"
#include "PictureFixerApp.h"
#include "MyPictureModel.h"
#include "ThreadInfo.h"

#include <algorithm>
#include <utility>
#include "FileProcessor.h"

#include "MemDbg.h"

ProcessingEngine::ProcessingEngine( MyPictureModel* model_i ) :
    defaultFileMask( "*.jpg" ),
    fix( false ),
    semaphore( maxThreads, maxThreads ),
    model( model_i )
{
}

ProcessingEngine::~ProcessingEngine()
{
}

void ProcessingEngine::StartProcessing( std::string directory, bool fix_i )
{
    fix = fix_i;
    std::string noParent;
    //ProcessDir( nullptr, noParent, directory );
    MyDirWorker* newThread = new MyDirWorker( *this, noParent, directory );
    newThread->Run();
}

void ProcessingEngine::ProcessDir( wxThread* owningThread_i, std::string parent_i, std::string directory_i )
{
    if ( wxDir::Exists( directory_i ) )
    {
        wxDir dir( directory_i );
        if ( dir.IsOpened() )
        {
            wxLogStatus( "%d, Processing Directory %s", wxThread::GetCurrentId(), directory_i );
            //model->AddChildContainer( parent_i, directory_i );
            if ( dir.HasSubDirs() )
            {
                wxString dirName;
                dir.GetFirst( &dirName, wxEmptyString, wxDIR_DIRS );
                do
                {
                    {
                        wxCriticalSectionLocker locker( wxGetApp().criticalSection );
                        if ( wxGetApp().isShuttingDown() )
                        {
                            // Aplication is shutting down. Just exit.
                            return;
                        }
                    }

                    if ( dirName == ".xvpics" )
                    {
                        continue;
                    }
                    dirName = directory_i + wxFileName::GetPathSeparator() + dirName;
                    ProcessDir( owningThread_i, directory_i, dirName );
                } while ( dir.GetNext( &dirName ) );
            }

            if ( dir.HasFiles() )
            {
                wxString fileName;
                dir.GetFirst( &fileName, defaultFileMask );
                do
                {
                    {
                        wxCriticalSectionLocker locker( wxGetApp().criticalSection );
                        if ( wxGetApp().isShuttingDown() )
                        {
                            // Aplication is shutting down. Just exit.
                            break;
                        }
                    }

                    // check if just this thread was asked to exit
                    if ( owningThread_i->TestDestroy() )
                        break;

                    fileName = directory_i + wxFileName::GetPathSeparator() + fileName;
                    MyFileWorker* newThread = new MyFileWorker( this, directory_i, fileName, semaphore );
                    semaphore.Wait();
                    newThread->Run();
                } while ( dir.GetNext( &fileName ) );
            }
        }
    }
}

void ProcessingEngine::ProcessFile( std::string parent_i, std::string fileName_i )
{
    if ( wxFile::Exists( fileName_i ) )
    {
        wxFile file( fileName_i );
        if ( file.IsOpened() )
        {
            //wxLogMessage( "%d, Processing File %s", wxThread::GetCurrentId(), fileName_i );
            wxFileOffset length = file.Length();
            BufferType buffer( length );
            wxFileOffset read_bytes = file.Read( buffer.data(), length+1 );
            wxASSERT( read_bytes == length );
            wxASSERT( file.Eof() );
            std::vector<TagData> tagList;
            if ( ProcessFileData( buffer, tagList ) )
            {
                model->AddChildContainer( fileName_i );
                for ( TagData tag : tagList )
                {
                    model->AddChild( fileName_i, tag.to_string() );
                }
                if ( fix )
                {
                    file.Close();
                    FileProcessor process( fileName_i );
                    process.ProcessFile();
                }
            }
        }
    }
}

bool ProcessingEngine::ProcessFileData( const BufferType& buffer_i, TagListType& tagList )
{
    bool saveData = false;
    int numberPictures = 0;
    int numberUnmatchedSOIs = 0;

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

        if ( ( currentPosition == end ) || ( currentPosition + 1 == end ) )
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
        if ( newTag.getTag().getTag() == JPEG_Tag::StartOfImage )
        {
            ++numberUnmatchedSOIs;
        }
        else if ( newTag.getTag().getTag() == JPEG_Tag::EndOfImage )
        {
            --numberUnmatchedSOIs;
            if ( numberUnmatchedSOIs < 0 )
            {
                saveData = true;
            }
            else if ( numberUnmatchedSOIs == 0 )
            {
                ++numberPictures;
            }
        }
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
    if ( ( numberPictures > 1 ) || ( numberUnmatchedSOIs != 0 ) )
    {
        saveData = true;
    }

    return saveData;
}
