#ifndef MyContainerNode_H
#define MyContainerNode_H

#include <string>

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

private:
    const MyContainerNode* parent;
    MyPictureNodeInterface::MyPictureNodeInterfaceArray children;
    const std::string name;
};

#endif // !MyContainerNode_H
