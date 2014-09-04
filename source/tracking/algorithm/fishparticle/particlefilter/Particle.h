#ifndef PARTICLE_H_
#define PARTICLE_H_

class Particle
{
public:
	Particle(void);
	Particle(int x, int y, float a, int id);
	~Particle(void);

	/**
	 * Sets the x position of the particle.
	 * @param: x, the x position.
	 * @return: void.
	 */
	void setX(float x) { _x = x; }

	/**
	 * Gets the x position of the particle.
	 * @return: the x position.
	 */
	float getX() { return _x; }

	/**
	 * Sets the y position of the particle.
	 * @param: y, the y position.
	 * @return: void.
	 */
	void setY(float y) { _y = y; }

	/**
	 * Gets the y position of the particle.
	 * @return: the y position.
	 */
	float getY() { return _y; }

	/**
	 *
	 */
	void setAngleDegree(float angle_degree) { _angle_degree = angle_degree; }

	/**
	 *
	 */
	float getAngleDegree() { return _angle_degree; }

	/**
	 *
	 */
	void setWidth(int width) { _width = width; }

	/**
	 *
	 */
	float getWidth() { return _width; }

	/**
	 *
	 */
	void setHeight(int height) { _height = height; }

	/**
	 *
	 */
	float getHeight() { return _height; }
	
	/**
	 * TODO: Kommentare ausfüllen.
	 */
	void setBelief(double belief) { _belief = belief; }

	void incrementBelief( ) {_belief++;}

	void addBelief(Particle p) {_belief = _belief + p.getBelief();}

	/**
	 * TODO: Kommentare ausfüllen.
	 */
	double getBelief() { return _belief; }

	void setID(int id) { _id = id; };
	
	int getID() { return _id; };

	bool operator < (Particle p) {return p._belief < _belief; }

	void add( Particle p );
	void mul( double a );
	void mean( Particle p );
	void mean_weighted( Particle p, float w1 = 0, float w2 = 0);
	double mean_angle( double a, double b );
	double mean_angle_weighted( double a, double b, float w1, float w2);
	double dist(Particle p);

private:
	double _belief;
	float _x;
	float _y;
	float _angle_degree;

	float _width;
	float _height;

	

	int _id;
};

#endif
