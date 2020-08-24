#ifndef MyFileDataNode_H
#define MyFileDataNode_H

#include <string>
#include <ostream>

#include "MyPictureNodeInterface.h"
#include "MyContainerNode.h"

class MyFileDataNode :
    public MyPictureNodeInterface
{
public:
    MyFileDataNode( const MyContainerNode* parent_i, std::string name_i );
    ~MyFileDataNode() = default;

    virtual const MyContainerNode* getParent() const override;
    virtual const MyPictureNodeInterface::MyPictureNodeInterfaceArray& getChildren() const override;
    virtual void addChild( MyPictureNodeInterface* child_i ) override;
    virtual std::string getName() const override;
    virtual bool isContainer() const override { return false; }
    void Print( std::ostream& outstream, std::string prefix_i ) const override;

private:
    const MyContainerNode* parent;
    MyPictureNodeInterface::MyPictureNodeInterfaceArray children;
    const std::string name;
};

#endif // !MyFileDataNode_H
