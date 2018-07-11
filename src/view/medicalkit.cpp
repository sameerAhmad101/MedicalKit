#include "medicalkit.h"
#include "ui_medicalkit.h"



MedicalKit::MedicalKit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MedicalKit)
{

    ui->setupUi(this);
	
	initUI();

	connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openMedicalFile()));
	connect(ui->horizontalSlider_red, SIGNAL(valueChanged(int)), this, SLOT(seekSlider(int)));
	connect(ui->horizontalSlider_green_2, SIGNAL(valueChanged(int)), this, SLOT(seekSlider(int)));
	connect(ui->horizontalSlider_blue, SIGNAL(valueChanged(int)), this, SLOT(seekSlider(int)));
    connect(ui->pushButton_addText, SIGNAL(clicked(bool)), this, SLOT(addText()));
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

	vtkSmartPointer<vtkGenericOpenGLRenderWindow> openGlRenderWindow =
		vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_vtkWidget->SetRenderWindow(openGlRenderWindow);
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	m_renderWindow = m_vtkWidget->GetRenderWindow();
	m_renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_renderWindowInteractor->SetRenderWindow(m_renderWindow);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	try {
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
	

	m_renderWindowInteractor->SetInteractorStyle(style);
	initRGBSliders();
}


void MedicalKit::openMedicalFile()
{
	try
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open Image"), " ", tr("Image Files (*.raw, *.mhd, *.txt)"));

		m_rawFileService = new RawFileService(fileName, m_renderWindow);
		connect(ui->pushButton_startCutter, SIGNAL(clicked()), m_rawFileService, SLOT(startCutter()));

		if (m_rawFileService->openRawFile())
		{
			m_rawFileService->setupImage();

			m_renderWindow->Render();
			/*iren->Initialize();
			iren->Start();*/

		}
		else
		{
			std::cout << "Error !! : Can not open file" << std::endl;
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
}

void MedicalKit::initRGBSliders()
{
	ui->horizontalSlider_green_2->setRange(0, 10);

	ui->horizontalSlider_blue->setRange(0, 10);

	ui->horizontalSlider_red->setRange(0, 10);
}


void MedicalKit::seekSlider(int value)
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

void MedicalKit::addText()
{
    m_textLabel = new TextLabelService(m_renderWindow);
}
