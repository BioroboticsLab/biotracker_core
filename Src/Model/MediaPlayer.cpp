#include "MediaPlayer.h"
#include "Utility/misc.h"

// Settings related
#include "util/types.h"
#include "Controller/IControllerCfg.h"

using namespace BioTrackerUtilsMisc; // getTimeAndDate

MediaPlayer::MediaPlayer(QObject* parent)
: IModel(parent)
{
    _cfg              = static_cast<IControllerCfg*>(parent)->getConfig();
    m_currentFPS      = 0;
    m_fpsOfSourceFile = 0;
    _imagew           = 0;
    _imageh           = 0;
    m_useCuda         = false;
    m_RecI            = false;
    m_RecO            = false;
    m_videoc          = std::make_shared<VideoCoder>(_cfg->RecordFPS, _cfg);
    m_NameOfCvMat     = "Original";

    m_TrackingIsActive = false;
    m_recd             = false;
    m_recordScaled     = false;
    // Initialize PlayerStateMachine and a Thread for the Player
    //    // Do not set a Parent for MediaPlayerStateMachine in order to run
    //    the Player in the QThread!

    m_PlayerThread = new QThread(this);
    m_PlayerThread->setObjectName("PlayerThread");
    m_Player = new MediaPlayerStateMachine();
    m_Player->setConfig(_cfg);

    // Connect MediaPlayer with PlayerStateMachine

    // Load ImageStreams in StateMachine
    QObject::connect(this,
                     &MediaPlayer::loadVideoStream,
                     m_Player,
                     &MediaPlayerStateMachine::receiveLoadVideoCommand);
    QObject::connect(this,
                     &MediaPlayer::loadCameraDevice,
                     m_Player,
                     &MediaPlayerStateMachine::receiveLoadCameraDevice);
    QObject::connect(this,
                     &MediaPlayer::loadPictures,
                     m_Player,
                     &MediaPlayerStateMachine::receiveLoadPictures);

    // Controll the Player
    QObject::connect(this,
                     &MediaPlayer::nextFrameCommand,
                     m_Player,
                     &MediaPlayerStateMachine::receiveNextFramCommand);
    QObject::connect(this,
                     &MediaPlayer::pauseCommand,
                     m_Player,
                     &MediaPlayerStateMachine::receivePauseCommand);
    QObject::connect(this,
                     &MediaPlayer::playCommand,
                     m_Player,
                     &MediaPlayerStateMachine::receivePlayCommand);
    QObject::connect(this,
                     &MediaPlayer::prevFrameCommand,
                     m_Player,
                     &MediaPlayerStateMachine::receivePrevFrameCommand);
    QObject::connect(this,
                     &MediaPlayer::stopCommand,
                     m_Player,
                     &MediaPlayerStateMachine::receiveStopCommand);
    QObject::connect(this,
                     &MediaPlayer::goToFrame,
                     m_Player,
                     &MediaPlayerStateMachine::receiveGoToFrame);

    QObject::connect(this,
                     &MediaPlayer::pauseCommand,
                     this,
                     &MediaPlayer::receiveTrackingPaused);
    QObject::connect(this,
                     &MediaPlayer::stopCommand,
                     this,
                     &MediaPlayer::receiveTrackingPaused);

    QObject::connect(this,
                     &MediaPlayer::toggleRecordImageStreamCommand,
                     m_Player,
                     &MediaPlayerStateMachine::receivetoggleRecordImageStream);

    // Handle PlayerStateMachine results
    QObject::connect(m_Player,
                     &MediaPlayerStateMachine::emitPlayerParameters,
                     this,
                     &MediaPlayer::receivePlayerParameters);
    QObject::connect(m_Player,
                     &MediaPlayerStateMachine::emitPlayerParameters,
                     this,
                     &MediaPlayer::fwdPlayerParameters);

    // Handle next state operation
    QObject::connect(m_Player,
                     &MediaPlayerStateMachine::emitPlayerOperationDone,
                     this,
                     &MediaPlayer::receivePlayerOperationDone);
    QObject::connect(this,
                     &MediaPlayer::runPlayerOperation,
                     m_Player,
                     &MediaPlayerStateMachine::receiveRunPlayerOperation);

    QObject::connect(m_Player,
                     &MediaPlayerStateMachine::emitNextMediaInBatch,
                     this,
                     &MediaPlayer::emitNextMediaInBatch,
                     Qt::DirectConnection);
    QObject::connect(m_Player,
                     &MediaPlayerStateMachine::emitNextMediaInBatchLoaded,
                     this,
                     &MediaPlayer::emitNextMediaInBatchLoaded,
                     Qt::DirectConnection);

    // Move the PlayerStateMachine to the Thread
    m_Player->moveToThread(m_PlayerThread);

    // Start the Thread
    m_PlayerThread->start();
}

MediaPlayer::~MediaPlayer()
{
    stopCommand();
    m_PlayerThread->quit();
    if (!m_PlayerThread->wait(2000)) {
        m_PlayerThread->terminate();
        m_PlayerThread->wait();
    }
}

void MediaPlayer::setTrackingActive()
{
    m_TrackingIsActive = true;
}

void MediaPlayer::setTrackingDeactive()
{
    m_TrackingIsActive = false;
}

bool MediaPlayer::getPlayState()
{
    return m_Play;
}

bool MediaPlayer::getForwardState()
{
    return m_Forw;
}

bool MediaPlayer::getBackwardState()
{
    return m_Back;
}

bool MediaPlayer::getStopState()
{
    return m_Stop;
}

bool MediaPlayer::getPauseState()
{
    return m_Paus;
}

bool MediaPlayer::getRecIState()
{
    return m_RecI;
}

bool MediaPlayer::getRecOState()
{
    return m_RecO;
}

bool MediaPlayer::getTrackingState()
{
    return m_TrackingIsActive;
}

size_t MediaPlayer::getTotalNumberOfFrames()
{
    return m_TotalNumbFrames;
}

size_t MediaPlayer::getCurrentFrameNumber()
{
    return m_CurrentFrameNumber;
}

double MediaPlayer::getFpsOfSourceFile()
{
    return m_fpsOfSourceFile;
}

double MediaPlayer::getCurrentFPS()
{
    return m_currentFPS;
}

double MediaPlayer::getTargetFPS()
{
    return m_targetFPS;
}
void MediaPlayer::setTargetFPS(double fps)
{
    m_targetFPS = fps;
    m_Player->receiveTargetFps(fps);
}

QString MediaPlayer::getCurrentFileName()
{
    return m_CurrentFilename;
}

cv::Mat MediaPlayer::getCurrentFrame()
{
    return m_CurrentFrame;
}

int MediaPlayer::reopenVideoWriter()
{
    QRectF r = m_gv->sceneRect();

    if (_imagew != r.width() || _imageh != r.height() || !m_recd) {

        if (m_useCuda) {
#ifdef WITH_CUDA

            _imagew  = r.width();
            _imageh  = r.height();
            m_videoc = std::make_shared<VideoCoder>(_cfg->RecordFPS, _cfg);
            EncodeConfig* cfg = m_videoc->m_nvEncoder->getEncodeConfig();
            cfg->width        = _imagew;
            cfg->height       = _imageh;
            cfg->fps          = 30;
            cfg->codec        = NV_ENC_H264;
            cfg->inputFormat =
                NV_ENC_BUFFER_FORMAT_NV12; // NV_ENC_BUFFER_FORMAT_YUV444
                                           // //NV_ENC_BUFFER_FORMAT_NV12
            const std::string f   = getTimeAndDate("ViewCapture", ".avi");
            char*             chr = strdup(f.c_str());
            m_videoc->start();
            free(chr);
            m_recd = true;
#else
            std::cout << "Error on recording: Asked to use cudacodec although "
                         "build is not cuda-enabled. Not recording."
                      << std::endl;
#endif

        } else {
            if (m_videoWriter && m_videoWriter->isOpened())
                m_videoWriter->release();
            _imagew = r.width();
            _imageh = r.height();

            int codec     = cv::VideoWriter::fourcc('X', '2', '6', '4');
            m_videoWriter = std::make_shared<cv::VideoWriter>(
                getTimeAndDate("./ViewCapture", ".avi"),
                codec,
                30,
                cv::Size(r.width(), r.height()),
                1);
            m_recd = m_videoWriter->isOpened();
        }
    }
    return m_recd;
}

QString MediaPlayer::takeScreenshot(GraphicsView* gv)
{
    QRectF rscene = gv->sceneRect();
    QRectF rview  = gv->rect();
    QSize  size;
    if (!m_recordScaled)
        size = rscene.size().toSize();
    else
        size = rview.size().toSize();
    auto image   = QImage(size, QImage::Format_RGB32);
    auto painter = QPainter(&image);

    if (!m_recordScaled)
        gv->scene()->render(&painter);
    else
        gv->render(&painter);

    auto filePath = QString::fromStdString(
        getTimeAndDate(_cfg->DirScreenshots.toStdString(), ".png"));

    image.save(filePath);

    return filePath;
}

void MediaPlayer::receiveTrackingPaused()
{
}

void MediaPlayer::receivePlayerParameters(
    std::shared_ptr<const playerParameters> param)
{

    m_Back = param->m_Back;
    m_Paus = param->m_Paus;
    m_Play = param->m_Play;
    m_Stop = param->m_Stop;
    m_Forw = param->m_Forw;
    m_RecI = param->m_RecI;
    m_RecO = param->m_RecO;

    m_CurrentFilename    = param->m_CurrentFilename;
    m_CurrentFrameNumber = param->m_CurrentFrameNumber;
    m_fpsOfSourceFile    = param->m_fpsSourceVideo;
    m_TotalNumbFrames    = param->m_TotalNumbFrames;

    if (param->m_CurrentFrame && !param->m_CurrentFrame->empty()) {
        m_CurrentFrame = *param->m_CurrentFrame;

        Q_EMIT renderCurrentImage(m_CurrentFrame, m_NameOfCvMat);

        if (m_TrackingIsActive) {
            Q_EMIT trackCurrentImage(m_CurrentFrame,
                                     static_cast<uint>(m_CurrentFrameNumber));
        } else {
            Q_EMIT signalVisualizeCurrentModel(
                static_cast<uint>(m_CurrentFrameNumber));
            Q_EMIT signalCurrentFrameNumberToPlugin(
                static_cast<uint>(m_CurrentFrameNumber));
        }

        if (m_recd) {
            QRectF rscene = m_gv->sceneRect();
            QRectF rview  = m_gv->rect();
            QSize  size;
            if (!m_recordScaled)
                size = rscene.size().toSize();
            else
                size = rview.size().toSize();
            if (m_image.size() != size) {
                m_image = QImage(size, QImage::Format_RGB32);
                if (m_painter.isActive())
                    m_painter.end();
                m_painter.begin(&m_image);
            }

            if (!m_recordScaled)
                m_gv->scene()->render(&m_painter);
            else
                m_gv->render(&m_painter);

            auto view = cv::Mat(m_image.height(),
                                m_image.width(),
                                CV_8UC(m_image.depth() / 8),
                                m_image.bits(),
                                m_image.bytesPerLine());

            auto copy = view.clone();
            cv::cvtColor(copy, copy, cv::ColorConversionCodes::COLOR_BGR2RGB);
            m_videoc->add(copy);
        }
    } else {
        qWarning()
            << "MediaPlayer: Received player parameters with invalid image.";
    }

    Q_EMIT notifyView();
}

void MediaPlayer::rcvPauseState(bool state)
{
    _paused = state;

    if (!state) {
        m_currentFPS = 0;
    }
}

void MediaPlayer::receivePlayerOperationDone()
{
    end    = std::chrono::system_clock::now();
    long s = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                 .count();
    if (!_paused) {
        m_currentFPS = floor(1.0 / (double(s) / 1000.0));
    } else {
        m_currentFPS = 0;
    }

    emit runPlayerOperation();

    start = std::chrono::system_clock::now();
}

int MediaPlayer::toggleRecordImageStream()
{
    Q_EMIT toggleRecordImageStreamCommand();
    return 0;
}

int MediaPlayer::toggleRecordGraphicsScenes(GraphicsView* gv)
{

    m_gv           = gv;
    QRectF rscene  = m_gv->sceneRect();      // 0us
    QRectF rview   = m_gv->rect();           // 0us
    QSize  s1      = rscene.size().toSize(); // 0us
    QSize  s2      = rview.size().toSize();  // 0us
    m_recordScaled = _cfg->RecordScaledOutput;
    if (!m_recordScaled)
        m_recd = m_videoc->toggle(s1.width(), s1.height(), 30);
    else
        m_recd = m_videoc->toggle(s2.width(), s2.height(), 30);
    return m_recd;
}
