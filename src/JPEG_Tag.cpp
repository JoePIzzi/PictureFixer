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

std::string JPEG_Tag::to_string() const
{
    std::string text;
    switch ( tag )
    {
    case NonDifferentialHuffmanBaslineDCT:
        text.assign( "NonDifferentialHuffmanBaslineDCT" );
        break;


    case NonDifferentialHuffmanExtendedDCT:
        text.assign( "NonDifferentialHuffmanExtendedDCT" );
        break;


    case NonDifferentialHuffmanProgressiveDCT:
        text.assign( "NonDifferentialHuffmanProgressiveDCT" );
        break;


    case NonDifferentialHuffmanLosslessDCT:
        text.assign( "NonDifferentialHuffmanLosslessDCT" );
        break;


    case DifferentialHuffmanDCT:
        text.assign( "DifferentialHuffmanDCT" );
        break;


    case DifferentailHuffmanProgressiveDCT:
        text.assign( "DifferentailHuffmanProgressiveDCT" );
        break;


    case DifferentailHuffmanLosslessDCT:
        text.assign( "DifferentailHuffmanLosslessDCT" );
        break;


    case JPEGExtensions:
        text.assign( "JPEGExtensions" );
        break;


    case NonDifferentialArithmeticExtendedDCT:
        text.assign( "NonDifferentialArithmeticExtendedDCT" );
        break;


    case NonDifferentialArithmeticProgressiveDCT:
        text.assign( "NonDifferentialArithmeticProgressiveDCT" );
        break;


    case NonDifferentialArithmeticLosslessDCT:
        text.assign( "NonDifferentialArithmeticLosslessDCT" );
        break;


    case DifferentialArithmeticSequentialDCT:
        text.assign( "DifferentialArithmeticSequentialDCT" );
        break;


    case DifferentialArithmeticProgressiveDCT:
        text.assign( "DifferentialArithmeticProgressiveDCT" );
        break;


    case DifferentialArithmeticLosslessDCT:
        text.assign( "DifferentialArithmeticLosslessDCT" );
        break;


    case DefineArithmeticCoding:
        text.assign( "DefineArithmeticCoding" );
        break;


    case RST0:
        text.assign( "RST0" );
        break;


    case RST1:
        text.assign( "RST1" );
        break;


    case RST2:
        text.assign( "RST2" );
        break;


    case RST3:
        text.assign( "RST3" );
        break;


    case RST4:
        text.assign( "RST4" );
        break;


    case RST5:
        text.assign( "RST5" );
        break;


    case RST6:
        text.assign( "RST6" );
        break;


    case RST7:
        text.assign( "RST7" );
        break;


    case StartOfImage:
        text.assign( "StartOfImage" );
        break;


    case EndOfImage:
        text.assign( "EndOfImage" );
        break;


    case StartOfScan:
        text.assign( "StartOfScan" );
        break;


    case DefineQuantizationTable:
        text.assign( "DefineQuantizationTable" );
        break;


    case DefineNumberOfLines:
        text.assign( "DefineNumberOfLines" );
        break;


    case DefineRestartInterval:
        text.assign( "DefineRestartInterval" );
        break;


    case DefineHierarchicalProgression:
        text.assign( "DefineHierarchicalProgression" );
        break;


    case ExpandReferenceComponent:
        text.assign( "ExpandReferenceComponent" );
        break;


    case App0:
        text.assign( "App0" );
        break;


    case App1:
        text.assign( "App1" );
        break;


    case App2:
        text.assign( "App2" );
        break;


    case App3:
        text.assign( "App3" );
        break;


    case App4:
        text.assign( "App4" );
        break;


    case App5:
        text.assign( "App5" );
        break;


    case App6:
        text.assign( "App6" );
        break;


    case App7:
        text.assign( "App7" );
        break;


    case App8:
        text.assign( "App8" );
        break;


    case App9:
        text.assign( "App9" );
        break;


    case AppA:
        text.assign( "AppA" );
        break;


    case AppB:
        text.assign( "AppB" );
        break;


    case AppC:
        text.assign( "AppC" );
        break;


    case AppD:
        text.assign( "AppD" );
        break;


    case AppE:
        text.assign( "AppE" );
        break;


    case AppF:
        text.assign( "AppF" );
        break;


    case JPG0:
        text.assign( "JPG0" );
        break;


    case JPG1:
        text.assign( "JPG1" );
        break;


    case JPG2:
        text.assign( "JPG2" );
        break;


    case JPG3:
        text.assign( "JPG3" );
        break;


    case JPG4:
        text.assign( "JPG4" );
        break;


    case JPG5:
        text.assign( "JPG5" );
        break;


    case JPG6:
        text.assign( "JPG6" );
        break;


    case JPG7:
        text.assign( "JPG7" );
        break;


    case JPG8:
        text.assign( "JPG8" );
        break;


    case JPG9:
        text.assign( "JPG9" );
        break;


    case JPGA:
        text.assign( "JPGA" );
        break;


    case JPGB:
        text.assign( "JPGB" );
        break;


    case JPGC:
        text.assign( "JPGC" );
        break;


    case JPGD:
        text.assign( "JPGD" );
        break;


    case COMMENT:
        text.assign( "COMMENT" );
        break;

    }
    return text;
}

bool JPEG_Tag::determineNeedsLength( Marker_values tag_i )
{
    return std::find( markersWithoutLength.begin(), markersWithoutLength.end(), tag ) == markersWithoutLength.end();
}
