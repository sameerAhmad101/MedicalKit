#include "singleimage.h"
#include "ui_singleimage.h"

SingleImage::SingleImage(Patient *patient, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleImage)
{
    ui->setupUi(this);
	m_patient = patient;
    initUI();
	initImageslist();
}

SingleImage::~SingleImage()
{
    delete ui;
}

void SingleImage::initUI()
{
    m_vtkWidget = new QVTKOpenGLWidget;
    ui->verticalLayout_qvtkWidget->addWidget(m_vtkWidget);
    //    ui->horizontalSlider_slicesSlider->setVisible(false);

	auto imageFrame = new ImageFrame(this);
	imageFrame->setText("clikableLabel");
	ui->horizontalLayout_2->addWidget(imageFrame);

    m_renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_vtkWidget->SetRenderWindow(m_renderWindow);
	m_dcmFileService = new DcmFileService(m_renderWindow);

	m_renderWindows_v.emplace(std::make_pair("w0", m_renderWindow));
	m_renderWindow->SetWindowName("w0");

    m_menuBar = new QMenuBar();
    m_fileMenu = new QMenu("File");
    m_menuBar->addMenu(m_fileMenu);
    m_fileMenu->addAction("فتح صورة واحدة");
    m_fileMenu->addAction("فتح مجلد الصور");
  //  connect(m_fileMenu->actions()[0], SIGNAL(triggered(bool)), this, SLOT(openMedicalFile()));
    //connect(m_fileMenu->actions()[1], SIGNAL(triggered(bool)), this, SLOT(openDicomdirectory()));
    this->layout()->setMenuBar(m_menuBar);
    initRGBSliders();
    initPatientInfo();
	m_toolsBar = new QToolBar(this);
	QAction * a = new QAction();
	connect(a, SIGNAL(triggered()), this, SLOT(addNewViewPort()));
	m_toolsBar->addAction(a);
	m_toolsBar->setLayoutDirection(Qt::LayoutDirection::RightToLeft);

	QGridLayout *_gridLayout = new QGridLayout();
	QPushButton *_pushButton_openSingleDicom = new QPushButton("open Dicom Image");
	QPushButton *_pushButton_openDicomSeries = new QPushButton("open Dicom Series");
	_gridLayout->addWidget(_pushButton_openSingleDicom, 0, 0, 1, 1);
	_gridLayout->addWidget(_pushButton_openDicomSeries, 0, 1, 1, 1);
	QSplitter *_spliiter = new QSplitter();
	QSplitter *_spliiter1 = new QSplitter();
	m_vtkWidget->setLayout(_gridLayout);
	_gridLayout->addWidget(_spliiter, 1, 1, 1, 1);
	_gridLayout->addWidget(_spliiter1, 1, 0, 1, 1);

	QCoreApplication::instance()->installEventFilter(this);
}

void SingleImage::initRGBSliders()
{
    ui->horizontalSlider_green->setRange(0, 255);
    ui->horizontalSlider_blue->setRange(0, 255);
    ui->horizontalSlider_red->setRange(0, 255);
    connect(ui->horizontalSlider_red, SIGNAL(valueChanged(int)), this, SLOT(seekRGBSlider(int)));
    connect(ui->horizontalSlider_green, SIGNAL(valueChanged(int)), this, SLOT(seekRGBSlider(int)));
    connect(ui->horizontalSlider_blue, SIGNAL(valueChanged(int)), this, SLOT(seekRGBSlider(int)));
}

void SingleImage::openMedicalFile(DcmFileService *dcmFileService)
{
    m_fileDialog->DontUseNativeDialog;
    try
    {
        QString fileName = m_fileDialog->getOpenFileName(this,
            tr("Open Image"), " ", tr(" All Files(*.*);; Raw File( *.mhd, *.txt);; DICOM File(*.dcm)"));

        std::string fileExtension = fileName.toStdString().substr(fileName.toStdString().find_last_of(".") + 1);
        if (fileExtension == "txt" || fileExtension == "mhd")
        {
            m_rawFileService = new RawFileService(fileName, m_renderWindow);

//            connect(ui->horizontalSlider_cutterX, SIGNAL(valueChanged(int)), m_rawFileService, SLOT(moveCutter(int)));
//            connect(ui->horizontalSlider_cutterY, SIGNAL(valueChanged(int)), m_rawFileService, SLOT(moveCutter(int)));
//            connect(ui->horizontalSlider_cutterZ, SIGNAL(valueChanged(int)), m_rawFileService, SLOT(moveCutter(int)));
//			connect(ui->pushButton_startCutter, SIGNAL(clicked(bool)), m_rawFileService, SLOT(startCutter()));
//			connect(ui->pushButton_isurface,SIGNAL(clicked(bool)),m_rawFileService, SLOT(extractSourface()));
            //connect(ui->pushButton_startCutter, SIGNAL(clicked()), m_rawFileService, SLOT(startCutter()));
            //ui->horizontalSlider_contrast->setRange(0,1000);
            //connect(ui->horizontalSlider_contrast, SIGNAL(valueChanged(int)), m_rawFileService, SLOT(moveCutter(int)));
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
            if(!dcmFileService){
				dcmFileService->openSingleFile(fileName.toStdString());
            }
            else
            {
				dcmFileService->openSingleFile(fileName.toStdString());
            }
        }
    }
        catch (std::exception &e)
        {
            std::cout << "errorooror" << e.what();
        }
}

void SingleImage::seekRGBSlider(int value)
{

    int blue = ui->horizontalSlider_blue->value();
    int green = ui->horizontalSlider_green->value();
    int red = ui->horizontalSlider_red->value();
    m_rgbColor[0] = static_cast<double>(red) / static_cast<double>(150);
    m_rgbColor[1] = static_cast<double>(green) / static_cast<double>(150);
    m_rgbColor[2] = static_cast<double>(blue) / static_cast<double>(150);
    m_rawFileService->setSkinColor(m_rgbColor[0], m_rgbColor[1], m_rgbColor[2]);
    m_renderWindow->Render();
}

void SingleImage::openDicomdirectory(DcmFileService *dcmFileService){
    QString slicesDirectory =
            m_fileDialog->getExistingDirectory(this, tr("Open Directory"),"/home",
                                       QFileDialog::ShowDirsOnly
                                       | QFileDialog::DontResolveSymlinks);
    if(!dcmFileService){
		dcmFileService = new DcmFileService(m_renderWindow);
    }

	dcmFileService->openFileSeries(slicesDirectory.toStdString());
    ui->horizontalSlider_slicesSlider->setRange(0, dcmFileService->getSlicesCount());
     connect(ui->horizontalSlider_slicesSlider, SIGNAL(valueChanged(int)), dcmFileService, SLOT(requestSlice(int)));
}

void SingleImage::initImageslist() {
	QScrollArea *area = new QScrollArea(this);
	area->setWidgetResizable(true);
	area->setWidget(new QWidget);
	QGridLayout *grid = new QGridLayout;
	area->widget()->setLayout(grid);
	ui->dockWidget_images->setWidget(area);
	int imageCounter = 0;
for(int row = 0; row < m_patient->getImages().size() / 2; row++)
{
    for(int column = 0; column < 2; column++)
    {
		QImage *image = new QImage();
		auto dcmSeric = new DcmFileService(m_renderWindow);
		dcmSeric->dcm2png(m_patient->getImages().at(imageCounter));
		std::string _patientImagePath = m_patient->getImages().at(imageCounter);
		std::string _fileName = _patientImagePath.substr(0, _patientImagePath.find_last_of(".")) + ".png";
		QString imagePath = QString::fromStdString(_fileName);
		image->load(imagePath);
		auto imageFrame = new ImageFrame(this);
		imageFrame->setPixmap(QPixmap::fromImage(*image));
        //imageFrame->setMaximumWidth(ui->dockWidget_images->size().width() / 6 - 4);
        //imageFrame->setMaximumHeight(ui->dockWidget_images->size().width() / 6 - 4);
		imageFrame->setScaledContents(true);
		imageFrame->setStyleSheet("border: 1px solid black; margin: 0; padding: 1px;");
		imageFrame->setImagePath(m_patient->getImages().at(imageCounter));
		imageFrame->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
		connect(imageFrame, SIGNAL(clicked()), this, SLOT(patientImageCliked()));
        grid->addWidget(imageFrame, row, column);
		imageCounter++;
	}
}
}

void SingleImage::patientImageCliked() {

	ImageFrame *clikedImage = qobject_cast<ImageFrame*>(sender());
	if (clikedImage) {
		if (m_dcmFileService) {
			//m_renderWindow->GetRenderers()->GetFirstRenderer()->Delete();
			m_dcmFileService->openSingleFile(clikedImage->getImagePath());
		}
		else {
			m_dcmFileService = new DcmFileService(m_renderWindow);
			m_dcmFileService->openSingleFile(clikedImage->getImagePath());
		}
	}
}

void SingleImage::initPatientInfo(){
    if(m_patient){
        ui->label_firstNameValue->setText(QString::fromStdString(m_patient->getFirstName()));
        ui->label_patientAgeVlaue->setText(QString(m_patient->getAge()));
        ui->label_patientLastNameValue->setText(QString::fromStdString(m_patient->getLastName()));
        ui->label_patientLocationValue->setText(QString::fromStdString(m_patient->getLocation()));
        ui->label_patientNationalNumberValue->setText(QString::fromStdString(m_patient->getNationalNumber()));
        viewPatientImage(m_patient->getPersonalImage());
    }
}

void SingleImage::viewPatientImage(std::string &imagePath) {
    QImage image;
    image.load(QString::fromStdString(imagePath));
    ui->label_patientImage->setPixmap(QPixmap::fromImage(image));
}

std::string SingleImage::generateReinderWindowId(){
	for (int i = 0; i <= m_renderWindows_v.size(); i++) {
		std::stringstream tmp;
		tmp << "w" << i;
		if (m_renderWindows_v.find(tmp.str()) == m_renderWindows_v.end()) {
			return tmp.str();
		   }
		}
}

void SingleImage::addNewViewPort() {
	if (m_renderWindows_v.size() <= 3) {
		QGridLayout *_gridLayout = new QGridLayout();
		QPushButton *_pushButton_openSingleDicom = new QPushButton();
		_pushButton_openSingleDicom->setIcon(QIcon(QPixmap("view/assets/DICOM.SVG")));
		_pushButton_openSingleDicom->setMinimumSize(35, 35);
		_pushButton_openSingleDicom->setIconSize(QSize(30, 30));
		QSizePolicy _sizePolicy;
		_sizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);
		_sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
		_pushButton_openSingleDicom->setSizePolicy(_sizePolicy);


		QPushButton *_pushButton_openDicomSeries = new QPushButton();
		_gridLayout->addWidget(_pushButton_openSingleDicom, 0, 0, 1, 1);
		_gridLayout->addWidget(_pushButton_openDicomSeries, 0, 1, 1, 1);
		QSplitter *_spliiter = new QSplitter();
		QSplitter *_spliiter1 = new QSplitter();
		_gridLayout->addWidget(_spliiter, 1, 1, 1, 1);
		_gridLayout->addWidget(_spliiter1, 1, 0, 1, 1);

        QVTKOpenGLWidget *_openGlWidget = new QVTKOpenGLWidget;
		vtkSmartPointer<vtkGenericOpenGLRenderWindow> _renderWIndow =
			vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
		_renderWIndow->SetWindowName(generateReinderWindowId().c_str());
		auto _interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		_renderWIndow->SetInteractor(_interactor);
		_openGlWidget->SetRenderWindow(_renderWIndow);

		ui->verticalLayout_viewports->addWidget(_openGlWidget);
		std::string  _windowId = generateReinderWindowId();
		m_renderWindows_v.emplace(std::make_pair(_windowId, _renderWIndow));
		DcmFileService *_dcmFileService = new DcmFileService(_renderWIndow);
		m_OpenedDcmFiles_v.emplace(std::make_pair(_windowId, _dcmFileService));

		_openGlWidget->setLayout(_gridLayout);
		connect(_pushButton_openSingleDicom, SIGNAL(clicked(bool)), this, SLOT(openDicomFileViewport()));
		
	}
}

void SingleImage::openDicomFileViewport() {
	auto senderWidget = static_cast<QWidget*>(sender());
	auto vtkWindow = static_cast<QVTKOpenGLWidget*>(senderWidget->parentWidget());
	auto dcmfileService = m_OpenedDcmFiles_v[vtkWindow->GetRenderWindow()->GetWindowName()];
	//dcmfileService->updateRenderWindow(m_renderWindows_v[vtkWindow->GetRenderWindow()->GetWindowName()]);
	//m_renderWindow = m_renderWindows_v[vtkWindow->GetRenderWindow()->GetWindowName()];
	openMedicalFile(dcmfileService);

}

bool SingleImage::eventFilter(QObject *obj, QEvent *e)
{
	//auto vtkWidget = qobject_cast<QVTKOpenGLWidget*>(obj);
	//if (vtkWidget) {
	//	if (m_renderWindows_v.size() > 1) {
	//		 m_dcmFileService->updateRenderWindow(m_renderWindows_v[vtkWidget->GetRenderWindow()->GetWindowName()]);
	//	}
	//}
	return false;
}

// ImageFrame calss

ImageFrame::ImageFrame(QWidget* parent)
	: QLabel(parent) {
	//;
}

ImageFrame::~ImageFrame(){}

void ImageFrame::mousePressEvent(QMouseEvent *e) {
	if (e->button() == Qt::MouseButton::LeftButton) {
		emit clicked();
	}
}

void ImageFrame::setImagePath(std::string &imagePath) {
	m_imagePath = imagePath;
}

std::string ImageFrame::getImagePath() {
	return m_imagePath;
}

