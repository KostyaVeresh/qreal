#pragma once

#include <QtXml/QDomElement>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QFileDialog>

#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtWidgets/QButtonGroup>
#include <QtCore/QMap>


#include <qrutils/graphicsUtils/abstractItemView.h>
#include <qrkernel/ids.h>
#include "mainwindow/gestEdit/gestScene.h"
#include "mainwindow/gestEdit/gestItem.h"


#include "pluginManager/editorManagerInterface.h"
#include "models/details/logicalModel.h"

#include "view/editorView.h"
#include "gestXmlLoader.h"
#include "gestScene.h"
#include "gestParser.h"
#include <qrutils/outFile.h>


namespace Ui {
class GestEdit;
}

//typedef QList< QList<QPoint> > PathVector;
//typedef QPair<QList< QList<QPoint> >, QStringList> UsersGestures;


namespace qReal{

class GestEdit : public QWidget
{
	Q_OBJECT

public:
	explicit GestEdit(QWidget *parent = NULL);
	GestEdit(qReal::models::details::LogicalModel *model, QPersistentModelIndex const &index, int const &role
		, bool useTypedPorts);
	void load(QString const &text);

	~GestEdit();

signals:
	void saveSignal();

private:
	/// Sets headers to the file, where figure is saved as points
	/// @param fileName name of the file
	QDomElement getFirstDomElement(const QString &fileName);

	/// Watches all item-figures on a Scene and generates information about every element
	/// @return list of element's properties
	QList<QDomElement> generateGraphics();

	/// Sets header tag and writes information about all item-figures in XML (as object file)
	void generateDom();

	/// Writes formal XML file, where item-figures represented as objects
	/// @param fileName name of XML file
	void exportToXml(QString const &fileName);

	/// Writes formal XML file, where item-figures represented as amount of points
	/// @param fileName name of XML file
	void exportInPoints(QString const &fileName);

	/// Union buttons in group
	void initButtonGroup();

	/// Higlights only one button
	void setHighlightOneButton(QAbstractButton *oneButton);

	/// Creates pair of
	///	first list of paths of item-figures (amount of points)
	/// second list of item-figures names
	QPair<QList< QList<QPoint> >, QStringList> createGest();

	/// Makes top left point from coordinates of all item-figures
	/// @return top left point, where x - the most left(small) coordinate, y - most top(small) coordinate
	QPoint getTopLeftPoint() const;

	/// Sets header tag and writes information about all item-figures in XML (as amount of points)
	/// @param gesture pair of: first list of paths of item-figures (amount of points), second list of item-figures names
	/// @param doc document representation
	/// @param fileName name of file
	void saveDocument(const QPair<QList< QList<QPoint> >, QStringList> &gesture, QDomDocument doc,
								 QString const & fileName);
	void init();

	//GestEditor::Scene *mScene;
	qreal mX1;
	qreal mX2;
	qreal mY1;
	qreal mY2;
	QDomElement mDomElement;

	GestEditor::GestScene *mScene;  // Has ownership.
	QGraphicsItemGroup mItemGroup;
	QList<QAbstractButton *> mButtonGroup;  // Doesn't have direct ownership (owned by mUi).
	QDomDocument mDocument;
	QPoint mTopLeftPicture;
	Ui::GestEdit *mUi;  // Has ownership.

	// TODO: lolwut? Use assist API instead.
	qReal::models::details::LogicalModel *mModel;  // Doesn't have ownership.
	QPersistentModelIndex const mIndex;
	int const mRole;
	Id mId;
	EditorManagerInterface *mEditorManager;  // Doesn't have ownership.
	IdList mGraphicalElements;
	MainWindow *mMainWindow;  // Doesn't have ownership.
	EditorView *mEditorView;  // Doesn't have ownership.

	bool mUseTypedPorts;

private slots:
	/// Calls Scene's drawLine method
	void drawLine();

	/// Calls Scene's drawRect method
	void drawRect();

	/// Calls Scene's drawEllipse method
	void drawEllipse();

	/// Calls Scene's drawCurve method
	void drawCurve();

	/// Calls Scene's deleteSelectedItem method
	void deleteItem();

	/// Calls Scene's refresh method
	void clear();

	/// Opens XML file and Adds figures from file
	void open();

	/// Saves figures in XML file as objects
	void saveToXml();

	/// Saves figures in XML file as points
	void saveInPoints();

	/// Resrts higlight from buttons
	void resetHighlightAllButtons();


};
}
