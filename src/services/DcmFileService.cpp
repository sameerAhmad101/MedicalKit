#include "DcmFileService.h"


DcmFileService::DcmFileService(vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow) {
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
	m_lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	m_imageColor = vtkSmartPointer<vtkImageMapToColors>::New();
	m_imageColor->SetLookupTable(m_lookupTable);
	m_lookupTable->SetRange(-100,50);
    m_lookupTable->SetValueRange(0.0, 0.0); // from black to white
    m_lookupTable->SetSaturationRange(0.0, 1.0); // no color saturation
	m_lookupTable->SetRampToLinear();
	m_lookupTable->Build();
}

DcmFileService::~DcmFileService() {
	//;
}

void DcmFileService::openSingleFile(std::string fileName) {
	try {
		m_dcmfileName = fileName;
		m_dcmImageReader->SetFileName(m_dcmfileName.c_str());
		m_dcmImageReader->Update();
		m_imageColor->SetInputConnection(m_dcmImageReader->GetOutputPort());
		m_dcmImageViewer->SetInputConnection(m_dcmImageReader->GetOutputPort());
		m_dcmImageViewer->SetRenderWindow(m_renderWindow);
		m_dcmImageViewer->SetupInteractor(m_interactor);
		m_dcmImageViewer->Render();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}

void DcmFileService::openFileSeries(std::string fileDirectory) {
try{
		 m_directoryName = fileDirectory;
	     m_dcmImageReader->SetDirectoryName(m_directoryName.c_str());
	     m_dcmImageReader->Update();
	     m_dcmImageViewer->SetInputConnection(m_dcmImageReader->GetOutputPort());
         m_slicesCount = m_dcmImageViewer->GetSliceMax();
         m_dcmImageViewer->SetRenderWindow(m_renderWindow);
         m_dcmImageViewer->SetupInteractor(m_interactor);

		 m_sliceCounterProperty = vtkSmartPointer<vtkTextProperty>::New();
		 m_sliceCounterProperty->SetFontSize(20);
		 m_sliceCounterProperty->SetFontFamilyToCourier();
		 
		 m_sliceCounterMapper = vtkSmartPointer<vtkTextMapper>::New();
		 m_sliceCounterActor = vtkSmartPointer<vtkActor2D>::New();
		 updateSlicesCount(m_dcmImageViewer->GetSlice());
		 m_sliceCounterActor->SetDisplayPosition(10, 30);
		 m_sliceCounterMapper->SetTextProperty(m_sliceCounterProperty);
		 m_sliceCounterActor->SetMapper(m_sliceCounterMapper);
		 m_dcmImageViewer->GetRenderer()->AddActor2D(m_sliceCounterActor);


		 m_patientInfoActor = vtkSmartPointer<vtkActor2D>::New();
		 m_patientInfoMapper = vtkSmartPointer<vtkTextMapper>::New();
		 m_patientInfoProperty = vtkSmartPointer<vtkTextProperty>::New();
		 std::stringstream m_patientInfoText;
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
updateSlicesCount(slice);
m_dcmImageViewer->Render();
}

std::string DcmFileService::dcm2png(const std::string &fileName) {
	try {
		auto _imageReader = vtkSmartPointer<vtkDICOMImageReader>::New();
		_imageReader->SetFileName(fileName.c_str());
		_imageReader->Update();
		auto _image = _imageReader->GetOutput();
		auto _shitScale = vtkSmartPointer<vtkImageShiftScale>::New();
		_shitScale->SetOutputScalarTypeToUnsignedChar();
		_shitScale->SetInputConnection(_imageReader->GetOutputPort());
		_shitScale->SetShift(-1.0 * _image->GetScalarRange()[0]);
		auto _oldRange = _image->GetScalarRange()[1] - _image->GetScalarRange()[0];
		auto _newRange = 255;
		_shitScale->SetScale(_newRange / _oldRange);
		_shitScale->Update();
		auto _pngWriter = vtkSmartPointer<vtkPNGWriter>::New();
		auto _fileName = fileName.substr(0, fileName.find_last_of("."));
		_fileName += ".png";
		_pngWriter->SetFileName((_fileName).c_str());
		_pngWriter->SetInputConnection(_shitScale->GetOutputPort());
		_pngWriter->Write();
		return _fileName;
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
        return NULL;
	}
}

void DcmFileService::updateSlicesCount(const int sliceNum) {
	std::stringstream tmp;
	tmp << "Slice " << sliceNum << "/ " << m_dcmImageViewer->GetSliceMax();
	m_sliceCounterMapper->SetInput(tmp.str().c_str());
}

void DcmFileService::updateRenderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow) {
	m_renderWindow = renderWindow;
	m_dcmImageViewer->SetRenderWindow(renderWindow);
}