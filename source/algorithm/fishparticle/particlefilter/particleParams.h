#ifndef PARTICLEPARAMS_H
#define PARTICLEPARAMS_H

#include <memory>
#include <QApplication>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QSlider>
#include <QObject>
#include <QLabel>
#include "utility/SpinBoxWithSlider.h"

namespace PARTICLEFILTERPARAM
{
    static const std::string NUMBER_OF_PARTICLES      = "PARTICLEFILTERPARAM.NUMBER_OF_PARTICLES";
    static const std::string GAUSSIAN_BLUR_SIZE       = "PARTICLEFILTERPARAM.GAUSSIAN_BLUR_SIZE";
    static const std::string CLUSTERING_EPSILON       = "PARTICLEFILTERPARAM.CLUSTERING_EPSILON";
    static const std::string MINIMUM_SCORE            = "PARTICLEFILTERPARAM.MINIMUM_SCORE";
    static const std::string MAX_PARTICLES_PER_BUCKET = "PARTICLEFILTERPARAM.MAX_PARTICLES_PER_BUCKET";
    static const std::string BUCKET_SIZE              = "PARTICLEFILTERPARAM.BUCKET_SIZE";
    static const std::string PARTICLE_WIGGLE_DISTANCE = "PARTICLEFILTERPARAM.PARTICLE_WIGGLE_DISTANCE";
    static const std::string NUM_CLUSTERS             = "PARTICLEFILTERPARAM.NUM_CLUSTERS";
}

class Settings;

class ParticleParams : public QObject
{
	Q_OBJECT
public:
	ParticleParams( QWidget *parent, Settings & settings );
	~ParticleParams();
	std::shared_ptr<QWidget> getParamsWidget();
	void loadParamsFromSettings();
	void saveParamsToSettings();

	int      getNumParticles()           const { return _numParticles; }
	int      getClusteringEpsilon()      const { return _clusteringEpsilon; }
	double   getMinScore()               const { return _minScore; }
	unsigned getMaxParticlesPerBucket()  const { return _maxParticlesPerBucket; }
	unsigned getBucketSize()             const { return _bucketSize; }
	double   getParticleWiggleDistance() const { return _particleWiggleDistance; }
	unsigned getNumberOfClusters()       const { return _numClusters; }
	int      getGaussianBlurSize()       const { return _gaussianBlurSize; }


private:
	std::shared_ptr<QWidget> _paramsFrame;
	Settings & _settings;
	//parameters:

	/**
	* The number of particles used by the algorithm. Higher value leads to
	* better accuracy, more detection and lower framerate.
	* default: 1000
	*/
	int _numParticles;

	/**
	* The termination criterion for the particle clustering (k-means). Will
	* terminate if clusters move less than this value (pixels). Smaller: more
	* accurate, but slower.
	* type: int, value > 0
	* default: 3
	*/
	int _clusteringEpsilon;

	/**
	* The minimum score a particle will get by the observer. Smaller value leads
	* to more clustering around particles with high scores.
	* default: 10.0
	*/
	double _minScore;

	/**
	* The maximum number of particles allowed in a section of the frame. Small
	* number leads to fish losing all particles, high number leads to few fish
	* attracting all particles.
	* default: 25
	*/
	unsigned _maxParticlesPerBucket;

	/**
	* The width and height (pixels) of the buckets the frame is divided into.
	* type: unsigned
	* default: 15
	*/
	unsigned _bucketSize;

	/**
	* The standard deviation by which a low-scoring particle will be randomly
	* moved during resampling. Less for higher-scoring particles.
	* type: double
	* default: 7.0
	*/
	double _particleWiggleDistance;

	/**
	* number of clusters (k-means). Must be accurate.
	* default: 5
	*/
	unsigned _numClusters;

	/**
	* The size of the gaussian blur applied to the background subtracted image.
	* type: int, value % 2 == 1, value > 0
	* default: 7
	*/
	int _gaussianBlurSize;

	//gui elements to set paramaters:
	SpinBoxWithSlider       *_numPartSlide;
	SpinBoxWithSlider       *_clusteringEpsSlide;
	DoubleSpinBoxWithSlider *_minScoreSlide;
	SpinBoxWithSlider       *_maxBucketPartSlide;
	SpinBoxWithSlider       *_bucketSizeSlide;
	SpinBoxWithSlider       *_gaussianBlurSizeSlide;
	DoubleSpinBoxWithSlider *_wiggleDistanceSlide;
	SpinBoxWithSlider       *_numClustersSlide;

private slots:

	void numPartChanged(int val)           { _numParticles = val; }
	void clusteringEpsChanged(int val)     {_clusteringEpsilon = val; }
	void minScoreChanged(double val)       { _minScore = val; }
	void maxBucketPartChanged(int val)     { _maxParticlesPerBucket = static_cast<unsigned>(val); }
	void bucketSizeChanged(int val)        { _bucketSize = static_cast<unsigned>(val); }
	void gaussianBlurSizeChanged(int val)  { _gaussianBlurSize = val; }
	void wiggleDistanceChanged(double val) { _particleWiggleDistance = val; }
	void numClustersChanged(int val)       { _numClusters = static_cast<unsigned>(val); }

private:

	void initParamsFrame();
	void init();
};
#endif // !PARTICLEPARAMS

