#include "ParticleFishTracker.h"

#include <opencv2/opencv.hpp>

#include "particlefilter/ParticleBrightnessObserver.h"

#include <utility> // std::move


static bool compareReverseParticleScorePredicate(const Particle& p1, const Particle& p2);

/**
* Constructs a new instance using the tracking and special particle tracker settings set in settings.
*/
ParticleFishTracker::ParticleFishTracker(Settings& settings, QWidget *parent)
    : TrackingAlgorithm(settings, parent)
    , _preprocessor(settings)
    , _rng(123)
    , _max_score(0)
    , _min_score(0)
    , _clusters(settings)
{
}

ParticleFishTracker::~ParticleFishTracker(void)
{
}

/**
* Does the main work, detecting tracked objects (fish) and building a history for those objects.
*/
void ParticleFishTracker::track( unsigned long, cv::Mat& frame) {
	try {
		// TODO check if frameNumber is jumping -> should lead to reseed

		// (1) Preprocess frame
		_prepared_frame = _preprocessor.preProcess(frame);

		// (2) Resampling (importance resampling) or seeding
		if (_current_particles.empty()) {
			// TODO params for this algorithm in settings.
			seedParticles(1000, 0, 0, frame.cols, frame.rows);
		} else {
			ParticleBrightnessObserver observer(_prepared_frame);
			_sum_scores = 0;
			for (Particle& p : _current_particles) {
				observer.score(p);
				if (p.getScore() > _max_score) {
					_max_score = p.getScore();
				}
				if (p.getScore() < _min_score) {
					_min_score = p.getScore();
				}
				_sum_scores += p.getScore();
			}
			// Resample
			// - Sort for better performance (big scores first)
			std::sort(_current_particles.begin(), _current_particles.end(), compareReverseParticleScorePredicate);
			// - importance resampling
			importanceResample();
		}

		// (3) Clustering
		_clusters.cluster(_current_particles, 5);

		// (4) Store results in history
		// TODO
	} catch (cv::Exception exc) {
		emit notifyGUI(exc.what(), MSGS::FAIL);
	}
}

/**
* Creates a set of new particles based on the old ones.
* For each of the new particles an ancestor particle is chosen randomly from
* the old particles, weighted proportionally to the old particles' score. The
* new particle inherits all properties from its ancestor, but will be slightly
* moved randomly (gaussian) in all dimensions.
*/
void ParticleFishTracker::importanceResample() {
	// Make a copy and generate new particles.
	size_t random_new_particles = _current_particles.size() / 100;
	std::vector<unsigned> cluster_counts(_clusters.centers().rows);
	std::vector<Particle> old_particles = std::move(_current_particles);
	_current_particles.clear();

	for (size_t i = 0; i < old_particles.size()-random_new_particles; i++) {
		size_t index = 0;
		float rand = _rng.uniform(0.f, _sum_scores);
		for (float position = 0; position + old_particles[index].getScore() < rand; ) {
			position += old_particles[index].getScore();
			++index;
		}
		Particle to_wiggle = old_particles[index];
		//if (!(++cluster_counts[_clusters.getClosestClusterIndex(to_wiggle)] > 200) || retry_count > 10) {
		wiggleParticle(to_wiggle);
		_current_particles.push_back(to_wiggle);
	}
	seedParticles(random_new_particles, 0, 0, _prepared_frame.cols, _prepared_frame.rows);
}

/**
* Moves a particle randomly (gaussian) in all its dimensions. Low-scoring
* particles are wiggled more. The wiggling is restricted to within the
* _prepared_frame's dimensions.
*/
void ParticleFishTracker::wiggleParticle(Particle& to_wiggle) {
	float wiggle_distance;
	if (_max_score != _min_score) {
		wiggle_distance = 7 * ((_max_score - to_wiggle.getScore()) / (_max_score - _min_score));
	} else {
		wiggle_distance = 7;
	}
	to_wiggle.setX(to_wiggle.getX() + _rng.gaussian(wiggle_distance));
	to_wiggle.setY(to_wiggle.getY() + _rng.gaussian(wiggle_distance));
	cutParticleCoords(to_wiggle);
}

/**
* Restricts a particle's x and y coordinates to the current frame's size.
*/
void ParticleFishTracker::cutParticleCoords(Particle& to_cut) {
	if (to_cut.getX() < 0) {
		to_cut.setX(0);
	}
	if (to_cut.getX() >= _prepared_frame.cols) {
		to_cut.setX(_prepared_frame.cols-1);
	}
	if (to_cut.getY() < 0) {
		to_cut.setY(0);
	}
	if (to_cut.getY() >= _prepared_frame.rows) {
		to_cut.setY(_prepared_frame.rows-1);
	}
}

/**
* Fills the list of current particles (_current_particles) with num_particles
* uniformly distributed particles.
*/
void ParticleFishTracker::seedParticles(unsigned num_particles, int min_x, int min_y, int max_x, int max_y) {
	for (size_t i = 0; i<num_particles; i++) {
		int x = _rng.uniform(min_x, max_x);
		int y = _rng.uniform(min_y, max_y);
		// TODO include random angle
		float a = 0;

		Particle newParticle(x, y, a, _current_particles.size() + 1);
		_current_particles.push_back(newParticle);
	}
}

/**
* Draws the result of the tracking for the current frame.
*/
void ParticleFishTracker::paint(cv::Mat& image) {
	if (!_prepared_frame.empty()) {
		cv::cvtColor(_prepared_frame, image, CV_GRAY2BGR);
		for (const Particle& p : _current_particles) {
			if (_min_score >= _max_score) {
				cv::circle(image, cv::Point(p.getX(), p.getY()), 1, cv::Scalar(0, 255, 0), -1);
			} else {
				// Scale the score of the particle to get a nice color based on score.
				unsigned scaled_score = (p.getScore() - _min_score)	/ (_max_score - _min_score) * 230;
				cv::circle(image, cv::Point(p.getX(), p.getY()), 1, cv::Scalar(0, 20 + scaled_score, 0), -1);
			}
		}

		for (int i = 0; i < _clusters.centers().rows; i++) {
			cv::Mat center = _clusters.centers().row(i);
			cv::circle(image, cv::Point(center), 4, cv::Scalar(0, 0, 255));
		}
	}
}

/**
* This algorithm has state, this can be used to reset that state. It will not
* delete the tracked objects' history.
*/
void ParticleFishTracker::reset() {
	_preprocessor.reset();
	_min_score = 0;
	_max_score = 0;
	_clusters.clear();
	// TODO reset more...?
}

void ParticleFishTracker::mouseMoveEvent		( QMouseEvent * ){}
void ParticleFishTracker::mousePressEvent		( QMouseEvent * ){}
void ParticleFishTracker::mouseReleaseEvent		( QMouseEvent * ){}
void ParticleFishTracker::mouseWheelEvent		( QWheelEvent * ){}

/**
* Predicate used by this algorithm to sort particles, highest to lowest score.
*/
static bool compareReverseParticleScorePredicate(const Particle& p1, const Particle& p2) {
	return p1.getScore() > p2.getScore();
}
