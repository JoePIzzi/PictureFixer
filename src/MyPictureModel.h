
#ifndef MyPictureModel_H
#define MyPictureModel_H

#include <wx/dataview.h>

#include <string>

#include "MyPictureNodeInterface.h"

class MyContainerNode;

class MyPictureModel :
    public wxDataViewModel
{
public:
	MyPictureModel();
	MyPictureModel( const MyPictureModel& ) = default;
	~MyPictureModel();

	virtual unsigned int GetChildren( const wxDataViewItem& item, wxDataViewItemArray& children ) const override;
	virtual unsigned int GetColumnCount() const override;
	virtual wxString GetColumnType( unsigned int col ) const override;
	virtual wxDataViewItem GetParent( const wxDataViewItem& item ) const override;
	virtual void GetValue( wxVariant& variant, const wxDataViewItem& item, unsigned int col ) const override;
	virtual bool IsContainer( const wxDataViewItem& item ) const override;
	virtual bool SetValue( const wxVariant& variant, const wxDataViewItem& item, unsigned int col ) override;
	void AddChildContainer( std::string parent_i, std::string child_i );
	void AddChild( std::string parent_i, std::string child_i );
	MyContainerNode* findElement( std::string name_i );

	void StartProcessing( std::string directory_i );
	
private:
	MyPictureNodeInterface* FindNode( const wxDataViewItem& item ) const;
	MyContainerNode* root_node;
};

#endif // !MyPictureModel_H