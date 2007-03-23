/*****************************************************************************
* $Id: $
*
* Project:  Intergraph Raster Format support
* Purpose:  Read selected types of Intergraph Raster Format
* Author:   Ivan Lucena, ivan@ilucena.net
*
******************************************************************************
* Copyright (c) 2007, Ivan Lucena
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files ( the "Software" ),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*****************************************************************************/

#include "IngrTypes.h"

//  ----------------------------------------------------------------------------
//     Intergraph IntergraphRasterBand
//  ----------------------------------------------------------------------------

class IntergraphRasterBand : public GDALPamRasterBand
{
    friend class IntergraphDataset;

protected:
    GDALColorTable *poColorTable;
    uint32          nDataOffset;
    uint32          nBlockBufSize;
    uint32          nBytesRead;
    uint32          nBandStart;
    uint8           nRGBIndex;

    INGR_Format      eFormat;
    bool            bTreatAsBitmap;

    GByte	       *pabyBlockBuf;
    uint32          nTiles;

    INGR_TileItem    *pahTiles;

    INGR_HeaderOne   hHeaderOne;
    INGR_HeaderTwoA  hHeaderTwo;
    INGR_TileHeader     hTileDir;

public:
    IntergraphRasterBand( IntergraphDataset *poDS, 
        int nBand,
        int nBandOffset,
        GDALDataType eType = GDT_Unknown);
    ~IntergraphRasterBand();

    virtual double GetMinimum( int *pbSuccess = NULL );
    virtual double GetMaximum( int *pbSuccess = NULL );    
    virtual GDALColorTable *GetColorTable();
    virtual GDALColorInterp GetColorInterpretation();
    virtual CPLErr IReadBlock( int nBlockXOff, int nBlockYOff, void *pImage );
    virtual CPLErr IWriteBlock( int nBlockXOff, int nBlockYOff, void *pImage );
    virtual CPLErr SetColorTable( GDALColorTable *poColorTable ); 
    virtual CPLErr SetStatistics( double dfMin, double dfMax, double dfMean, double dfStdDev );

protected:
    CPLErr  LoadBlockBuf( int nBlockXOff, int nBlockYOff );

    void ReshapeBlock( int nBlockXOff, int nBlockYOff, GByte *pabyBlock );
    void FlushBandHeader( void );
};

//  ----------------------------------------------------------------------------
//     Intergraph IntergraphRGBBand
//  ----------------------------------------------------------------------------

class IntergraphRGBBand : public IntergraphRasterBand
{

public:
    IntergraphRGBBand( IntergraphDataset *poDS, 
        int nBand,
        int nBandOffset,
        int nRGorB );

    virtual GDALColorInterp GetColorInterpretation();
    virtual CPLErr IReadBlock( int nBlockXOff, int nBlockYOff, void *pImage );
};

//  ----------------------------------------------------------------------------
//     Intergraph IntergraphBitmapBand
//  ----------------------------------------------------------------------------

class IntergraphBitmapBand : public IntergraphRasterBand
{
public:
    IntergraphBitmapBand( IntergraphDataset *poDS, 
        int nBand,
        int nBandOffset );

    virtual CPLErr IReadBlock( int nBlockXOff, int nBlockYOff, void *pImage );
};

//  ----------------------------------------------------------------------------
//     Intergraph IntergraphJPEGBand
//  ----------------------------------------------------------------------------

class IntergraphJPEGBand : public IntergraphRasterBand
{
private:
    FILE        *fp_jpeg;
    char        *psz_jpeg;

    struct jpeg_decompress_struct sDInfo;
    struct jpeg_error_mgr sJErr;
    int    nQLevel;
    int    nQuality;

public:
    IntergraphJPEGBand( IntergraphDataset *poDS, 
        int nBand,
        int nBandOffset );

    virtual CPLErr IReadBlock( int nBlockXOff, int nBlockYOff, void *pImage );
};

