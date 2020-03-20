#ifndef MyPictureNodeInterface_H
#define MyPictureNodeInterface_H

#include <vector>
#include <string>

class MyPictureNodeInterface
{
public:
	typedef std::vector<MyPictureNodeInterface*> MyPictureNodeInterfaceArray;

	virtual const MyPictureNodeInterface* getParent() const = 0;
	virtual const MyPictureNodeInterfaceArray& getChildren() const = 0;
	virtual void addChild( MyPictureNodeInterface* child_i ) = 0;
	virtual std::string getName() const = 0;
	virtual bool isContainer() const = 0;

private:

};

#endif // !MyPictureNodeInterface_H
