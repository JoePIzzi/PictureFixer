/*
 * MainWindow.h
 *
 *  Created on: Jan 2, 2020
 *      Author: joepi
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/dataview.h>
#include <wx/event.h>

#include <memory>
#include <string>
#include <fstream>

#include "MyPictureModel.h"

class MainWindow : public wxFrame
{
public:
    enum Control_IDs
    {
        ID_DirectoryName = wxID_HIGHEST + 1,
        ID_DirectoryBrowseButton,
        ID_DirectoryProcessButton,
        ID_DirectoryTree,
        ID_FileProcessButton,
        ID_FileName
    };

    MainWindow( MyPictureModel* theModel );

    void AddDirectory( wxThreadEvent& event );
    void AddFile( wxThreadEvent& event );
    void AddData( wxThreadEvent& event );
    void AddItem( wxThreadEvent& event );
    void processingDone();

private:

    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );
    void BrowseDirectory( wxCommandEvent& event );
    void ReportIssues_click( wxCommandEvent& event );
    void StartProcessing();
    void FixIsuses_click( wxCommandEvent& event );
    void CreateDataViewControl( wxPanel* parent_i );
    void ClearTree( wxDataViewEvent& event );
    wxDataViewItem& FindTreeElement( const std::string& parent );
    DECLARE_EVENT_TABLE()

    MyPictureModel* model;
    wxTextCtrl* directoryBox;
    bool fix;
    wxButton* directoryProcessingButton;
    wxButton* fileProcessingButton;
    wxDataViewCtrl* dataTree;
    wxLog* oldLog;
    std::ofstream* logFile;
};

#endif /* MAINWINDOW_H_ */
