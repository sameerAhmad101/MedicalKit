#include "TextLabelService.h"


TextLabelService::TextLabelService(vtkSmartPointer<vtkRenderWindow> renderWindow)
{
        m_renderWindow = renderWindow;
		m_textActor = vtkSmartPointer<vtkTextActor>::New();
		m_textActor->SetInput("This is a test");
		m_textActor->GetTextProperty()->SetColor(0.0, 1.0, 0.0);
		m_textLabel = vtkSmartPointer<vtkTextWidget>::New();
		m_textRepresentation = vtkSmartPointer<vtkTextRepresentation>::New();
		m_textRepresentation->GetPositionCoordinate()->SetValue(.15, .15);
		m_textRepresentation->GetPosition2Coordinate()->SetValue(.7, .2);
		double size [2] = {6, 5};
		m_textRepresentation->GetSize(size);
		m_textLabel->SetRepresentation(m_textRepresentation);
		m_textLabel->SetInteractor(m_renderWindow->GetInteractor());
		m_textLabel->SetTextActor(m_textActor);
	    m_textCallback = vtkSmartPointer<TextCallback>::New();
	    m_textLabel->AddObserver(vtkCommand::StartInteractionEvent, m_textCallback);
        m_renderWindow->GetRenderers()->GetFirstRenderer()->AddActor(m_textActor);
        m_textLabel->On();
        m_renderWindow->Render();
		//m_textLabel->AddObserver(vtkCommand::InteractionEvent, m_textCallback);
		//	m_textSource = vtkSmartPointer<vtkVectorText>::New();
		//	m_textSource->SetText("Hello");

		// Create a mapper
		//	m_textMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		//m_textMapper->SetInputConnection(m_textSource->GetOutputPort());

		// Create a subclass of vtkActor: a vtkFollower that remains facing the camera
			/*m_cameraFollower = vtkSmartPointer<vtkFollower>::New();
			m_cameraFollower->SetMapper(m_textMapper);
			m_renderWindow->GetRenderers()->GetFirstRenderer()->AddActor(m_cameraFollower);
			m_cameraFollower->SetCamera(m_renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera());
			m_renderWindow->GetRenderers()->GetFirstRenderer()->AddActor(m_textActor);
			m_textCallback = vtkSmartPointer<TextCallback>::New();
			m_textSource->AddObserver(vtkCommand::InteractionEvent, m_textCallback);
			m_textSource->AddObserver(vtkCommand::StartInteractionEvent, m_textCallback);*/
			//m_textSource->AddObserver(vtkCommand::EndInteractionEvent, m_textCallback);
		//	m_renderWindow->Render(); 

}
