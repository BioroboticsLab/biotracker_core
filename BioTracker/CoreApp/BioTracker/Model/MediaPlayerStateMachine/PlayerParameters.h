#ifndef PLAYERPARAMETERS_H
#define PLAYERPARAMETERS_H

struct playerParameters {

    // PlayerStates
    bool m_Play;
    bool m_Forw;
    bool m_Back;
    bool m_Stop;
    bool m_Paus;

    // The other information
    size_t m_TotalNumbFrames;
    QString m_CurrentFilename;
    size_t m_CurrentFrameNumber;
    std::shared_ptr<cv::Mat> m_CurrentFrame;
    double m_fpsSourceVideo;

};

#endif // PLAYERPARAMETERS_H
