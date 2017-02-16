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

#include <QTextStream>
#include "loadsaveproject.h"
#include "xmlutils.h"
#include "pantalladiagrama.h"
#include "ficheros/ficheros.h"
#include "model/selfrelacion.h"
#include "model/relacion.h"
#include "hub.h"
#include "model/entity.h"
#include "model/entityattibute.h"
#include <QTextDocumentWriter>
#include <QPrinter>
#include <cmath>
#include <QGraphicsScene>

QString LoadSaveProject::m_lastError;
LoadSaveProject::LoadSaveProject()
{
}
bool LoadSaveProject::guardar(const QString &fileName,PantallaDiagrama *pantalla){
	QDomDocument doc;

	QDomProcessingInstruction xml = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(xml);

	QDomElement root = doc.createElement("diagrama");
	root.setAttribute("nombreProyecto", pantalla->nombreDiagrama());
	root.setAttribute("version", versionActual);
	root.setAttribute("nombreAutor", pantalla->nombreAutor());
	root.setAttribute("formula",pantalla->formula());
	doc.appendChild(root);

	if(pantalla->dbms())
		XmlUtils::appendStringElement(doc,root,"DataBase",pantalla->dataBaseId());
//TODO: Descomentar
//	if(pantalla->canExportORM())
//		XmlUtils::appendStringElement(doc,root,"ORM",pantalla->ORMId());
	QDomElement itemList = doc.createElement("lista-item");
	root.appendChild(itemList);

	foreach (Entity *item, pantalla->itemsByType<Entity>()) {
		itemList.appendChild(TablaToXml(item,doc));
	}

	foreach (ItemLinea *item, pantalla->itemsByType<ItemLinea>()) {
		itemList.appendChild(itemToXml(item,doc));
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream stream(&file);
		doc.save(stream, 2);
		file.close();
	}
	else{
		m_lastError = trUtf8("No se puede escribir el fichero, compruebe que tiene permisos de escritura.");
		return false;
	}

	pantalla->setNombreFichero(fileName);
	return true;
}
bool LoadSaveProject::exportaraPNG(QString &filename,PantallaDiagrama *pantalla){
	QRectF boundingRect= pantalla->itemsBoundingRect().adjusted(-5, -5, 5, 5);
	QSize size(int(ceil(boundingRect.width())),int(ceil(boundingRect.height())));
	QImage image(size, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&image);
	painter.fillRect(image.rect(), Qt::white);
	pantalla->print(&painter, image.rect(), boundingRect);
	if(!filename.endsWith(".png"))
		filename +=".png";
	if(!image.save(filename, "PNG",10)){
		m_lastError=trUtf8("No se puede guardar la imagen %1, verifique que tiene permisos de escritura.").arg(filename);
		return false;
	}
	return true;
}
bool LoadSaveProject::exportarMapeoODT(QString filename,PantallaDiagrama * pantalla,int ejemplos){
	QTextDocumentWriter doc(filename,"odf");
	if(!doc.write(generarMapeo(pantalla,ejemplos))){
		m_lastError=trUtf8("No se puede guardar el archivo %1, verifique que tiene permisos de escritura.").arg(filename);
		return false;
	}
	return true;
}

bool LoadSaveProject::exportarDiccinarioDatosODT(QString filename, PantallaDiagrama *pantalla, const QString &format)
{
	QString html = generarDiccionarioDatos(pantalla);
	QTextDocumentWriter doc(filename);
	if(format == "html"){
		doc.setFormat("HTML");
	} else if (format == "odt"){
		doc.setFormat("ODF");
	} else {
		m_lastError = trUtf8("No se ha especificado un formato válido");
		return false;
	}
	QTextDocument document(pantalla);
	document.setHtml(html);
	if(!doc.write(&document)){
		m_lastError=trUtf8("No se puede guardar el archivo %1, verifique que tiene permisos de escritura.").arg(filename);
		return false;
	}
	return true;
}

bool LoadSaveProject::exportarMapeoPNG(QString filename,PantallaDiagrama * pantalla,int ejemplos){
	QTextDocument *doc =generarMapeo(pantalla,ejemplos);
	QImage image(doc->size().width(),doc->size().height(), QImage::Format_RGB32);
	QPainter painter(&image);
	painter.fillRect(image.rect(), Qt::white);
	doc->drawContents(&painter);
	if(!image.save(filename, "PNG",10)){
		m_lastError=trUtf8("No se puede guardar la imagen %1, verifique que tiene permisos de escritura.").arg(filename);
		return false;
	}
	return true;
}
bool LoadSaveProject::exportarYAML(QString filename,PantallaDiagrama * pantalla){
	QString tx;
	tx.append("#Exportado con Kata Kuntur - Modelador de Datos\r\n");
	tx.append("propel:\r\n");
	foreach (Entity *item, pantalla->itemsByType<Entity>()) {
		QString salda,saldb;
		salda.append("\r\n  " + item->nombre().toLower() + ":");
		foreach (EntityAttibute *column, item->columnas()) {
			if(!column->isForeignKey()){
				QString ss =" ";
				if(column->isPrimaryKey())
					ss =", primaryKey: true ";
				salda.append("\r\n    " + column->name() + ":  {" +column->toText() +ss + "}");
			}
			else{
				if(column->isPrimaryKey()){
					saldb.append("\r\n    " + column->name() + ":  {" +column->toText() +", primaryKey: true, foreignTable:" +column->foreignTable()->nombre().toLower()+", foreignReference: "+column->name()+"}");
				}
				else{
					saldb.append("\r\n    " + column->name() + ":  {" +column->toText() +", foreignTable:" +column->foreignTable()->nombre().toLower()+", foreignReference: "+column->name()+"}");
				}
			}
		}
		salda +=saldb;
		tx.append("\r\n" + salda);
	}
	Ficheros fich;
	if(!fich.exportar(tx,filename)){
		m_lastError =trUtf8("No se puede guardar el archivo %1, verifique que tiene permisos de escritura..");
		return false;
	}
	return true;
}

QTextDocument * LoadSaveProject::generarMapeo(PantallaDiagrama *pantalla,int ejemplos){
	QTextDocument *doc = new QTextDocument(pantalla);
	QString html;
	html +="<html>\n";
	html +="<body>\n";
	foreach (Entity *item, pantalla->itemsByType<Entity>()) {
		html += "<p>&nbsp;</p>";
		html += "<h3>"+item->nombre() +"</h3>\n";
		html +="<table style=\"border-color:#000000;border-style:solid;width:100%\" valign=\"top\" border=\"1\"  cellpadding=\"0\" cellspacing=\"0\">" ;
		QString tr1,tr2;
		tr1 +="<td>Columna<br />Tipo<br />Nulabilidad</td>";
		foreach(EntityAttibute *col, item->columnas()){
			tr1 +="<td align=\"center\">"+col->name()+"<br />";
			if(col->isPrimaryKey())
				tr1 +=" PK ";
			if(col->isForeignKey())
				tr1 +=" FK ";
			tr1 +="<br />";
			if(col->isNull())
				tr1 +="N";
			else
				tr1 +="NN";
			tr1 +="</td>\n";
			tr2 +="<td></td>\n";
		}
		html +="<tr>\n";
		html += tr1;
		html +="</tr>\n";
		for(int i =0;i<ejemplos;i++){
			html +="<tr>\n";
			html +="<td>Ejemplo "+QString::number(i+1)+":</td>";
			html += tr2;
			html +="</tr>\n";
		}
		html +="</table>\n";
	}
	html +="</body>\n";
	html +="</html>\n";
	doc->setHtml(html);
	return doc;
}

QString LoadSaveProject::generarDiccionarioDatos(PantallaDiagrama *pantalla)
{
	QTextDocument *doc = new QTextDocument(pantalla);
	QString html;
	html +="<html>\n";
	html +="<body>\n";
	html += "<h2>" + trUtf8("Proyecto:") + pantalla->nombreDiagrama() +"</h2>";

	foreach (Entity *item, pantalla->itemsByType<Entity>()) {
		html += "<h3>"+item->nombre() +"</h3>\n";
		html +="<table style=\"width:100%;border-color:#000000;border-style:solid\" valign=\"top\" border=\"1\"  cellpadding=\"0\" cellspacing=\"0\">" ;
		html +="<tr>";
		html +="<td>"+trUtf8("Nº")+"</td>";
		html +="<td>"+trUtf8("Atributo")+"</td>";
		html +="<td>"+trUtf8("Llave Primaria")+"</td>";
		html +="<td>"+trUtf8("Nulo")+"</td>";
		html +="<td>"+trUtf8("Descripción")+"</td>";
		html += "</tr>";
		int n = 1;
		foreach(EntityAttibute *col, item->columnas()){

			html +="<tr>";
			html +="<td>"+QString::number(n)+"</td>";
			html +="<td>"+col->name()+"</td>";
			html +="<td>"+(col->isPrimaryKey()? trUtf8("SÍ"):trUtf8("NO"))+"</td>";
			html +="<td>"+(col->isNull()? trUtf8("SÍ"):trUtf8("NO"))+"</td>";
			html +="<td>"+(col->isForeignKey()?trUtf8("Llave Foránea."):"") + col->notes()+"</td>";
			html += "</tr>";
			n++;
		}
		html +="</table>\n";
	}
	html +="</body>\n";
	html +="</html>\n";
	doc->setHtml(html);
	return html;
}

PantallaDiagrama *LoadSaveProject::cargar(const QString &fileName){
	PantallaDiagrama *pantalla=NULL;
	QDomDocument doc;
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly)) {
		if (!doc.setContent(&file)) {
			m_lastError = trUtf8("El fichero no es un formato de diagrama válido.");
			return NULL;
		}
		file.close();
	}
	else {
		m_lastError = trUtf8("No se puede abrir el archivo %1 para leerlo. Verifique que el archivo existe y que tenga permisos de lectura.").arg(fileName);
		return NULL;
	}
	pantalla = new PantallaDiagrama();
	pantalla->setNombreFichero(fileName);
	QDomElement root = doc.firstChildElement("diagrama");
	pantalla->setNombreDiagrama(root.attribute("nombreProyecto", ""));
	pantalla->setNombreAutor(root.attribute("nombreAutor", PantallaDiagrama::nombreAutorPredeterminado));
	pantalla->setFormula(root.attribute("formula",PantallaDiagrama::foreignKeysName));
	int version =root.attribute("version", "0").toInt();
	if(version ==0){
		m_lastError = trUtf8("No está soportado esta versión del diagrama, Utilice la última versión de QCaseModel para pasarlo a una version compatible con Kata Kuntur.");
		return NULL;
	}
	if(version >versionActual){
		m_lastError = trUtf8("La versión actual soportada por el modelador es la %1 sin embargo el diagrama que intenta abrir tiene una versión %2. Debes descargar la última versión de <a href=\"http://katakuntur.jeanmazuelos.com/downloads\">Kata Kuntur</a> para abrirlo.").arg(versionActual).arg(version);
		return NULL;
	}

	//Carga el DBMS
	if(version < 6)
		pantalla->setDataBaseKey(XmlUtils::readStringElement(root,"DBMS",""));
	else
		pantalla->setDataBaseId(XmlUtils::readStringElement(root,"DataBase",""));

	//Carga el modelador ORM
	//TODO: Uncomment
	//if(version < 8)
	//	pantalla->setORMKey(XmlUtils::readStringElement(root,"ORM-Model",""));
	//else
	//	pantalla->setORMId(XmlUtils::readStringElement(root,"ORM",""));

	QDomElement itemListElement = root.firstChildElement("lista-item");
	QDomElement itemElement = itemListElement.firstChildElement("item");
	while (!itemElement.isNull()) {
		//QString itemTypeName = itemElement.attribute("type");
		BaseItem *item = itemFromXml(itemElement,pantalla,version);
		if (item){
			//item->cargarXml(itemElement,version, pantalla);
			pantalla->addItem(item);
		}
		itemElement = itemElement.nextSiblingElement("item");
	}
	pantalla->setSceneRect(QRectF());
	return pantalla;
}

QDomElement LoadSaveProject::itemToXml(BaseItem *item,QDomDocument doc, int version){
	Q_ASSERT(item);
	Q_UNUSED(version);

	QString tipo = item->typeName();
	//modelo-tabla
	//modelo-relacion
	//modelo-selfrelacion
	if(tipo=="modelo-tabla"){
		Entity * tabla = qobject_cast<Entity *>(item);
		return TablaToXml(tabla,doc);
	}
	if(tipo=="modelo-relacion"){
		Relacion * relacion = qobject_cast<Relacion *>(item);
		return RelacionToXml(relacion,doc);
	}
	if(tipo=="modelo-selfrelacion"){
		SelfRelacion * selfrelacion = qobject_cast<SelfRelacion *>(item);
		return SelfRelacionToXml(selfrelacion,doc);
	}
	//SI el item es conocido no debe llegar aquí :)
	Q_ASSERT(true);
	return QDomElement();
}

BaseItem * LoadSaveProject::itemFromXml(QDomElement element,PantallaDiagrama * pantalla,int version){
	QString itemTypeName = element.attribute("type");
	//modelo-tabla
	//modelo-relacion
	//modelo-selfrelacion
	if(itemTypeName=="modelo-tabla"){
		return tablaFromXml(element,version);
	}
	if(itemTypeName=="modelo-relacion"){
		return relacionFromXml(element,pantalla,version);
	}
	if(itemTypeName=="modelo-selfrelacion"){
		return selfRelacionFromXml(element,pantalla,version);
	}
	//qWarning() << "Tipo desconocido:" << itemTypeName;
	return NULL;
}

//GUARDADO DE ITEMS
QDomElement LoadSaveProject::TablaToXml(Entity* tabla,QDomDocument doc)
{
	QDomElement element = doc.createElement("item");
	element.setAttribute("type", tabla->typeName());
	element.setAttribute("id", tabla->id().toString());
	XmlUtils::appendStringElement(doc, element, "nombre", tabla->nombre());
	XmlUtils::appendStringElement(doc, element, "descripcion", tabla->descripcion());
	XmlUtils::appendPointElement(doc, element, "posicion", tabla->pos());

	QDomElement listacolumnas = doc.createElement("lista-columnas");
	element.appendChild(listacolumnas);
	foreach (EntityAttibute *column, tabla->columnas()) {
		if(column->isForeignKey())
			continue;
		QDomElement columnElement = doc.createElement("columna");
		listacolumnas.appendChild(columnElement);
		columnElement.setAttribute("id", column->id().toString());
		columnElement.setAttribute("nombre", column->name());
		columnElement.setAttribute("tipoDato", column->dataType());
		columnElement.setAttribute("llaveUnica", column->llaveUnica());
		columnElement.setAttribute("llaveprimaria", column->isPrimaryKey());
		columnElement.setAttribute("notas", column->notes());
		columnElement.setAttribute("nulo", column->isNull());
	}
	return element;
}
QDomElement LoadSaveProject::RelacionToXml(Relacion* relacion,QDomDocument doc)
{
	QDomElement element = doc.createElement("item");
	element.setAttribute("type", relacion->typeName());
	element.setAttribute("id", relacion->id().toString());
	//appendPointElement(doc, element, "position", relacion->pos());
	QDomElement inicio = doc.createElement("inicio");
	element.appendChild(inicio);
	Conector * conec = relacion->conector(ItemLinea::StartConector);
	XmlUtils::appendPointElement(doc, inicio, "posicion", conec->pos());
	XmlUtils::appendFloatElement(doc, inicio, "angulo", conec->angle());
	XmlUtils::appendStringElement(doc, inicio,"label",relacion->labelInicio());
	XmlUtils::appendEnumElement(doc, inicio, "cardinalidad", relacion->cardinalidadInicio(), relacion, "Cardinalidad");
	XmlUtils::appendEnumElement(doc,inicio, "cardinalidadLinea", relacion->cardinalidadLineaInicio(), relacion, "CardinalidadLinea");
	if (conec->hub()) {
		XmlUtils::appendStringElement(doc, inicio,"owner",conec->hub()->owner()->id().toString());
	}

	QDomElement final = doc.createElement("final");
	element.appendChild(final);
	conec = relacion->conector(ItemLinea::EndConector);
	XmlUtils::appendPointElement(doc, final, "posicion", conec->pos());
	XmlUtils::appendFloatElement(doc, final, "angulo", conec->angle());
	XmlUtils::appendStringElement(doc, final,"label",relacion->labelFinal());
	XmlUtils::appendEnumElement(doc, final, "cardinalidad", relacion->cardinalidadFinal(), relacion, "Cardinalidad");
	XmlUtils::appendEnumElement(doc, final, "cardinalidadLinea", relacion->cardinalidadLineaFinal(), relacion, "CardinalidadLinea");
	if (conec->hub()) {
		XmlUtils::appendStringElement(doc, final,"owner",conec->hub()->owner()->id().toString());
	}
	return element;
}
QDomElement LoadSaveProject::SelfRelacionToXml(SelfRelacion *selfrelacion,QDomDocument doc)
{
	QDomElement element = doc.createElement("item");
	element.setAttribute("type", selfrelacion->typeName());
	element.setAttribute("id", selfrelacion->id().toString());
	QDomElement inicio = doc.createElement("inicio");
	element.appendChild(inicio);
	Conector * con =selfrelacion->conector(ItemLinea::StartConector);
	XmlUtils::appendPointElement(doc, inicio, "posicion", con->pos());
	XmlUtils::appendFloatElement(doc, inicio, "angulo", con->angle());
	XmlUtils::appendStringElement(doc, inicio,"label",selfrelacion->labelInicio());

	QDomElement final = doc.createElement("final");
	element.appendChild(final);
	con = selfrelacion->conector(ItemLinea::EndConector);
	XmlUtils::appendPointElement(doc, final, "posicion", con->pos());
	XmlUtils::appendFloatElement(doc, final, "angulo", con->angle());
	XmlUtils::appendStringElement(doc, final,"label",selfrelacion->labelFinal());
	if (con) {
		XmlUtils::appendStringElement(doc, element,"owner",con->hub()->owner()->id().toString());
	}
	return element;
}

Entity * LoadSaveProject::tablaFromXml(QDomElement element,int version)
{
	Entity * tabla = new Entity();
	tabla->setId(element.attribute("id", QUuid().toString()));
	switch(version){
	case 1:
	case 2:
	{
		QDomElement tableElement;
		tabla->setPos(XmlUtils::readPointElement(element, "position"));
		tableElement = element.firstChildElement("tabla");
		if (!tableElement.isNull()) {
			tabla->setNombre(XmlUtils::readStringElement(tableElement, "nombre"));
			tabla->setDescripcion(XmlUtils::readStringElement(tableElement, "descripcion"));
			QDomElement columnListElement = tableElement.firstChildElement("lista-columnas");
			QDomElement columnElement = columnListElement.firstChildElement("columna");
			while (!columnElement.isNull()) {
				EntityAttibute *column = new EntityAttibute(tabla);
				column->setName(columnElement.attribute("nombre"));
				column->setDataType(columnElement.attribute( "tipo-dato"));
				column->setLlaveUnica((columnElement.attribute( "llaveUnica").toInt() ? "1" : QString()));
				column->setPrimaryKey(columnElement.attribute( "llaveprimaria").toInt());
				column->setNotes(columnElement.attribute( "notas"));
				column->setNull(columnElement.attribute( "nulo").toInt());
				tabla->agregarColumna(column);
				columnElement = columnElement.nextSiblingElement("columna");
			}
		}
	}
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		QPointF p = XmlUtils::readPointElement(element, "posicion");
		tabla->setPos(QPointF(qRound(p.x()),qRound(p.y())));
		tabla->setNombre(XmlUtils::readStringElement(element, "nombre"));
		tabla->setDescripcion(XmlUtils::readStringElement(element, "descripcion"));
		QDomElement columnListElement = element.firstChildElement("lista-columnas");
		QDomElement columnElement = columnListElement.firstChildElement("columna");
		while (!columnElement.isNull()) {
			EntityAttibute *column = new EntityAttibute(tabla);
			if(version>=4)
				column->setId(columnElement.attribute("id"));
			column->setName(columnElement.attribute("nombre"));
			column->setDataType(columnElement.attribute( "tipoDato"));
			if(version <= 6)
				column->setLlaveUnica((columnElement.attribute( "llaveUnica").toInt() ? "1" : QString()));
			else
				column->setLlaveUnica(columnElement.attribute( "llaveUnica"));
			column->setPrimaryKey(columnElement.attribute( "llaveprimaria").toInt());
			column->setNotes(columnElement.attribute( "notas"));
			column->setNull(columnElement.attribute( "nulo").toInt());
			tabla->agregarColumna(column);
			columnElement = columnElement.nextSiblingElement("columna");
		}
		break;
	}
	return tabla;
}
Relacion * LoadSaveProject::relacionFromXml(QDomElement element,PantallaDiagrama *document, int version){
	Relacion *relacion= new Relacion();
	relacion->setId(element.attribute("id", QUuid().toString()));
	//relacion->setPos(readPointElement(element, "position"));
	QDomElement elIcicio,elFinal;
	switch(version){
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		elIcicio = element.firstChildElement("inicio");
		elFinal = element.firstChildElement("final");
		QString ownera =XmlUtils::readStringElement(elIcicio,"owner","");
		if(!ownera.isEmpty()){
			ItemMovible *owner = qobject_cast<ItemMovible *>(document->itemById(ownera));
			relacion->setInicioObject(owner);
		}

		QString ownerb =XmlUtils::readStringElement(elFinal,"owner","");
		if(!ownerb.isEmpty()){
			ItemMovible *owner = qobject_cast<ItemMovible *>(document->itemById(ownerb));
			relacion->setFinalObject(owner);
		}

		relacion->setLabelInicio(XmlUtils::readStringElement(elIcicio,"label",""));
		relacion->setCardinalidadInicio(XmlUtils::readEnumElement(elIcicio, "cardinalidad", Relacion::Uno, relacion, "Cardinalidad"));
		relacion->setCardinalidadLineaInicio(XmlUtils::readEnumElement(elIcicio, "cardinalidadLinea", Relacion::Puede, relacion, "CardinalidadLinea"));
		relacion->conector(ItemLinea::StartConector)->setPos(XmlUtils::readPointElement(elIcicio,"posicion"));
		relacion->conector(ItemLinea::StartConector)->setAngle(XmlUtils::readFloatElement(elIcicio,"angulo"));

		relacion->setLabelFinal(XmlUtils::readStringElement(elFinal,"label",""));
		relacion->setCardinalidadLineaFinal(XmlUtils::readEnumElement(elFinal, "cardinalidadLinea", Relacion::Puede, relacion, "CardinalidadLinea"));
		relacion->setCardinalidadFinal(XmlUtils::readEnumElement(elFinal, "cardinalidad", Relacion::Uno,relacion, "Cardinalidad"));
		relacion->conector(ItemLinea::EndConector)->setPos(XmlUtils::readPointElement(elFinal,"posicion"));
		relacion->conector(ItemLinea::EndConector)->setAngle(XmlUtils::readFloatElement(elFinal,"angulo"));
		//HACK:
		//Se quita el hack, al parecer ya no es necesario
		//relacion->updateLayout();
		//relacion->updateCache();
		//end
		break;
	}
	return relacion;
}
SelfRelacion *LoadSaveProject::selfRelacionFromXml(QDomElement element,PantallaDiagrama *document,int version){
	SelfRelacion * selfrelacion = new SelfRelacion;
	switch(version){
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		selfrelacion->setId(element.attribute("id", QUuid().toString()));
		selfrelacion->setPos(XmlUtils::readPointElement(element, "position"));
		QDomElement analisis;

		analisis = element.firstChildElement("inicio");
		selfrelacion->setLabelInicio(XmlUtils::readStringElement(analisis,"label",""));
		selfrelacion->conector(ItemLinea::StartConector)->setPos(XmlUtils::readPointElement(analisis,"posicion"));
		selfrelacion->conector(ItemLinea::StartConector)->setAngle(XmlUtils::readFloatElement(analisis,"angulo"));


		analisis = element.firstChildElement("final");
		selfrelacion->setLabelFinal(XmlUtils::readStringElement(analisis,"label",""));
		selfrelacion->conector(ItemLinea::EndConector)->setPos(XmlUtils::readPointElement(analisis,"posicion"));
		selfrelacion->conector(ItemLinea::EndConector)->setAngle(XmlUtils::readFloatElement(analisis,"angulo"));

		QString ownera =XmlUtils::readStringElement(element,"owner","");
		if(!ownera.isEmpty()){
			ItemMovible *owner = qobject_cast<ItemMovible *>(document->itemById(ownera));
				selfrelacion->setInicioObject(owner);
		}
		//HACK:
		//Se quita el hack, al parecer ya no es necesario
		//selfrelacion->updateLayout();
		//selfrelacion->updateCache();
		//end
		break;
	}
	return selfrelacion;
}

QString LoadSaveProject::lastError(){
	return m_lastError;
}
