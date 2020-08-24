#ifndef JPEG_Tag_H
#define JPEG_Tag_H

#include <wx/types.h>

#include <string>
#include <vector>

class JPEG_Tag
{
public:
    enum Marker_values
    {
        JPEG_Tag_ID = 0xFF,
        NonDifferentialHuffmanBaslineDCT = 0xC0,
        NonDifferentialHuffmanExtendedDCT = 0xC1,
        NonDifferentialHuffmanProgressiveDCT = 0xC2,
        NonDifferentialHuffmanLosslessDCT = 0xC4,
        DifferentialHuffmanDCT = 0xC5,
        DifferentailHuffmanProgressiveDCT = 0xC6,
        DifferentailHuffmanLosslessDCT = 0xC7,
        JPEGExtensions = 0xC8,
        NonDifferentialArithmeticExtendedDCT = 0xC9,
        NonDifferentialArithmeticProgressiveDCT = 0xCA,
        NonDifferentialArithmeticLosslessDCT = 0xCB,
        DifferentialArithmeticSequentialDCT = 0xCD,
        DifferentialArithmeticProgressiveDCT = 0xCE,
        DifferentialArithmeticLosslessDCT = 0xCF,
        DefineArithmeticCoding = 0xCC,
        RST0 = 0xD0,
        RST1 = 0xD1,
        RST2 = 0xD2,
        RST3 = 0xD3,
        RST4 = 0xD4,
        RST5 = 0xD5,
        RST6 = 0xD6,
        RST7 = 0xD7,
        StartOfImage = 0xD8,
        EndOfImage = 0xD9,
        StartOfScan = 0xDA,
        DefineQuantizationTable = 0xDB,
        DefineNumberOfLines = 0xDC,
        DefineRestartInterval = 0xDD,
        DefineHierarchicalProgression = 0xDE,
        ExpandReferenceComponent = 0xDF,
        App0 = 0xE0,
        App1 = 0xE1,
        App2 = 0xE2,
        App3 = 0xE3,
        App4 = 0xE4,
        App5 = 0xE5,
        App6 = 0xE6,
        App7 = 0xE7,
        App8 = 0xE8,
        App9 = 0xE9,
        AppA = 0xEA,
        AppB = 0xEB,
        AppC = 0xEC,
        AppD = 0xED,
        AppE = 0xEE,
        AppF = 0xEF,
        JPG0 = 0xF0,
        JPG1 = 0xF1,
        JPG2 = 0xF2,
        JPG3 = 0xF3,
        JPG4 = 0xF4,
        JPG5 = 0xF5,
        JPG6 = 0xF6,
        JPG7 = 0xF7,
        JPG8 = 0xF8,
        JPG9 = 0xF9,
        JPGA = 0xFA,
        JPGB = 0xFB,
        JPGC = 0xFC,
        JPGD = 0xFD,
        COMMENT = 0xFE
    };

    JPEG_Tag();
    JPEG_Tag( Marker_values tag_i );
    JPEG_Tag( wxByte byte_i );
    ~JPEG_Tag() = default;

    bool hasLength() const { return needsLength; }
    std::string to_string() const;
    Marker_values getTag() const { return tag; }

private:
    bool determineNeedsLength( Marker_values tag_i );
    
    Marker_values tag;
    bool needsLength;
    static const std::vector<Marker_values> markersWithLength;
};

#endif // !JPEG_Tag_H
