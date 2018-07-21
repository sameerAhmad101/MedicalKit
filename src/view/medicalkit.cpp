#include "medicalkit.h"
#include "ui_medicalkit.h"



MedicalKit::MedicalKit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MedicalKit)
{

    ui->setupUi(this);
	
	initUI();

    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openMedicalFile()));
    connect(ui->actionOpen_Dicom_Series, SIGNAL(triggered(bool)), this, SLOT(openDicomdirectory()));
    connect(ui->horizontalSlider_red, SIGNAL(valueChanged(int)), this, SLOT(seekRGBSlider(int)));
    connect(ui->horizontalSlider_green_2, SIGNAL(valueChanged(int)), this, SLOT(seekRGBSlider(int)));
    connect(ui->horizontalSlider_blue, SIGNAL(valueChanged(int)), this, SLOT(seekRGBSlider(int)));
    connect(ui->horizontalSlider_slicesSlider, SIGNAL(valueChanged(int)), this, SLOT(seekSlicesSlider(int)));
    connect(ui->pushButton_addText, SIGNAL(clicked(bool)), this, SLOT(addText()));
    m_fileDialog = new QFileDialog(this);
}

MedicalKit::~MedicalKit()
{
	if (m_rawFileService)
	{
		delete m_rawFileService;
	}
    delete ui;

}


void MedicalKit::initUI()
{
	m_vtkWidget = new QVTKOpenGLWidget;
	ui->verticalLayout_vtkWidget->addWidget(m_vtkWidget);
    ui->horizontalSlider_slicesSlider->setVisible(false);
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> openGlRenderWindow =
		vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_vtkWidget->SetRenderWindow(openGlRenderWindow);
	m_renderWindow = m_vtkWidget->GetRenderWindow();
	m_renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_renderWindowInteractor->SetRenderWindow(m_renderWindow);
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	/*try {
		QFile File("StyleSheet1.css");
		File.open(QFile::ReadOnly | QFile::Text);
		if (File.isOpen())
		{
			this->setStyleSheet(File.readAll());
		}
		else
		{
			std::cout << "ERROR : Can not Load Stylesheet!.";
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
	*/

	m_renderWindowInteractor->SetInteractorStyle(style);
	initRGBSliders();
}


void MedicalKit::openMedicalFile()
{

	m_fileDialog->DontUseNativeDialog;
	QString defaultFilter = tr(" (*.dcm,*.raw, *.mhd, *.txt)");
	try
	{
		QString fileName = m_fileDialog->getOpenFileName(this,
			tr("Open Image"), " ", tr(" All Files(*.*);; Raw File( *.mhd, *.txt);; DICOM File(*.dcm)"));

		std::string fileExtension = fileName.toStdString().substr(fileName.toStdString().find_last_of(".") + 1);
		if (fileExtension == "txt" || fileExtension == "mhd")
		{
			m_rawFileService = new RawFileService(fileName, m_renderWindow);
			connect(ui->pushButton_startCutter, SIGNAL(clicked()), m_rawFileService, SLOT(startCutter()));
			if (m_rawFileService->openRawFile())
			{
				m_rawFileService->setupImage();
				m_renderWindow->Render();
			}
			else
			{
				std::cout << "Error !! : Can not open file" << std::endl;
			}
		}
		else if (fileExtension == "dcm")
		{
            if(!m_dcmFileService){
                 m_dcmFileService = new DcmFileService(m_renderWindow);
            }

        }
	}
		catch (std::exception &e)
		{
			std::cout << "errorooror" << e.what();
		}
		


}

void MedicalKit::openDicomdirectory(){
    QString slicesDirectory = m_fileDialog->getExistingDirectory(this, tr("Open Directory"),"/home",
                                       QFileDialog::ShowDirsOnly
                                       | QFileDialog::DontResolveSymlinks);
    if(!m_dcmFileService){
         m_dcmFileService = new DcmFileService(m_renderWindow);
    }
    std::cout << slicesDirectory.toStdString();
    m_dcmFileService->openFileSeries(slicesDirectory.toStdString());
    ui->horizontalSlider_slicesSlider->setVisible(true);
    ui->horizontalSlider_slicesSlider->setRange(0,m_dcmFileService->getSlicesCount());
}

void MedicalKit::initRGBSliders()
{
	ui->horizontalSlider_green_2->setRange(0, 10);

	ui->horizontalSlider_blue->setRange(0, 10);

	ui->horizontalSlider_red->setRange(0, 10);
}


void MedicalKit::seekRGBSlider(int value)
{

	int blue = ui->horizontalSlider_blue->value();
	int green = ui->horizontalSlider_green_2->value();
	int red = ui->horizontalSlider_red->value();

	m_rgbColor[0] = static_cast<double>(red) / static_cast<double>(10);
	m_rgbColor[1] = static_cast<double>(green) / static_cast<double>(10);
	m_rgbColor[2] = static_cast<double>(blue) / static_cast<double>(10);
	
	m_rawFileService->setSkinColor(m_rgbColor[0], m_rgbColor[1], m_rgbColor[2]);

	m_renderWindow->Render();
}

void MedicalKit::seekSlicesSlider(int value)
{
  m_dcmFileService->requestSlice(value);
}

void MedicalKit::addText()
{
    m_textLabel = new TextLabelService(m_renderWindow);
}
