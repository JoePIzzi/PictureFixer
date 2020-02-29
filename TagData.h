#ifndef TagData_H
#define TagData_H

#include "BufferType.h"
#include "JPEG_Tag.h"

#include <wx/types.h>

class TagData
{
public:
    TagData( int position_i );
    TagData( int position_i, wxByte identifier_i );
    ~TagData() = default;

    int getPosition() const { return position; }
    int getLength() const { return length; }
    bool isBad() const { return bad; }
    JPEG_Tag getTag() const { return tag; }

    void setLength( int length_i ) { length = length_i; }
    void setTag( wxByte identifier_i );
    void setBad() { bad = true; }

private:
    int position;
    JPEG_Tag tag;
    int length;
    bool bad;
};

#endif // !TagData_H
