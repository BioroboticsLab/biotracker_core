#ifndef PARTICLE_H_
#define PARTICLE_H_

/**
* Represents a 3D (x, y, angle) particle used in the particle filter. Has an ID
* and a few utility methods (for calculating distances to other particles etc.).
*/
class Particle
{
public:
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
	float getX() const { return _x; }

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
	float getY() const { return _y; }

	/**
	 * Standard setter for the angle of the particle. IN DEGREES.
	 */
	void setAngleDegree(float angle_degree) { _angle_degree = angle_degree; }

	/**
	 * Standard getter for the angle of the particle.
	 */
	float getAngleDegree() const { return _angle_degree; }

	/**
	* Sets the score for this particle, given by the observer.
	*/
	void setScore(float score) {
		_score = score;
	}

	/**
	* Returns the score for this particle, given by the observer.
	*/
	float getScore() const {
		return _score;
	}

	/**
	 * Sets the ID of the particle. IDs	should be unique within a set of particles.
	 */
	void setID(int id) { _id = id; };
	
	/**
	 * Gets the ID of the particle.	IDs should be unique within a set of particles.
	 */
	int getID() const { return _id; };

	double dist(const Particle& p) const;

private:
	/**
	 * The x position of the particle. One of the particle dimensions.
	 */
	float _x;

	/**
	 * The y position of the particle. One of the particle dimensions.
	 */
	float _y;

	/**
	 * The angle of the particle. One of the particle dimensions.
	 */
	float _angle_degree;

	/**
	* The score, given by the observer, for this particle.
	*/
	float _score;

	/**
	 * The ID of this particle. Should be unique within a set of particles.
	 */
	int _id;
};

#endif
