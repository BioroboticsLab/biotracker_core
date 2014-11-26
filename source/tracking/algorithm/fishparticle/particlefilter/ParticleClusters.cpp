#include "ParticleClusters.h"

#include <opencv2/opencv.hpp>


ParticleClusters::ParticleClusters(Settings& settings, ParticleParams& params)
	: _settings(settings)
	, _params(params)
	, _centers(),
	_labels()
{
}


ParticleClusters::~ParticleClusters(void)
{
}

void ParticleClusters::cluster(const std::vector<Particle>& particles, unsigned num_clusters) {
	// Prepare arguments for cv::kmeans.
	cv::Mat_<float> data(static_cast<int>(particles.size()), 2);
	for (int i = 0; i < data.rows; i++) {
		data[i][0] = particles[i].getX();
		data[i][1] = particles[i].getY();
	}
	int k = num_clusters;
	cv::Mat bestLabels;
	cv::TermCriteria criteria(cv::TermCriteria::EPS, 0, _params.getClusteringEpsilon());
	int attempts = 1;
	int flags = cv::KMEANS_PP_CENTERS;
	cv::Mat centers;

	// Do the work.
	cv::kmeans(data, k, bestLabels, criteria, attempts, flags, centers);

	// Store the results.
	_centers = centers;
	_labels = bestLabels;
}

unsigned ParticleClusters::getClosestClusterIndex(const Particle& particle) {
	unsigned min_distance_center = 0;
	float min_distance = FLT_MAX;
	for (int i = 0; i < _centers.rows; i++) {
		Particle center_particle(_centers.at<float>(i, 0), _centers.at<float>(i, 1), 0, 0);
		float distance = center_particle.dist(particle);
		if (distance < min_distance) {
			min_distance = distance;
			min_distance_center = i;
		}
	}
	return min_distance_center;
}

const cv::Mat ParticleClusters::centers() {
	return _centers;
}

const cv::Mat ParticleClusters::labels() {
	return _labels;
}

void ParticleClusters::clear() {
	// TODO delete all cluster data
}
