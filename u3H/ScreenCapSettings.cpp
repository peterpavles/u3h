#include "ScreenCapSettings.h"



ScreenCapSetting::ScreenCapSetting(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
}

ScreenCapSetting::~ScreenCapSetting()
{

}



void ScreenCapSetting::on_pushButton_cancel_clicked()
{
	close();
}

void ScreenCapSetting::on_pushButton_applyOptions_clicked()
{
	QString bits(ui.comboBox_bits->currentText().split(" ")[0]);
	QString	scal(ui.comboBox_scaling->currentText().split(" ")[0]);
	QString refresh(ui.comboBox_refresh->currentText().split(" ")[0]);
	
	emit ApplyOptions(bits, scal, refresh);

	close();
}