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
    ThreadInfo( const ThreadInfo& ) = default;
    ThreadInfo& operator=( const ThreadInfo& rhs ) = default;

    ThreadInfo( const wxDataViewItem& parent_i, const wxDataViewItem& child_i ) :
        parent( parent_i ),
        entry( child_i )
    {

    }

    wxDataViewItem parent;
    wxDataViewItem entry;
};


#endif // !ThreadInfo_H
