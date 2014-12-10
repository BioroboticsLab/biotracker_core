#ifndef GRID_PARTICLE_BUCKETS_H_
#define GRID_PARTICLE_BUCKETS_H_

#include <array>
#include <vector>

#include "Particle.h"

class GridParticleBuckets
{
public:
	GridParticleBuckets(std::size_t max_per_bucket, std::size_t real_rows, std::size_t real_cols, std::size_t bucket_width, std::size_t bucket_height);
	~GridParticleBuckets(void);

	bool putInBucket(const Particle &particle);

private:
	const std::size_t _real_rows;
	const std::size_t _real_cols;
	const std::size_t _bucket_width;
	const std::size_t _bucket_height;
	const std::size_t _max_per_bucket;

	std::vector<std::vector<std::size_t>> _buckets;

	std::size_t& getBucketForParticle(const Particle& particle);
};

#endif
