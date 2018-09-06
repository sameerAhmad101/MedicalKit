#include "view/medicalkit.h"
#include "view/DarkStyle.h"
#include "view/framelesswindow/framelesswindow.h"
#include "view/intro.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MedicalKit *w = new MedicalKit;
    Intro * intro = new Intro;
	a.setStyle(new DarkStyle);

	FramelessWindow framelessWindow;
	framelessWindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));
    framelessWindow.setContent(intro);
	framelessWindow.show();

    return a.exec();
}
