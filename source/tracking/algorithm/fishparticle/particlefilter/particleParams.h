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
#include "settings/Settings.h"

class ParticleParams : public QObject
{
	Q_OBJECT
public:
	ParticleParams( QWidget *parent, Settings & settings );
	~ParticleParams();	
    std::shared_ptr<QWidget> getParamsWidget();
	void loadParamsFromSettings();
	void saveParamsToSettings();

	int			getNumParticles()			{ return _numParticles; }
	int			getClusteringEpsilon()		{ return _clusteringEpsilon; }
	double		getMinScore()				{ return _minScore; }
	unsigned	getMaxParticlesPerBucket()	{ return _maxParticlesPerBucket; }
	unsigned	getBucketSize()				{ return _bucketSize; }
	double		getParticleWiggleDistance()	{ return _particleWiggleDistance; }
	unsigned	getNumberOfClusters()		{ return _numClusters; }
	int			getGaussianBlurSize()		{ return _gaussianBlurSize; }
	

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
	QSlider *	_numPartSlide;
	QLineEdit * _numPartEdit;
	QSlider *	_clusteringEpsSlide;
	QLineEdit * _clusteringEpsEdit;
	QSlider *	_minScoreSlide;
	QLineEdit * _minScoreEdit;
	QSlider *	_maxBucketPartSlide;
	QLineEdit *	_maxBucketPartEdit;
	QSlider *	_bucketSizeSlide;
	QLineEdit * _bucketSizeEdit;
	QSlider *	_gaussianBlurSizeSlide;
	QLineEdit * _gaussianBlurSizeEdit;
	QSlider *	_wiggleDistanceSlide;
	QLineEdit * _wiggleDistanceEdit;
	QSlider *	_numClustersSlide;
	QLineEdit * _numClustersEdit;

    void initParamsFrame();
	void init();
	void makeConnects();

	public slots:
	void paramSlided();
	void paramEdited();
};
#endif // !PARTICLEPARAMS

