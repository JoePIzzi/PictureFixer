/*
 * MainWindow.cpp
 *
 *  Created on: Jan 2, 2020
 *      Author: joepi
 */

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/stdpaths.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/utils.h>
#endif

#include "MainWindow.h"
#include "PictureFixerApp.h"
#include "ProcessingEngine.h"
#include "ThreadInfo.h"
#include "MyPictureModel.h"

#include <string>
#include <algorithm>
#include "FileProcessor.h"

#include "MemDbg.h"

//wxDEFINE_EVENT( wxEVT_ADD_DIRECTORY_EVENT, wxThreadEvent );
//wxDEFINE_EVENT( wxEVT_ADD_FILE_EVENT, wxThreadEvent );
//wxDEFINE_EVENT( wxEVT_ADD_DATA_EVENT, wxThreadEvent );

BEGIN_EVENT_TABLE( MainWindow, wxFrame )
    EVT_MENU( wxID_ABOUT, MainWindow::OnAbout )
    EVT_MENU( wxID_EXIT, MainWindow::OnQuit )
    EVT_BUTTON( ID_DirectoryBrowseButton, MainWindow::BrowseDirectory )
    EVT_BUTTON( ID_DirectoryProcessButton, MainWindow::ReportIssues_click )
    EVT_BUTTON( ID_FileProcessButton, MainWindow::FixIsuses_click )
    EVT_DATAVIEW_ITEM_CONTEXT_MENU( ID_DirectoryTree, MainWindow::ClearTree )
    //EVT_THREAD( wxEVT_ADD_DIRECTORY_EVENT, MainWindow::AddDirectory )
    //EVT_THREAD( wxEVT_ADD_FILE_EVENT, MainWindow::AddFile )
    //EVT_THREAD( wxEVT_ADD_DATA_EVENT, MainWindow::AddData )
END_EVENT_TABLE()

MainWindow::MainWindow( MyPictureModel* theModel ) :
    wxFrame( nullptr, wxID_ANY, std::string( "PictureFixer Application" ) ),
    model( theModel ),
    fix( false )
{
//    SetIcon( wxIcon( mondrain_xpm ) );
    SetTitle( std::string( "Picture Fixer Application" ) );
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append( wxID_EXIT, "E&xit\tAlt-X", "Quit this program." );

    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append( wxID_ABOUT, "&About...\tF1", "Show about dialog." );

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append( fileMenu, "&File" );
    menuBar->Append( helpMenu, "&Help" );

    SetMenuBar( menuBar );

    CreateStatusBar( 2 );

    wxTextCtrl* logWindow = new wxTextCtrl( this, wxID_ANY, wxString(), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    logWindow->SetMinSize( wxSize( 1000, 600 ) );
    oldLog = wxLog::SetActiveTarget( new wxLogTextCtrl( logWindow ) );
    wxLogMessage( "This is the log window." );

    wxPanel* mainPanel = new wxPanel( this, wxID_ANY );
 
    wxBoxSizer* directoryBoxSizer = new wxBoxSizer( wxHORIZONTAL );

    directoryBox = new wxTextCtrl( mainPanel, ID_DirectoryName, "U:\\Users\\pizzijw\\Pictures\\Camera",
        wxDefaultPosition, wxDefaultSize );
    directoryBoxSizer->Add( directoryBox, 1, wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 5 );

    wxButton* directoryBrowseButton = new wxButton( mainPanel, ID_DirectoryBrowseButton, "...", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    directoryBoxSizer->Add( directoryBrowseButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );

    directoryProcessingButton = new wxButton( mainPanel, ID_DirectoryProcessButton, "Report Issues", wxDefaultPosition, wxDefaultSize, 0 );
    directoryBoxSizer->Add( directoryProcessingButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );

    fileProcessingButton = new wxButton( mainPanel, ID_FileProcessButton, "Fix Issues", wxDefaultPosition, wxDefaultSize, 0 );
    directoryBoxSizer->Add( fileProcessingButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );

    //CreateDataViewControl( mainPanel );

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer( wxVERTICAL );
    itemBoxSizer8->Add( directoryBoxSizer, 0, wxALIGN_TOP | wxEXPAND, 0 );
    //itemBoxSizer8->Add( dataTree, 1, wxGROW | wxALL, 5 );
    mainPanel->SetSizerAndFit( itemBoxSizer8 );

    wxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
    mainSizer->Add( mainPanel, 0, wxGROW );
    mainSizer->Add( logWindow, 1, wxGROW );
    SetSizerAndFit( mainSizer );
}

void MainWindow::OnQuit( wxCommandEvent& event )
{
    {
        wxCriticalSectionLocker locker( wxGetApp().criticalSection );
        wxGetApp().postShutdown();
    }
    Close();
}

void MainWindow::OnAbout( wxCommandEvent& event )
{
    std::string msg( "Hello and welcome to " );
    msg.append( "wxWidgets 3.0.4" );

    wxMessageBox( msg, "About Picture Fixer App", wxOK | wxICON_INFORMATION, this );
}

void MainWindow::BrowseDirectory( wxCommandEvent& event )
{
    std::string defaultDir( "U:/Users/pizzijw/Pictures/Camera" );
    wxDirDialog directoryDialog( this, "Select Directory to Process...", defaultDir, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST );
    int result = directoryDialog.ShowModal();
    if ( result == wxID_OK )
    {
        directoryBox->SetLabelText( directoryDialog.GetPath() );
    }
}

void MainWindow::ReportIssues_click( wxCommandEvent& event )
{
    fix = false;
    StartProcessing();
}

void MainWindow::StartProcessing()
{
    model->Clear();
    model->StartProcessing( directoryBox->GetLabelText(), fix );
    directoryProcessingButton->Disable();
    fileProcessingButton->Disable();
    wxBeginBusyCursor();
}

void MainWindow::FixIsuses_click( wxCommandEvent& event )
{
    fix = true;
    StartProcessing();
}

void MainWindow::CreateDataViewControl( wxPanel* parent_i )
{
    dataTree = new wxDataViewCtrl( parent_i, ID_DirectoryTree, wxDefaultPosition, wxDefaultSize );
    dataTree->AssociateModel( model );
    dataTree->SetMinSize( wxSize( 1000, 600 ) );
    //model->DecRef();
    wxDataViewColumn* currentColumn = new wxDataViewColumn( "Entry", new wxDataViewTextRenderer, 0, 1000, wxALIGN_LEFT,
        wxDATAVIEW_COL_RESIZABLE );
    dataTree->AppendColumn( currentColumn );
}

void MainWindow::ClearTree( wxDataViewEvent& event )
{
    wxMessageDialog confirm( this, "Clear window?", "Confirm", wxOK | wxCANCEL | wxOK_DEFAULT );
    if ( confirm.ShowModal() == wxID_OK )
    {
        //wxLog::SetActiveTarget( oldLog );
        //logFile->close();
        //delete logFile;
        dataTree->Freeze();
        model->Clear();
        dataTree->Thaw();
    }
}

void MainWindow::AddItem( wxThreadEvent& event )
{
    ThreadInfo* data = event.GetPayload<ThreadInfo*>();
    dataTree->GetModel()->ItemAdded( data->parent, data->entry );
    //dataTree->Expand( data->parent );
}

void MainWindow::processingDone()
{
    wxEndBusyCursor();
    directoryProcessingButton->Enable();
    fileProcessingButton->Enable();
    if ( !IsActive() )
    {
        RequestUserAttention();
    }
    if ( !fix )
    {
        std::string dirName = directoryBox->GetLabelText();
        int lastSeparator = dirName.find_last_of( '\\' );
        dirName.erase( 0, lastSeparator + 1 );
        std::string LogFilename( "../../LogFile_" );
        LogFilename.append( dirName );
        LogFilename.append( ".csv" );
        std::ofstream logFile( LogFilename, std::ios::out );
        model->Print( logFile );
    }    
    wxBell();
}
