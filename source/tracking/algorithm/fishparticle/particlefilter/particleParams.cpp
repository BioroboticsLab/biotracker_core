#include "particleParams.h"

ParticleParams::ParticleParams(QWidget *parent, Settings & settings) :
QObject(parent)
, _paramsFrame(std::make_shared<QFrame>())
, _settings(settings)
, _numParticles(1000)
, _clusteringEpsilon(3)
, _minScore(10.0)
, _maxParticlesPerBucket(25)
, _bucketSize(15)
, _particleWiggleDistance(7.0)
, _numClusters(5)
, _gaussianBlurSize(7)
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

	_clusteringEpsSlide = new QSlider(_paramsFrame.get());
	_clusteringEpsSlide->setMaximum(10);
	_clusteringEpsSlide->setMinimum(0);
	_clusteringEpsSlide->setOrientation(Qt::Horizontal);
	_clusteringEpsSlide->setValue(_clusteringEpsilon);
	_clusteringEpsEdit = new QLineEdit(_paramsFrame.get());
	_clusteringEpsEdit->setText(QString::number(_clusteringEpsilon));
	_clusteringEpsEdit->setValidator(new QIntValidator(0, _clusteringEpsSlide->maximum(), _paramsFrame.get()));

	_minScoreSlide = new QSlider(_paramsFrame.get());
	_minScoreSlide->setMaximum(100);
	_minScoreSlide->setMinimum(0);
	_minScoreSlide->setOrientation(Qt::Horizontal);
	_minScoreSlide->setValue(_minScore);
	_minScoreEdit = new QLineEdit(_paramsFrame.get());
	_minScoreEdit->setText(QString::number(_minScore));
	_minScoreEdit->setValidator(new QDoubleValidator((double)0, (double) _minScoreSlide->maximum(), 0, _paramsFrame.get()));

	_maxBucketPartSlide = new QSlider(_paramsFrame.get());
	_maxBucketPartSlide->setMaximum(100);
	_maxBucketPartSlide->setMinimum(0);
	_maxBucketPartSlide->setOrientation(Qt::Horizontal);
	_maxBucketPartSlide->setValue(_maxParticlesPerBucket);
	_maxBucketPartEdit = new QLineEdit(_paramsFrame.get());
	_maxBucketPartEdit->setText(QString::number(_maxParticlesPerBucket));
	_maxBucketPartEdit->setValidator(new QIntValidator(0, _maxBucketPartSlide->maximum(), _paramsFrame.get()));

	_bucketSizeSlide = new QSlider(_paramsFrame.get());
	_bucketSizeSlide->setMaximum(1000);
	_bucketSizeSlide->setMinimum(0);
	_bucketSizeSlide->setOrientation(Qt::Horizontal);
	_bucketSizeSlide->setValue(_bucketSize);
	_bucketSizeEdit = new QLineEdit(_paramsFrame.get());
	_bucketSizeEdit->setText(QString::number(_bucketSize));
	_bucketSizeEdit->setValidator(new QIntValidator(0, _bucketSizeSlide->maximum(), _paramsFrame.get()));

	_gaussianBlurSizeSlide = new QSlider(_paramsFrame.get());
	_gaussianBlurSizeSlide->setMaximum(20);
	_gaussianBlurSizeSlide->setMinimum(0);
	_gaussianBlurSizeSlide->setOrientation(Qt::Horizontal);
	_gaussianBlurSizeSlide->setValue(_gaussianBlurSize);
	_gaussianBlurSizeEdit = new QLineEdit(_paramsFrame.get());
	_gaussianBlurSizeEdit->setText(QString::number(_bucketSize));
	_gaussianBlurSizeEdit->setValidator(new QIntValidator(0, _gaussianBlurSizeSlide->maximum(), _paramsFrame.get()));

	_wiggleDistanceSlide = new QSlider(_paramsFrame.get());
	_wiggleDistanceSlide->setMaximum(20);
	_wiggleDistanceSlide->setMinimum(0);
	_wiggleDistanceSlide->setOrientation(Qt::Horizontal);
	_wiggleDistanceSlide->setValue(_particleWiggleDistance);
	_wiggleDistanceEdit = new QLineEdit(_paramsFrame.get());
	_wiggleDistanceEdit->setText(QString::number(_particleWiggleDistance));
	_wiggleDistanceEdit->setValidator(new QDoubleValidator((double)0, (double)_wiggleDistanceSlide->maximum(), 0, _paramsFrame.get()));

	_numClustersSlide = new QSlider(_paramsFrame.get());
	_numClustersSlide->setMaximum(20);
	_numClustersSlide->setMinimum(0);
	_numClustersSlide->setOrientation(Qt::Horizontal);
	_numClustersSlide->setValue(_numClusters);
	_numClustersEdit = new QLineEdit(_paramsFrame.get());
	_numClustersEdit->setText(QString::number(_numClusters));
	_numClustersEdit->setValidator(new QIntValidator(0, _numClustersSlide->maximum(), _paramsFrame.get()));



    QFormLayout *layout = new QFormLayout(_paramsFrame.get());
	QHBoxLayout *row[8];
	for (auto &i : row)	
		i = new QHBoxLayout();	

	row[0]->addWidget(new QLabel("Number of Particles", _paramsFrame.get()));
    row[0]->addWidget(_numPartSlide);
    row[0]->addWidget(_numPartEdit);

	row[1]->addWidget(new QLabel("Clustering Epsilon", _paramsFrame.get()));
	row[1]->addWidget(_clusteringEpsSlide);
	row[1]->addWidget(_clusteringEpsEdit);

	row[2]->addWidget(new QLabel("Minimum Score", _paramsFrame.get()));
	row[2]->addWidget(_minScoreSlide);
	row[2]->addWidget(_minScoreEdit);

	row[3]->addWidget(new QLabel("Maximum Bucket Particles", _paramsFrame.get()));
	row[3]->addWidget(_maxBucketPartSlide);
	row[3]->addWidget(_maxBucketPartEdit);

	row[4]->addWidget(new QLabel("Bucket size", _paramsFrame.get()));
	row[4]->addWidget(_bucketSizeSlide);
	row[4]->addWidget(_bucketSizeEdit);

	row[5]->addWidget(new QLabel("Gaussian Blur Size", _paramsFrame.get()));
	row[5]->addWidget(_gaussianBlurSizeSlide);
	row[5]->addWidget(_gaussianBlurSizeEdit);

	row[6]->addWidget(new QLabel("Particle Wiggle Distance", _paramsFrame.get()));
	row[6]->addWidget(_wiggleDistanceSlide);
	row[6]->addWidget(_wiggleDistanceEdit);

	row[7]->addWidget(new QLabel("Number of Clusters", _paramsFrame.get()));
	row[7]->addWidget(_numClustersSlide);
	row[7]->addWidget(_numClustersEdit);
    
	
	for (QHBoxLayout * i : row)
		layout->addRow(i);
}

std::shared_ptr<QWidget> ParticleParams::getParamsWidget()
{
    return _paramsFrame;
}

void ParticleParams::makeConnects()
{
	QObject::connect(this->_numPartSlide, SIGNAL(valueChanged(int)), this, SLOT(paramSlided()));
	QObject::connect(this->_numPartEdit, SIGNAL(editingFinished()), this, SLOT(paramEdited()));
	
	QObject::connect(this->_clusteringEpsSlide, SIGNAL(valueChanged(int)), this, SLOT(paramSlided()));
	QObject::connect(this->_clusteringEpsEdit, SIGNAL(editingFinished()), this, SLOT(paramEdited()));

	QObject::connect(this->_minScoreSlide, SIGNAL(valueChanged(int)), this, SLOT(paramSlided()));
	QObject::connect(this->_minScoreEdit, SIGNAL(editingFinished()), this, SLOT(paramEdited()));

	QObject::connect(this->_maxBucketPartSlide, SIGNAL(valueChanged(int)), this, SLOT(paramSlided()));
	QObject::connect(this->_maxBucketPartEdit, SIGNAL(editingFinished()), this, SLOT(paramEdited()));

	QObject::connect(this->_bucketSizeSlide, SIGNAL(valueChanged(int)), this, SLOT(paramSlided()));
	QObject::connect(this->_bucketSizeEdit, SIGNAL(editingFinished()), this, SLOT(paramEdited()));

	QObject::connect(this->_gaussianBlurSizeSlide, SIGNAL(valueChanged(int)), this, SLOT(paramSlided()));
	QObject::connect(this->_gaussianBlurSizeEdit, SIGNAL(editingFinished()), this, SLOT(paramEdited()));

	QObject::connect(this->_wiggleDistanceSlide, SIGNAL(valueChanged(int)), this, SLOT(paramSlided()));
	QObject::connect(this->_wiggleDistanceEdit, SIGNAL(editingFinished()), this, SLOT(paramEdited()));

	QObject::connect(this->_numClustersSlide, SIGNAL(valueChanged(int)), this, SLOT(paramSlided()));
	QObject::connect(this->_numClustersEdit, SIGNAL(editingFinished()), this, SLOT(paramEdited()));
	
}

void ParticleParams::loadParamsFromSettings()
{
	try
	{
		_numParticles = _settings.getValueOfParam<int>(PARTICLEFILTERPARAM::NUMBER_OF_PARTICLES);
		_clusteringEpsilon = _settings.getValueOfParam<int>(PARTICLEFILTERPARAM::CLUSTERING_EPSILON);
		_minScore = _settings.getValueOfParam<double>(PARTICLEFILTERPARAM::MINIMUM_SCORE);
		_maxParticlesPerBucket = (unsigned)_settings.getValueOfParam<int>(PARTICLEFILTERPARAM::MAX_PARTICLES_PER_BUCKET);
		_bucketSize = (unsigned) _settings.getValueOfParam<int>(PARTICLEFILTERPARAM::BUCKET_SIZE);
		_particleWiggleDistance = _settings.getValueOfParam<double>(PARTICLEFILTERPARAM::PARTICLE_WIGGLE_DISTANCE);
		_numClusters = (unsigned) _settings.getValueOfParam<int>(PARTICLEFILTERPARAM::NUM_CLUSTERS);
		_gaussianBlurSize = _settings.getValueOfParam<int>(PARTICLEFILTERPARAM::GAUSSIAN_BLUR_SIZE);
	}	
	catch (std::invalid_argument&)
	{
		//TODO: send GUI MSG that no previous settings were found
	}
}

void ParticleParams::saveParamsToSettings()
{
	_settings.setParam(PARTICLEFILTERPARAM::NUMBER_OF_PARTICLES, std::to_string(_numParticles));
	_settings.setParam(PARTICLEFILTERPARAM::CLUSTERING_EPSILON, std::to_string(_clusteringEpsilon));
	_settings.setParam(PARTICLEFILTERPARAM::MINIMUM_SCORE, std::to_string(_minScore));
	_settings.setParam(PARTICLEFILTERPARAM::MAX_PARTICLES_PER_BUCKET, std::to_string(_maxParticlesPerBucket));
	_settings.setParam(PARTICLEFILTERPARAM::BUCKET_SIZE, std::to_string(_bucketSize));
	_settings.setParam(PARTICLEFILTERPARAM::PARTICLE_WIGGLE_DISTANCE, std::to_string(_particleWiggleDistance));
	_settings.setParam(PARTICLEFILTERPARAM::NUM_CLUSTERS, std::to_string(_numClusters));
	_settings.setParam(PARTICLEFILTERPARAM::GAUSSIAN_BLUR_SIZE, std::to_string(_gaussianBlurSize));
}


void ParticleParams::paramSlided()
{
	_numParticles = _numPartSlide->value();
	_numPartEdit->setText( QString::number(_numParticles) );

	_numClusters = _numClustersSlide->value();
	_numClustersEdit->setText(QString::number(_numClusters));

	_minScore = _minScoreSlide->value();
	_minScoreEdit->setText(QString::number(_minScore));

	_clusteringEpsilon = _clusteringEpsSlide->value();
	_clusteringEpsEdit->setText(QString::number(_clusteringEpsilon));

	_maxParticlesPerBucket = _maxBucketPartSlide->value();
	_maxBucketPartEdit->setText(QString::number(_maxParticlesPerBucket));

	_gaussianBlurSize = _gaussianBlurSizeSlide->value();
	_gaussianBlurSizeEdit->setText(QString::number(_gaussianBlurSize));

	_particleWiggleDistance = _wiggleDistanceSlide->value();
	_wiggleDistanceEdit->setText(QString::number(_particleWiggleDistance));

	_bucketSize = _bucketSizeSlide->value();
	_bucketSizeEdit->setText(QString::number(_bucketSize));
}

void ParticleParams::paramEdited()
{
	_numPartSlide->setValue( _numPartEdit->text().toInt() );
	_numClustersSlide->setValue(_numClustersEdit->text().toUInt());
	_minScoreSlide->setValue(_minScoreEdit->text().toDouble());
	_clusteringEpsSlide->setValue(_clusteringEpsEdit->text().toInt());
	_maxBucketPartSlide->setValue(_maxBucketPartEdit->text().toUInt());
	_gaussianBlurSizeSlide->setValue(_gaussianBlurSizeEdit->text().toInt());
	_wiggleDistanceSlide->setValue(_wiggleDistanceEdit->text().toDouble());
	_bucketSizeSlide->setValue(_bucketSizeEdit->text().toUInt());
}
