/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/DataDialog.h
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

#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <wx/dialog.h>

// begin wxGlade: dependencies
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

class Document;
class MDateSpin;
class MDegSpin;

class wxButton;
class wxEvent;
class wxCheckBox;
class wxChoice;
class wxPanel;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

struct DataDialogModel;

/*************************************************//**
*
* \brief dialog for birth data
*
******************************************************/
class DataDialog: public wxDialog
{
	DECLARE_CLASS( DataDialog )

public:
    // begin wxGlade: DataDialog::ids
    // end wxGlade

	DataDialog(wxWindow* parent, Document* );
	~DataDialog();

private:
    // begin wxGlade: DataDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	Document *doc;
	DataDialogModel *model;

	void model2doc();
	void doc2model();
	void model2gui();
	void gui2model();

	void updateDateDetailFields( const bool isvalid = true );
	bool doApply();

	void OnSetCurrentDate( wxCommandEvent& );
	void OnDefaultLocation( wxCommandEvent& );
	void OnSearchLocation( wxCommandEvent& );
	void OnOK( wxCommandEvent& );
	void OnCancel( wxCommandEvent& );
	void OnApply( wxCommandEvent& ) { doApply(); }
	void OnLocalTime( wxCommandEvent& );
	void OnInputControlChanged( wxCommandEvent& );

protected:
    // begin wxGlade: DataDialog::attributes
    wxTextCtrl* text_name;
    wxStaticText* label_date;
    MDateSpin* text_date;
    wxStaticText* label_datefmt;
    wxStaticText* label_time;
    MDegSpin* text_time;
    wxStaticText* label_timefmt;
    wxStaticText* label_date_formatted;
    wxStaticText* label_calendar;
    wxPanel* panel_date_formatted;
    wxButton* button_now;
    wxStaticText* label_locname;
    wxTextCtrl* text_locname;
    wxStaticText* label_long;
    MDegSpin* text_longitude;
    wxStaticText* label_long_fmt;
    wxChoice* choice_ew;
    wxStaticText* label_lat;
    MDegSpin* text_latitude;
    wxStaticText* label_lat_fmt;
    wxChoice* choice_ns;
    wxButton* button_defloc;
    wxButton* button_searchloc;
    wxStaticText* label_tz;
    MDegSpin* text_tz;
    wxStaticText* label_tzfmt;
    wxChoice* choice_tzew;
    wxStaticText* label_dst;
    MDegSpin* text_dst;
    wxStaticText* label_dstfmt;
    wxButton* button_localtime;
    wxTextCtrl* text_remark;
    wxButton* button_ok;
    wxButton* button_apply;
    wxButton* button_cancel;
    // end wxGlade
};


#endif // DATADIALOG_H
