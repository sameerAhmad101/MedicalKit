#pragma once
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>


namespace service{
	class TextCallback;
}

class TextCallback : public vtkCommand
{
public:
	TextCallback();
	~TextCallback();
	static TextCallback *New()
	{
		return new TextCallback;
	}



	virtual void Execute(vtkObject *caller, unsigned long event, void*);

	vtkSmartPointer<vtkActor> m_actor;

	void SetActor(vtkSmartPointer<vtkActor> actor);
};