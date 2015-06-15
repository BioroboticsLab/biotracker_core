#ifndef PARTICLE_CLUSTERS_H_
#define PARTICLE_CLUSTERS_H_

#include <opencv2/opencv.hpp>

#include "Particle.h"
#include "tracking/algorithm/fishparticle/particlefilter/particleParams.h"

/**
* A class for clustering particles based on their 2D (x, y) coordinates into k
* clusters using k-means clustering.
*/
class ParticleClusters
{
public:
	explicit ParticleClusters(ParticleParams& params);
	~ParticleClusters(void);
	void cluster(const std::vector<Particle>& particles, unsigned num_clusters);
	unsigned getClosestClusterIndex(const Particle& particle);
	const cv::Mat centers();
	const cv::Mat labels();
	void clear();
private:
	/**
	* The parameters used by this algorithm.
	*/
	ParticleParams& _params;

	/**
	* The resulting center coordinates obtained from the k-means algorithm.
	*/
	cv::Mat _centers;

	/**
	* The labeling map, mapping data points (the particle's index) to the
	* cluster center index.
	*/
	cv::Mat _labels;
};

#endif
