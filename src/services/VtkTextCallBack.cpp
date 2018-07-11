#include "VtkTextCallBack.h"


TextCallback::TextCallback()
{
	//;
}

TextCallback::~TextCallback()
{
	//;
}




void TextCallback::SetActor(vtkSmartPointer<vtkActor> actor)
{
	m_actor = actor;
}

void TextCallback::Execute(vtkObject *caller, unsigned long event, void*)
{
	cout << "dfafsdfsfsdf" << std::endl;
	switch (event)
	{
	case vtkCommand::StartInteractionEvent:
		{
		   cout << "dfafsdfsfsdf" << std::endl;
		break;
		}
	case vtkCommand::InteractionEvent:
	{
		cout << "dfafsdfsfsdf" << std::endl;
		break;
	}
	case vtkCommand::EndInteractionEvent:
	{
		cout << "dfafsdfsfsdf" << std::endl;
		break;
	}
	case vtkCommand::MouseWheelForwardEvent:
	{
		cout << "dfafsdfsfsdf" << std::endl;
		break;
	}
	case vtkCommand::MouseWheelBackwardEvent:
	{
		cout << "dfafsdfsfsdf" << std::endl;
		break;
	}
	case vtkCommand::KeyPressEvent:
	{
		cout << "dfafsdfsfsdf" << std::endl;
		break;
	}
	default:
	{
		//;
	}
	}
}
