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

#include "uifile.h"
#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QHash>
#include "vars.h"
#include "model/entityattibute.h"
#include "model/entity.h"
#include "ficheros/SQL/sqlcore.h"
#include "utils.h"
#include "classfile.h"
struct LayoutOrg{
	int row;
	int col;
};
class UiFile::UiFilePrivate{
public:
	UiFilePrivate(){
		uiElements.append(m_doc.createElement("resources"));
		uiElements.append(m_doc.createElement("connections"));
		m_i=0;
//		<customwidgets>
//		 <customwidget>
//		  <class>AComboModel</class>
//		  <extends>QWidget</extends>
//		  <header location="global">acombomodel.h</header>
//		  <container>1</container>
//		 </customwidget>
//		</customwidgets>
		customWidgets = m_doc.createElement("customwidgets");
		addCustomWidget(customWidgets,"KDataPanel","kdatapanel.h","QWidget");
		addCustomWidget(customWidgets,"KComboModel","kcombomodel.h","QComboBox");
	}
	~UiFilePrivate(){
	}
	void addCustomWidget(QDomElement &element,QString className,QString hFile,QString baseClass){
		QDomElement cus = m_doc.createElement("customwidget");
		QDomElement clas= m_doc.createElement("class");
		//"AComboModel"
		clas.appendChild(m_doc.createTextNode(className));
		cus.appendChild(clas);
		//"QComboBox"
		clas= m_doc.createElement("extends");
		clas.appendChild(m_doc.createTextNode(baseClass));
		cus.appendChild(clas);

		//"acombomodel.h"
		clas= m_doc.createElement("header");
		clas.setAttribute("location","global");
		clas.appendChild(m_doc.createTextNode(hFile));
		cus.appendChild(clas);

		clas= m_doc.createElement("container");
		clas.appendChild(m_doc.createTextNode("1"));
		cus.appendChild(clas);

		element.appendChild(cus);
	}

	void addProperty(QDomElement &element,QString propertyName,UiFile::UiPropertyType type,QString value){
		QDomElement prop =m_doc.createElement("property");
		prop.setAttribute("name",propertyName);
		QDomElement val=m_doc.createElement(propertyToString(type));
		val.appendChild(m_doc.createTextNode(value));
		prop.appendChild(val);
		element.appendChild(prop);
	}
	void addProperty(QString widgetName,QString propertyName,UiFile::UiPropertyType type,QString value){
		if(elements.contains(widgetName)){
			QDomElement doc = elements.value(widgetName);
			addProperty(doc,propertyName,type,value);
			elements.insert(widgetName,doc);
		}
	}
	void addWidget(QString classType,QString name){
		QDomElement wid = m_doc.createElement("widget");
		wid.setAttribute("class",classType);
		wid.setAttribute("name",name);
		elements.insert(name,wid);
	}

	QString propertyToString(UiFile::UiPropertyType type){
		switch(type){
		case UiFile::String:
			return "string";
			break;
		case UiFile::Number:
			return "number";
			break;
		case UiFile::Bool:
			return "bool";
		}
		Q_ASSERT(false);
		return "";
	}
	QList<QDomElement> actionElement;
	QList<QDomElement> uiElements;
	QHash<QString,QDomElement> elements;
	QMap<QString,LayoutOrg> layoutElements;
//	QString m_classname;
	QString m_className;
	QString m_father;
	QString m_window;
	QDomDocument m_doc;
	QDomElement customWidgets;
	//FROM WIDGETEXPORT
	QString m_windowTitle;
	QString m_widget;
	QString m_subdir;
	QString m_codeConstructor;
	QString m_new;
	QString m_modify;
	int m_i;
	QString m_central_atribute;
	QString m_central_class;
	QString m_limpiar;
	QString m_buscar;
	QString m_guardar;
	ClassFile clas;
};

UiFile::UiFile():d(new UiFilePrivate)
{
}
UiFile::~UiFile()
{
	delete d;
}
void UiFile::setClassName(QString name){
	d->m_className = name;
}
void UiFile::setOwnerClassName(QString value){
		d->m_central_class =Utils::getTitleCase(value);
		d->m_central_atribute ="m_" + Utils::getUniformName(value);
}
QString UiFile::className(){
	return d->m_className;
}
void UiFile::setWindowTitle(QString value){
	d->m_window = value;
}

QString UiFile::fatherClassName(){
	return d->m_father;
}

void  UiFile::setFatherClassName(QString value){
	d->m_father = value;
}

void UiFile::addWidget(int row,int col,QString classType,QString name,QString primayText,int minLimit){
	d->addWidget(classType,name);
	if(!primayText.isEmpty()){
		d->addProperty(name,"text",String,primayText);
	}
	if(minLimit!=0){
		d->addProperty(name,"maxLength",Number,QString::number(minLimit));
	}
	LayoutOrg es;
	es.col =col;
	es.row=row;
	d->layoutElements.insert(name,es);
}
void UiFile::addWidgetForData(int row,QString dataType,QString name,QString primayText,int minLimit){
	name =Utils::getControlAbbreviation(Utils::getClassFromData(dataType)) + name;
	addWidget(row,1,Utils::getClassFromData(dataType),name,primayText,minLimit);
}

void UiFile::addQLabel(int row,QString name,QString primayText){
	addWidget(row,0,"QLabel","lb"+name,primayText,0);
}
void UiFile::addColumElement(EntityAttibute *col,SQLCore *dbms){
	if(!col->isForeignKey()){
		QString codeconnect="\n\tconnect(%1,SIGNAL(%2),%3,SLOT(%4));";
		QString normal=Utils::getUniformName(col->name());
		QString title= Utils::getTitleCase(normal);
		QString titleWithSpaces =Utils::getTitleUniformName(col->name());
		QString tip;
		QString m_dataTipe;
		if(!col->isPrimaryKey())
			tip = dbms->toStandarQtType(col->dataType());
		else{
			tip = "Model";
			d->m_new +="\n\t" + d->m_central_atribute +"->setAsNew();";
			m_dataTipe=dbms->toStandarQtType(col->dataType());
		}
		QString num =col->dataType().mid(col->dataType().indexOf("(")+1,col->dataType().length() - col->dataType().indexOf("(")-2);
		addQLabel(d->m_i,title,titleWithSpaces);
		addWidgetForData(d->m_i,tip,title,QString(),num.toInt());
		QString controlName=Utils::getClassFromData(tip);
		QString wname =Utils::getControlAbbreviation(controlName)+title;
		QString wsignal;
		QString wslot;
		if(controlName=="QLineEdit"){
			wsignal="textEdited(QString)";
			wslot="setText(QString)";
			m_dataTipe="QString";
		}
		else if(controlName=="QDateTimeEdit"){
			wsignal="dateTimeChanged(QDateTime)";
			wslot="setDateTime(QDateTime)";
			m_dataTipe="QDateTime";
		}
		else if(controlName=="QSpinBox"){
			wsignal="valueChanged(int)";
			wslot="setValue(int)";
			m_dataTipe="int";
		}
		else if(controlName=="QCheckBox"){
			wsignal="toggled(bool)";
			wslot="setChecked(bool)";
			m_dataTipe="bool";
		}
		else if(controlName=="QComboBox"){
			//wsignal="dateTimeChanged(QDateTime)";
			//wslot="setDateTime(QDateTime)";
			//m_dataTipe="QDateTime";
		}
		else if(controlName=="QDateEdit"){
			wsignal="dateChanged(QDate)";
			wslot="setDate(QDate)";
			m_dataTipe="QDate";
		}
		else if(controlName=="QDoubleSpinBox"){
			wsignal="valueChanged(double)";
			wslot="setValue(double)";
			m_dataTipe="double";
		}
		else if(controlName=="QTimeEdit"){
			wsignal="timeChanged(QTime)";
			wslot="setTime(QTime)";
			m_dataTipe="QTime";
		}
		else if(controlName=="KComboModel"){
			wsignal="selectedRow(QVariant)";
			wslot="setValue("+m_dataTipe+")";
		}
		else if(controlName=="KSelectFile"){
			wsignal="valueChanged(const QByteArray)";
			wslot="setValue("+m_dataTipe+")";
		}
		if(col->isPrimaryKey()){
			d->m_codeConstructor +="\n\tthis->ui->kcm"+title+"->setDataClass("+d->m_central_atribute+");";
			d->m_modify +="\n\t" + d->m_central_atribute +"->setAsNew();";
			d->m_new +="\n\tthis->ui->kcm"+title+"->setEnableDropDown(false);";
			d->m_modify +="\n\tthis->ui->kcm"+title+"->setEnableDropDown(true);\n\tthis->buscar();";
			d->m_buscar +="\n\tthis->ui->kcm"+title+"->showPopup();";
			if(!dbms->isAutoincrementDataType(col->dataType())){
				tip =dbms->toStandarQtType(col->dataType());
				d->m_codeConstructor +=codeconnect.arg("this->ui->" + wname).arg("editTextChanged("+tip+")").arg(d->m_central_atribute).arg("set"+title +"("+tip+")");
				d->m_codeConstructor +=codeconnect.arg(d->m_central_atribute).arg(normal+"Changed("+tip+")").arg("this->ui->" + wname).arg("setText("+tip+")");
			}
		}
		else{
			d->m_codeConstructor +=codeconnect.arg("this->ui->" + wname).arg(wsignal).arg(d->m_central_atribute).arg(col->isPrimaryKey()? "loadFromPrimaryKey(QVariant)" : "set"+title+"("+m_dataTipe+")");
			d->m_codeConstructor +=codeconnect.arg(d->m_central_atribute).arg(normal+"Changed("+m_dataTipe+")").arg("this->ui->" + wname).arg(wslot);
		}
	}
	else{
		QString m_normal=Utils::getUniformName(col->name());
		QString m_title= Utils::getTitleCase(m_normal);
		QString m_titleWithSpaces =Utils::getTitleUniformName(col->name());
		addQLabel(d->m_i,m_title,m_titleWithSpaces);
		addWidgetForData(d->m_i,"Model",m_title,QString(),0);
		d->m_codeConstructor +="\n\tthis->ui->kcm"+m_title+"->setDataClass("+d->m_central_atribute+"->"+Utils::getUniformName(col->foreignTable()->nombre())+"());";
		d->m_codeConstructor +="\n\tthis->ui->kcm"+m_title+"->setEditableText(false);";
	}
	d->m_i++;
}
void UiFile::save(QString directory){
	QDomDocument doc;
	QDomProcessingInstruction xml = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(xml);
	QDomElement ui = doc.createElement("ui");
	ui.setAttribute("version","4.0");
	QDomElement cl = doc.createElement("class");
	cl.appendChild(doc.createTextNode(d->m_className));
	ui.appendChild(cl);
	QDomElement widget =doc.createElement("widget");
	widget.setAttribute("class",d->m_father);
	widget.setAttribute("name",d->m_className);

	QDomElement property = doc.createElement("property");
	property.setAttribute("name","geometry");

	QDomElement rect = doc.createElement("rect");

	QDomElement prop;
	prop= doc.createElement("x");
	prop.appendChild(doc.createTextNode("0"));
	rect.appendChild(prop);

	prop = doc.createElement("y");
	prop.appendChild(doc.createTextNode("0"));
	rect.appendChild(prop);

	prop = doc.createElement("width");
	prop.appendChild(doc.createTextNode("400"));
	rect.appendChild(prop);

	prop = doc.createElement("height");
	prop.appendChild(doc.createTextNode("300"));
	rect.appendChild(prop);

	property.appendChild(rect);
	widget.appendChild(property);

	property = doc.createElement("property");
	property.setAttribute("name","windowTitle");
	QDomElement el =doc.createElement("string");
	el.appendChild(doc.createTextNode(d->m_window));
	property.appendChild(el);
	widget.appendChild(property);
	QMapIterator<QString, LayoutOrg> i(d->layoutElements);

	if(i.hasNext()){
		QDomElement lay = doc.createElement("layout");
		lay.setAttribute("class","QFormLayout");
		lay.setAttribute("name","formLayout");
		while (i.hasNext()) {
			i.next();
			Q_ASSERT(d->elements.contains(i.key()));
			QDomElement item = doc.createElement("item");
			item.setAttribute("row",i.value().row);
			item.setAttribute("column",i.value().col);
			item.appendChild(d->elements.value(i.key()));
			lay.appendChild(item);
		}
		widget.appendChild(lay);
	}

	for(int i =0;i<d->actionElement.count();i++)
		widget.appendChild(d->actionElement.value(i));
	ui.appendChild(widget);
	ui.appendChild(d->customWidgets);
	for(int i =0;i< d->uiElements.count();i++)
		ui.appendChild(d->uiElements.value(i));

	doc.appendChild(ui);
	QFile file(directory + "/"+d->m_className.toLower()+".ui");
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream stream(&file);
		doc.save(stream, 2);
		file.close();
	}
#ifdef QT_DEBUG
	else{
		qDebug()<<"No se puede escribir el fichero, compruebe que tiene permisos de escritura.";
	}
#endif
	d->clas.setClassName(d->m_className);
	d->clas.setFatheClassName(d->m_father);
	//SAVE H and CPP Files
	d->m_codeConstructor = "\n\tui=new Ui::"+d->m_className+"();\n\tui->setupUi(this);\n\t"+d->m_central_atribute +"= new "+d->m_central_class +"(this);" +d->m_codeConstructor;
	d->clas.addConstructor("QWidget *parent",d->m_codeConstructor,"0");
	d->clas.addHInclude("model/"+d->m_central_class.toLower() +".h");

	d->clas.addAttribute(Variables::Protected,Variables::Normal,d->m_central_class+ " *",d->m_central_atribute);
	d->clas.addFunction(Variables::Public,Variables::Virtual,"void","limpiar","","\t" + d->m_central_atribute + "->clear();\n\tKDataPanel::limpiar();");
	d->clas.addFunction(Variables::Public,Variables::Virtual,"bool","guardar","","\treturn " + d->m_central_atribute + "->save();");//\n\treturn KDataPanel::guardar();");
	d->clas.addFunction(Variables::Public,Variables::Virtual,"bool","eliminar","","\treturn " + d->m_central_atribute + "->erase();");//\n\treturn KDataPanel::eliminar();");
	d->clas.addFunction(Variables::Public,Variables::Virtual,"void","cancelar","","\tKDataPanel::cancelar();");
	d->clas.addFunction(Variables::Public,Variables::Virtual,"void","cerrar","","\tKDataPanel::cerrar();");
	d->clas.addFunction(Variables::Public,Variables::Virtual,"bool","buscar","",d->m_buscar+"\n\treturn KDataPanel::buscar();");
	if(!d->m_new.isEmpty()){
		d->clas.addFunction(Variables::Public,Variables::Virtual,"void","nuevo","", d->m_new+ "\n\tKDataPanel::nuevo();");
	}
	if(!d->m_modify.isEmpty()){
		d->clas.addFunction(Variables::Public,Variables::Virtual,"void","modificar","",d->m_modify+ "\n\tKDataPanel::modificar();");
	}
	d->clas.addDestructor("","\tdelete ui;");
	d->clas.addNameSPace("Ui","\tclass " + d->m_className +";");
	d->clas.addCppInclude("ui_"+d->m_className.toLower()+".h");
	d->clas.addAttribute(Variables::Private,Variables::Normal,"Ui::"+d->m_className+" *","ui");
	d->clas.save(directory);
}

void
UiFile::addCppInclude(QString include){
	d->clas.addCppInclude(include);
}

void UiFile::loadFile(QString fileName){
	QDomDocument m_doc;
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly)) {
		if (!m_doc.setContent(&file)) {
			qDebug()<<"El fichero no es un documento xml válido.";
			return;
		}
		file.close();
	}
	else {
		qDebug()<<"No se puede abrir el documento "<<fileName<<"para leerlo.Compruebe que el fichero exista y que tenga permisos.";
		//return NULL;
		return;
	}
	d->actionElement.clear();
	d->layoutElements.clear();
	d->uiElements.clear();
	d->elements.clear();
	QDomElement root = m_doc.firstChildElement("ui");
	QDomElement resor=root.firstChildElement("resources");
	if(!resor.isNull())
		d->uiElements.append(resor);
	QDomElement conec=root.firstChildElement("connections");
	if(!conec.isNull())
		d->uiElements.append(conec);
	QDomElement widget = root.firstChildElement("widget");
	QDomElement conection= widget.firstChildElement("action");
	while (!conection.isNull()) {
		d->actionElement.append(conection);
		conection = conection.nextSiblingElement("action");
	}
}
