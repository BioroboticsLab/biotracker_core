#ifndef COMPLEX_TRACKER_H_
#define COMPLEX_TRACKER_H_

#include "source/tracking/TrackingAlgorithm.h"
#include "preprocessing/FramePreprocessor.h"
#include "particlefilter/Particle.h"
#include "particlefilter/ParticleClusters.h"
#include "particlefilter/particleParams.h"

#include <mutex>

//QT Stuff
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

/**
* Implements a particle filtering algorithm to track multiple fish in a tank.
*/
class ParticleFishTracker :
	public TrackingAlgorithm
{
	Q_OBJECT
public:
    ParticleFishTracker(Settings& settings, QWidget *parent);
	virtual ~ParticleFishTracker(void);
	virtual void track( ulong frameNumber, cv::Mat& frame ) override;
	virtual void paintOverlay(QPainter *painter) override;
	virtual void reset() override;
    std::shared_ptr<QWidget> getToolsWidget	() override;
    std::shared_ptr<QWidget> getParamsWidget() override;

    public slots:
		void switchMode();

private:
    std::shared_ptr<QWidget> _toolsWidget;

	// indicating which image shall be viewed: original or tracked image
	bool _showOriginal;
	// corresponding switching button
    QPushButton *_modeBut;

	/**
	* The frame to be drawn the next time the GUI requests one.
	*/
	cv::Mat _prepared_frame;

	/**
	* The particles we are currently dealing with.
	*/
	std::vector<Particle> _current_particles;

	/**
	* Prevents a crash when drawing and modifying the particle list at the same time.
	*/
	std::mutex _current_particles_access_mutex;

	/**
	* The RNG used to generate random data for placing new particles.
	*/
	cv::RNG _rng;

	/**
	* The max. score the observer ever gave a particle.
	*/
	float _max_score;

	/**
	* The min. score the observer ever gave a particle.
	*/
	float _min_score;

	/**
	* The sum of all scores in _current_particles. Used for importance
	* resampling.
	*/
	float _sum_scores;

	/**
	* The parameters used by this algorithm.
	*/
	ParticleParams _params;

	/**
	* Used to preprocess the image (mainly background subtraction).
	*/
	FramePreprocessor _preprocessor;

	ParticleClusters _clusters;

	void seedParticles(size_t num_particles, int min_x, int min_y, int max_x, int max_y);

	void importanceResample();

	void wiggleParticle(Particle& to_wiggle);

	void cutParticleCoords(Particle& to_cut);

    void initToolsWidget();

};


#endif
