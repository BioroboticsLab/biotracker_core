#ifndef GRID_PARTICLE_BUCKETS_H_
#define GRID_PARTICLE_BUCKETS_H_

#include <array>
#include <vector>

#include "Particle.h"

class GridParticleBuckets
{
public:
	GridParticleBuckets(std::size_t max_per_bucket, std::size_t real_rows, std::size_t real_cols, std::size_t bucket_rows, std::size_t bucket_cols);
	~GridParticleBuckets(void);

	bool putInBucket(Particle particle);

private:
	const std::size_t _real_rows;
	const std::size_t _real_cols;
	const std::size_t _bucket_rows;
	const std::size_t _bucket_cols;
	const std::size_t _max_per_bucket;

	std::vector<std::vector<std::vector<Particle>>> _buckets;

	std::vector<Particle>& getBucketForParticle(const Particle& particle)
};

#endif
