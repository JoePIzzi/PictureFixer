// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "MyPictureModel.h"
#include "MyContainerNode.h"
#include "MyFileDataNode.h"
#include "ProcessingEngine.h"
#include "ThreadInfo.h"
#include "PictureFixerApp.h"
#include "MainWindow.h"

#include "MemDbg.h"

MyPictureModel::MyPictureModel() :
    engine( 0 ),
    root_node( new MyContainerNode( nullptr, "Camera" ) )
{
}

MyPictureModel::~MyPictureModel()
{
    delete engine;
    delete root_node;
}

unsigned int MyPictureModel::GetChildren( const wxDataViewItem& item, wxDataViewItemArray& children ) const
{
    if ( !item.IsOk() )
    {
        children.Add( wxDataViewItem( root_node ) );
        return 1;
    }
    MyPictureNodeInterface* node = FindNode( item );

    if ( !node->isContainer() )
    {
        return 0;
    }

    for ( auto iter : node->getChildren() )
    {
        children.Add( wxDataViewItem( iter ) );
    }
    return children.size();
}

unsigned int MyPictureModel::GetColumnCount() const
{
    return 1;
}

wxString MyPictureModel::GetColumnType( unsigned int col ) const
{
    return wxString( "std::string" );
}

wxDataViewItem MyPictureModel::GetParent( const wxDataViewItem& item ) const
{
    MyPictureNodeInterface* node = FindNode( item );
    const MyContainerNode* parent = static_cast<const MyContainerNode*>( node->getParent() );
    return wxDataViewItem( const_cast<MyContainerNode*>( parent ) );
}

void MyPictureModel::GetValue( wxVariant& variant, const wxDataViewItem& item, unsigned int col ) const
{
    variant = static_cast<const MyPictureNodeInterface*>( item.GetID() )->getName();
}

bool MyPictureModel::IsContainer( const wxDataViewItem& item ) const
{
    MyPictureNodeInterface* node = FindNode( item );

    return node->isContainer();
}

bool MyPictureModel::SetValue( const wxVariant& variant, const wxDataViewItem& item, unsigned int col )
{
    return false;
}

void MyPictureModel::AddChildContainer( std::string child_i )
{
    MyContainerNode* parentElement = nullptr;
    wxFileName childFile( child_i );
    parentElement = findParent( childFile, root_node );
    wxASSERT_MSG( parentElement != nullptr, "Could not find parent to add child to." );
    MyContainerNode* childNode = new MyContainerNode( parentElement, childFile.GetFullName() );
    parentElement->addChild( childNode );
    //wxDataViewItem childItem( childNode );
    //wxDataViewItem parentItem( parentElement );
    //ItemAdded( parentItem, childItem );
    //notifyItemAdded( parentItem, childItem );
}

void MyPictureModel::AddChild( std::string parent_i, std::string child_i )
{
    wxFileName parentFile( parent_i );
    MyContainerNode* grandParentElement = findParent( parentFile, root_node );
    MyContainerNode* parentElement = grandParentElement->getChild( parentFile.GetFullName() );
    wxASSERT_MSG( parentElement != nullptr, "Could not find parent to add child to." );
    MyFileDataNode* childNode = new MyFileDataNode( parentElement, child_i );
    parentElement->addChild( childNode );
    //wxDataViewItem childItem( childNode );
    //wxDataViewItem parentItem( parentElement );
    //ItemAdded( parentItem, childItem );
    //notifyItemAdded( parentItem, childItem );
}

void MyPictureModel::StartProcessing( std::string directory_i, bool fix_i )
{
    ProcessingEngine* engine = new ProcessingEngine( this );
    engine->StartProcessing( directory_i, fix_i );
}

void MyPictureModel::Print( std::ostream& outstream ) const
{
    std::string empty;
    root_node->Print( outstream, empty );
}

MyPictureNodeInterface* MyPictureModel::FindNode( const wxDataViewItem& item ) const
{
    if ( !item.IsOk() )
    {
        return root_node;
    }
    return static_cast<MyPictureNodeInterface*>( item.GetID() );
}

MyContainerNode* MyPictureModel::findElement( std::string name_i )
{
    MyContainerNode* element = root_node;
    if ( element->getName() == name_i )
    {
        return element;
    }

    if ( element->isContainer() )
    {
        for ( MyPictureNodeInterface* child : root_node->getChildren() )
        {
            if ( child->isContainer() )
            {
                MyContainerNode* childContainer = static_cast<MyContainerNode*>( child );
                if ( childContainer->getFinalPart() == name_i )
                {
                    element = childContainer;
                    break;
                }
                childContainer = childContainer->getChild( name_i );
                if ( childContainer != nullptr )
                {
                    element = childContainer;
                    break;
                }
            }
        }
    }
    return element;
}

MyContainerNode* MyPictureModel::findParent( wxFileName& child_i, MyContainerNode* parent_i )
{
    PartListType pathParts = child_i.GetDirs();
    if ( ( !pathParts.IsEmpty() ) && ( pathParts[0] == "Users" ) )
    {
        pathParts.RemoveAt( 0 ); // remove Users
        if ( ( !pathParts.IsEmpty() ) && ( pathParts[0] == "pizzijw" ) )
        {
            pathParts.RemoveAt( 0 ); // remove pizzijw
            if ( ( !pathParts.IsEmpty() ) && ( pathParts[0] == "Pictures" ) )
            {
                pathParts.RemoveAt( 0 ); // remove Pictures
                if ( ( !pathParts.IsEmpty() ) && ( pathParts[0] == "Camera" ) )
                {
                    pathParts.RemoveAt( 0 ); // remove Camera
                }
            }
        }
    }
    MyContainerNode* element = findParent( pathParts, parent_i );
    return element;
}

MyContainerNode* MyPictureModel::findParent( MyPictureModel::PartListType& pathParts, MyContainerNode* element_i )
{
    MyContainerNode* element = element_i;
    if ( !pathParts.IsEmpty() )
    {
        std::string currentPart = pathParts.front();
        pathParts.RemoveAt( 0 );
        bool found = false;
        
        {
            wxMutexLocker lock( element->getMutex() );
            for ( MyPictureNodeInterface* child : element->getChildren() )
            {
                if ( child->isContainer() )
                {
                    if ( child->getName() == currentPart )
                    {
                        MyContainerNode* childContainer = static_cast<MyContainerNode*>( child );
                        element = findParent( pathParts, childContainer );
                        found = true;
                        break;
                    }
                }
            }
        }
        if ( !found )
        {
            MyContainerNode* newChild = new MyContainerNode( element, currentPart );
            element->addChild( newChild );
            element = newChild;
            while ( !pathParts.IsEmpty() )
            {
                currentPart = pathParts[0];
                pathParts.RemoveAt( 0 );
                newChild = new MyContainerNode( element, currentPart );
                element->addChild( newChild );
                element = newChild;
            };
        }
    }
    return element;
}

void MyPictureModel::Clear()
{
    delete root_node;
    root_node = new MyContainerNode( nullptr, "Camera" );
    Cleared();
}

void MyPictureModel::notifyItemAdded( const wxDataViewItem& parent_i, const wxDataViewItem& child_i )
{
    wxThreadEvent myContainerEvent( wxEVT_THREAD, wxEVT_ITEM_ADDED_EVENT );
    ThreadInfo* info = new ThreadInfo( parent_i, child_i );
    myContainerEvent.SetPayload( info );
    wxGetApp().QueueEvent( myContainerEvent.Clone() );
}
