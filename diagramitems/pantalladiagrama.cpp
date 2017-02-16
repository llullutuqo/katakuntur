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

#include <cmath>
#include "conector.h"
#include "model/relacion.h"
#include "model/selfrelacion.h"
#include "hub.h"
#include <QGraphicsItem>
#include <QUndoStack>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAbstractItemModel>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QKeyEvent>
#include <QStyle>
#include <QSet>
#include <QPainter>
#include <QPrinter>
#include "pantalladiagrama.h"
#include "comandos.h"
#include "ficheros/SQL/sqlfactory.h"
#include "ficheros/ficheros.h"
#include "model/entity.h"
#include <QStringListModel>

//%t -> nombre de tabla propietaria
//%c -> nombre de la columna
//%tf -> nombre de la tabla foránea
QString PantallaDiagrama::foreignKeysName = "%t_%c";

class PantallaDiagrama::DiagramDocumentPrivate
{
public:
	DiagramDocumentPrivate() :
		m_vista(PantallaDiagrama::Logica),
		m_modo(PantallaDiagrama::Seleccionado),
		m_line(NULL),
		m_dbms(NULL),
		m_baseItem(NULL)
	{
		m_fileName = QObject::trUtf8("Nuevo diagrama*");
		m_formula =PantallaDiagrama::foreignKeysName;
		isNew = true;
	}
	~DiagramDocumentPrivate(){
		if(m_dbms)
			delete m_dbms;
		delete m_baseItem;
	}

	QMap<QString, int> counters;

	QMap<ItemMovible *, QPointF> m_movedItems;
	ModoVista m_vista;
	QUndoStack *m_undoStack;
	Modo m_modo;
	QString m_fileName;
	QString m_diag;
	QString m_autor;
	QString m_formula;
	QGraphicsLineItem *m_line;
	SQLCore *m_dbms;
	BaseItem *m_baseItem;
	bool isNew;
};

QString PantallaDiagrama::nombreAutorPredeterminado="";
int PantallaDiagrama::minimoMovimiento =10;
PantallaDiagrama::PantallaDiagrama(QObject *parent)
	: QGraphicsScene(parent), d(new DiagramDocumentPrivate)
{
	d->m_undoStack = new QUndoStack(this);
	setSceneRect(QRectF(-3000,-3000,6000,6000));
	setNombreAutor(nombreAutorPredeterminado);
}

PantallaDiagrama::~PantallaDiagrama()
{
	delete d;
}
QString PantallaDiagrama::nombreFichero() {
	return d->m_fileName;
}
void PantallaDiagrama::setNombreFichero(const QString &fileName) {
	d->m_fileName = fileName;
	d->isNew = false;
}
const QString PantallaDiagrama::nombreDiagrama() {
	return d->m_diag;
}

const QString PantallaDiagrama::nombreAutor()
{
	return d->m_autor;
}
void PantallaDiagrama::setNombreDiagrama(const QString &nombre) {
	if(d->m_diag!=nombre){
		d->m_diag = nombre;
		emit nombreChanged(nombre);
	}
}

void PantallaDiagrama::setNombreAutor(const QString &nombre)
{
	if(nombre.isEmpty()|nombre.isNull())
		return;
	if(d->m_autor!=nombre){
		d->m_autor = nombre;
		emit nombreAutorChanged(nombre);
	}
}

QUndoStack *PantallaDiagrama::undoStack() const {
	return d->m_undoStack;
}

SQLCore * PantallaDiagrama::dbms(){
	return d->m_dbms;
}

bool PantallaDiagrama::isNew()
{
	return d->isNew;
}

PantallaDiagrama::Modo
PantallaDiagrama::modo()
{
	return d->m_modo;
}

void
PantallaDiagrama::setModo(Modo modo)
{
	d->m_modo = modo;
	emit modoChanged(modo);
}

PantallaDiagrama::ModoVista
PantallaDiagrama::vista()
{
	return d->m_vista;
}

void
PantallaDiagrama::setVista(ModoVista vista)
{
	if(d->m_vista==vista)
		return;
	d->m_vista = vista;
	QList<Entity *> tab = itemsByType<Entity>();
	foreach(Entity *t,tab)
		t->actualizarBoceto();
	emit vistaChanged(vista);
}

bool PantallaDiagrama::hasChanges(){
	return this->undoStack()->canUndo();
}

QList<BaseItem *>
PantallaDiagrama::itemsSeleccionados()
{
	QList<BaseItem *> result;
	foreach(QGraphicsItem *item, selectedItems()) {
		BaseItem *typedItem = dynamic_cast<BaseItem *>(item);
		if (typedItem) {
			result.append(typedItem);
		}
	}
	return result;
}
void PantallaDiagrama::deseleccionarTodo(){
	foreach(QGraphicsItem *item, selectedItems()) {
		item->setSelected(false);
	}
}

void
PantallaDiagrama::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	switch(d->m_modo){
	case AgregarEntidad:
		if (event->button() == Qt::LeftButton) {
			Entity *table = new Entity(d->m_baseItem);
			table->setInitialName(1 + d->counters[table->typeName()]++);
			table->setPos(event->scenePos());
			undoStack()->push(new ComandoAgregarItem(this,table));
			clearSelection();
			table->setSelected(true);
			setModo(Seleccionado);
			event->accept();
			return;
		}
		break;
	case AgregarRelacion:
		if (event->button() == Qt::LeftButton) {
			d->m_line = new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));
			d->m_line->setZValue(9999.99);
			addItem(d->m_line);
			event->accept();
			return;
		}
		break;
	case Seleccionado:
		QGraphicsScene::mousePressEvent(event);
		//se determina si hay items seleccionados
		if (selectedItems().count() > 0 && event->button() == Qt::LeftButton) {
			foreach(QGraphicsItem *item, selectedItems()){
				ItemMovible * ob = qgraphicsitem_cast<ItemMovible *>(item);
				if(ob){
					d->m_movedItems[ob] = ob->pos();
				}
			}
		}
		break;
	default:
		break;
	}


}
void
PantallaDiagrama::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (d->m_line) {
		d->m_line->setLine(QLineF(d->m_line->line().p1(), event->scenePos()));
		event->accept();
		return;
	}
	QGraphicsScene::mouseMoveEvent(event);
}
void
PantallaDiagrama::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	switch(d->m_modo){
	case Seleccionado:
		if(d->m_movedItems.count()>0 && event->button() == Qt::LeftButton){
			QMapIterator<ItemMovible *, QPointF> i(d->m_movedItems);
			while (i.hasNext()) {
				i.next();
				//Si las posiciones son diferentes, se asume que han sido movidos
				//En caso contrario no pasa nada
				if(i.key()->pos()!=i.value()){
					QPointF f = i.key()->pos() - i.value();
					//Si el movimiento en las cordenadas en menor que menorMovimiento se vuelve a su estado anterior
					if(abs(f.x()) >=minimoMovimiento || abs(f.y()) >= minimoMovimiento){
						undoStack()->push(new ComandoMoverObjeto(i.key(), i.value()));
					}
					else{
						i.key()->setPos(i.value());
					}
				}
			}
			d->m_movedItems.clear();
		}
		break;
	case AgregarRelacion:
		if (d->m_line) {
			removeItem(d->m_line);
			/*Para crear autorelaciones la linea provicional debe tener
			un tamaño mayor a 42*/
			if(d->m_line->line().length()>42){
				Entity *source = qgraphicsitem_cast<Entity *>(itemAt(d->m_line->line().p1(),QTransform()));
				Entity *target = qgraphicsitem_cast<Entity *>(itemAt(event->scenePos(),QTransform()));
				ItemLinea *line = NULL;
				if (source && target ) {
					bool setpred=false;
					if(source != target){
						line = new Relacion(d->m_baseItem);
						line->setInicioObject(source);
						line->setFinalObject(target);
						setpred=true;
					}
					else{
						line = new SelfRelacion(d->m_baseItem);
						line->setInicioObject(source);
					}
					undoStack()->push(new ComandoAgregarItem(this,line));
					if(setpred)
						((Relacion *)line)->setCardinalidadFinal(Relacion::Muchos);
				}
			}
			delete d->m_line;
			d->m_line = NULL;
			setModo(Seleccionado);
			event->accept();
			return;
		}
		break;
	default:
		;
	}
	QGraphicsScene::mouseReleaseEvent(event);
}
void PantallaDiagrama::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent ){
	if(mouseEvent->button()==Qt::LeftButton){
		emit doubleClick();
	}
}

Entity *
PantallaDiagrama::selectedTable()
{
	QList<BaseItem *> items = itemsSeleccionados();
	if (items.size() != 1)
		return NULL;
	return qgraphicsitem_cast<Entity *>(items[0]);
}

void
PantallaDiagrama::eliminarItemsSeleccionados()
{
	if(itemsSeleccionados().count()<0)
		return;
	undoStack()->push(new ComandoEliminarItems(this));
}

void PantallaDiagrama::agregarItem(BaseItem * item){
	undoStack()->push(new ComandoAgregarItem(this,item));
}

BaseItem *
PantallaDiagrama::itemById(const QUuid &id)
{
	foreach (BaseItem *item, itemsByType<BaseItem>())
		if (item->id() == id)
			return item;
	return 0;
}

void
PantallaDiagrama::print(QPrinter *printer)
{
	QPainter painter(printer);
	QRectF source =itemsBoundingRect();
	if(source.height() <source.width())
		printer->setOrientation(QPrinter::Landscape);
	print(&painter, QRectF(), source);
}
void
PantallaDiagrama::print(QPainter *painter, const QRectF &target, const QRectF &source)
{
	clearSelection();
	render(painter, target, source);
}
bool PantallaDiagrama::canExportarSql(){
	return d->m_dbms;
}

const QString PantallaDiagrama::dataBaseId()
{
	if(d->m_dbms)
		return d->m_dbms->id();
	return QString();
}

const QString PantallaDiagrama::dataBaseKey()
{
	if(d->m_dbms)
		return d->m_dbms->nombreDBMS();
	return QString();
}

void PantallaDiagrama::setDataBaseId(const QString id)
{
	if(id.isEmpty()){
		if(d->m_dbms){
			delete d->m_dbms;
			d->m_dbms = NULL;
		}
		return;
	}
	if(!d->m_dbms || (d->m_dbms && d->m_dbms->id() != id)){
		d->m_dbms = SQLFactory::createById(id);
		if(!d->m_dbms){
			qWarning("No existe el gestor seleccionado");
		}
	}
}

void PantallaDiagrama::setDataBaseKey(const QString key)
{
	if(key.isEmpty()){
		if(d->m_dbms){
			delete d->m_dbms;
			d->m_dbms = NULL;
		}
		return;
	}
	const QString id = SQLFactory::getId(key);
	if(!id.isEmpty())
		setDataBaseId(id);
}

const QString PantallaDiagrama::formula()
{
	return d->m_formula;
}

void PantallaDiagrama::setFormula(const QString formula)
{
	if(formula!= d->m_formula){
		d->m_formula = formula;
		emit formulaChanged(formula);
	}
}

//-->EXPORTAR
void PantallaDiagrama::exportarSQL(QString nombreFichero){
	if(!this->canExportarSql())
		return;
	d->m_dbms->exportarScript(this,nombreFichero,this->nombreDiagrama(),QString(),SQLCore::Normal);
}


//-->END EXPORTAR
