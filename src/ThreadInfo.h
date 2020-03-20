#ifndef ThreadInfo_H
#define ThreadInfo_H

#include "TagData.h"

#include <wx/clntdata.h>
#include <string>
#include <vector>

class ThreadInfo :
    public wxClientData
{
public:
    ThreadInfo() = default;
    ThreadInfo( std::string parent_i, std::string data_i ) :
        parent( parent_i ),
        entry( data_i )
    {

    }

    ThreadInfo( std::string parent_i, std::vector<TagData>& data_i ) :
        parent( parent_i ),
        data( data_i )
    {

    }
    std::string parent;
    std::string entry;
    std::vector<TagData> data;
};


#endif // !ThreadInfo_H
