/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef PLAYERPARAMETERS_H
#define PLAYERPARAMETERS_H

/**
 * The playerParameters struct holds all data types of the current MediaPlayer state.
 */
struct playerParameters {

    // PlayerStates
    bool m_Play;
    bool m_Forw;
    bool m_Back;
    bool m_Stop;
	bool m_Paus;
	bool m_RecI;
	bool m_RecO;

    // The other information
    size_t m_TotalNumbFrames;
	QString m_CurrentFilename;
	std::string m_CurrentTitle;
    size_t m_CurrentFrameNumber;
    std::shared_ptr<cv::Mat> m_CurrentFrame;
    double m_fpsSourceVideo;
    double m_fpsTarget;
    std::vector<std::string> m_batchItems;
};

#endif // PLAYERPARAMETERS_H
