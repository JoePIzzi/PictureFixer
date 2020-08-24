// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/log.h>
#include <wx/debug.h>
#endif

#include <Windows.h>

#include <cinttypes>
#include <cstdint>
#include "MyFileDataNode.h"

MyFileDataNode::MyFileDataNode( const MyContainerNode* parent_i, std::string name_i ) :
    parent( parent_i ),
    name( name_i )
{
    //wxLogMessage( "0x%08" PRIX32 ",Create,0x%016" PRIxPTR ",\"%s\",\"%s\"", timeGetTime(), reinterpret_cast<uintptr_t>( this ), parent->getName(), name );
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

void MyFileDataNode::Print( std::ostream& outstream, std::string prefix_i ) const
{
    outstream << prefix_i << name << std::endl;
}
