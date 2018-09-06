#include "RawFileService.h" 


RawFileService::RawFileService(QString filePath, vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow)
{
	m_rawFilePath = filePath;
	m_renderWindow = renderWindow;
	m_skin = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> inter = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_renderWindowInteractor->SetRenderWindow(m_renderWindow);

	m_interactorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	m_renderWindowInteractor->SetInteractorStyle(m_interactorStyle);
	m_rawImageReader = vtkSmartPointer<vtkMetaImageReader>::New();
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

		m_rawImageReader->SetFileName(m_rawFilePath.toStdString().c_str());
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
	m_skinExtractor =
		vtkSmartPointer<vtkMarchingCubes>::New();
	m_skinExtractor->SetInputConnection(m_rawImageReader->GetOutputPort());
	m_skinExtractor->SetValue(0, 500);

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();

	skinMapper->SetInputConnection(m_skinExtractor->GetOutputPort());
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
	// Define a spherical clip function to clip the isosurface
	m_clipFunction = vtkSmartPointer<vtkPlane>::New();
	m_clipFunction->SetOrigin(100, 52, 15);
	m_clipFunction->SetNormal(1, 0, 0);

	// Clip the isosurface with a sphere
	vtkSmartPointer<vtkClipDataSet> skinClip =
		vtkSmartPointer<vtkClipDataSet>::New();
	skinClip->SetInputConnection(m_skinExtractor->GetOutputPort());
	skinClip->SetClipFunction(m_clipFunction);
	skinClip->SetValue(0);
	skinClip->GenerateClipScalarsOn();
	skinClip->Update();

	vtkSmartPointer<vtkDataSetMapper> skinMapper =
		vtkSmartPointer<vtkDataSetMapper>::New();
	skinMapper->SetInputConnection(skinClip->GetOutputPort());
	skinMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> skin =
		vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);
	skin->GetProperty()->SetDiffuseColor(1, .49, .25);

	vtkSmartPointer<vtkProperty> backProp =
		vtkSmartPointer<vtkProperty>::New();
	backProp->SetDiffuseColor(0.8900, 0.8100, 0.3400);
	skin->SetBackfaceProperty(backProp);

	// Define a model for the "lens". Its geometry matches the implicit
	// sphere used to clip the isosurface
		m_lensModel = vtkSmartPointer<vtkPlaneSource>::New();
		m_lensModel->SetOrigin(10, 0, 0);
		m_lensModel->SetCenter(100,52,10);

	// Sample the input volume with the lens model geometry
	vtkSmartPointer<vtkProbeFilter> lensProbe =
		vtkSmartPointer<vtkProbeFilter>::New();
	lensProbe->SetInputConnection(m_lensModel->GetOutputPort());
	lensProbe->SetSourceConnection(m_rawImageReader->GetOutputPort());

	// Clip the lens data with the isosurface value
	vtkSmartPointer<vtkClipDataSet> lensClip =
		vtkSmartPointer<vtkClipDataSet>::New();
	lensClip->SetInputConnection(lensProbe->GetOutputPort());
	lensClip->SetValue(500);
	lensClip->GenerateClipScalarsOff();
	lensClip->Update();

	// Define a suitable grayscale lut
	vtkSmartPointer<vtkLookupTable> bwLut =
		vtkSmartPointer<vtkLookupTable>::New();
	bwLut->SetTableRange(0, 2048);
	bwLut->SetSaturationRange(0, 0);
	bwLut->SetHueRange(0, 0);
	bwLut->SetValueRange(.2, 1);
	bwLut->Build();

	vtkSmartPointer<vtkDataSetMapper> lensMapper =
		vtkSmartPointer<vtkDataSetMapper>::New();
	lensMapper->SetInputConnection(lensClip->GetOutputPort());
	lensMapper->SetScalarRange(lensClip->GetOutput()->GetScalarRange());
	lensMapper->SetLookupTable(bwLut);

	vtkSmartPointer<vtkActor> lens =
		vtkSmartPointer<vtkActor>::New();
	lens->SetMapper(lensMapper);
	m_renderer->RemoveActor(m_skin);
	m_renderer->AddActor(lens);
	m_renderer->AddActor(skin);
	m_renderWindow->Render();

}

void RawFileService::moveCutter(int x)
{
    QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());

    std::cout << senderSignalIndex() << std::endl;
	m_lensModel->SetCenter(x, 52, 15);
	m_clipFunction->SetOrigin(x, 52, 15);
	m_renderWindow->Render();
}

void RawFileService::extractSourface()
{
	// Set the colors.
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();
	std::array<unsigned char, 4> skinColor{ { 255, 125, 64 } };
	colors->SetColor("SkinColor", skinColor.data());
	std::array<unsigned char, 4> bkg{ { 51, 77, 102, 255 } };
	colors->SetColor("BkgColor", bkg.data());
	vtkSmartPointer<vtkStripper> skinStripper =
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(m_skinExtractor->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();
	m_skin->SetMapper(skinMapper);
	m_skin->GetProperty()->SetDiffuseColor(colors->GetColor3d("SkinColor").GetData());
	m_skin->GetProperty()->SetSpecular(.3);
	m_skin->GetProperty()->SetSpecularPower(20);
	m_skin->GetProperty()->SetOpacity(.5);

	m_bonesExtractor =
		vtkSmartPointer<vtkMarchingCubes>::New();
	m_bonesExtractor->SetInputConnection(m_rawImageReader->GetOutputPort());
	m_bonesExtractor->SetValue(0, 1150);

	vtkSmartPointer<vtkStripper> boneStripper =
		vtkSmartPointer<vtkStripper>::New();
	boneStripper->SetInputConnection(m_bonesExtractor->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> boneMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	boneMapper->SetInputConnection(boneStripper->GetOutputPort());
	boneMapper->ScalarVisibilityOff();

	m_boneActor =
		vtkSmartPointer<vtkActor>::New();
	m_boneActor->SetMapper(boneMapper);
	m_boneActor->GetProperty()->SetDiffuseColor(colors->GetColor3d("Ivory").GetData());

	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(m_rawImageReader->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outline =
		vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

	m_renderer->AddActor(outline);
	m_renderer->AddActor(m_boneActor);

	m_renderWindow->Render();
}