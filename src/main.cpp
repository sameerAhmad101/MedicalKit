#include "view/medicalkit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MedicalKit w;
    w.show();

    return a.exec();
}
