#include "gestScene.h"

using namespace GestEditor;

static const double delta = 0.01;

GestScene::GestScene(QGraphicsView *view)
{
	mItemType = none;
	mGraphicsItem = NULL;
	mLastAdded = NULL;
	mView = view;
}

void GestScene::drawLine()
{
	mItemType = line;
	stateSelection();
}

void GestScene::drawRect()
{
	mItemType = rectangle;
	stateSelection();
}

void GestScene::drawEllipse()
{
	mItemType = ellipse;
	stateSelection();
}

void GestScene::drawCurve()
{
	mItemType = curve;
	stateSelection();
}

void GestScene::addNone()
{
	mItemType = none;
}

void GestScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	stateSelection();
	stateMove();
	if (event->button() == Qt::LeftButton) { // avoiding double click problem
		int x1 = event->scenePos().x();
		int y1 = event->scenePos().y();
		switch (mItemType) {
		case ellipse:
			mEllipse = new GestEllipse(x1, y1, x1 + delta, y1 + delta);
			addItem(this->mEllipse);
			mLastAdded = mEllipse;
			break;
		case rectangle:
			mRect = new GestRectangle(x1, y1, x1 + delta, y1 + delta);
			addItem(this->mRect);
			mLastAdded = mRect;
			break;
		case line:
			mLine = new GestLine(x1, y1, x1 + delta, y1 + delta);
			addItem(mLine);
			mLastAdded = mLine;
			break;
		case curve:
			mCurve = new GestCurve(x1, y1, x1 + delta, y1 + delta);
			addItem(mCurve);
			mLastAdded = mCurve;
			break;
		case none:
			stateSelection();
			mGraphicsItem = dynamic_cast<GestItem *>(this->itemAt(event->scenePos(), QTransform()));
			break;
		}
		updateGraphicsItem();
		forPressResize(event);
		invalidate();
	}
}

void GestScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseMoveEvent(event);
	switch (mItemType) {
	case ellipse:
		reshapeEllipse(event);
		break;
	case rectangle:
		reshapeRect(event);
		break;
	case line:
		reshapeLine(event);
		break;
	case curve:
		reshapeCurve(event);
	default:
		break;
	}
	updateGraphicsItem();
	reshapeItem(event);
	invalidate();
}

void GestScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseReleaseEvent(event);
	switch (mItemType) {
	case ellipse:
		reshapeEllipse(event);
		break;
	case rectangle:
		reshapeRect(event);
		break;
	case line:
		reshapeLine(event);
		break;
	case curve:
		reshapeCurve(event);
		break;
	default:
		break;
	}

	reshapeItem(event);

	if (mLastAdded != NULL) {
		mLastAdded->updateScalingRects(mLastAdded->sceneBoundingRect().topLeft(), mLastAdded->sceneBoundingRect().bottomRight());
		mLastAdded->updatePorts();
	}

	emitResetButtons();
	uniteFigures();
	performIdealTransform();
	updateSceneState();

	invalidate();
}

void GestScene::emitResetButtons()
{
	emit resetHighlightAllButtons();
	mItemType = none;
}

void GestScene::updateGraphicsItem()
{
	if (mGraphicsItem != NULL) {
		mLastAdded = mGraphicsItem;
		mGraphicsItem->updateScalingRects(
					mGraphicsItem->sceneBoundingRect().topLeft(), mGraphicsItem->sceneBoundingRect().bottomRight());
		mGraphicsItem->updatePorts();
	}
}

void GestScene::reshapeLine(QGraphicsSceneMouseEvent *event) {
	mLine->setX2andY2(event->scenePos().x(), event->scenePos().y());
	invalidate();
}

void GestScene::reshapeRect(QGraphicsSceneMouseEvent *event) {
	mRect->setX2andY2(event->scenePos().x(), event->scenePos().y());
	invalidate();
}

void GestScene::reshapeEllipse(QGraphicsSceneMouseEvent *event) {
	mEllipse->setX2andY2(event->scenePos().x(), event->scenePos().y());
	invalidate();
}

void GestScene::reshapeCurve(QGraphicsSceneMouseEvent *event)
{
	mCurve->setX2andY2(event->scenePos().x(), event->scenePos().y());
	invalidate();
}

void GestScene::forPressResize(QGraphicsSceneMouseEvent *event)
{
	if (mGraphicsItem != NULL) {
		mGraphicsItem->changeDragState(event->scenePos().x(), event->scenePos().y());
	}
}

void GestScene::reshapeItem(QGraphicsSceneMouseEvent *event)
{
	if (mGraphicsItem != NULL) {
		mGraphicsItem->setOrientation();
		mGraphicsItem->resizeItem(event);
	}
}

void GestScene::stateMove()
{
	if (mItemType != none) {
		foreach (QGraphicsItem *item, this->items()) {
			item->setFlag(QGraphicsItem::ItemIsMovable, false);
		}
	} else {
		foreach (QGraphicsItem *item, this->items()) {
			item->setFlag(QGraphicsItem::ItemIsMovable, true);
		}
	}
}

void GestScene::stateSelection()
{
	if (mItemType != none) {
		foreach (QGraphicsItem *item, this->items()) {
			item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		}
		mGraphicsItem = NULL;
	} else {
		foreach (QGraphicsItem *item, this->items()) {
			item->setFlag(QGraphicsItem::ItemIsSelectable, true);
		}
	}
}

void GestScene::refresh()
{
	foreach (QGraphicsItem *item, this->items()) {
		removeItem(item);
	}
}

void GestScene::deleteSelectedItem()
{
	if (mGraphicsItem != NULL) {
		this->removeItem(mGraphicsItem);
	}
}

void GestScene::performIdealTransform()
{
	if (mLastAdded != NULL) {
		mLastAdded->transformToIdeal();
	}
}

void GestScene::uniteFigures()
{
	foreach (QGraphicsItem *item, this->items()) {
		GestItem *current = dynamic_cast<GestItem *>(item);
		if (mLastAdded != current && mLastAdded != NULL) {
			current->intersectPorts(mLastAdded);
			current->updatePorts();
			mLastAdded->updatePorts();
		}
	}
}

void GestScene::findConnectedPorts()
{
	mConnectedPorts.clear();
	foreach (QGraphicsItem *item1, this->items()) {
		GestItem *current1 = dynamic_cast<GestItem *>(item1);
		foreach (QGraphicsItem *item2, this->items()) {
			GestItem *current2 = dynamic_cast<GestItem *>(item2);
			if (current1 != current2) {
				updateConnectedPorts(&current1->mDockingPoints, &current2->mDockingPoints);
			}
		}
	}
}

void GestScene::updateConnectedPorts(QVector<QPointF> *points1, QVector<QPointF> *points2)
{
	foreach(QPointF const &point1, *points1) {
		foreach (QPointF const &point2, *points2) {
			if (point1 == point2 && !mConnectedPorts.contains(point1)) {
				mConnectedPorts << point1;
			}
		}
	}
}

void GestScene::drawPorts()
{
	findConnectedPorts();
	foreach (QGraphicsItem *item1, this->items()) {
		GestItem *current1 = dynamic_cast<GestItem *>(item1);
		QVector<QPointF> pointsForCurItem;
		foreach (QPointF connectPort, mConnectedPorts) {
			QPointF pointIn = current1->mapFromScene(connectPort);
			if (current1->contains(pointIn)) {
				pointsForCurItem << connectPort;
			}
		}
		current1->insertConnectedPorts(pointsForCurItem);
	}
}

void GestScene::setLastAdded(GestItem *item)
{
	mLastAdded = item;
}

void GestScene::centerView()
{
	if (mLastAdded != NULL) {
		mView->centerOn(mLastAdded);
	}
	updateSceneState();
}

void GestScene::updateSceneState()
{
	foreach (QGraphicsItem *item, this->items()) {
		GestItem *current = dynamic_cast<GestItem *>(item);
		current->updateScalingRects(
				current->sceneBoundingRect().topLeft(), current->sceneBoundingRect().bottomRight());
		current->updatePorts();
	}
	uniteFigures();
	drawPorts();
}


