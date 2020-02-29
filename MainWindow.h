/*
 * MainWindow.h
 *
 *  Created on: Jan 2, 2020
 *      Author: joepi
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/dataview.h>
#include <wx/event.h>
#include <string>
#include "ProcessingEngine.h"

wxDECLARE_EVENT( EVT_ADD_DIRECTORY_EVENT, wxThreadEvent );
wxDECLARE_EVENT( EVT_ADD_FILE_EVENT, wxThreadEvent );
wxDECLARE_EVENT( EVT_ADD_DATA_EVENT, wxThreadEvent );

class MainWindow : public wxFrame
{
public:
    enum Control_IDs
    {
        ID_DirectoryName,
        ID_BrowseButton,
        ID_ProcessButton,
        ID_DirectoryTree,
        ID_AddDirectory,
        ID_AddFile,
        ID_AddData
    };

    MainWindow( ProcessingEngine& theEngine );

    void AddDirectory( wxThreadEvent& event );
    void AddFile( wxThreadEvent& event );
    void AddData( wxThreadEvent& event );

private:

    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );
    void BrowseDirectory( wxCommandEvent& event );
    void ProcessButton_click( wxCommandEvent& event );
    wxDataViewItem& FindTreeElement( const std::string& parent );
    DECLARE_EVENT_TABLE()

    ProcessingEngine& engine;
    wxTextCtrl* directoryBox;
    wxDataViewTreeCtrl* dataTree;
    wxDataViewItem* rootItem;
};

#endif /* MAINWINDOW_H_ */
