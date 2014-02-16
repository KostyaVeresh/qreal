#include "gestParser.h"
#include <QList>
#include <QPoint>
#include <QStringList>

using namespace GestEditor;

const QString comma = ", ";
const QString pointDelimeter = " : ";
const QString pathDelimeter = " | ";

QString GestParser::pathToString(QList<QList<QPoint> > const &gesture)
{
	QString result = "";
	foreach (QList<QPoint> path, gesture)
	{
		foreach (QPoint point, path)
		{
			result += pointToString(point) + pointDelimeter;
		}
		result += pathDelimeter;
	}
	// deleting last pathDelimeter
	result.chop(3);
	return result;
}

QString GestParser::pointToString(QPoint const &p)
{
	return QString("%1").arg(p.x()) + comma + QString("%1").arg(p.y());
}
