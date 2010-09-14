#include "zoomtool.h"
#include "qdeclarativeviewobserver_p.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QRectF>
#include <QMenu>
#include <QAction>

#include <QDebug>

namespace QmlViewer {

ZoomTool::ZoomTool(QDeclarativeViewObserver *view) :
        AbstractFormEditorTool(view),
        m_rubberbandManipulator(reinterpret_cast<QGraphicsObject *>(QDeclarativeViewObserverPrivate::get(view)->manipulatorLayer), view),
        m_smoothZoomMultiplier(0.05f),
        m_currentScale(1.0f)
{
    m_zoomTo100Action = new QAction(tr("Zoom to &100%"), this);
    m_zoomInAction = new QAction(tr("Zoom In"), this);
    m_zoomOutAction = new QAction(tr("Zoom Out"), this);
    m_zoomInAction->setShortcut(QKeySequence(Qt::Key_Plus));
    m_zoomOutAction->setShortcut(QKeySequence(Qt::Key_Minus));

    connect(m_zoomTo100Action, SIGNAL(triggered()), SLOT(zoomTo100()));
    connect(m_zoomInAction, SIGNAL(triggered()), SLOT(zoomIn()));
    connect(m_zoomOutAction, SIGNAL(triggered()), SLOT(zoomOut()));
}

ZoomTool::~ZoomTool()
{

}

void ZoomTool::mousePressEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();

    QPointF scenePos = view()->mapToScene(event->pos());

    if (event->buttons() & Qt::RightButton) {
        QMenu contextMenu;
        contextMenu.addAction(m_zoomTo100Action);
        contextMenu.addSeparator();
        contextMenu.addAction(m_zoomInAction);
        contextMenu.addAction(m_zoomOutAction);
        contextMenu.exec(event->globalPos());
    } else if (event->buttons() & Qt::LeftButton) {
        m_dragBeginPos = scenePos;
        m_dragStarted = false;
    }
}

void ZoomTool::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();

    QPointF scenePos = view()->mapToScene(event->pos());

    if (event->buttons() & Qt::LeftButton
        && QPointF(scenePos - m_dragBeginPos).manhattanLength() > Constants::DragStartDistance / 3
        && !m_dragStarted)
    {
        m_dragStarted = true;
        m_rubberbandManipulator.begin(m_dragBeginPos);
        return;
    }

    if (m_dragStarted)
        m_rubberbandManipulator.update(scenePos);

}

void ZoomTool::mouseReleaseEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
    QPointF scenePos = view()->mapToScene(event->pos());

    if (m_dragStarted) {
        m_rubberbandManipulator.end();

        int x1 = qMin(scenePos.x(), m_rubberbandManipulator.beginPoint().x());
        int x2 = qMax(scenePos.x(), m_rubberbandManipulator.beginPoint().x());
        int y1 = qMin(scenePos.y(), m_rubberbandManipulator.beginPoint().y());
        int y2 = qMax(scenePos.y(), m_rubberbandManipulator.beginPoint().y());

        QPointF scenePosTopLeft = QPoint(x1, y1);
        QPointF scenePosBottomRight = QPoint(x2, y2);

        QRectF sceneArea(scenePosTopLeft, scenePosBottomRight);

        m_currentScale = qMin(view()->rect().width() / sceneArea.width(),
                              view()->rect().height() / sceneArea.height());


        QTransform transform;
        transform.scale(m_currentScale, m_currentScale);

        view()->setTransform(transform);
        view()->setSceneRect(sceneArea);
    } else {
        Qt::KeyboardModifier modifierKey = Qt::ControlModifier;
#ifdef Q_WS_MAC
        modifierKey = Qt::AltModifier;
#endif
        if (event->modifiers() & modifierKey) {
            zoomOut();
        } else {
            zoomIn();
        }
    }
}

void ZoomTool::zoomIn()
{
    m_currentScale = nextZoomScale(ZoomIn);
    scaleView(view()->mapToScene(m_mousePos));
}

void ZoomTool::zoomOut()
{
    m_currentScale = nextZoomScale(ZoomOut);
    scaleView(view()->mapToScene(m_mousePos));
}

void ZoomTool::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
}


void ZoomTool::hoverMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
}


void ZoomTool::keyPressEvent(QKeyEvent * /*event*/)
{
}

void ZoomTool::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() != Qt::Vertical)
        return;

    Qt::KeyboardModifier smoothZoomModifier = Qt::ControlModifier;
    if (event->modifiers() & smoothZoomModifier) {
        int numDegrees = event->delta() / 8;
        m_currentScale += m_smoothZoomMultiplier * (numDegrees / 15.0f);

        scaleView(view()->mapToScene(m_mousePos));

    } else if (!event->modifiers()) {
        if (event->delta() > 0) {
            m_currentScale = nextZoomScale(ZoomIn);
        } else if (event->delta() < 0) {
            m_currentScale = nextZoomScale(ZoomOut);
        }
        scaleView(view()->mapToScene(m_mousePos));
    }
}

void ZoomTool::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
    {
        m_currentScale = ((event->key() - Qt::Key_0) * 1.0f);
        scaleView(view()->mapToScene(m_mousePos)); // view()->mapToScene(view()->rect().center())
        break;
    }

    default:
        break;
    }

}

void ZoomTool::itemsAboutToRemoved(const QList<QGraphicsItem*> &/*itemList*/)
{
}

void ZoomTool::clear()
{
    view()->setCursor(Qt::ArrowCursor);
}

void ZoomTool::selectedItemsChanged(const QList<QGraphicsItem*> &/*itemList*/)
{
}

void ZoomTool::scaleView(const QPointF &centerPos)
{

    QTransform transform;
    transform.scale(m_currentScale, m_currentScale);
    view()->setTransform(transform);

    QPointF adjustedCenterPos = centerPos;
    QSize rectSize(view()->rect().width() / m_currentScale,
                   view()->rect().height() / m_currentScale);

    QRectF sceneRect;
    if (qAbs(m_currentScale - 1.0f) < Constants::ZoomSnapDelta) {
        adjustedCenterPos.rx() = rectSize.width() / 2;
        adjustedCenterPos.ry() = rectSize.height() / 2;
    }

    if (m_currentScale < 1.0f) {
        adjustedCenterPos.rx() = rectSize.width() / 2;
        adjustedCenterPos.ry() = rectSize.height() / 2;
        sceneRect.setRect(view()->rect().width() / 2 -rectSize.width() / 2,
                          view()->rect().height() / 2 -rectSize.height() / 2,
                          rectSize.width(),
                          rectSize.height());
    } else {
        sceneRect.setRect(adjustedCenterPos.x() - rectSize.width() / 2,
                          adjustedCenterPos.y() - rectSize.height() / 2,
                          rectSize.width(),
                          rectSize.height());
    }

    view()->setSceneRect(sceneRect);
}

void ZoomTool::zoomTo100()
{
    m_currentScale = 1.0f;
    scaleView(view()->mapToScene(view()->rect().center()));
}

qreal ZoomTool::nextZoomScale(ZoomDirection direction) const
{
    static QList<qreal> zoomScales =
            QList<qreal>()
            << 0.125f
            << 1.0f / 6.0f
            << 0.25f
            << 1.0f / 3.0f
            << 0.5f
            << 2.0f / 3.0f
            << 1.0f
            << 2.0f
            << 3.0f
            << 4.0f
            << 5.0f
            << 6.0f
            << 7.0f
            << 8.0f
            << 12.0f
            << 16.0f
            << 32.0f
            << 48.0f;

    if (direction == ZoomIn) {
        for(int i = 0; i < zoomScales.length(); ++i) {
            if (zoomScales[i] > m_currentScale || i == zoomScales.length() - 1)
                return zoomScales[i];
        }
    } else {
        for(int i = zoomScales.length() - 1; i >= 0; --i) {
            if (zoomScales[i] < m_currentScale || i == 0)
                return zoomScales[i];
        }
    }

    return 1.0f;
}

} // namespace QmlViewer
