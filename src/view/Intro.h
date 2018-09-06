#ifndef INTRO_H
#define INTRO_H

#include <QMainWindow>
#include <qpixmap.h>
#include "singleimage.h"
#include "imageseries.h"
#include "medicalkit.h"

namespace Ui {
class Intro;
}

class Intro : public QMainWindow
{
    Q_OBJECT

public:
    explicit Intro(QWidget *parent = 0);
    ~Intro();

private:
    Ui::Intro *ui;
	MedicalKit *m_medicalKit;
};

#endif // INTRO_H
