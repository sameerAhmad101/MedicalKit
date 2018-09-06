#include "intro.h"
#include "ui_intro.h"

Intro::Intro(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Intro)
{
    ui->setupUi(this);
	m_medicalKit = new MedicalKit();
	QPixmap _visualizerPixMap("view/assets/visualizerImage.svg");
	ui->label_visualizerImage->setPixmap(_visualizerPixMap);
	ui->label_visualizerImage->setStyleSheet("padding : 30px");
	this->setStyleSheet("background : #06666D");
	connect(ui->pushButton_clinicImage, SIGNAL(clicked(bool)), m_medicalKit, SLOT(show()));
}

Intro::~Intro()
{
    delete ui;
}
