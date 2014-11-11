#include "particleParams.h"

ParticleParams::ParticleParams(QWidget *parent, Settings & settings) :
QObject(parent)
, _paramsFrame(std::make_shared<QFrame>())
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
    initParamsFrame();
	makeConnects();
}

void ParticleParams::initParamsFrame()
{
    _numPartSlide = new QSlider(_paramsFrame.get());
	_numPartSlide->setMaximum(5000);
	_numPartSlide->setMinimum(0);
	_numPartSlide->setOrientation(Qt::Horizontal);
	_numPartSlide->setValue(_numParticles);
    _numPartEdit = new QLineEdit(_paramsFrame.get());
	_numPartEdit->setText(QString::number(_numParticles));
    _numPartEdit->setValidator(new QIntValidator(0, _numPartSlide->maximum(), _paramsFrame.get()));

    QFormLayout *layout = new QFormLayout(_paramsFrame.get());
    QHBoxLayout *row = new QHBoxLayout();

    row->addWidget(new QLabel("Number of Particles", _paramsFrame.get()));
    row->addWidget(_numPartSlide);
    row->addWidget(_numPartEdit);
    layout->addRow(row);
}

std::shared_ptr<QWidget> ParticleParams::getParamsWidget()
{
    return _paramsFrame;
}

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
	catch (std::invalid_argument&)
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
