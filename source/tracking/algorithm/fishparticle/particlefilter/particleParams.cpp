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
}

void ParticleParams::initParamsFrame()
{
	const auto p = _paramsFrame.get();
	QFormLayout *layout = new QFormLayout(_paramsFrame.get());

	_numPartSlide          = new SpinBoxWithSlider(      p, layout, "Number of Particles",      0 , 5000 , _numParticles);
	_clusteringEpsSlide    = new SpinBoxWithSlider(      p, layout, "Clustering Epsilon",       0 ,   10 , _clusteringEpsilon);
	_minScoreSlide         = new DoubleSpinBoxWithSlider(p, layout, "Minimum Score",            0.,  100., _minScore);
	_maxBucketPartSlide    = new SpinBoxWithSlider(      p, layout, "Maximum Bucket Particles", 0 ,  100 , _maxParticlesPerBucket);
	_bucketSizeSlide       = new SpinBoxWithSlider(      p, layout, "Bucket size",              0 , 1000 , _bucketSize);
	_gaussianBlurSizeSlide = new SpinBoxWithSlider(      p, layout, "Gaussian Blur Size",       1 ,   21 , _gaussianBlurSize, 2);
	_wiggleDistanceSlide   = new DoubleSpinBoxWithSlider(p, layout, "Particle Wiggle Distance", 0.,   20., _particleWiggleDistance);
	_numClustersSlide      = new SpinBoxWithSlider(      p, layout, "Number of Clusters",       0 ,   20 , _numClusters);

	QObject::connect(_numPartSlide,          SIGNAL(valueChanged(int)),
	                 this,                   SLOT(numPartChanged(int)));
	QObject::connect(_clusteringEpsSlide,    SIGNAL(valueChanged(int)),
	                 this,                   SLOT(clusteringEpsChanged(int)));
	QObject::connect(_minScoreSlide,         SIGNAL(valueChanged(double)),
	                 this,                   SLOT(minScoreChanged(double)));
	QObject::connect(_maxBucketPartSlide,    SIGNAL(valueChanged(int)),
	                 this,                   SLOT(maxBucketPartChanged(int)));
	QObject::connect(_bucketSizeSlide,       SIGNAL(valueChanged(int)),
	                 this,                   SLOT(bucketSizeChanged(int)));
	QObject::connect(_gaussianBlurSizeSlide, SIGNAL(valueChanged(int)),
	                 this,                   SLOT(gaussianBlurSizeChanged(int)));
	QObject::connect(_wiggleDistanceSlide,   SIGNAL(valueChanged(double)),
	                 this,                   SLOT(wiggleDistanceChanged(double)));
	QObject::connect(_numClustersSlide,      SIGNAL(valueChanged(int)),
	                 this,                   SLOT(numClustersChanged(int)));
}

std::shared_ptr<QWidget> ParticleParams::getParamsWidget()
{
	return _paramsFrame;
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
