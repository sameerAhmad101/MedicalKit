#pragma once
#include <vtkSmartPointer.h>
#include <vtkTextWidget.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTextRepresentation.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVectorText.h>
#include <vtkPolyDataMapper.h>
#include <vtkFollower.h>
#include <vtkCommand.h>
#include <vtkLinearExtrusionFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkDataSetMapper.h>

#include <array>
#include<map>
#include <QObject>

// My includes
#include "VtkTextCallBack.h"


namespace service
{
	class TextLabelService;
}

class TextLabelService{
public:
	TextLabelService(vtkSmartPointer<vtkRenderWindow> renderWindow);

	~TextLabelService();
private:
	vtkSmartPointer<vtkRenderWindow> m_renderWindow;
	vtkSmartPointer<vtkTextWidget> m_textLabel;
	vtkSmartPointer<vtkTextActor> m_textActor;
	vtkSmartPointer<vtkTextRepresentation> m_textRepresentation;

	vtkSmartPointer<vtkVectorText> m_textSource;
	vtkSmartPointer<vtkPolyDataMapper> m_textMapper;
	vtkSmartPointer<vtkFollower> m_cameraFollower;// makes the text faces camera.
	vtkSmartPointer<vtkActor> m_actor;
	vtkSmartPointer<TextCallback> m_textCallback;

	std::string m_text = "text";
	std::array<double, 3> m_textColor = {.1, .0, .0};
	std::array<double, 3> m_textPosition = {.0, .0, .0};
	//static std:map<vtkSmartPointer<vtkActor>> m_labels;
protected:
};
