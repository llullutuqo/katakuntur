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

#include "ormkatakowi.h"
#include "uifile.h"
#include "classfile.h"
#include "utils.h"
#include "classfile.h"
#include "htmlfile.h"
#include "qhpfile.h"

#include <QDebug>
#include "model/entity.h"
#include "model/attribute.h"
#include "ficheros/ficheros.h"
#include <QDir>
#include "propertiesdialog.h"
#include "ficheros/SQL/sqlcore.h"
#include "pantalladiagrama.h"
/*
  Los archivos generados deben ser:
  Core:
  <tabla>.cpp
  <tabla>.h
  wg<tabla>.cpp
  wg<tabla>.h
  wg<tabla>.ui
  model.pri
  Ayuda:
  doc.???
  <tabla>.html
*/
class ORMKataKowi::ORMKataKowiPrivate{
public:
	ORMKataKowiPrivate(){
		docDirectoryName="doc";
		modelDirectoryName="model";
		widgetsDirectoryName ="widgets";
		widgetsPrefix="Wg";
		sep ="/";
		target="template";
		genDoc= false;
		genORM= true;
		genWid= false;
		genFull = false;
		genQtHelpFile=true;
	}

	QString directoryBase;
	QString docDirectoryName;
	QString modelDirectoryName;
	QString widgetsDirectoryName;
	QString target;
	bool genDoc;
	bool genORM;
	bool genWid;
	bool genFull;
	bool genQtHelpFile;

	QString widgetsPrefix;
	QString sep;
};

ORMKataKowi::ORMKataKowi(QObject *parent):
	ORMKit(parent),
	d(new ORMKataKowiPrivate())
{
}

ORMKataKowi::~ORMKataKowi()
{
	delete d;
}
const char *ORMKataKowi::nombreORM(){
	return staticNombreORM();
}

const char *ORMKataKowi::languageORM(){
	return staticLanguageORM();
}

bool ORMKataKowi::exportProject(PantallaDiagrama *diagram, const QString basedir){
	PropertiesDialog pr;
	pr.setObject(this);
	if(pr.exec()!=1){
		return false;
	}
	QString subdirModel=d->modelDirectoryName + d->sep;
	QString subdirWidgets=d->widgetsDirectoryName + d->sep;
	QString subdirDoc=d->docDirectoryName + d->sep;
	QString widgetPrefix=d->widgetsPrefix;
	QString profile,lheaders,lsources,lforms,lheadersw,lheaderscw;
	QDir ds(d->directoryBase);
	ds.mkdir(subdirModel);
	if(d->genWid)
		ds.mkdir(subdirWidgets);
	if(d->genDoc)
		ds.mkdir(subdirDoc);
	Ficheros files;
	QhpFile helpFile;
	lsources="\n\nSOURCES +=";
	lheaders="\n\nHEADERS +=";
	lforms="\n\nFORMS +=";
	foreach(Entity *item, diagram->itemsByType<Entity>()){
		QString m_beforeSave;
		QString m_description;
		QString tableName = Utils::getUniformName(item->nombre());
		QString titleTableName=Utils::getTitleCase(tableName);
		QString lenum,lslots,lsignals,linclude,lget,lprivate,cinclude;
		QString lintial,lprimaryKey,lcolumns,lsenials,ldep,lset,lgetfunctions,lsetfunctions;
		QString pred_values;

		UiFile widget;
		/*Detectamos si se desea ecportar las Widgets*/
		if(d->genWid){
			widget.setWindowTitle(titleTableName);
			widget.setClassName(widgetPrefix +titleTableName);
			widget.setOwnerClassName(titleTableName);
			QString filename =widgetPrefix +titleTableName+".ui";
			filename=filename.toLower();
			widget.loadFile(d->directoryBase+d->sep+subdirWidgets+filename);
			widget.setFatherClassName("KDataPanel");
		}

		/*Detectamos si se desea generar la documentación*/
		HtmlFile html;
		if(d->genDoc){
			html.setPageTitle(QString::fromUtf8("Pestaña - ")+titleTableName);
			html.setName(widgetPrefix +titleTableName);
			if(d->genQtHelpFile)
				helpFile.addFile(titleTableName,html.fileName());
		}
		int i =0;
		lenum = "\n\tenum{";
		lsignals="\nsignals:";
		lslots="\npublic slots:";
		lprivate="\nprivate:";
		foreach(EntityAttibute *col, item->columnas()){
			QString normal;
			QString enumName;
			QString title;
			QString primitiveData;
			QString returnValue;
			QString htmDesc;
			QString dataTipe;
			if(!col->isForeignKey()){
				normal = Utils::getUniformName(col->name());
				title = Utils::getTitleCase(normal);
				primitiveData=diagram->dbms()->toStandarQtType(col->dataType());
				enumName = title;
				QString toOther=primitiveData;
				returnValue="\n\treturn getVariantValue("+title+"Key).to"+Utils::getTitleCase(toOther.replace("Q",""))+"();";
				lsetfunctions +="\nvoid "+titleTableName+"::set"+title+"("+primitiveData+" value){";
				lsetfunctions +="\n\tif("+normal+"()!=value){";
				lsetfunctions +="\n\t\tsetVariantValue("+title+"Key,value);";
				lsetfunctions +="\n\t\temit "+normal+"Changed(value);";
				lsetfunctions +="\n\t}";
				lsetfunctions +="\n}";
				htmDesc=col->notes();
				dataTipe = col->dataType();
			}
			else{
				//normalTable = normal
				normal = Utils::getUniformName(col->foreignTable()->nombre());
				title = Utils::getTitleCase(normal);
				primitiveData = title + " *";
				qDebug()<<"tipo for:"<<col->dataType();
				dataTipe= diagram->dbms()->tipoDatoLLaveFoanea(col->dataType());
				//QString normalTable=Utils::getUniformName(col->nombre());
				enumName=Utils::getTitleCase(Utils::getUniformName(col->name()));
				cinclude += "\nclass " +title + ";";
				linclude += "\n#include \"" + subdirModel +normal.toLower() + ".h\"";
				widget.addCppInclude(subdirModel +normal.toLower() + ".h");
				lprivate+="\n\t"+primitiveData + "m_" + normal +";";
				//lintial+="\n\tm_" + normalTable +"=NULL;";
				lintial +="\n\tm_"+normal+ "= new "+title+"(this);";
				lintial +="\n\tm_"+normal+ "->setLoadingDependences(false);";
				pred_values+="\n\tm_"+normal+ "->setAsNew();";

				QString toOther=diagram->dbms()->toStandarQtType(col->dataType());
				ldep +="\n\tm_" +normal + "->loadFromPrimaryKey(getVariantValue("+enumName+"Key));";
				lset +="\n\tm_"+normal+ "->setAsNew();";
				lset +="\n\tm_"+normal+ "->set"+Utils::getTitleCase(Utils::getUniformName(col->reference()))+"(getVariantValue("+enumName+"Key).to"+Utils::getTitleCase(toOther.replace("Q",""))+"());";
				QString codes = Utils::getTitleCase(Utils::getUniformName(col->foreignTable()->nombre())) +"::" + Utils::getTitleCase(Utils::getUniformName(col->reference()));
				m_beforeSave +="\n\tsetVariantValue("+enumName+"Key,m_"+normal+"->getVariantValue("+codes+"Key));";

				returnValue="\n\treturn m_"+normal+";";

				lsetfunctions +="\nvoid "+titleTableName+"::set"+title+"("+title+" *value){";
				lsetfunctions +="\n\tif(m_"+normal+"!=value){";
				lsetfunctions +="\n\t\tm_"+normal+"=value;";
				lsetfunctions +="\n\t\temit "+normal+"Changed(value);";
				lsetfunctions +="\n\t}";
				lsetfunctions +="\n}";
				htmDesc =trUtf8("Debe seleccionar un %1. ").arg(enumName)+col->notes();
				if(d->genDoc){
					QString fortitleTableName=Utils::getTitleCase(Utils::getUniformName(col->foreignTable()->nombre()));
					html.addSeeAlso(fortitleTableName,widgetPrefix + fortitleTableName+".html");
				}
			}
			if(!col->isNull()&&!diagram->dbms()->isAutoincrementDataType(dataTipe)){
				lintial +="\n\taddCantBeNullColumn("+enumName+"Key,"+trUtf8("\"El valor de %1 no puede ser nulo \"").arg(title)+");";
			}
			lenum +="\n\t\t" + enumName + "Key=" + QString::number(i) + ",";
			lsignals += "\n\tvoid " + normal +"Changed("+primitiveData+" value);";
			lslots += "\n\tvoid set" + title +"("+primitiveData+" value);";
			lget += "\n\t"+primitiveData+" " + normal +"();";
			if(col->isPrimaryKey()){
				lprimaryKey+="<<\"" + col->name().toLower()+"\"";
				//CORREGIR
				m_description +="\n\treturn getVariantValue("+enumName+"Key).toString();";
			}
			else
				lcolumns+="<<\"" + col->name().toLower()+"\"";
			lsenials+="\n\temit " + normal +"Changed("+normal+"());";
			lgetfunctions +="\n"+primitiveData+" "+titleTableName+"::"+normal+"(){";
			lgetfunctions +=returnValue;
			lgetfunctions +="\n}";
			if(d->genWid)
				widget.addColumElement(col,diagram->dbms());
			if(d->genDoc)
				html.addAtribute(title,htmDesc);
			i++;
		}
		lenum += "\n\t};";
		//GENERATING H FILE
		QString hfile;
		hfile +="#ifndef " + tableName.toUpper()+ "_H";
		hfile +="\n#define " + tableName.toUpper()+ "_H";
		hfile +="\n#include \"kdataclass.h\"";
		hfile +=cinclude;
		hfile +="\nclass "+ titleTableName +" : public KDataClass";
		hfile +="\n{";
		hfile +="\n\tQ_OBJECT";
		hfile +="\npublic:";
		hfile +="\n\t" + titleTableName +"(QObject *parent);";
		hfile +=lenum;
		hfile +=lget;
		hfile +="\n\tstatic const QHash<KDataClass::Columns,QStringList > columns();";
		hfile +="\n\tvirtual const QHash<KDataClass::Columns,QStringList > getColumns();";
		hfile +="\n\tvirtual const char * tableName();";
		hfile +="\n\tstatic const char * table();";
		hfile +="\n\tvirtual void beforeSave();";
		hfile +="\n\tvirtual void beforeErase();";
		hfile +="\n\tvirtual QString description();";
		hfile +="\nprotected:";
		hfile +="\n\tvirtual void emitSignals();";
		hfile +="\n\tvirtual void setPredValue();";
		hfile +="\n\tvirtual void loadDependences();";
		hfile +="\n\tvirtual void setPrimaryKeyToDependences();";
		hfile +=lslots;
		hfile +=lsignals;
		//hfile +=lslots;
		if(lprivate!="\nprivate:")
			hfile +=lprivate;
		hfile +="\n};";
		hfile +="\n#endif // " + tableName.toUpper()+ "_H\n";
		files.exportar(hfile,d->directoryBase+ "/" + subdirModel + tableName.toLower() + ".h" );
		//GENERATING CPP FILE
		QString cfile;
		cfile +="#include \""+tableName.toLower() + ".h\"";
		cfile +=linclude;
		cfile +="\n" + titleTableName + "::" + titleTableName + "(QObject *parent):KDataClass(parent)";
		cfile += "\n{";
		cfile += lintial;
		cfile += "\n\tm_numeroColumnas="+QString::number(i)+";";
		cfile += "\n\tloadClass();";
		cfile += "\n}";

		cfile +="\nconst QHash<KDataClass::Columns,QStringList > "+titleTableName+"::columns(){";
		cfile +="\n\tQHash<KDataClass::Columns,QStringList> sald;";
		cfile +="\n\tsald.insert(KDataClass::PrimaryKey,QStringList()"+lprimaryKey+");";
		cfile +="\n\tsald.insert(KDataClass::Normal,QStringList()"+lcolumns+");";
		cfile +="\n\treturn sald;";
		cfile +="\n}";
		cfile +="\nconst QHash<KDataClass::Columns,QStringList > "+titleTableName+"::getColumns(){";
		cfile +="\n\treturn columns();";
		cfile +="\n}";
		cfile +="\nconst char *"+titleTableName+"::tableName(){";
		cfile +="\n\treturn table();";
		cfile +="\n}";
		cfile +="\nconst char *"+titleTableName+"::table(){";
		//TODO: verificar con a exportación real de la tabla
		cfile +="\n\treturn \""+item->nombre()+"\";";
		cfile +="\n}";
		cfile +="\nvoid "+titleTableName+"::beforeSave(){";
		cfile +=m_beforeSave;
		cfile +="\n\t//TODO: Coloque aqui su codigo personalizado antes de ejecutar la sentencia save.";
		cfile +="\n}";
		cfile +="\nvoid " + titleTableName + "::beforeErase(){";
		cfile +="\n\t//TODO: Coloque aqui su codigo personalizado antes de ejecutar la sentencia erase.";
		cfile +="\n}";
		cfile +="\nvoid "+titleTableName+"::emitSignals(){";
		cfile +=lsenials;
		cfile +="\n}";
		cfile +="\nvoid "+titleTableName+"::setPredValue(){";
		cfile +=pred_values;
		cfile +="\n//TODO: Put here your predeterminates values,Example:";
		cfile +="\n//setVariantValue(Edad,14);";
		cfile +="\n}";
		cfile +="\nvoid "+titleTableName+"::loadDependences(){";
		cfile +=ldep;
		cfile +="\n}";

		cfile +="\nvoid "+titleTableName+"::setPrimaryKeyToDependences(){";
		cfile +=lset;
		cfile +="\n}";
		cfile +="\nQString "+titleTableName+"::description(){";
		cfile +=m_description;
		cfile +="\n}";
		cfile +="\n//INIT PROPERTIES";
		cfile +=lgetfunctions;
		cfile +=lsetfunctions;
		cfile +="\n//END PROPERTIES";
		cfile +="\n";
		files.exportar(cfile,d->directoryBase + "/" + subdirModel + tableName.toLower() + ".cpp" );
		lsources += "\t" + subdirModel + tableName.toLower() + ".cpp\\\n";
		lheaders += "\t" + subdirModel + tableName.toLower() + ".h\\\n";
		lheadersw +="#include \"widgets/wg" + tableName.toLower() + ".h\"\n";
		lheaderscw +="class Wg"+titleTableName+";\n";
		if(d->genWid){
			widget.save(d->directoryBase + "/" + subdirWidgets);
			lheaders += "\t"+ subdirWidgets + widget.className().toLower() +".h\\\n";
			lsources  += "\t"+ subdirWidgets + widget.className().toLower() +".cpp\\\n";
			lforms += "\t" + subdirWidgets + widget.className().toLower() +".ui\\\n";
		}
		if(d->genDoc)
			html.save(d->directoryBase + "/" +subdirDoc);

	}
	lheaders += "\twidgets.h\\\n";
	lheaders += "\tclasswidgets.h\\\n";
	lsources =lsources.left(lsources.length()-2);
	lheaders = lheaders.left(lheaders.length()-2);
	if(d->genWid)
		lforms = lforms.left(lforms.length()-2);
	profile +=lsources;
	profile +=lheaders;
	if(d->genWid)
		profile +=lforms;
	profile +="\n";
	files.exportar(lheadersw,d->directoryBase+"/widgets.h");
	files.exportar(lheaderscw,d->directoryBase+"/classwidgets.h");
	files.exportar(profile,d->directoryBase+"/model.pri");
	if(d->genQtHelpFile && d->genDoc)
		helpFile.save(d->directoryBase+"/"+subdirDoc);
	if(d->genFull){
		/*Copiamos los archivos plantilla al directorio base*/
		QString dir = d->directoryBase+ d->sep;
		QFile::copy(basedir + "/../plantillas/mainformproject/main.cpp",dir +"main.cpp");
		QFile::copy(basedir + "/../plantillas/mainformproject/mainwindow.cpp",dir +"mainwindow.cpp");
		QFile::copy(basedir + "/../plantillas/mainformproject/mainwindow.h",dir +"mainwindow.h");
		QFile::copy(basedir + "/../plantillas/mainformproject/mainwindow.ui",dir +"mainwindow.ui");

		QFile fi(basedir + "/../plantillas/mainformproject/template.pro");
		if(fi.open(QIODevice::ReadOnly | QIODevice::Text)){
			QString da=fi.readAll();
			QFile f(dir + d->target +".pro");
			f.open(QIODevice::WriteOnly | QIODevice::Text);
			QTextStream out(&f);
			out<<da.replace("[target]",d->target);
		}
		else{
			qDebug()<<"Error al abrir el archivo de plantilla, erro no:"<<fi.error();
		}
	}
	return true;
}

/*ATRIBUTOS DE CONFIGURACION*/
void ORMKataKowi::setDirectoryBase(QString directory){
	d->directoryBase = directory;
}

QString ORMKataKowi::directoryBase(){
	return d->directoryBase;
}
void ORMKataKowi::setDocDirectoryName(QString name){
	d->docDirectoryName = name;
}

QString ORMKataKowi::docDirectoryName(){
	return d->docDirectoryName;
}

void ORMKataKowi::setModelDirectoryName(QString name){
	d->modelDirectoryName = name;
}

QString ORMKataKowi::modelDirectoryName(){
	return d->modelDirectoryName;
}

QString ORMKataKowi::widgetsDirectoryName(){
	return d->widgetsDirectoryName;
}

void ORMKataKowi::setWidgetsDirectoraName(QString name){
	d->widgetsDirectoryName = name;
}

QString ORMKataKowi::widgetsPrefix(){
	return d->widgetsPrefix;
}

void ORMKataKowi::setWidgetsPrefix(QString name){
	d->widgetsPrefix = name;
}

bool ORMKataKowi::generateDocumentation(){
	return d->genDoc;
}
void ORMKataKowi::setGenerateDocumentation(bool value){
	d->genDoc=value;
}

bool ORMKataKowi::generateWidgets(){
	return d->genWid;
}
void ORMKataKowi::setGenerateWidgets(bool value){
	d->genWid= value;
}

//bool CppQt4::generateORM(){
//	return d->genORM;
//}
//void CppQt4::setGenerateORM(bool value){
//	d->genORM = value;
//}

bool ORMKataKowi::generateFullProject(){
	return d->genFull;
}
void ORMKataKowi::setGenerateFullProject(bool value){
	d->genFull = value;
}

bool ORMKataKowi::generateQhcpFile(){
	return d->genQtHelpFile;
}
void ORMKataKowi::setGenerateQhcpFile(bool value){
	d->genQtHelpFile =value;
}

QString ORMKataKowi::target(){
	return d->target;
}

void ORMKataKowi::setTarget(QString target){
	d->target = target;
}
