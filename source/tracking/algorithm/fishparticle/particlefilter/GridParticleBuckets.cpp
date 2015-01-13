#include "GridParticleBuckets.h"

/**
* Constructs a new object (can be used for a single resampling step),
* initializing everything so that putInBucket() can be called without further
* preparation.
* @param max_per_bucket The maximum amount of particles allowed in a single
*                       grid cell.
* @param real_rows The number of rows (pixels) in the preprocessed frame.
* @param real_cols The number of columns (pixels) in the preprocessed frame.
* @param bucket_width The width (in columns) of a single bucket/grid cell. Must
*                     not be 0.
* @param bucket_height The height (in rows) of a single bucket/grid cell. Must
*                      not be 0.
*/
GridParticleBuckets::GridParticleBuckets(std::size_t max_per_bucket, std::size_t real_rows, std::size_t real_cols,
										 std::size_t bucket_width, std::size_t bucket_height) : 
		_real_rows(real_rows), _real_cols(real_cols), _bucket_width(bucket_width),
			_bucket_height(bucket_height), _max_per_bucket(max_per_bucket), _buckets()
{
	_buckets.resize((real_cols/bucket_width) + 1);
	for (auto& v : _buckets) {
		v.resize((real_rows/bucket_height) + 1);
	}
}

GridParticleBuckets::~GridParticleBuckets(void)
{
}

/**
* Sorts a particle into its corresponding grid cell, increasing that cell's
* count by one.
* Returns true if the particle still fits into the cell, returns false if that
* cell's maximum particle count has already been reached.
*/
bool GridParticleBuckets::putInBucket(const Particle &particle) {
	std::size_t& candidate = getBucketForParticle(particle);
	if (candidate >= _max_per_bucket) {
		return false;
	}
	++candidate;
	return true;
}

/**
* Returns a reference to the count of the grid cell at the particle's 2D (x,y)
* position.
*/
std::size_t& GridParticleBuckets::getBucketForParticle(const Particle& particle) {
	float x = particle.getX();
	float y = particle.getY();

	std::size_t bucket_x = static_cast<size_t>(x / _bucket_width);
	std::size_t bucket_y = static_cast<size_t>(y / _bucket_height);

	return _buckets[bucket_x][bucket_y];
}
