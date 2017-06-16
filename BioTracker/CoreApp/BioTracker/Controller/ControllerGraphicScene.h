/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef CONTROLLERGRAPHICSCENE_H
#define CONTROLLERGRAPHICSCENE_H

#include "Interfaces/IController/IController.h"
#include "QGraphicsObject"
#include "QPointer"

/**
 * The ControllerGraphicScene class controlls the component for rendering TextureObjects and TrackedComponents on a GraphhicsView.
 * Each PixmapItem represents a cv::Mat. The cv::Mat comes from the TextureObjects-Component of BioTracker.
 * The GraphicsItem represents the compositum structure of IViewTrackedComponent classes. These structure comes from the Plugin and handed over by the PluginController of the MainApplication.
 *
 */
class ControllerGraphicScene : public IController {
    Q_OBJECT
  public:
    ControllerGraphicScene(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    /**
     * This methode add a the root node of the compositum structure of IViewTrackedComponent classes to the GraphicsView canvas.
     * All child nodes will be rendered on a GraphicsView canvas as well.
     */
    void addGraphicsItem(QGraphicsItem* item);
    /**
     * This methode adds a IViewGraphicsPixmapItem to the GraphicsView canvas for rendering.
     */
	void addTextureObject(QGraphicsItem* item);

    // IController interface
  protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;
    void connectControllerToController() override;

  private:
    IView* m_GraphicsView;

    IModel* m_Element;
    QPointer< IModel > m_NullModel;
};

#endif // CONTROLLERGRAPHICSCENE_H
