// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
    // Include your minimal set of headers here, or wx.h
#   include <wx/types.h>
#endif

#include "TagData.h"

TagData::TagData( int position_i ) :
    position( position_i ),
    tag(),
    length( 0 ),
    bad( false )
{
}

TagData::TagData( int position_i, wxByte identifier_i ) :
    position( position_i ),
    tag( identifier_i ),
    length( 0 ),
    bad( false )
{
}

void TagData::setTag( wxByte identifier_i )
{
    tag = JPEG_Tag( identifier_i );
}
