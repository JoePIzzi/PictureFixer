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

MyPictureModel::MyPictureModel() :
	root_node( new MyContainerNode( nullptr, "" ) )
{
}

MyPictureModel::~MyPictureModel()
{
	delete root_node;
}

unsigned int MyPictureModel::GetChildren( const wxDataViewItem& item, wxDataViewItemArray& children ) const
{
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
	return 2;
}

wxString MyPictureModel::GetColumnType( unsigned int col ) const
{
	switch ( col )
	{
	case 0:
		return wxString( "std::string" );

	case 1:
		return wxString( "unsigned integer" );
	}
	
	return wxString();
}

wxDataViewItem MyPictureModel::GetParent( const wxDataViewItem& item ) const
{
	MyPictureNodeInterface* node = FindNode( item );
	const MyContainerNode* parent = static_cast<const MyContainerNode*>(node->getParent());
	return wxDataViewItem( const_cast<MyContainerNode*>(parent) );
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

void MyPictureModel::AddChildContainer( std::string parent_i, std::string child_i )
{
	MyContainerNode* parent_element = nullptr;
	if ( parent_i.length() == 0 )
	{
		parent_element = root_node;
	}
	else
	{
		parent_element = findElement( parent_i );
	}
	wxASSERT_MSG( parent_element != nullptr, "Could not find parent to add child to." );
	MyContainerNode* child_node = new MyContainerNode( parent_element, child_i );
	parent_element->addChild( child_node );
	wxDataViewItem child_item( child_node );
	wxDataViewItem parent_item( parent_element );
	ItemAdded( parent_item, child_item );
}

void MyPictureModel::AddChild( std::string parent_i, std::string child_i )
{
	MyContainerNode* parent_element = findElement( parent_i );
	wxASSERT_MSG( parent_element != nullptr, "Could not find parent to add child to." );
	MyFileDataNode* child_node = new MyFileDataNode( parent_element, child_i );
	parent_element->addChild( child_node );
	wxDataViewItem child_item( child_node );
	wxDataViewItem parent_item( parent_element );
	ItemAdded( parent_item, child_item );
}

void MyPictureModel::StartProcessing( std::string directory_i )
{
	ProcessingEngine* engine = new ProcessingEngine( this );
	engine->StartProcessing( directory_i );
}

MyPictureNodeInterface* MyPictureModel::FindNode( const wxDataViewItem& item ) const
{
	if ( !item.IsOk() )
	{
		return root_node;
	}
	return static_cast<MyPictureNodeInterface*>(item.GetID());
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
				MyContainerNode* childContainer = static_cast<MyContainerNode*>(child);
				if ( childContainer->getName() == name_i )
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
