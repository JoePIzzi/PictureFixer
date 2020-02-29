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
#endif

#include "MainWindow.h"
#include "ProcessingEngine.h"
#include <string>

wxDEFINE_EVENT( EVT_ADD_DIRECTORY_EVENT, wxThreadEvent );
wxDEFINE_EVENT( EVT_ADD_FILE_EVENT, wxThreadEvent );
wxDEFINE_EVENT( EVT_ADD_DATA_EVENT, wxThreadEvent );

BEGIN_EVENT_TABLE( MainWindow, wxFrame )
    EVT_MENU( wxID_ABOUT, MainWindow::OnAbout )
    EVT_MENU( wxID_EXIT, MainWindow::OnQuit )
    EVT_BUTTON( ID_BrowseButton, MainWindow::BrowseDirectory )
    EVT_BUTTON( ID_ProcessButton, MainWindow::ProcessButton_click )
    EVT_COMMAND( ID_AddDirectory, EVT_ADD_DIRECTORY_EVENT, MainWindow::AddDirectory )
    EVT_COMMAND( ID_AddFile, EVT_ADD_FILE_EVENT, MainWindow::AddFile )
    EVT_COMMAND( ID_AddData, EVT_ADD_DATA_EVENT, MainWindow::AddData )
END_EVENT_TABLE()

MainWindow::MainWindow( ProcessingEngine& theEngine ) :
    wxFrame( nullptr, wxID_ANY, std::string( "PictureFixer Application" ) ),
    engine( theEngine )
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
    SetStatusText( "Welcome to wxWidgets!" );

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer( wxVERTICAL );
    SetSizer( itemBoxSizer8 );

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer( wxHORIZONTAL );
    itemBoxSizer8->Add( itemBoxSizer9, 0, wxALIGN_TOP | wxEXPAND, 0 );

    directoryBox = new wxTextCtrl( this, ID_DirectoryName, "U:/Users/pizzijw/Pictures/Camera" );
    itemBoxSizer9->Add( directoryBox, 1, wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 5 );

    wxButton* itemButton11 = new wxButton( this, ID_BrowseButton, "...", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer9->Add( itemButton11, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );

    wxButton* itemButton12 = new wxButton( this, ID_ProcessButton, "Process", wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add( itemButton12, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );

    dataTree = new wxDataViewTreeCtrl( this, ID_DirectoryTree, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
    itemBoxSizer8->Add( dataTree, 1, wxALIGN_CENTER_HORIZONTAL | wxALL | wxEXPAND, 5 );
    rootItem = new wxDataViewItem();
    dataTree->AppendContainer( *rootItem, "Camera" );
}

void MainWindow::AddDirectory( wxThreadEvent& event )
{
    std::pair<std::string, std::string> data = *static_cast<std::pair<std::string, std::string>*>( event.GetPayload() );
    std::string parent = data.first;
    std::string directory = data.second;
    wxDataViewItem& parentViewItem = FindTreeElement( parent );
    dataTree->AppendContainer( parentViewItem, directory );
    Update();
}

void MainWindow::AddFile( wxThreadEvent& event )
{
    std::pair<std::string, std::string> data = *static_cast<std::pair<std::string, std::string>*>( event.GetPayload() );
    std::string parent = data.first;
    std::string file = data.second;
    wxDataViewItem& parentViewItem = FindTreeElement( parent );
    dataTree->AppendItem( parentViewItem, file );
    Update();
}

void MainWindow::AddData( wxThreadEvent& event )
{
    std::pair<std::string, std::string> eventData = *static_cast<std::pair<std::string, std::string>*>( event.GetPayload() );
    std::string parent = eventData.first;
    std::string data = eventData.second;
    wxDataViewItem& parentViewItem = FindTreeElement( parent );
    dataTree->SetItemData( parentViewItem, reinterpret_cast<wxClientData*>(&data) );
    Update();
}

void MainWindow::OnQuit( wxCommandEvent &event )
{
    Close();
}

void MainWindow::OnAbout( wxCommandEvent &event )
{
    std::string msg( "Hello and welcome to " );
    msg.append( "wxWidgets 3.0.4" );

    wxMessageBox( msg, "About Picture Fixer App", wxOK | wxICON_INFORMATION, this );
}

void MainWindow::BrowseDirectory( wxCommandEvent & event )
{
    std::string defaultDir( "U:/Users/pizzijw/Pictures/Camera" );
    wxDirDialog directoryDialog( this, "Select Directory to Process...", defaultDir, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST );
    directoryDialog.ShowModal();
    directoryBox->SetLabelText( directoryDialog.GetPath() );
}

void MainWindow::ProcessButton_click( wxCommandEvent & event )
{
    engine.ProcessDir( "Camera", directoryBox->GetLabelText() );
}

wxDataViewItem& MainWindow::FindTreeElement( const std::string& parent )
{
    return *rootItem;
}
