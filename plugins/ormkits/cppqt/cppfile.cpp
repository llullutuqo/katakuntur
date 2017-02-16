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

#include "cppfile.h"
#include <QDebug>
class CppFile::CppFilePrivate{
public:
	CppFilePrivate(){

	}
	QString createStandarFunction(QString dataType,QString Name,QString arguments,QString code){
		QString codes;
		codes +="\n" + dataType +" "+m_className+"::" +Name +"(" + arguments+"){";
		codes += "\n" + code;
		codes +="\n}";
		return codes;
	}
	QString m_include;
	QString m_functions;
	QString m_properties;
	QString m_consttructor;
	QString m_className;
	QString m_fatherClass;
};

CppFile::CppFile(QObject *parent):Ficheros(parent),d(new CppFilePrivate)
{
}
void CppFile::addFunction(QString dataType,QString Name,QString arguments,QString code){
	d->m_functions +=d->createStandarFunction(dataType,Name,arguments,code);
}

void CppFile::addConstructor(QString arguments,QString code){
	d->m_consttructor +="\n" +d->m_className+"::" + d->m_className +"("+arguments+ ")";
	if(!arguments.isEmpty())
		d->m_consttructor +=":\n\t"+d->m_fatherClass+"("+getArgumentName(arguments)+")";
	d->m_consttructor +="{";
	d->m_consttructor += "\n " + code;
	d->m_consttructor +="\n}";
}
void CppFile::addDestructor(QString arguments,QString code){
	d->m_consttructor +="\n" +d->m_className+"::~" + d->m_className +"("+arguments+ ")";
	if(!arguments.isEmpty())
		d->m_consttructor +=":\n\t"+d->m_fatherClass+"("+getArgumentName(arguments)+")";
	d->m_consttructor +="\n{";
	d->m_consttructor += "\n " + code;
	d->m_consttructor +="\n}";
}

void CppFile::addIncludeFile(QString includeFile){
	d->m_include +="\n#include \"" +  includeFile +"\"";

}

void CppFile::addStandarProperty(QString dataType,QString titlePropertyName){
	QString uniformName = QString(titlePropertyName.at(0).toUpper()) + titlePropertyName.right(titlePropertyName.length() -1);
	QString setCode,getCode;
	setCode +="\nif(m_"+titlePropertyName+"!=value)";
	setCode +="\n\t m_"+titlePropertyName+"=value;";
	d->m_properties += d->createStandarFunction(dataType, uniformName,"","return m_" +titlePropertyName+";");
	d->m_properties += d->createStandarFunction(dataType, "set"+titlePropertyName,"",setCode);
}

void CppFile::addApuProperty(QString dataType,QString tittleCasePropertyName){
	Q_UNUSED(dataType);
	Q_UNUSED(tittleCasePropertyName);
}
QString CppFile::getArgumentName(QString name){
	int n=name.indexOf(" ");
	name =name.mid(n+1,name.length()-2);
	if(name.startsWith("*"))
		name = name.right(name.length()-1);
	return name;
}

bool CppFile::save(QString dir){
	QString cfile;
	cfile +=d->m_include;
	cfile += d->m_consttructor;
	cfile += d->m_functions;
	cfile +="//INIT PROPERTIES";
	cfile += d->m_properties;
	cfile +="//END PROPERTIES";
	cfile +="\n";
	return Ficheros::exportar(cfile,dir +"/"+m_filename);
}

QString CppFile::className(){
	return d->m_className;
}

void CppFile::setClassName(QString value){
	if(d->m_className!=value){
		d->m_className = value;
		m_filename = value.toLower()+".cpp";
	}
}
QString CppFile::fatherClassName(){
	return d->m_fatherClass;
}

void CppFile::setFatherClassName(QString value){
	if(d->m_fatherClass!=value)
		d->m_fatherClass = value;
}

