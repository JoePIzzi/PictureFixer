#ifndef FileProcessor_H
#define FileProcessor_H

#include <wx/types.h>
#include <wx/file.h>

#include <string>

#include "JPEG_Tag.h"

class FileProcessor
{
public:
	FileProcessor( std::string fileName_i );
	~FileProcessor() = default;

	void ProcessFile();

private:
	typedef std::vector<uint8_t> BufferType;
	typedef BufferType::iterator BufferIterator;

	BufferIterator FindTag( BufferIterator& currentPosition, BufferType& buffer, JPEG_Tag::Marker_values desiredTag );
	void WriteMidData( BufferIterator start, BufferIterator end );
	void WritePostData( BufferIterator start, BufferIterator end );
	void WriteJPG( BufferIterator start, BufferIterator end );
	void WriteFileData( std::string filename, BufferIterator start, BufferIterator end );
	std::string fileName;
	std::string dirName;
	int count;
};


#endif // !FileProcessor_H
