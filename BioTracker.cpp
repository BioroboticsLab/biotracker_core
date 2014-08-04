#include "BioTracker.h"

BioTracker::BioTracker(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
}

BioTracker::~BioTracker()
{

}
void BioTracker::initGui(){
	ui.button_playPause->setEnabled(false);
}