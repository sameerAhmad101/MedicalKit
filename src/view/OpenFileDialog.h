#pragma once
#pragma warning (push, 0)
#include <QFileDialog>
#include <QGridLayout>
#include <qcheckbox.h>
#pragma warning (pop, 0)

class OpenFileDialog : public QFileDialog {
public:
	OpenFileDialog();
	~OpenFileDialog();
};