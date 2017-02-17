/****************************************************************************
 ** Kata Kuntur is Copyright (C) 2010 - 2017 Jean Mazuelos(LlulluTuqo)
 ** Contact   : http://jeanmazuelos.com/contact
 **
 ** This file is part of Kata Kuntur - Data Modelling Tool.
 ** LGPL V3 License Usage
 **
 ** You should use this file under the terms of the
 ** GNU LESSER GENERAL PUBLIC LICENSE - https://www.gnu.org/licenses/lgpl-3.0.txt
 **
 ** Additionally, any Redistributions of source code must retain the above
 ** copyright notice, this condition and the following disclaimer.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ****************************************************************************/

#ifndef PANTALLADIAGRAMA_H
#define PANTALLADIAGRAMA_H
#include <QSet>
#include <QUuid>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "diagramitems_global.h"
class ItemMovible;
class Linea;
class BaseItem;
class Entity;
class SQLCore;
class QPrinter;
class QUndoStack;
class QGraphicsSceneMouseEvent;
class QGraphicsLineItem;
class QAbstractItemModel;
class QPainter;

class DIAGRAMITEMSSHARED_EXPORT PantallaDiagrama : public QGraphicsScene
{
	Q_OBJECT
	Q_PROPERTY(QString nombre READ nombreDiagrama WRITE setNombreDiagrama NOTIFY nombreChanged)
	Q_PROPERTY(QString autor READ nombreAutor WRITE setNombreAutor NOTIFY nombreAutorChanged)
	Q_PROPERTY(QString formula READ formula WRITE setFormula NOTIFY formulaChanged)
public:
	PantallaDiagrama(QObject *parent = 0);
	~PantallaDiagrama();
	enum Modo {
		Ninguno = 0,
		Seleccionado =1,
		AgregarEntidad = 2,
		AgregarRelacion = 3,
		AgregarVista = 4
	};
	Modo modo();
	void setModo(Modo modo);

	enum ModoVista {
		Logica=0,
		Fisica=1
	};
	ModoVista vista();
	void setVista(ModoVista vista);

	Entity *selectedTable();
	void eliminarItemsSeleccionados();

	QList<BaseItem *> itemsSeleccionados();
	void deseleccionarTodo();

	QString nombreFichero();
	void setNombreFichero(const QString &fileName);

	const QString nombreDiagrama();

	const QString nombreAutor();

	BaseItem *itemById(const QUuid &id);

	QUndoStack *undoStack() const;

	void agregarItem(BaseItem * item);
	template <class T> QList<T *>
	itemsByType()
	{
		QList<T *> result;
		foreach(QGraphicsItem *item, items(Qt::AscendingOrder)) {
			T *typedItem = dynamic_cast<T *>(item);
			if (typedItem) {
				result.append(typedItem);
			}
			else if(item->type() == T::Type) {
				result.append(static_cast<T *>(item));
			}
		}
		return result;
	}
	bool hasChanges();

	void print(QPainter *painter, const QRectF &target = QRectF(), const QRectF &source = QRectF());
	void print(QPrinter *printer);
	bool canExportarSql();
	void exportarSQL(QString nombreFichero);
	static int versionPantallaDiagrama() { return 2; }

	const QString dataBaseId();
	const QString dataBaseKey();
	SQLCore * dbms();

	const QString formula();

	bool isNew();
	static QString nombreAutorPredeterminado;
	static int minimoMovimiento;
	static QString foreignKeysName;
signals:
	void nombreChanged(const QString nombre);
	void formulaChanged(const QString formula);
	void nombreAutorChanged(const QString value);
	void modoChanged(PantallaDiagrama::Modo modo);
	void vistaChanged(ModoVista vista);
	void doubleClick();

public slots:
	void setDataBaseId(const QString id);
	void setDataBaseKey(const QString key);
	void setNombreDiagrama(const QString &nombre);
	void setNombreAutor(const QString &nombre);

	void setFormula(const QString formula);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent );

private:
	class DiagramDocumentPrivate;
	DiagramDocumentPrivate *const d;
};

#endif // PANTALLADIAGRAMA_H
