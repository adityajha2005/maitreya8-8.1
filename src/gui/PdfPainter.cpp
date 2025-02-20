/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfPainter.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2017 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

#include "PdfPainter.h"
#include "PdfBase.h"

#include "Conf.h"
#include "maitreya.h"
#include "FontProvider.h"
#include "guibase.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "Sheet.h"
#include "SymbolProvider.h"

#include <wx/log.h>

extern Config *config;

//#define PDF_TEXT_ZOOMFACTOR 1
#define PDF_TEXT_ZOOMFACTOR .004
#define PDF_DEFAULT_FGCOLOR *wxBLACK
#define PDF_DEFAULT_BGCOLOR *wxWHITE
#define PDF_PEN_PENWIDTH_FACTOR .5

/*****************************************************
**
**   PdfPainter   ---   Constructor
**
******************************************************/
PdfPainter::PdfPainter( BasePdfDocument *pdfdoc )
{
	pdf = pdfdoc;
	drawmode = wxPDF_STYLE_DRAW;

	defaultLineStyle = new wxPdfLineStyle( pdf->GetLineStyle());
	defaultTextColor = new wxPdfColour( pdf->GetTextColour() );
}

/*****************************************************
**
**   PdfPainter   ---   Destructor
**
******************************************************/
PdfPainter::~PdfPainter()
{
	delete defaultLineStyle;
	delete defaultTextColor;
}

/*****************************************************
**
**   PdfPainter   ---   setTransparentBrush
**
******************************************************/
void PdfPainter::setTransparentBrush()
{
	setBrush( MBrush( *wxBLACK, wxTRANSPARENT ));
}

/*****************************************************
**
**   PdfPainter   ---   setBrush
**
******************************************************/
void PdfPainter::setBrush( const MBrush &b )
{
	// TODO howto scale width und height 
	if ( b.style == wxSOLID )
	{
		if ( b.color.IsOk() )
		{
			drawmode = wxPDF_STYLE_FILL;
			pdf->SetFillColour( b.color );
		}
		else wxLogError( wxT( "bitmap in PdfPainter::setBrush is not transparent but color is not okay" ));
	}
	else if ( b.style == wxSTIPPLE )
	{
		wxImage image;
		//wxBitmap *bitmap = b.GetStipple();
		wxBitmap bitmap = ImageProvider::get()->getFileBasedBitmap( b.filename, b.rotateHue );
		if ( bitmap.IsOk())
		{
			image = bitmap.ConvertToImage();

			double width = image.GetWidth() / 3;
			double height = image.GetHeight() / 3;

			// setup a hash value for the image
			wxString s = createImageHash( &image );

			pdf->AddPattern( s, image, width, height );
			drawmode = wxPDF_STYLE_FILL;
			pdf->SetFillPattern( s );
			pdf->SetDrawPattern( s );
		}
		else wxLogError( wxT( "bitmap in PdfPainter::setBrush not okay" ));
	}
	else if ( b.style != wxTRANSPARENT )
	{
		// TODO SetFillPattern( wxString name )
		if ( b.color.IsOk() )
		{
			drawmode = wxPDF_STYLE_FILL;
			pdf->SetFillColour( b.color );
		}
		else wxLogError( wxT( "bitmap in PdfPainter::setBrush is not transparent but color is not okay" ));
		//setBrush( b.GetColour() );
	}
	else // must be transparent
	{
		drawmode = wxPDF_STYLE_DRAW;
	}
}

wxColour colorToGreyScale( const wxColour&c )
{
	int g = ( c.Red() + c.Green() + c.Blue()) / 3.0;
	return wxColour( g, g, g );
}

/*****************************************************
**
**   PdfPainter   ---   getDefaultBrush
**
******************************************************/
MBrush PdfPainter::getDefaultBrush()
{
	return MBrush( PDF_DEFAULT_BGCOLOR, wxTRANSPARENT );
}

/*****************************************************
**
**   PdfPainter   ---   setPen
**
******************************************************/
void PdfPainter:: setPen( const wxPen &p )
{
	//const double defaultWidth = 1;

	double width = p.GetWidth() * PDF_PEN_PENWIDTH_FACTOR;

	wxPdfLineStyle style;
	switch ( p.GetStyle() )
	{
		case wxDOT:
		{
			wxPdfArrayDouble dash1;
			dash1.Add( .5 );
			style = wxPdfLineStyle( width, wxPDF_LINECAP_NONE, wxPDF_LINEJOIN_MITER, dash1, 10.);
		}
		break;
		case wxLONG_DASH:
		{
			wxPdfArrayDouble dash2;
			dash2.Add( 3 );
			style  = wxPdfLineStyle( width, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash2, 0.);
			break;
		}
		break;
		case wxSHORT_DASH:
		{
			wxPdfArrayDouble dash3;
			dash3.Add( 1.5 );
			style  = wxPdfLineStyle( width, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash3, 0.);
		}
		case wxDOT_DASH:
		{
			wxPdfArrayDouble dash4;
			dash4.Add( 1.5 );
			dash4.Add( 3 );
			style  = wxPdfLineStyle( width, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash4, 0.);
		}
		break;
		default:
		// Noting to do
		break;
	}

	style.SetWidth( width );
	if ( p.GetColour().IsOk() )
	{
		style.SetColour( p.GetColour() );
		pdf->SetDrawColour( p.GetColour());
	}
	pdf->SetLineStyle( style );
}

/*****************************************************
**
**   PdfPainter   ---   setPenColor
**
******************************************************/
void PdfPainter::setPenColor( const wxColour &c )
{
	wxPdfLineStyle style = pdf->GetLineStyle();
	style.SetColour( c );
	pdf->SetLineStyle( style );
}

/*****************************************************
**
**   PdfPainter   ---   getPenColor
**
******************************************************/
wxColour PdfPainter::getPenColor()
{
	return PDF_DEFAULT_FGCOLOR;
}

/*****************************************************
**
**   PdfPainter   ---   getDefaultPen
**
******************************************************/
wxPen PdfPainter::getDefaultPen()
{
	return wxPen( PDF_DEFAULT_FGCOLOR, 1, wxSOLID );
}

/*****************************************************
**
**   PdfPainter   ---   setDefaults
**
******************************************************/
void PdfPainter::setDefaults()
{
	setPen( getDefaultPen());
	setBrush( getDefaultBrush());
	pdf->SetTextColour( PDF_DEFAULT_FGCOLOR );
}

/*****************************************************
**
**   PdfPainter   ---  setTextColor
**
******************************************************/
void PdfPainter::setTextColor( const wxColour &c )
{
	pdf->SetTextColour( c );
}

/*****************************************************
**
**   PdfPainter   ---  getDefaultTextColor
**
******************************************************/
wxColour PdfPainter::getDefaultTextColor()
{
	return PDF_DEFAULT_FGCOLOR;
}

/*****************************************************
**
**   PdfPainter   ---  getTextExtent
**
******************************************************/
MPoint PdfPainter::getTextExtent( const wxString &s )
{
	const double w = pdf->GetStringWidth( s );
	const double h = pdf->GetFontSize();
	//printf( "PdfPainter::getTextExtent w %f h %f\n", w, h );
	return MPoint( w, h );
}

/*****************************************************
**
**   PdfPainter   ---   getTextZoomFactor
**
******************************************************/
double PdfPainter::getTextZoomFactor( const double &z )
{
	return PDF_TEXT_ZOOMFACTOR * z;
}

/*****************************************************
**
**   PdfPainter   ---  drawRectangle
**
******************************************************/
void PdfPainter::drawRectangle( const double &x, const double &y, const double &w, const double &h, const double rnd )
{
	//const double r = .3;
	const double r = 1.0;
	if ( rnd )
	{
		pdf->RoundedRect( x, y, w, h, r * rnd, wxPDF_CORNER_ALL, drawmode );
	}
	else
	{
		pdf->Rect( x, y, w, h, drawmode );
	}
}

/*****************************************************
**
**   PdfPainter   ---  drawRectangle
**
******************************************************/
void PdfPainter::drawRectangle( const wxRect &rect )
{
	drawRectangle( rect.x, rect.y, rect.width, rect.height );
}

/*****************************************************
**
**   PdfPainter   ---   drawPolygon
**
******************************************************/
void PdfPainter::drawPolygon(int n, MPoint points[], wxCoord xoffset, wxCoord yoffset )
{
	wxPdfArrayDouble x, y;
	for ( int i = 0; i < n; i++ )
	{
		x.Add( points[i].real() + xoffset );
		y.Add( points[i].imag() + yoffset );
	}
	pdf->Polygon( x, y, drawmode );
}

/*****************************************************
**
**   PdfPainter   ---  drawLine
**
******************************************************/
void PdfPainter::drawLine( const double &x1, const double &y1, const double &x2, const double &y2 )
{
	pdf->Line( x1, y1, x2, y2 );
}

/*****************************************************
**
**   PdfPainter   ---  drawBitmap
**
******************************************************/
void PdfPainter::drawBitmap( const wxBitmap &bmp, const double &x, const double &y, const bool& /*transparent*/ )
{
	pdf->Image( wxT( "image" ), bmp.ConvertToImage(), x, y );
}

/*****************************************************
**
**   PdfPainter   ---  drawTextFormatted
**
******************************************************/
void PdfPainter::drawTextFormatted( const MRect &r, const wxString &t, const int& align )
{
	double oldx = pdf->GetX();
	double oldy = pdf->GetY();
	double x1 = r.x;
	double y1 = r.y;

	MPoint p = getTextExtent( t );

	if ( align & Align::Top )
	{
		y1 = r.y + p.imag();
	}
	else if ( align & Align::Bottom )
	{
		// add a certain part of p.imag, otherwise text will be too deep e.g. in SBC
		y1 = r.y + r.height - .2 * p.imag();
	}
	else if ( align & Align::VCenter )
	{
		y1 = r.y + .5 * ( r.height + p.imag() );
	}
	if ( align & Align::HCenter )
	{
		x1 = r.x + .5 * ( r.width - p.real() );
	}
	else if ( align & Align::Right )
	{
		x1 = r.x + r.width - p.real();
	}
	pdf->Text( x1, y1, t );
	//drawEllipse( x1, y1, 2, 2 );
	pdf->SetXY( oldx, oldy );
}

/*****************************************************
**
**   PdfPainter   ---   drawEllipse
**
******************************************************/
void PdfPainter::drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax )
{
	pdf->Ellipse( x0 + xmax/2 , y0 + ymax/2, xmax/2, ymax/2 ); // Cannot be filled
}

/*****************************************************
**
**   PdfPainter   ---   drawArc
**
******************************************************/
void PdfPainter::drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
                          const double &w1, const double &w2 )
{
	pdf->Ellipse( x0 + xmax/2 , y0 + ymax/2, xmax/2, ymax/2, 0, w1, w2 );
}

/*****************************************************
**
**   PdfPainter   ---   drawPoint
**
******************************************************/
void PdfPainter::drawPoint( const double &x, const double &y )
{
	pdf->Line( x, y, x, y );
}

/*****************************************************
**
**   PdfPainter   ---   drawRotatedText
**
******************************************************/
void PdfPainter::drawRotatedText( wxString s, const double &x, const double &y, const double &alfa )
{
	MPoint p = getTextExtent( s );

	const double w = .5 * p.real();
	const double h = .5 * p.imag();

	// in pdf mode the anchor is located at the left sid of the text. y value is in the middle
	const double px = x - w * cos( alfa * DEG2RAD ) +  h * sin( alfa * DEG2RAD );
	const double py = y + w * sin( alfa * DEG2RAD ) + h * cos( alfa * DEG2RAD );

	/*
	const int rw = 1;
	setDefaultPen();
	//drawRectangle( x - rw, y - rw, 2 * rw, 2 * rw );
	drawRectangle( px - rw, py - rw, 2 * rw, 2 * rw );
	*/

	pdf->RotatedText( px, py, s, alfa );
}

/*****************************************************
**
**   PdfPainter   ---   setFont
**
******************************************************/
void PdfPainter::setFont( const wxFont &font )
{
	//printf( "PdfPainter::setFont size is %d\n", font.GetPointSize());
	currentFont = font;
	pdf->SetFont( font );
}

