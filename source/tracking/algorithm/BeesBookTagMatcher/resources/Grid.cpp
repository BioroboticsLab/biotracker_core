#include "Grid.h"

namespace decoder {

// === Constructors and initializer ===
void Grid::init(float size, float angle, float tilt, int x, int y, Ellipse ell, bool permutation, ScoringMethod scoringMethod) {
	this->size = size;

	this->angle = angle;
	this->tilt = tilt;

	this->x = x;
	this->y = y;

	this->permutation = permutation;

	_score.metric = scoringMethod;
	if (scoringMethod == BINARYCOUNT) {
		_score.value = BINARYCOUNT_INIT;
	} else {
		_score.value = FISHER_INIT;
	}

	this->ell = ell;

	// Need to binarize the image, because we need it for scoring
	if (this->ell.transformedImage.type() != CV_8U) {
		Mat grayImage;
		cvtColor(this->ell.transformedImage, grayImage, CV_BGR2GRAY);
		this->ell.transformedImage = grayImage;
	}

	// Binarize image first (just for new Scoring)
	adaptiveThreshold(this->ell.transformedImage, this->ell.binarizedImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 21, 3);
}

Grid::Grid(float size, float angle, float tilt,  int x,  int y, Ellipse ell, ScoringMethod scoringMethod) {
	init(size, angle, tilt, x, y, ell, false, scoringMethod);
}

Grid::Grid(float size, float angle, float tilt,  int x,  int y, Ellipse ell, bool permutation, ScoringMethod scoringMethod) {
	init(size, angle, tilt, x, y, ell, permutation, scoringMethod);
}

Grid::Grid(ScoringMethod scoringMethod) {
	init(0, 0, 0, 0, 0, Ellipse(), false, scoringMethod);
}

Grid::Grid(float size, ScoringMethod scoringMethod) {
	init(size, 0, 0, 0, 0, Ellipse(), false, scoringMethod);
}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}

// ===

// === Scoring methods ===
Grid::ScoringMethod Grid::scoringMethod() {
	return _score.metric;
}

double Grid::score() {

	// determine whether the grid is a dummy or not
	if (_score.metric == BINARYCOUNT && _score.value == BINARYCOUNT_INIT && ell.binarizedImage.total() > 0) {
		_score.value = binaryCountScore();
	} else if (_score.metric == FISHER && _score.value == FISHER_INIT && ell.transformedImage.total() > 0) {
		_score.value = fisherScore();
	}
	return _score.value;
}

double Grid::binaryCountScore() {

	Mat &binImg = ell.binarizedImage;

	Mat scores (3, 1, CV_64FC1);
	// for each cell calculate its size (cell size) and its mean intensity (means)
	for (int j = 12; j < 15; j++) {
		Mat mask = Mat(binImg.rows, binImg.cols, binImg.type(), Scalar(0));

		vector< vector <Point> > conts;
		conts.push_back(renderGridCell(j));
		drawContours(mask, conts, 0, Scalar(1), CV_FILLED);

		Mat whiteCellPixel = binImg.mul(mask); // just keep the pixel within the cell
		int whitePixelAmount = countNonZero(whiteCellPixel);
		int blackPixelAmount = countNonZero(mask - whiteCellPixel);

		if (j == 12 || j == 13) {
			// white inner half circle or white outer border
			scores.at<double>(14 - j) =  ((double) blackPixelAmount) / ((double) whitePixelAmount);
		} else if (j == 14) {
			//supposed black inner half circle
			scores.at<double>(14 - j) =  ((double) whitePixelAmount) / ((double) blackPixelAmount);
		}
    }

	return sum(scores)[0];
}

double Grid::fisherScore() {
	// 37/46 = 80.43% with Sb = |black - white| look into other intervariances
	// 41/47 = 89.13% with kind of A scaling
	// determine best orientation
	Mat &roi = ell.transformedImage;

	float Sb = 0;
	float Sw = 0;

	double black = -1;
	double white = -1;

	Mat smoothROI;
	GaussianBlur(roi, smoothROI, Size(7, 7), 0, 0, BORDER_DEFAULT);
	minMaxIdx(smoothROI, &black, &white);

	Mat means = Mat(15, 1, CV_32F);
	Mat labels = Mat(15, 1, CV_32S);
	TermCriteria max_it = TermCriteria(TermCriteria::COUNT, 3, 2);
	Mat centers;
	vector<Mat> masks;

	// for each cell calculate its size (cellsize) and its mean intensity (means)
	for (int j = 0; j < 15; j++) {
		Mat mask = Mat(roi.rows, roi.cols, roi.type(), Scalar(0));
		vector< vector <Point> > conts;
		conts.push_back(renderGridCell(j));
		drawContours(mask, conts, 0, Scalar(255), CV_FILLED);
		masks.push_back(mask);

		Scalar mean;
		Scalar std;
		meanStdDev(roi, mean, std, mask);

		means.at<float>(j) = mean[0];
	}

	// assume the color for each cell
	for ( int j = 0; j < 15; j++) {
		if (abs(black - means.at<float>(j)) < abs(white - means.at<float>(j))) {
			// cell is assumed black
			labels.at<int>(j) = 0;
		} else {
			// cell is assumed white
			labels.at<int>(j) = 1;
		}
	}

	// tag design 12,13 are white, 14 is black
	labels.at<int>(12) = 1;
	labels.at<int>(13) = 1;
	labels.at<int>(14) = 0;

	kmeans(means, 2, labels, max_it, 1, KMEANS_USE_INITIAL_LABELS, centers);

	// tag design 12,13 are white, 14 is black
	labels.at<int>(12) = 1;
	labels.at<int>(13) = 1;
	labels.at<int>(14) = 0;


	black = centers.at<float>(0, 0);
	white = centers.at<float>(1, 0);

	if (_score.value != -1) {
		for (int lab = 0; lab < labels.rows; lab++) {
			if (labels.at<int>(lab) == 0 && lab < 12) {
				if (abs(means.at<float>(lab) - white) < abs(means.at<float>(lab) - black)) {
					cout << "eh, uppsb" << endl;
				}
			} else if (lab < 12){
				if (abs(means.at<float>(lab) - white) > abs(means.at<float>(lab) - black)) {
					cout << "eh, uppsw" << endl;
				}
			}
			cout << labels.at<int>(lab);
			if (lab == 11) {
				cout << " | ";
			}
		}
		cout << endl;
		return -1;
	}

	float Swb = 0;
	int nw_b = 0;
	float Sww = 0;
	int nw_w = 0;

	// calculate intra variance (Score between - Sw)
	for (int j = 0; j < 15; j++) {
		float vari = 0;
		for (int r = 0; r < roi.cols; r++) {
			for (int c = 0; c < roi.rows; c++) {
				if (masks[j].at<unsigned char>(c, r) == 255) {
					if (labels.at<int>(j) == 0) {
						vari += (roi.at<unsigned char>(c, r) - black) * (roi.at<unsigned char>(c, r) - black);
						nw_b++;
					} else if (labels.at<int>(j) == 1) {
						vari += (roi.at<unsigned char>(c, r) - white) * (roi.at<unsigned char>(c, r) - white);
						nw_w++;
					} else {
						cout << "something went wrong" << endl;
					}
				}
			}
		}

		if (j == 13) {
			Sww += 2.5 * vari;
		} else if (j == 14) {
			Swb += 2.5 * vari;
		} else if (labels.at<int>(j) == 0) {
			Swb += vari;
		} else if (labels.at<int>(j) == 1) {
			Sww += vari;
		}
	}

	Sw = Sww /((float) nw_w) + Swb /((float) nw_b);

	if (Sw == 0) {
		cout << "Sw=0 - what now?" << endl;
	}

	// calculate inter variance (Score between - Sb)
	// inter variance = sum((mu - mi)^2)/n

	float mb = 0;
	int nb = 0;
	float mw = 0;
	int nw = 0;

	for (int j = 0; j < 15; j++) {

		if (labels.at<int>(j) == 1) {
			mw += means.at<float>(j);
			nw++;
		} else if (labels.at<int>(j) == 0) {
			mb += means.at<float>(j);
			nb++;

		} else {
			cout << "something went wrong" << endl;
		}
		//Sb += (means.at<float>(j,0) - muinter) * (means.at<float>(j,0) - muinter);
	}

	//Sb /= 15;
	mb /= (float) nb;
	mw /= (float) nw;

	Sb = abs(mb - mw) * abs(mb - mw);

	// how much "black" is covered?

	Mat tagMask = Mat(roi.rows, roi.cols, CV_8UC1, Scalar(0));
	// different center!! ellipse stuff!!!
	circle(tagMask, ell.cen, (int) (size * TRR), Scalar(1), CV_FILLED);

	Mat matMask = Mat(roi.rows, roi.cols, CV_8UC1, Scalar(0));
	circle(matMask, Point(x, y), (int) (size * ORR), Scalar(1), CV_FILLED);

	Mat tagBlack;
	threshold(roi, tagBlack, (black + white) / 2, 255, CV_THRESH_BINARY_INV);
	tagBlack = tagBlack.mul(tagMask);

	Mat matBlack;
	threshold(roi, matBlack, (black + white)/2, 255, CV_THRESH_BINARY_INV);
	matBlack = matBlack.mul(matMask);

	float blackratio = ((float) countNonZero(matBlack)) / ((float) countNonZero(tagBlack));
	Sb *= blackratio;

	// use fisher score Sb/Sw
	return Sb / Sw;
}

// ======


vector<Point> Grid::renderScaledGridCell(unsigned short cell, double scale, int offset) {

	// TODO caching???
	vector<Point> cont;
	vector<Point> cont2;

	// Outer cells
	if (cell < 12) {
		double outerInnerRadiusDiff = ORR * size - IORR * size;
		double outerCircleRadius = IORR * size + outerInnerRadiusDiff * 0.5 + (outerInnerRadiusDiff * 0.5 * scale);
		double innerCircleRadius = IORR * size + outerInnerRadiusDiff * 0.5 - (outerInnerRadiusDiff * 0.5 * scale);

		int arcStart = -180 + cell * 30 + 15 * (1 - scale);
		int arcEnd = -180 + (cell + 1) * 30 - 15 * (1 - scale);
		// outer arc
		ellipse2Poly(Point2f(x, y), Size2f(outerCircleRadius, outerCircleRadius), angle, arcStart, arcEnd, 1, cont); // opencv ellipse2poly
		// inner arc
		ellipse2Poly(Point2f(x, y), Size2f(innerCircleRadius, innerCircleRadius), angle, arcStart, arcEnd, 1, cont2);
		// join outer and inner arc
		cont.insert(cont.end(), cont2.rbegin(), cont2.rend());
		return move(cont);
	} else if (cell == 13) {
		// supposed white inner half circle
		ellipse2Poly(Point2f(x, y), Size2f(IRR * size * scale, IRR * size * scale), angle, -180 + offset * 30, offset * 30, 1, cont);
	} else if (cell == 14) {
		//supposed black inner half circle
		ellipse2Poly(Point2f(x, y), Size2f(IRR * size * scale, IRR * size * scale), angle, 180 + offset *30, offset * 30, 1, cont);
	} else if (cell == 12) {
		// outer (white) border
		double outerInnerRadiusDiff = TRR * size - ORR * size;
		double outerCircleRadius = ORR * size + outerInnerRadiusDiff * 0.5 + (outerInnerRadiusDiff * 0.5 * scale);
		double innerCircleRadius = ORR * size + outerInnerRadiusDiff * 0.5 - (outerInnerRadiusDiff * 0.5 * scale);

		vector < Point > cont2;
		ellipse2Poly(Point2f(x, y), Size(outerCircleRadius, outerCircleRadius), angle + 90, 0, 360, 1,cont);
		ellipse2Poly(Point2f(x, y), Size(innerCircleRadius, innerCircleRadius), angle + 90, 0, 360, 1,cont2);
		cont.insert(cont.end(), cont2.rbegin(), cont2.rend());
	}

	return move(cont);
}

vector<Point> Grid::renderGridCell(unsigned short cell, int offset) {
	return renderScaledGridCell(cell, 1, offset);
}


// === operators ===

bool Grid::operator>(Grid &g) {
	ScoringMethod metric = scoringMethod();
	assert (g.scoringMethod() == metric); // both grids need the same scoring method

	if (metric == BINARYCOUNT) {
		return score() < g.score();
	} else {
		return score() > g.score();
	}
}

bool Grid::operator<(Grid &g) {
	assert (g.scoringMethod() == scoringMethod());

	if (score() == g.score()) {
		return false;
	} else {
		return !(*this > g);
	}

}

vector<float> Grid::generateEdge(int radius, int width, bool useBinaryImage) {

	// Uses some kind of super resolution with getMeanAlongLine

	Mat &image = useBinaryImage ? ell.binarizedImage : ell.transformedImage;

	int outerRadius = width > 1 ? radius + ceil(double(width) / 2) : radius;

	// Using the Bresenham algorithm to generate a circle
	int x = outerRadius;
	int y = 0;
	int err = 1 - x;

	vector< vector<float> > subEdges (8);

	subEdges[0].push_back(getMeanAlongLine(this->x - outerRadius, this->y, this->x, this->y, width, useBinaryImage));
	subEdges[2].push_back(getMeanAlongLine(this->x, this->y - outerRadius, this->x, this->y, width, useBinaryImage));
	subEdges[4].push_back(getMeanAlongLine(this->x + outerRadius, this->y, this->x, this->y, width, useBinaryImage));
	subEdges[6].push_back(getMeanAlongLine(this->x, this->y + outerRadius, this->x, this->y, width, useBinaryImage));

	// Generating the octant in clockwise order
	while (x > y + 1) {

		y++;
		if (err < 0) {
			err += 2 * y + 1;
		} else {
			x--;
			err += 2 * (y - x + 1);
		}

		subEdges[0].push_back(getMeanAlongLine(this->x - x, this->y - y, this->x, this->y, width, useBinaryImage));
		subEdges[1].push_back(getMeanAlongLine(this->x - y, this->y - x, this->x, this->y, width, useBinaryImage));
		subEdges[2].push_back(getMeanAlongLine(this->x + y, this->y - x, this->x, this->y, width, useBinaryImage));
		subEdges[3].push_back(getMeanAlongLine(this->x + x, this->y - y, this->x, this->y, width, useBinaryImage));
		subEdges[4].push_back(getMeanAlongLine(this->x + x, this->y + y, this->x, this->y, width, useBinaryImage));
		subEdges[5].push_back(getMeanAlongLine(this->x + y, this->y + x, this->x, this->y, width, useBinaryImage));
		subEdges[6].push_back(getMeanAlongLine(this->x - y, this->y + x, this->x, this->y, width, useBinaryImage));
		subEdges[7].push_back(getMeanAlongLine(this->x - x, this->y + y, this->x, this->y, width, useBinaryImage));

		// This part is sometimes useful for debugging
		//image.at<unsigned char>(Point(this->x - x, this->y - y)) = 255;
		//image.at<unsigned char>(Point(this->x - y, this->y - x)) = 255;
		//image.at<unsigned char>(Point(this->x + y, this->y - x)) = 255;
		//image.at<unsigned char>(Point(this->x + x, this->y - y)) = 255;
		//image.at<unsigned char>(Point(this->x + x, this->y + y)) = 255;
		//image.at<unsigned char>(Point(this->x + y, this->y + x)) = 255;
		//image.at<unsigned char>(Point(this->x - y, this->y + x)) = 255;
		//image.at<unsigned char>(Point(this->x - x, this->y + y)) = 255;
	}


	// Merge all subedges
	vector<float> mergedEdges;
	for (int i = 0; i < 8; i++) {
		vector<float> subEdge = subEdges[i];
		// Reverse some octant to get the wished order (some octant has anti-clockwise order)
		if (i % 2 == 1) {
			reverse(subEdge.begin(), subEdge.end());
		}
		mergedEdges.insert(mergedEdges.end(), subEdge.begin(), subEdge.end());
	}

	// Move the beginning of the Edge, depending on the angle
	int a = (int) (angle + 360) % 360;
	int firstIdx = (int) (a / 360.0 * mergedEdges.size()) % mergedEdges.size();
	vector<float> edge;
	edge.insert(edge.end(), mergedEdges.begin() + firstIdx, mergedEdges.end());
	edge.insert(edge.end(), mergedEdges.begin(), mergedEdges.begin() + firstIdx);

	return edge;
}

Mat Grid::generateEdgeAsMat(int radius, int width, bool useBinaryImage) {
	vector<float> edge = generateEdge(radius, width, useBinaryImage);

	Mat newEdge (edge.size(), 1, CV_32FC1);
	for (unsigned int i = 0; i < edge.size(); i++) {
		newEdge.at<float>(i) = edge[i];
	}

	return newEdge;
}

float Grid::getMeanAlongLine(int xStart, int yStart, int xEnd, int yEnd, int size, bool useBinaryImage) {

	// It's possibly better just to get the position of the pixel along the line, but lets fuck performance
	Mat &image = useBinaryImage ? ell.binarizedImage : ell.transformedImage;

	Mat profile (size, 1, CV_8UC1);
	int x = xStart;
	int y = yStart;
	// Distances according to axis
	int dx =  abs(xEnd - xStart);
	int dy = -abs(yEnd - yStart);
	// Step size for the directions (because it's a line)
	int sx = xStart < xEnd ? 1 : -1;
	int sy = yStart < yEnd ? 1 : -1;
	// Error to determine the next pixel to step on
	int err = dx + dy;
	int e2;

	for(int i = 0; i < size; i++){
		profile.at<unsigned char>(i) = image.at<unsigned char>(Point(x, y));

		e2 = 2*err;
		if (e2 > dy) {
			err += dy;
			x += sx;
		}
		if (e2 < dx) {
			err += dx;
			y += sy;
		}
	}

	Scalar mean;
	Scalar std;
	meanStdDev(profile, mean, std);

	return mean[0];
}
// ======

// ======= DEBUG METHODS ========
Mat Grid::drawGrid(double scale, bool useBinaryImage) {
	Mat draw; // Matrix the image will be drawn into
	Mat &roi = useBinaryImage ? ell.binarizedImage : ell.transformedImage;
	roi.copyTo(draw);

	if (roi.type() == CV_8UC1) {
		// the grid is drawn with several colors so a RGB image is needed
		cvtColor(draw, draw, CV_GRAY2BGR);
	}

	// contour vector
	vector< vector <Point> > conts;

	int ites = 16;
	vector < Point > cont;

	// render half of the inner circle (circular matrix design)
	ellipse2Poly(Point2f(x, y), Size2f(IRR * size, IRR * size), angle, 0, -180, 1, cont);
	vector < Point > cont2;

	// take first and last vertex of the polygon to get the respective diameter of the inner circle
	cont2.push_back(cont[0]);
	cont2.push_back(cont[cont.size() - 1]);
	conts.push_back(cont2);
	drawContours(draw, conts, 0, Scalar(255, 0, 0), 1);

	conts.clear();

	for (int i = 0; i < ites; i++) {
		conts.push_back(renderScaledGridCell(i, scale, 0));
	}

	drawContours(draw, conts, -1, Scalar(255, 0, 0), 1);
	drawContours(draw, conts, 0, Scalar(0, 255, 0), 1);

	return draw;
}

Mat Grid::drawGrid() {
	return drawGrid(1, false);
}

Mat Grid::drawGrid(double scale) {
	return drawGrid(scale, false);
}

Mat Grid::drawGrid(bool useBinaryImage) {
	return drawGrid(1, useBinaryImage);
}

}
