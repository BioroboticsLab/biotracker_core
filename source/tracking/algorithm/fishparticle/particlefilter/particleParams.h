#ifndef PARTICLEPARAMS_H
#define PARTICLEPARAMS_H

#include <QApplication>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QSlider>
#include <QObject>
#include <QLabel>
#include "settings/Settings.h"

class ParticleParams : public QObject
{
	Q_OBJECT
public:
	ParticleParams( QWidget *parent, Settings & settings );
	~ParticleParams();	
	QWidget* getParamsWidget();
	QWidget* getToolsWidget();	
	void loadParamsFromSettings();
	void saveParamsToSettings();
	int getNumParticles();

private:
	Settings & _settings;
	//parameters:	
	int _numParticles;
	float _resampleProportion;
	int _resampleSteps;
	float _noiseOfCoordinates;
	float _noiseOfAngle;
	int _maxParticlesInFamily;
	float _observerPosVar;
	float _observerAngleVar;
	int _observerScoreMinimum;
	QWidget * _parent;

	//gui elements to set paramaters:
	QSlider * _numPartSlide;
	QLineEdit * _numPartEdit;

	void initGuiElements();
	void init();
	void makeConnects();

	public slots:
	void refreshNumParticles();
	void paramEdited();
};
#endif // !PARTICLEPARAMS

