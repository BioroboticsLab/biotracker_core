#include "GridParticleBuckets.h"

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

bool GridParticleBuckets::putInBucket(Particle particle) {
	std::vector<Particle>& candidate = getBucketForParticle(particle);
	if (candidate.size() >= _max_per_bucket) {
		return false;
	}
	candidate.push_back(particle);
	return true;
}

std::vector<Particle>& GridParticleBuckets::getBucketForParticle(const Particle& particle) {
	float x = particle.getX();
	float y = particle.getY();

	std::size_t bucket_x = x / _bucket_width;
	std::size_t bucket_y = y / _bucket_height;

	return _buckets[bucket_x][bucket_y];
}
