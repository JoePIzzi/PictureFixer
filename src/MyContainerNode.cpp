#include "MyContainerNode.h"

MyContainerNode::MyContainerNode( const MyContainerNode* parent_i, std::string name_i ) :
    parent( parent_i ),
    name( name_i )
{
}

const MyContainerNode* MyContainerNode::getParent() const
{
    return parent;
}

const MyPictureNodeInterface::MyPictureNodeInterfaceArray& MyContainerNode::getChildren() const
{
    return children;
}

void MyContainerNode::addChild( MyPictureNodeInterface* child_i )
{
    children.push_back( child_i );
}

std::string MyContainerNode::getName() const
{
    return name;
}

MyContainerNode* MyContainerNode::getChild( std::string name_i )
{
    MyContainerNode* element = nullptr;
    if ( name == name_i )
    {
        return this;
    }

	for ( MyPictureNodeInterface* rawChild : children )
	{
        if ( rawChild->isContainer() )
        {
            MyContainerNode* child = static_cast<MyContainerNode*>(rawChild);
            if ( child->getName() == name_i )
            {
                element = child;
                break;
            }
            element = child->getChild( name_i );
            if ( element != nullptr )
            {
                break;
            }

        }
    }
    return element;
}
