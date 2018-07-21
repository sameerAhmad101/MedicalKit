#include "RawFileService.h" 


RawFileService::RawFileService(QString filePath, vtkSmartPointer<vtkRenderWindow> renderWindow)
{
	m_rawFilePath = filePath;
	m_renderWindow = renderWindow;
	m_skin = vtkSmartPointer<vtkActor>::New();
}

RawFileService::~RawFileService()
{
	//;
}

bool RawFileService::openRawFile()
{
	bool is_opened = false;
	try 
	{
		// Convert Qstring to char *
		QByteArray ba = m_rawFilePath.toLatin1();
		const char * file_name = ba.data();
		m_rawImageReader = vtkSmartPointer<vtkMetaImageReader>::New();
		m_rawImageReader->SetFileName(file_name);
		is_opened = true;
	}
	catch(std::exception &e)
	{
		is_opened = false;
		std::cout << e.what();
	}
	return is_opened;
}

void RawFileService::setupImage()
{
	readHeaderFile();
if(m_rawFileType == "MET_SHORT")
{
	vtkSmartPointer<vtkMarchingCubes> skinExtractor =
		vtkSmartPointer<vtkMarchingCubes>::New();
	skinExtractor->SetInputConnection(m_rawImageReader->GetOutputPort());
	skinExtractor->SetValue(0, 500);

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();

	skinMapper->SetInputConnection(skinExtractor->GetOutputPort());
	skinMapper->ScalarVisibilityOff();

	m_skin->SetMapper(skinMapper);
}
	m_renderer =   vtkSmartPointer<vtkRenderer>::New();
	m_renderer->AddActor(m_skin);

	vtkSmartPointer<vtkBoxWidget2> box = vtkSmartPointer<vtkBoxWidget2>::New();
	m_renderWindow->AddRenderer(m_renderer);

}

void RawFileService::setSkinColor(double red, double green, double blue)
{
	m_skin->GetProperty()->SetColor(red, green, blue);
}


bool RawFileService::readHeaderFile()
{
	bool is_read = false;
	try
	{
		std::fstream headerFile;
		headerFile.open(m_rawFilePath.toStdString()	, ios::in);
		std::string line = "";
		std::array<std::string, 3> property;
		if (headerFile.is_open())
		{
			is_read = true;
			while (!headerFile.eof())
			{
				std::getline(headerFile, line);
				std::stringstream linStream(line);
				linStream >> property[0] >> property[1] >> property[2];
				if (property[0] == "ElementType")
				{
					m_rawFileType = property[2];
				}
			}
		}
		else
		{
			std::cout << "ERROR !! : Can not parse mhd file" << std::endl;
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
    return is_read;
}


void RawFileService::startCutter()
{
	m_cutterplane = vtkSmartPointer<vtkPlane>::New();
	m_cutterplane->SetOrigin(m_skin->GetCenter());
	m_cutterplane->SetNormal(1, 1, 1);



	// Setup cutter

	m_cutter = vtkSmartPointer<vtkCutter>::New();
	m_cutter->SetCutFunction(m_cutterplane);
	m_cutter->SetInputConnection(m_rawImageReader->GetOutputPort());
	m_cutter->Update();

	vtkSmartPointer<vtkPolyDataMapper> cutterMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	cutterMapper->SetInputConnection(m_cutter->GetOutputPort());

	vtkSmartPointer<vtkActor> planeActor =
		vtkSmartPointer<vtkActor>::New();
	planeActor->SetMapper(cutterMapper);
	planeActor->GetProperty()->SetColor(1.0, 1, 0);
	planeActor->GetProperty()->SetLineWidth(2);

	m_renderer->AddActor(planeActor);
	m_renderer->AddActor(planeActor);

	m_renderWindow->Render();
}
