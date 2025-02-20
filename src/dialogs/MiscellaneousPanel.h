/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/MiscellaneousPanel.h
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

#ifndef MISCELANEOUSPANEL_H
#define MISCELANEOUSPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class ViewConfig;

/*************************************************//**
*
* 
*
******************************************************/
class MiscellaneousPanel: public ConfigPanel
{
	DECLARE_CLASS( MiscellaneousPanel )

public:
    // begin wxGlade: MiscellaneousPanel::ids
    // end wxGlade

	MiscellaneousPanel( wxWindow* parent );
	~MiscellaneousPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: MiscellaneousPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	ViewConfig *view;

protected:
    // begin wxGlade: MiscellaneousPanel::attributes
    wxCheckBox* check_showstatusinfo;
    wxStaticText* label_recent;
    wxSpinCtrl* spin_recent_files;
    wxStaticText* label_file_backup;
    wxChoice* choice_file_backup;
    wxStaticText* label_frame_width;
    wxSpinCtrl* spin_frame_width;
    wxStaticText* label_log_target;
    wxChoice* choice_logging;
    wxStaticText* label_loglevel;
    wxChoice* choice_loglevel;
    wxStaticText* label_graphic_size_dialog;
    wxChoice* choice_graphic_size_dialog;
    wxCheckBox* check_overwrite;
    // end wxGlade
}; // wxGlade: end class


#endif // MISCELANEOUSPANEL_H

