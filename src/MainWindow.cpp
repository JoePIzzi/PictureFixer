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
#include "ThreadInfo.h"
#include "MyPictureModel.h"
#include <string>

wxDEFINE_EVENT( wxEVT_ADD_DIRECTORY_EVENT, wxThreadEvent );
wxDEFINE_EVENT( wxEVT_ADD_FILE_EVENT, wxThreadEvent );
wxDEFINE_EVENT( wxEVT_ADD_DATA_EVENT, wxThreadEvent );

BEGIN_EVENT_TABLE( MainWindow, wxFrame )
    EVT_MENU( wxID_ABOUT, MainWindow::OnAbout )
    EVT_MENU( wxID_EXIT, MainWindow::OnQuit )
    EVT_BUTTON( ID_BrowseButton, MainWindow::BrowseDirectory )
    EVT_BUTTON( ID_ProcessButton, MainWindow::ProcessButton_click )
    //EVT_THREAD( wxEVT_ADD_DIRECTORY_EVENT, MainWindow::AddDirectory )
    //EVT_THREAD( wxEVT_ADD_FILE_EVENT, MainWindow::AddFile )
    //EVT_THREAD( wxEVT_ADD_DATA_EVENT, MainWindow::AddData )
END_EVENT_TABLE()

MainWindow::MainWindow( MyPictureModel* theModel ) :
    wxFrame( nullptr, wxID_ANY, std::string( "PictureFixer Application" ) ),
    model( theModel )
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

    directoryBox = new wxTextCtrl( this, ID_DirectoryName, "U:\\Users\\pizzijw\\Pictures\\Camera" );
    itemBoxSizer9->Add( directoryBox, 1, wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 5 );

    wxButton* itemButton11 = new wxButton( this, ID_BrowseButton, "...", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer9->Add( itemButton11, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );

    wxButton* itemButton12 = new wxButton( this, ID_ProcessButton, "Process", wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add( itemButton12, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );

    createDataViewControl( itemBoxSizer8 );
}
//
//void MainWindow::AddDirectory( wxThreadEvent& event )
//{
//    ThreadInfo* data = event.GetPayload<ThreadInfo*>();
//    dynamic_cast<MyPictureModel*>(dataTree->GetModel())->AddChild( data->parent, data->entry );
//    Update();
//    delete data;
//}
//
//void MainWindow::AddFile( wxThreadEvent& event )
//{
//    ThreadInfo* data = event.GetPayload<ThreadInfo*>();
//    dynamic_cast<MyPictureModel*>( dataTree->GetModel() )->AddChild( data->parent, data->entry );
//    Update();
//    delete data;
//}
//
//void MainWindow::AddData( wxThreadEvent& event )
//{
//    ThreadInfo* data = event.GetPayload<ThreadInfo*>();
//    dynamic_cast<MyPictureModel*>( dataTree->GetModel() )->AddChild( data->parent, data->entry );
//    Update();
//    delete data;
//}

void MainWindow::OnQuit( wxCommandEvent& event )
{
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
    directoryDialog.ShowModal();
    directoryBox->SetLabelText( directoryDialog.GetPath() );
}

void MainWindow::ProcessButton_click( wxCommandEvent& event )
{
    model->StartProcessing( directoryBox->GetLabelText() );
}

void MainWindow::createDataViewControl( wxSizer* sizer_i )
{
    dataTree = new wxDataViewCtrl( this, ID_DirectoryTree, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
    sizer_i->Add( dataTree, 1, wxALIGN_CENTER_HORIZONTAL | wxALL | wxEXPAND, 5 );
    dataTree->AssociateModel( model );
    model->DecRef();
    //for ( unsigned int columnNumber = 0; columnNumber < model->GetColumnCount(); ++columnNumber )
    //{
    //    wxDataViewRenderer* renderer = 0;
    //    std::string columnName( "" );
    //    switch ( columnNumber )
    //    {
    //    case 0:
    //        columnName.assign( "Tag" ); 
    //        break;

    //    case 1:
    //        columnName.assign( "Position" );
    //        break;
    //    }
    //    if ( ( model->GetColumnType( columnNumber ) == "std::string" ) ||
    //        ( model->GetColumnType( columnNumber ) == "unsigned integer" ) )
    //    {
    //        renderer = new wxDataViewTextRenderer();
    //    }
    //    else
    //    {
    //        wxASSERT_MSG( false, "Need to implement code for column type!" );
    //    }
    //    dataTree->AppendColumn( new wxDataViewColumn( columnName, renderer, columnNumber ) );
    //}
}