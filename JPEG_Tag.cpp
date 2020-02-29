// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
    // Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

#include "JPEG_Tag.h"
#include <algorithm>

const std::vector<JPEG_Tag::Marker_values> JPEG_Tag::markersWithoutLength = { JPEG_Tag::RST0, JPEG_Tag::RST1, JPEG_Tag::RST2,
        JPEG_Tag::RST3, JPEG_Tag::RST4, JPEG_Tag::RST5,JPEG_Tag::RST6, JPEG_Tag::RST7, JPEG_Tag::StartOfImage, JPEG_Tag::EndOfImage, JPEG_Tag::StartOfScan };

JPEG_Tag::JPEG_Tag() :
    tag( JPEG_Tag_ID ),
    needsLength( false )
{
}

JPEG_Tag::JPEG_Tag( Marker_values tag_i ) :
    tag( tag_i ),
    needsLength( determineNeedsLength( tag_i ) )
{
}

JPEG_Tag::JPEG_Tag( wxByte byte_i ) :
    tag( static_cast<Marker_values>( byte_i ) ),
    needsLength( determineNeedsLength( tag ) )
{
}

bool JPEG_Tag::determineNeedsLength( Marker_values tag_i )
{
    return std::find( markersWithoutLength.begin(), markersWithoutLength.end(), tag ) == markersWithoutLength.end();
}
