#ifndef MyContainerNode_H
#define MyContainerNode_H

#include <string>
#include <ostream>

#include <wx/thread.h>
#include <wx/filename.h>

#include "MyPictureNodeInterface.h"

class MyContainerNode :
    public MyPictureNodeInterface
{
public:
    MyContainerNode( const MyContainerNode* parent_i, std::string name_i );
    ~MyContainerNode() = default;

    virtual const MyContainerNode* getParent() const override;
    virtual const MyPictureNodeInterface::MyPictureNodeInterfaceArray& getChildren() const override;
    virtual void addChild( MyPictureNodeInterface* child_i ) override;
    virtual std::string getName() const override;
    virtual bool isContainer() const override { return true; }
    MyContainerNode* getChild( std::string name_i );
    std::string getFinalPart() const;
    void Print( std::ostream& outstream, std::string prefix_i ) const override;
    wxMutex& getMutex() { return myMutex;  }

private:
    const MyContainerNode* parent;
    MyPictureNodeInterface::MyPictureNodeInterfaceArray children;
    const std::string name;
    mutable wxMutex myMutex;
};

#endif // !MyContainerNode_H
