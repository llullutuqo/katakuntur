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

#include "hfile.h"
class HFile::HFilePrivate{
public:
	HFilePrivate(){
		m_modelF ="\n\t%1 %2 %3(%4);";
		m_modelA ="\n\t%1 %2 %3;";
		enumIndex =0;
	}
	void addNormalProperty(QString dataType,QString titlePropertyName){
		QString uniformName = QString(titlePropertyName.at(0).toUpper()) + titlePropertyName.right(titlePropertyName.length() -1);
		addFunction(Variables::Public,Variables::Normal,dataType,uniformName,"");
		addFunction(Variables::PublicSlot,Variables::Normal,"void","set"+titlePropertyName,dataType + "value");
		addFunction(Variables::Signal,Variables::Normal,"void",uniformName + "Changed",dataType + "value");
	}
	void addFunction(Variables::Visibility visibility, Variables::Type type,QString dataType,QString name,QString arguments){
		QString vis;
		switch(type){
		case Variables::Normal:
			vis="";
			break;
		case Variables::Virtual:
			vis="virtual ";
			break;
		case Variables::Static:
			vis="static ";
			break;
		}

		switch(visibility){
		case Variables::Private:
			m_Private += m_modelF.arg(vis,dataType,name,arguments);
			break;
		case Variables::Public:
			m_Public += m_modelF.arg(vis,dataType,name,arguments);
			break;
		case Variables::Protected:
			m_Protected += m_modelF.arg(vis,dataType,name,arguments);
			break;
		case Variables::Signal:
			m_Signal += m_modelF.arg(vis,dataType,name,arguments);
			break;
		case Variables::PublicSlot:
			m_PublicSlot += m_modelF.arg(vis,dataType,name,arguments);
			break;
		}
	}
	QString m_Private;
	QString m_Public;
	QString m_Protected;
	QString m_Signal;
	QString m_PublicSlot;
	QString m_Include;
	QString m_Enum;
	QString m_modelF;
	QString m_modelA;
	QString m_className;
	QString m_fatherClass;
	QString m_constructor;
	QString m_nameSpace;
	int enumIndex;
};

HFile::HFile(QObject *parent):Ficheros(parent),d(new HFilePrivate)
{

}
enum Visibility{
	Private,
	Public,
	Protected,
	Signal,
	PublicSlot,
};
void HFile::addNameSPace(QString name,QString members){
	d->m_nameSpace +="\nnamespace "+name+" {";
	d->m_nameSpace +="\n"+	members;
	d->m_nameSpace +="\n}";
}

void HFile::addFunction(Variables::Visibility visibility, Variables::Type type,QString dataType,QString name,QString arguments){
	d->addFunction(visibility,type,dataType,name,arguments);
}

void HFile::addAttribute(Variables::Visibility visibility, Variables::Type type,QString dataType,QString name){
	QString vis;
	switch(type){
	case Variables::Normal:
		vis="";
		break;
	case Variables::Virtual:
		vis="virtual ";
		break;
	case Variables::Static:
		vis="static ";
		break;
	}

	switch(visibility){
	case Variables::Private:
		d->m_Private += d->m_modelA.arg(vis,dataType,name);
		break;
	case Variables::Public:
		d->m_Public += d->m_modelA.arg(vis,dataType,name);
		break;
	case Variables::Protected:
		d->m_Protected += d->m_modelA.arg(vis,dataType,name);
		break;
	case Variables::Signal:
		d->m_Signal += d->m_modelA.arg(vis,dataType,name);
		break;
	case Variables::PublicSlot:
		d->m_PublicSlot += d->m_modelA.arg(vis,dataType,name);
		break;
	}
}

void HFile::addStandarProperty(QString dataType,QString titlePropertyName){
	d->addNormalProperty(dataType,titlePropertyName);
	addAttribute(Variables::Private,Variables::Normal,dataType,"m_"+titlePropertyName);
}
QString HFile::fatherClassName(){
	return d->m_fatherClass;
}

void HFile::setFatherClassName(QString value){
	if(d->m_fatherClass!=value)
		d->m_fatherClass = value;
}

void HFile::addApuProperty(QString dataType,QString titlePropertyName){
	d->addNormalProperty(dataType,titlePropertyName);
	addEnum(titlePropertyName,d->enumIndex);
	d->enumIndex++;
}

void HFile::addConstructor(QString arguments){
	d->m_constructor +="\n\t" + d->m_className +"("+arguments+");";
}
void  HFile::addDestructor(QString arguments){
	d->m_constructor +="\n\t~" + d->m_className +"("+arguments+");";
}

void HFile::addEnum(QString enumName,int index){
	d->m_Enum += "\n\t\t" + enumName +  "=" + QString::number(index) +",";
}
void HFile::addIncludeFile(QString includeFile){
	d->m_Include += "\n#include \"" + includeFile +"\"";
}

bool HFile::save(QString dir){
	QString m_text;
	m_text +="#ifndef " + d->m_className.toUpper()+ "_H";
	m_text +="\n#define " + d->m_className.toUpper()+ "_H";
	m_text +="\n";
	m_text +=d->m_Include;
	m_text +=d->m_nameSpace;
	m_text +="\nclass "+ d->m_className +" : public "+ d->m_fatherClass;
	m_text +="\n{";
	m_text +="\n\tQ_OBJECT";
	m_text +="\npublic:";
	m_text +=d->m_constructor;
	if(!d->m_Enum.isEmpty()){
		m_text +="\n\tenum{";
		m_text +=d->m_Enum;
		m_text +="\n\t};";
	}
	m_text +=d->m_Public;
	if(!d->m_PublicSlot.isEmpty()){
		m_text += "\npublic slots:";
		m_text +=d->m_PublicSlot;
	}
	if(!d->m_Signal.isEmpty()){
		m_text += "\nsignals:";
		m_text +=d->m_Signal;
	}
	//m_text +=lslots;
	if(!d->m_Private.isEmpty()){
		m_text +="\nprivate:";
		m_text +=d->m_Private;
	}
	if(!d->m_Protected.isEmpty()){
		m_text +="\nprotected:";
		m_text +=d->m_Protected;
	}

	m_text +="\n};";
	m_text +="\n#endif // " + d->m_className.toUpper()+ "_H\n";
	return Ficheros::exportar(m_text,dir +"/"+m_filename);

}
QString HFile::className(){
	return d->m_className;
}

void HFile::setClassName(QString value){
	if(d->m_className!=value){
		d->m_className =value;
		m_filename = value.toLower()+".h";
	}
}
