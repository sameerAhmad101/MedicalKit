#include "DcmFileService.h"


DcmFileService::DcmFileService(vtkSmartPointer<vtkRenderWindow> renderWindow) {
	m_renderWindow = renderWindow;
	m_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	if (m_renderWindow->GetRenderers()->GetFirstRenderer() != nullptr) {
		m_renderer = m_renderWindow->GetRenderers()->GetFirstRenderer();
	}
	else {
		m_renderer = vtkSmartPointer<vtkRenderer>::New();
		m_renderWindow->AddRenderer(m_renderer);
	}
	m_dcmImageReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	m_dcmImageViewer = vtkSmartPointer<vtkImageViewer2>::New();
}

DcmFileService::~DcmFileService() {
	//;
}

void DcmFileService::openSingleFile(std::string fileName) {
	m_dcmfileName = fileName;
	m_dcmImageReader->SetFileName(m_dcmfileName.c_str());
	m_dcmImageReader->Update();
	m_dcmImageViewer->SetInputConnection(m_dcmImageReader->GetOutputPort());
	m_dcmImageViewer->SetRenderWindow(m_renderWindow);
	m_dcmImageViewer->SetupInteractor(m_interactor);
	m_dcmImageViewer->Render();
}
void DcmFileService::openFileSeries(std::string fileDirectory) {
    try{m_directoryName = fileDirectory;
	m_dcmImageReader->SetDirectoryName(m_directoryName.c_str());
	m_dcmImageReader->Update();
	m_dcmImageViewer->SetInputConnection(m_dcmImageReader->GetOutputPort());
    m_slicesCount = m_dcmImageViewer->GetSliceMax();
    m_dcmImageViewer->SetRenderWindow(m_renderWindow);
    m_dcmImageViewer->SetupInteractor(m_interactor);
    m_dcmImageViewer->Render();
}
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

int DcmFileService::getSlicesCount(){
    return m_slicesCount;
}

void DcmFileService::requestSlice(int slice)
{
m_dcmImageViewer->SetSlice(slice);
}
