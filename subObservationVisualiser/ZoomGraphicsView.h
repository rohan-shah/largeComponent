#ifndef LARGE_COMPONENT_ZOOM_GRAPHICS_HEADER_GUARD
#define LARGE_COMPONENT_ZOOM_GRAPHICS_HEADER_GUARD
#include <QGraphicsView>
namespace largeComponent
{
	class ZoomGraphicsView : public QGraphicsView
	{
		Q_OBJECT
	public:
		ZoomGraphicsView(QGraphicsScene* scene);
	protected:
		void wheelEvent(QWheelEvent* event);
		void keyPressEvent(QKeyEvent* event);
	};
}
#endif
