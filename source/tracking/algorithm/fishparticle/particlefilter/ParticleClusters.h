#ifndef PARTICLE_CLUSTERS_H_
#define PARTICLE_CLUSTERS_H_

#include <opencv2/opencv.hpp>

#include "Particle.h"
#include "tracking/algorithm/fishparticle/particlefilter/particleParams.h"

class ParticleClusters
{
public:
	explicit ParticleClusters(Settings& settings, ParticleParams& params);
	~ParticleClusters(void);
	void cluster(const std::vector<Particle>& particles, unsigned num_clusters);
	unsigned getClosestClusterIndex(const Particle& particle);
	const cv::Mat centers();
	const cv::Mat labels();
	void clear();
private:
	Settings& _settings;
	ParticleParams& _params;
	cv::Mat _centers;
	cv::Mat _labels;
};

#endif
