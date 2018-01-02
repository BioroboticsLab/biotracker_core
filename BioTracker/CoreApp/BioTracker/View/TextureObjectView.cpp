#include "TextureObjectView.h"
#include "Model/TextureObject.h"
#include "QGraphicsScene"
#include "View/GraphicsView.h"


TextureObjectView::TextureObjectView(QObject *parent, IController *controller, IModel *model) :
    IViewGraphicsPixmapItem(parent, controller, model)
{
    setZValue(-1);
	_oldBoundingRect = QRectF();
}

void TextureObjectView::getNotified()
{
    TextureObject *texture = dynamic_cast<TextureObject *>(getModel());
    QPixmap pma;
    pma.convertFromImage(texture->get());
    setPixmap(pma);

	//if frame is set, set the boundingrect of the scene to the size of the frame
	if (texture->height() > 1) {
		QGraphicsScene *scene = this->scene();

		QRectF currentBoundingRect = this->boundingRect();

		//check if bounding rect changed -> this means that a new video has been loaded right?
		if (currentBoundingRect != _oldBoundingRect) {
			scene->setSceneRect(currentBoundingRect);

			GraphicsView* view = dynamic_cast<GraphicsView*>(scene->views()[0]);
			view->fitInView(currentBoundingRect, Qt::KeepAspectRatio);
			
			_oldBoundingRect = currentBoundingRect;
		}

		
		//scene->setSceneRect(0, 0, pma.width(), pma.height());
		//printf("textureobjectView: %i, %i ", pma.width(), pma.height());
		//printf("sceneRect: %f, %f \n", scene->sceneRect().width(), scene->sceneRect().width());
		
		/*QRectF boundingRect = QRectF(0, 0, pma.width(), pma.height());
		QPen pen = QPen(Qt::black);
		scene->addRect(boundingRect, pen);

		pen = QPen(Qt::red);
		scene->addRect(scene->sceneRect(), pen);*/

	}
    update();
}

void TextureObjectView::connectModelView()
{
    QObject::connect(getModel(), &IModel::notifyView, this, &TextureObjectView::getNotified);

    getNotified();
}
