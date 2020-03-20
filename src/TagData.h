#ifndef TagData_H
#define TagData_H

#include "JPEG_Tag.h"

#include <wx/types.h>

#include <string>

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
    std::string to_string() const;

private:
    int position;
    JPEG_Tag tag;
    int length;
    bool bad;
};

#endif // !TagData_H
