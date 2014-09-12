#ifndef PARTICLE_CLUSTERS_H_
#define PARTICLE_CLUSTERS_H_

#include <opencv.hpp>

#include "Particle.h"
#include "source/settings/Settings.h"

class ParticleClusters
{
public:
	explicit ParticleClusters(Settings& settings);
	~ParticleClusters(void);
	void cluster(const std::vector<Particle>& particles, unsigned num_clusters);
	unsigned getClosestClusterIndex(const Particle& particle);
	const cv::Mat centers();
	const cv::Mat labels();
	void clear();
private:
	Settings& _settings;
	cv::Mat _centers;
	cv::Mat _labels;
};

#endif
