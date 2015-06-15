#ifndef GRID_PARTICLE_BUCKETS_H_
#define GRID_PARTICLE_BUCKETS_H_

#include <array>
#include <vector>

#include "Particle.h"

/**
* This class is intended to be used to avoid particles clustering too much in a
* small amount of 2D space.
* To ensure this, the tracking frame is divided by a grid into rectangular
* buckets. Then, the particles given to this object are sorted into one of
* those grid cells and the count per cell is stored and checked against the
* maximum allowed count per cell.
*/
class GridParticleBuckets
{
public:
	GridParticleBuckets(std::size_t max_per_bucket, std::size_t real_rows, std::size_t real_cols, std::size_t bucket_width, std::size_t bucket_height);
	~GridParticleBuckets(void);

	bool putInBucket(const Particle &particle);

private:
	/**
	* The width (in number of colums) of a single bucket. Must not be 0.
	*/
	const std::size_t _bucket_width;

	/**
	* The height (in number of rows) of a single bucket. Must not be 0.
	*/
	const std::size_t _bucket_height;

	/**
	* The maximum number of particles allowed in a single bucket.
	*/
	const std::size_t _max_per_bucket;

	/**
	* The buckets themselves, organized into a 2D array of columns and rows.
	*/
	std::vector<std::vector<std::size_t>> _buckets;

	std::size_t& getBucketForParticle(const Particle& particle);
};

#endif
