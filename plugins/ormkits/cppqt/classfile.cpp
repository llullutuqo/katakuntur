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

#include "classfile.h"
#include "hfile.h"
#include "cppfile.h"
class ClassFile::ClassFilePrivate{
public:
	ClassFilePrivate(QObject *obj){
		m_h = new HFile(obj);
		m_cpp = new CppFile(obj);
	}
	HFile *m_h;
	CppFile *m_cpp;
	QString m_father;
};

ClassFile::ClassFile(QObject * parent):d(new ClassFilePrivate(parent))
{
}
void ClassFile::save(QString directory){
	d->m_cpp->addIncludeFile(d->m_h->fileName());
	if(!d->m_father.isEmpty()){
		d->m_h->addIncludeFile(d->m_father.toLower()+".h");
	}
	d->m_h->save(directory);
	d->m_cpp->save(directory);
}
QString ClassFile::className(){
	return d->m_h->className();
}
void ClassFile::setClassName(QString value){
	d->m_h->setClassName(value);
	d->m_cpp->setClassName(value);
}
void ClassFile::setFatheClassName(QString value){
	if(d->m_father!=value){
		d->m_father=value;
		d->m_h->setFatherClassName(d->m_father);
		d->m_cpp->setFatherClassName(d->m_father);
	}
}

void ClassFile::addFunction(Variables::Visibility visibility,Variables::Type type,QString dataType,QString name,QString arguments,QString code){
	d->m_h->addFunction(visibility,type,dataType,name,arguments);
	d->m_cpp->addFunction(dataType,name,arguments,code);
}
void ClassFile::addConstructor(QString arguments,QString code,QString argumentInit){
	QString harg=arguments;
	if(!argumentInit.isEmpty())
		harg = harg+"="+argumentInit;
	d->m_h->addConstructor(harg);
	d->m_cpp->addConstructor(arguments,code);
}
void ClassFile::addDestructor(QString arguments,QString code){
	d->m_h->addDestructor(arguments);
	d->m_cpp->addDestructor(arguments,code);
}
void ClassFile::addAttribute(Variables::Visibility visibility, Variables::Type type,QString dataType,QString name){
	d->m_h->addAttribute(visibility,type,dataType,name);
}
void ClassFile::addCppInclude(QString includeFile){
	d->m_cpp->addIncludeFile(includeFile);
}
void ClassFile::addHInclude(QString includeFile){
	d->m_h->addIncludeFile(includeFile);
}
void ClassFile::addStandarProperty(QString dataType,QString tittleCasePropertyName){
	d->m_cpp->addStandarProperty(dataType,tittleCasePropertyName);
	d->m_h->addStandarProperty(dataType,tittleCasePropertyName);
}
void ClassFile::addApuProperty(QString dataType,QString tittleCasePropertyName){
	d->m_cpp->addApuProperty(dataType,tittleCasePropertyName);
	d->m_h->addApuProperty(dataType,tittleCasePropertyName);
}

void ClassFile::addNameSPace(QString name,QString members){
	d->m_h->addNameSPace(name,members);
}
void ClassFile::addEnum(QString enumName,int index){
	d->m_h->addEnum(enumName,index);
}
