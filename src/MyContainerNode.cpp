// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/log.h>
#endif

#include <cinttypes>
#include <cstdint>
#include <string>

#include <Windows.h>

#include "MyContainerNode.h"

#include "MemDbg.h"

MyContainerNode::MyContainerNode( const MyContainerNode* parent_i, std::string name_i ) :
    parent( parent_i ),
    name( name_i ),
    myMutex()
{
    std::string parentName;
    if ( parent_i != nullptr )
    {
        parentName.assign( parent->name );
    }
    //wxLogMessage( "0x%08" PRIX32 ",Create,0x%016" PRIXPTR ",\"%s\",\"%s\"", timeGetTime(), reinterpret_cast<uintptr_t>( this ), parentName.c_str(), name );
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
    wxMutexLocker lock( myMutex );
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

    wxMutexLocker lock( myMutex );
    for ( MyPictureNodeInterface* rawChild : children )
    {
        if ( rawChild->isContainer() )
        {
            MyContainerNode* child = static_cast<MyContainerNode*>( rawChild );
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

std::string MyContainerNode::getFinalPart() const
{
    int pos = name.find_last_of( wxFileName::GetPathSeparators() );
    std::string finalPart( name.substr( pos ) );
    return finalPart;
}

void MyContainerNode::Print( std::ostream& outstream, std::string prefix_i ) const
{
    outstream << prefix_i << name << std::endl;
    prefix_i.append( "," );
    wxMutexLocker lock( myMutex );
    for ( auto child : children )
    {
        child->Print( outstream, prefix_i );
    }
}
