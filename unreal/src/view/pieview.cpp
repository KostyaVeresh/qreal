#include <QtGui>

#include <iostream>

#include "pieview.h"
//#define _LONG_DEBUG
#include "dbg.h"

//#include "uml_comment.h"

#include "treeitem.h"
#include "element.h"
#include "edge.h"

#include <stdio.h>

PieView::PieView(QWidget *parent)
{ dbg;
	scene = new QGraphicsScene(-400,-300,800,600);
	view = new QGraphicsView(scene,this);
	view->setRenderHint(QPainter::Antialiasing);

	viewport()->hide();

	setFrameStyle(QFrame::NoFrame);   
	QHBoxLayout *l = new QHBoxLayout(this);
	l->setMargin(0);
	l->setSpacing(0);
	l->addWidget(view);
	setLayout(l); 
}

void PieView::dataChanged(const QModelIndex &topLeft,
		const QModelIndex &bottomRight)
{ dbg;
}

void PieView::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{ dbg;

	QList<QModelIndex> list;
	QList<QModelIndex>::Iterator it;

	list = deselected.indexes();
	//    for (it = list.begin(); it != list.end(); ++it) {
	//        items[(*it).internalId()]->setSelected(false);
	//    }

	list = selected.indexes();
	//    for (it = list.begin(); it != list.end(); ++it) {
	//        items[(*it).internalId()]->setSelected(true);
	//    }

}

bool PieView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{ dbg;
	/*    if (index.column() == 0)
	      return QAbstractItemView::edit(index, trigger, event);
	      else
	      return false; */
}

QModelIndex PieView::indexAt(const QPoint &point) const
{ dbg;
	return QModelIndex();
}

bool PieView::isIndexHidden(const QModelIndex & /*index*/) const
{ dbg;
	return false;
}

QModelIndex PieView::moveCursor(QAbstractItemView::CursorAction cursorAction,
		Qt::KeyboardModifiers /*modifiers*/)
{ dbg;
	return currentIndex();
}

int PieView::rows(const QModelIndex &index) const
{ dbg;
	return model()->rowCount(model()->parent(index));
}

void PieView::rowsInserted(const QModelIndex &parent, int start, int end)
{ dbg;
//	clearScene();

//	for (int row = start; row <= end; ++row) {
//		QModelIndex typeIndex = model()->index(row, 0, rootIndex());
//		QString type = model()->data(typeIndex).toString();

//		QModelIndex nameIndex = model()->index(row, 1, rootIndex());
//		QString name = model()->data(nameIndex).toString();

		/*	Element *foo = new Comment;
			items[model()->index(row, 0, rootIndex()).internalId()] = foo;
			foo->setInfo(type, name);
			scene->addItem(foo);*/
//	}

}

void PieView::setModel ( QAbstractItemModel * newModel )
{ dbg;
	QAbstractItemView::setModel(newModel);
	reset();
}

void PieView::clearScene ()
{
	QList<QGraphicsItem *> list = scene->items();
	QList<QGraphicsItem *>::Iterator it = list.begin();
	for (; it != list.end(); ++it) {
		if ( *it )
			delete *it;
	}

}

void PieView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{ dbg;

	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void PieView::scrollTo(const QModelIndex &index, ScrollHint)
{ dbg;
	//    view->ensureVisible(items[index.internalId()]);
}

void PieView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{ dbg;
}

void PieView::reset()
{ dbg;
	//	QAbstractItemView::reset();

	//	clearScene();
	//	items.clear();

	for (int row = 0; row < model()->rowCount(rootIndex()); ++row) {

		QString name = model()->data( model()->index(row, 0, rootIndex()) ).toString();
		QString type = model()->data( model()->index(row, 1, rootIndex()) ).toString();

		if ( TreeItem *ti = static_cast<TreeItem *>( (model()->index(row, 0, rootIndex())).internalPointer() ) ) {

			type = ti->getType();

			QString idx =  type + "/" + name;

			if ( ! items.contains(type + "/" + name) ) {
				if ( type == "eP2N" ) {
					Edge *foo = new Edge;

					foo->setSource(static_cast<Element *> (items["nFeatured/fvvkk"]));
					foo->setDest(static_cast<Element *> (items["nFeatured/fvkk2"]));

					scene->addItem(foo);
				} else {
					Element *last = new Element;
					items[type + "/" + name] = last;
					last->setInfo(ti->getType(), ti->getName());
					scene->addItem(last);
				}
			}
		} else { 
			qDebug("unable to cast model.internalPointer to TreeItem");
		}
	}

	//    scene->update(scene->sceneRect());
	// view->scale(1,1);
	scene->update(view->sceneRect());
}

QRect PieView::visualRect(const QModelIndex &index) const
{
	return QRect();
}

QRegion PieView::visualRegionForSelection(const QItemSelection &selection) const
{
	return QRegion();
}

int PieView::horizontalOffset() const
{
	return 0;
}

int PieView::verticalOffset() const
{
	return 0;
}
