/*
 * MainWindow.h
 *
 *  Created on: Jan 2, 2020
 *      Author: joepi
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/dataview.h>
#include <wx/event.h>

#include <memory>
#include <string>

#include "MyPictureModel.h"

class MainWindow : public wxFrame
{
public:
    enum Control_IDs
    {
        ID_DirectoryName = wxID_HIGHEST + 1,
        ID_BrowseButton,
        ID_ProcessButton,
        ID_DirectoryTree,
        //wxEVT_ADD_DIRECTORY_EVENT,
        //wxEVT_ADD_FILE_EVENT,
        //wxEVT_ADD_DATA_EVENT
    };

    MainWindow( MyPictureModel* theModel );

    void AddDirectory( wxThreadEvent& event );
    void AddFile( wxThreadEvent& event );
    void AddData( wxThreadEvent& event );

private:

    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );
    void BrowseDirectory( wxCommandEvent& event );
    void ProcessButton_click( wxCommandEvent& event );
    void createDataViewControl( wxSizer* sizer_i );
    wxDataViewItem& FindTreeElement( const std::string& parent );
    DECLARE_EVENT_TABLE()

    MyPictureModel* model;
    wxTextCtrl* directoryBox;
    wxDataViewCtrl* dataTree;
};

wxDECLARE_EVENT( wxEVT_ADD_DIRECTORY_EVENT, wxThreadEvent );
wxDECLARE_EVENT( wxEVT_ADD_FILE_EVENT, wxThreadEvent );
wxDECLARE_EVENT( wxEVT_ADD_DATA_EVENT, wxThreadEvent );

#endif /* MAINWINDOW_H_ */
