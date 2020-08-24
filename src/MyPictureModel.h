
#ifndef MyPictureModel_H
#define MyPictureModel_H

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <array>

#include <wx/dataview.h>
#include <wx/filename.h>
#include <wx/arrstr.h>

#include <string>

#include "MyPictureNodeInterface.h"

class MyContainerNode;
class ProcessingEngine;

class MyPictureModel :
    public wxDataViewModel
{
public:
	friend std::ostream& operator<<( std::ostream& outstream, const MyPictureModel& model_i );

	using PartListType = wxArrayString;
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
	void AddChildContainer( std::string child_i );
	void AddChild( std::string parent_i, std::string child_i );
	MyContainerNode* findElement( std::string name_i );
	MyContainerNode* findParent( wxFileName& child_i, MyContainerNode* parent_i );
	MyContainerNode* findParent( MyPictureModel::PartListType& pathParts, MyContainerNode* element_i );
	void Clear();

	void StartProcessing( std::string directory_i, bool fix_i );

	void Print( std::ostream& outstream ) const;
	
private:
	MyPictureNodeInterface* FindNode( const wxDataViewItem& item ) const;
	void notifyItemAdded( const wxDataViewItem& parent_i, const wxDataViewItem& child_i );
	ProcessingEngine* engine;
	MyContainerNode* root_node;
};

#endif // !MyPictureModel_H