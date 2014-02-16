#include "gestLine.h"

using namespace GestEditor;

GestLine::GestLine(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) :
	GestItem(x1, y1, x2, y2)
{
	mItemType = "line";
}

void GestLine::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->drawLine(mX1, mY1, mX2, mY2);
}

void GestLine::drawScalingRects(QPainter *painter)
{
	painter->setPen(mPen);
	mBrush.setColor(Qt::black);
	painter->setBrush(mBrush);
	QRectF LeftScalRect;
	QRectF RightScalRect;
	int drift = scalingRect / 2;
	switch(mOrient) {
	case LeftBottomRightTop:
	{
		LeftScalRect = QRectF(QPoint(mX1 - drift, mY1 - drift), QPoint(mX1 + drift, mY1 + drift));
		RightScalRect = QRectF(QPoint(mX2 - drift, mY2 - drift), QPoint(mX2 + drift, mY2 + drift));
		break;
	}
	case LeftTopRightBottom:
	{
		LeftScalRect = QRectF(QPoint(mX1 - drift, mY1 - drift), QPoint(mX1 + drift, mY1 + drift));
		RightScalRect = QRectF(QPoint(mX2 - drift, mY2 - drift), QPoint(mX2 + drift, mY2 + drift));
		break;
	}
	case RightBottomLeftTop:
	{
		RightScalRect = QRectF(QPoint(mX1 - drift, mY1 - drift), QPoint(mX1 + drift, mY1 + drift));
		LeftScalRect = QRectF(QPoint(mX2 - drift, mY2 - drift), QPoint(mX2 + drift, mY2 + drift));
		break;
	}
	case RightTopLeftBottom:
	{
		RightScalRect = QRectF(QPoint(mX1 - drift, mY1 - drift), QPoint(mX1 + drift, mY1 + drift));
		LeftScalRect = QRectF(QPoint(mX2 - drift, mY2 - drift), QPoint(mX2 + drift, mY2 + drift));
		break;
	}
	}
//	painter->drawRect(mLeftScalRect);
//	painter->drawRect(mRightScalRect);
	painter->drawRect(LeftScalRect);
	painter->drawRect(RightScalRect);
}

void GestLine::transformToIdeal()
{
	// for diagonal lines
	int size = std::min(rectForDraw().width(), rectForDraw().height());
	int autoChange = countRatio(partOfIdeal);
	if (sizePolicy() < autoChange) {
		switch(mOrient) {
		case LeftBottomRightTop:
			setX2andY2(mX1 + size, mY1 + size);
			break;
		case LeftTopRightBottom:
			setX2andY1(mX1 + size, mY2 + size);
			break;
		case RightBottomLeftTop:
			setX1andY2(mX2 + size, mY1 + size);
			break;
		case RightTopLeftBottom:
			setX1andY1(mX2 + size, mY2 + size);
			break;
		}
	}

	if (abs(rectForDraw().width()) < autoChange) {
		switch(mOrient) {
		case LeftBottomRightTop:
			setX2andY2(mX1, mY2);
			break;
		case LeftTopRightBottom:
			setX2andY2(mX1, mY2);
			break;
		case RightBottomLeftTop:
			setX2andY2(mX1, mY2);
			break;
		case RightTopLeftBottom:
			setX2andY2(mX1, mY2);
			break;
		}
	}

	if (abs(rectForDraw().height()) < autoChange) {
		switch(mOrient) {
		case LeftBottomRightTop:
			setX2andY2(mX2, mY1);
			break;
		case LeftTopRightBottom:
			setX2andY2(mX2, mY1);
			break;
		case RightBottomLeftTop:
			setX1andY1(mX1, mY2);
			break;
		case RightTopLeftBottom:
			setX1andY1(mX1, mY2);
			break;
		}
	}
}

bool GestLine::isIdeal() const
{
	return (mX1 == mX2 || mY1 == mY2 || rectForDraw().width() == rectForDraw().height());
}

void GestLine::drawIdeal(QPainter *painter) {
	int autoChange = countRatio(partOfIdeal);
	if (isIdeal()) {
		painter->setPen(mPenForIdeal);
		painter->drawLine(mX1, mY1, mX2, mY2);
	} else {
		if ((abs(rectForDraw().width()) < autoChange) || (abs(rectForDraw().height()) < autoChange)
				|| sizePolicy() < (autoChange / 2)) {// revealed, that partOfIdeal fot 45 line should be smaller
			painter->setPen(mPenForIdeal);
			painter->drawRect(rectForDraw());
		}
	}
}

void GestLine::changeDragState(qreal x, qreal y)
{
	QPoint clickPoint = QPoint(x, y);
	if (mLeftScalRect.contains(clickPoint)) {
		this->mDragState = TopLeft;
	} else if (mRightScalRect.contains(clickPoint)) {
			this->mDragState = BottomRight;
		} else {
			this->mDragState = None;
		}
}

void GestLine::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == TopLeft || mDragState == BottomRight) {
		GestItem::resizeItem(event);
	}
}

void GestLine::updateScalingRects(QPointF topLeft, QPointF bottomRight)
{
	qreal X1 = topLeft.x();
	qreal Y1 = topLeft.y();
	qreal X2 = bottomRight.x();
	qreal Y2 = bottomRight.y();
	switch(mOrient) {
	case LeftBottomRightTop:
	{
		mLeftScalRect = QRectF(QPoint(X1 , Y1), QPoint(X1 + 2 * scalingDrift, Y1 + 2 * scalingDrift));
		mRightScalRect = QRectF(QPoint(X2 - 2 * scalingDrift, Y2 - 2 * scalingDrift), QPoint(X2, Y2 ));
		break;
	}
	case LeftTopRightBottom:
	{
		mLeftScalRect = QRectF(QPoint(X1, Y2 - 2 * scalingDrift), QPoint(X1 + 2 * scalingDrift, Y2));
		mRightScalRect = QRectF(QPoint(X2 - 2 * scalingDrift, Y1), QPoint(X2, Y1 + 2 * scalingDrift));
		break;
	}
	case RightBottomLeftTop:
	{
		mRightScalRect = QRectF(QPoint(X1, Y2 - 2 * scalingDrift), QPoint(X1 + 2 * scalingDrift, Y2));
		mLeftScalRect = QRectF(QPoint(X2 - 2 * scalingDrift, Y1), QPoint(X2, Y1 + 2 * scalingDrift));
		break;
	}
	case RightTopLeftBottom:
	{
		mRightScalRect = QRectF(QPoint(X1, Y1), QPoint(X1 + 2 * scalingDrift, Y1 + 2 * scalingDrift));
		mLeftScalRect = QRectF(QPoint(X2 - 2 * scalingDrift, Y2 - 2 * scalingDrift), QPoint(X2, Y2));
		break;
	}
	}
}

void GestLine::updatePorts()
{
	mDockingRects.clear();
	mDockingRects << mLeftScalRect;
	mDockingRects << mRightScalRect;
	mDockingPoints.clear();
	mDockingPoints << mapToScene(getX1Y1());
	mDockingPoints << mapToScene(getX2Y2());
}

void GestLine::connectPoints(QPointF oldPoint, QPointF newPoint)
{
	int xDiff = oldPoint.x() - newPoint.x();
	int yDiff = oldPoint.y() - newPoint.y();
	if (oldPoint == mapToScene(getX1Y1())) {
		setX1andY1(mX1 - xDiff, mY1 - yDiff);
		// if one point is already connected, it shouldn't be disconnected
		if (!mConnectedPorts.contains(mapToScene(getX2Y2()))) {
			setX2andY2(mX2 - xDiff, mY2 - yDiff);
		}
	}
	if (oldPoint == mapToScene(getX2Y2())) {
		setX2andY2(mX2 - xDiff, mY2 - yDiff);
		// if one point is already connected, it shouldn't be disconnected
		if (!mConnectedPorts.contains(mapToScene(getX1Y1()))) {
			setX1andY1(mX1 - xDiff, mY1 - yDiff);
		}
	}
}

void GestLine::setXandY(QDomElement& dom)
{
	QPointF fstPointF = mapToScene(getX1Y1());
	QPointF sndPointF = mapToScene(getX2Y2());
	QPoint fstPoint = fstPointF.toPoint();
	QPoint sndPoint = sndPointF.toPoint();
	qreal x1 = fstPoint.x();
	qreal y1 = fstPoint.y();
	qreal x2 = sndPoint.x();
	qreal y2 = sndPoint.y();

	dom.setAttribute("y1", QString::number(y1));
	dom.setAttribute("x1", QString::number(x1));
	dom.setAttribute("y2", QString::number(y2));
	dom.setAttribute("x2", QString::number(x2));
}

QDomElement GestLine::generateItem(QDomDocument &document)
{
	QDomElement line = document.createElement("line");
	setXandY(line);
	return line;
}

QList<QPoint> GestLine::getCurve(QPoint topLeftPoint)
{
	QPointF point1 = mapToScene(QPoint(mX1, mY1));
	QPointF point2 = mapToScene(QPoint(mX2, mY2));
	QList<QPoint> component;
	component.push_back(point1.toPoint() - topLeftPoint);
	component.push_back(point2.toPoint() - topLeftPoint);
	return component;
}


