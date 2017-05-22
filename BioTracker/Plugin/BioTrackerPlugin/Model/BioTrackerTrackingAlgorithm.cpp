#include "BioTrackerTrackingAlgorithm.h"
#include <future>
#include "fish/NN2dMapper.h"
#include "TrackedComponents\TrackedComponentFactory.h"

/* void createTrajectories(int count, TrackedTrajectory* all) {
	//This should be done using a factory, right?

	for (int i = 0; i < count; i++) {
		TrackedTrajectory *t = new TrackedTrajectory();
		TrackedElement *e = new TrackedElement(t, "n.a.", i);
		e->setId(i);
		t->add(e);
		all->add(t);
	}
}*/

BioTrackerTrackingAlgorithm::BioTrackerTrackingAlgorithm(IModel *parameter, IModel *trajectory) : _ipp((TrackerParameter*)parameter)
{
	m_TrackingParameter = (TrackerParameter*)parameter;
	m_TrackedTrajectoryMajor = (TrackedTrajectory*)trajectory;
	//createTrajectories(2, m_TrackedTrajectoryMajor);
	m_frameCount = 0;
	//fs1.open("PrcBenchPre.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	//fs2.open("PrcBenchPost.txt", std::fstream::in | std::fstream::out | std::fstream::app);
}

std::vector<FishPose> BioTrackerTrackingAlgorithm::getLastPositionsAsPose() {
	//TODO: This seems kinda fragile: I just assume that the tree has this very certain structure:
	// Trajectory -> M Trajectories -> N TrackedElements
	// For every of M Trajectories grab the last (highest index) of TrackedElements.
	//TODO: If we are tracking somewhere in the middle, this is bad. Do it by id!
	std::vector<FishPose> last;
	for (int i = 0; i < m_TrackedTrajectoryMajor->numberOfChildrean(); i++) {
		TrackedTrajectory *t = (TrackedTrajectory*)m_TrackedTrajectoryMajor->getChild(i);
		TrackedElement *e = (TrackedElement *)t->getLastChild();
		last.push_back(e->getFishPose());
	}
	return last;
}

void BioTrackerTrackingAlgorithm::doTracking(std::shared_ptr<cv::Mat> p_image, uint framenumber)
{

	/*
	auto now = std::chrono::system_clock::now();
	auto now_ns = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	auto epoch = now_ns.time_since_epoch();
	auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	ulong duration = value.count();
	fs1 << framenumber << "," << duration << std::endl;*/

	_ipp.m_TrackingParameter = m_TrackingParameter;

	//Do the preprocessing
	std::map<std::string, std::shared_ptr<cv::Mat>> images = _ipp.preProcess(p_image);
	std::shared_ptr<cv::Mat> dilated = images.find(std::string("Dilated"))->second;
	std::shared_ptr<cv::Mat> greyMat = images.find(std::string("Greyscale"))->second;

	//Find blobs via ellipsefitting
	_bd.setMaxBlobSize(m_TrackingParameter->getMaxBlobSize());
	_bd.setMinBlobSize(m_TrackingParameter->getMinBlobSize());
	std::vector<BlobPose> blobs = _bd.getPoses(*dilated, *greyMat);

	NN2dMapper nn2d;
	std::vector<FishPose> fish = getLastPositionsAsPose();
	std::tuple<std::vector<FishPose>, std::vector<float>> poses = nn2d.getNewPoses(fish, blobs);

	//Insert new poses into data structure
	if (std::get<0>(poses).size() == m_TrackedTrajectoryMajor->numberOfChildrean()) { //TODO hardcoded
		for (int i = 0; i < m_TrackedTrajectoryMajor->numberOfChildrean(); i++) {
			TrackedTrajectory *t = (TrackedTrajectory*)m_TrackedTrajectoryMajor->getChild(i);
			TrackedElement *e = new TrackedElement(t, "n.a.", i);
			//int x = std::get<0>(poses)[i].position_cm().x;
			//int y = std::get<0>(poses)[i].position_cm().y;
			e->setFishPose(std::get<0>(poses)[i]);
			t->add(e);
		}
	}
	else {
		std::cout << "Error: did not track expected size! Size is: " << std::get<1>(poses).size() << std::endl;
	}

	//Draw stuff into the output
	std::shared_ptr<cv::Mat> rumgekritzel = std::make_shared<cv::Mat>();
	cvtColor(*dilated, *rumgekritzel, CV_GRAY2BGR);
	for (int i = 0; i < blobs.size(); i++) {
		cv::circle(*rumgekritzel, blobs[i].posPx(), 15, cv::Scalar(0, 255, 0), 3);
	}

	/* now = std::chrono::system_clock::now();
	 now_ns = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	 epoch = now_ns.time_since_epoch();
	 value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	 duration = value.count();
	fs2 << framenumber << "," << duration << std::endl;
	*/
	Q_EMIT emitCvMatA(rumgekritzel, QString("Drawing"));

	m_frameCount++;
	Q_EMIT emitTrackingDone();
}
