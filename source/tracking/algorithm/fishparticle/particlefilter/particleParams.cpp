#include "particleParams.h"

ParticleParams::ParticleParams(QWidget *parent, Settings & settings) :
_parent(parent)
, _settings(settings)
, _numParticles(1000)
{
	init();
}

ParticleParams::~ParticleParams()
{
	saveParamsToSettings();
}

void ParticleParams::init()
{
	loadParamsFromSettings();
	initGuiElements();
	makeConnects();
}

void ParticleParams::initGuiElements()
{
	_numPartSlide = new QSlider(_parent);
	_numPartSlide->setMaximum(5000);
	_numPartSlide->setMinimum(0);
	_numPartSlide->setOrientation(Qt::Horizontal);
	_numPartSlide->setValue(_numParticles);
	_numPartEdit = new QLineEdit(_parent);
	_numPartEdit->setText(QString::number(_numParticles));
	_numPartEdit->setValidator(new QIntValidator(0, _numPartSlide->maximum(), _parent));
}

QWidget* ParticleParams::getParamsWidget()
{
	QFrame *paramsFrame = new QFrame(_parent);
	QFormLayout *layout = new QFormLayout;
	QHBoxLayout *row = new QHBoxLayout;

	row->addWidget(new QLabel("Number of Particles"));
	row->addWidget(_numPartSlide);
	row->addWidget(_numPartEdit);	
	layout->addRow(row);
	paramsFrame->setLayout(layout);
	return paramsFrame;
}
QWidget* ParticleParams::getToolsWidget(){ return std::nullptr_t(); }

void ParticleParams::makeConnects()
{
	QObject::connect(this->_numPartSlide, SIGNAL( valueChanged(int) ), this, SLOT( refreshNumParticles() ));
	QObject::connect(this->_numPartEdit, SIGNAL(editingFinished()), this, SLOT(paramEdited()));
}

void ParticleParams::loadParamsFromSettings()
{
	try
	{
		_numParticles = _settings.getValueOfParam<int>(PARTICLEFILTERPARAM::NUMBER_OF_PARTICLES);
	}
	catch (const char *)
	{
		//no value in config.ini found -> set it to standard value
		_numParticles = 1000;
		_settings.setParam(PARTICLEFILTERPARAM::NUMBER_OF_PARTICLES, std::to_string(_numParticles));
	}
}

void ParticleParams::saveParamsToSettings()
{
	_settings.setParam(PARTICLEFILTERPARAM::NUMBER_OF_PARTICLES, std::to_string(_numParticles));
}

int ParticleParams::getNumParticles(){ return _numParticles; }

void ParticleParams::refreshNumParticles()
{
	_numParticles = _numPartSlide->value();
	_numPartEdit->setText( QString::number(_numParticles) );
}

void ParticleParams::paramEdited()
{
	_numPartSlide->setValue( _numPartEdit->text().toInt() );
}