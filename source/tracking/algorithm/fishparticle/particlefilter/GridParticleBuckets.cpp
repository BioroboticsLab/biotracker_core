#include "GridParticleBuckets.h"


GridParticleBuckets::GridParticleBuckets(std::size_t max_per_bucket, std::size_t real_rows, std::size_t real_cols,
										 std::size_t bucket_rows, std::size_t bucket_cols) : 
		_real_rows(real_rows), _real_cols(real_cols), _bucket_rows(bucket_rows),
			_bucket_cols(bucket_cols), _max_per_bucket(max_per_bucket), _buckets()
{
	_buckets.resize(bucket_rows);
	for (auto& v : _buckets) {
		v.resize(bucket_cols);
	}
}

GridParticleBuckets::~GridParticleBuckets(void)
{
}

bool GridParticleBuckets::putInBucket(Particle particle) {
	std::vector<Particle>& candidate = getBucketForParticle(particle);
	return true;
}

std::vector<Particle>& GridParticleBuckets::getBucketForParticle(const Particle& particle) {
	// TODO impl better, correct and without out-of-bounds.
	return _buckets[0][0];
}
