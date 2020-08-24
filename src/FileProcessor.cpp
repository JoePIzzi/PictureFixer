// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/debug.h>
#endif

#include "FileProcessor.h"
#include "JPEG_Tag.h"
#include "TagData.h"

#include "MemDbg.h"

FileProcessor::FileProcessor( std::string fileName_i ) :
    fileName( fileName_i ),
    dirName(),
    count( 0 )
{
}

void FileProcessor::ProcessFile()
{
    if ( wxFile::Exists( fileName ) )
    {
        wxFile file( fileName );
        if ( file.IsOpened() )
        {
            wxFileName tempName;
            dirName = "U:\\Users\\pizzijw\\Documents\\Photography\\Pictures\\Recovered Files";
            dirName.push_back( tempName.GetPathSeparator() );
            tempName.AssignTempFileName( dirName );
            dirName.append( tempName.GetName() );
            //wxRemoveFile( tempName.GetFullPath() );
            wxDir::Make( dirName, wxPATH_MKDIR_FULL );
            {
                std::string readmeFilename( dirName );
                readmeFilename.push_back( tempName.GetPathSeparator() );
                readmeFilename.append( "README.txt" );
                wxFile readme( readmeFilename, wxFile::write_excl );
                readme.Write( fileName );
            }
            wxLogMessage( "Processing File %s", fileName );
            wxFileOffset length = file.Length();
            BufferType buffer( length );
            wxFileOffset read_bytes = file.Read( buffer.data(), length + 1 );
            wxASSERT( read_bytes == length );
            wxASSERT( file.Eof() );
            const BufferIterator end = buffer.end();
            const BufferIterator begin = buffer.begin();
            BufferIterator currentPosition = buffer.begin();
            BufferIterator lastStart = buffer.end();
            BufferIterator lastEnd = buffer.begin();
            int currentIndex = 0;
            int lastStartIndex = length;
            int lastEndIndex = 0;
            while ( currentPosition != end )
            {
                // find start tag
                // if found
                //   if start tag is not beginning
                //   write found data
                // else
                //   write found data
                // find end tag
                // if found
                //   write picture data
                // else
                //   write incomplete picture?
                currentPosition = FindTag( currentPosition, buffer, JPEG_Tag::StartOfImage );
                // currentPosition now points to START of begin tag or one past end of file.
                lastStart = currentPosition;
                lastStartIndex = lastStart - begin;
                // handle no start position found.
                if ( currentPosition == end )
                {
                    WritePostData( lastEnd, currentPosition );
                }
                // handle data before start tag.
                else if ( currentPosition != lastEnd )
                {
                    WriteMidData( lastEnd, lastStart );
                    // advance lastEnd to account for written data
                    lastEnd = currentPosition;
                    lastEndIndex = lastEnd - begin;
                }
                else
                {
                    // advance currentPosition past the start tag.
                    ++currentPosition;
                    ++currentPosition;
                    currentPosition = FindTag( currentPosition, buffer, JPEG_Tag::EndOfImage );
                    currentIndex = currentPosition - begin;
                    // handle no end tag.
                    if ( currentPosition == end )
                    {
                        WritePostData( lastStart, currentPosition );
                    }
                    else
                    {
                        // currentPosition now points to the START of the end tag.
                        ++currentPosition;
                        ++currentPosition;
                        lastEnd = currentPosition;
                        lastEndIndex = lastEnd - begin;
                        WriteJPG( lastStart, lastEnd );
                    }
                }
                //++currentPosition;
            }
        }
    }
}

FileProcessor::BufferIterator FileProcessor::FindTag( BufferIterator& currentPosition, BufferType& buffer, JPEG_Tag::Marker_values desiredTag )
{
    const BufferType::iterator end = buffer.end();
    while ( currentPosition != end )
    {
        currentPosition = std::find( currentPosition, end, JPEG_Tag::JPEG_Tag_ID );
        if ( ( currentPosition == end ) || ( currentPosition + 1 == end ) )
        {
            break;
        }
        wxByte identifier = *( currentPosition + 1 );
        if ( ( identifier == 0 ) || ( identifier == 0xFF ) )
        {
            ++currentPosition;
            ++currentPosition;
            continue;
        }
        TagData tag( 0, identifier );
        if ( identifier == desiredTag )
        {
            break;
        }
        else if ( tag.getTag().hasLength() )
        {
            wxByte byte0 = *( ++currentPosition );
            wxByte byte1 = *( ++currentPosition );
            int length = byte0 * 256 + byte1;
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
        }
        else
        {
            ++currentPosition;
            ++currentPosition;
        }
    }
    return currentPosition;
}

void FileProcessor::WriteMidData( BufferIterator start, BufferIterator end )
{
    std::string outfileName( dirName );
    outfileName.push_back( wxFileName::GetPathSeparator() );
    outfileName.append( "DataBetween_" );
    outfileName.append( std::to_string( count ) );
    outfileName.append( "_" );
    outfileName.append( std::to_string( count + 1 ) );
    outfileName.append( ".dat" );
    WriteFileData( outfileName, start, end );
}

void FileProcessor::WritePostData( BufferIterator start, BufferIterator end )
{
    std::string outfileName( dirName );
    outfileName.push_back( wxFileName::GetPathSeparator() );
    outfileName.append( "DataAfter_" );
    outfileName.append( std::to_string( count ) );
    outfileName.append( ".dat" );
    WriteFileData( outfileName, start, end );
}

void FileProcessor::WriteJPG( BufferIterator start, BufferIterator end )
{
    wxFileName currentFile( fileName );
    std::string outfileName( dirName );
    outfileName.push_back( wxFileName::GetPathSeparator() );
    outfileName.append( currentFile.GetName() );
    outfileName.append( "." );
    ++count;
    outfileName.append( std::to_string( count ) );
    outfileName.append( ".jpg" );
    WriteFileData( outfileName, start, end );
}

void FileProcessor::WriteFileData( std::string filename_i, BufferIterator start, BufferIterator end )
{
    wxFile outFile( filename_i, wxFile::write_excl );
    if ( outFile.IsOpened() )
    {
        int length = end - start;
        outFile.Write( static_cast<void*>(&(*start)), length );
    }
    else
    {
        wxLogMessage( "Couldn't open %s.\n", filename_i );
    }
}
