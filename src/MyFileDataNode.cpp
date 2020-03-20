#include "MyFileDataNode.h"
#include <wx/debug.h>

MyFileDataNode::MyFileDataNode( const MyContainerNode* parent_i, std::string name_i ) :
    parent( parent_i ),
    name( name_i )
{
}

const MyContainerNode* MyFileDataNode::getParent() const
{
    return parent;
}

const MyPictureNodeInterface::MyPictureNodeInterfaceArray& MyFileDataNode::getChildren() const
{
    return children;
}

void MyFileDataNode::addChild( MyPictureNodeInterface* child_i )
{
    wxASSERT_MSG( false, "addChild called on FileDataNode element" );
}

std::string MyFileDataNode::getName() const
{
    return name;
}
