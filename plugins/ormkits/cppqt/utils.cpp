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

#include "utils.h"

Utils::Utils()
{
}

//TODO: OBSOLETO, CAMBIAR LO ANTES POSIBLE
QString Utils::getTitleUniformName(QString string){
	QString sald = string.toLower();
	sald =QString(sald.at(0).toUpper()) + sald.right(sald.length() -1);
	int index = sald.indexOf('_');
	while(index>-1){
		QChar ch = sald.at(index+1);
		sald.replace("_" + QString(ch) ," "+QString(ch.toUpper()));
		index = sald.indexOf('_',index);
	}
	return sald;
}
QString Utils::getUniformName(QString string){
	QString sald = string.toLower();
	int index = sald.indexOf('_');
	while(index>-1){
		QChar ch = sald.at(index+1);
		sald.replace("_" + QString(ch) ,ch.toUpper());
		index = sald.indexOf('_',index);
	}
	return sald;
}
QString Utils::getTitleCase(QString string){
	//string = getUniformName(string);
	return QString(string.at(0).toUpper()) + string.right(string.length() -1);
}

QString Utils::getClassFromData(QString data){
	if(data=="QString")
		return "QLineEdit";
	if(data=="int")
		return "QSpinBox";
	if(data=="bool")
		return "QCheckBox";
	if(data=="float"|| data =="double")
		return "QDoubleSpinBox";
	if(data=="Select")
		return "QComboBox";
	if(data=="QDate")
		return "QDateEdit";
	if(data=="QDateTime")
		return "QDateTimeEdit";
	if(data=="QTime")
		return "QTimeEdit";
	if(data=="QByteArray")
		return "KSelectFile";
	return "KComboModel";
}

QString Utils::getControlAbbreviation(QString value){
	if(value=="QLabel")
		return "lb";
	else if(value=="QCheckBox")
		return "ck";
	else if(value=="QLineEdit")
		return "le";
	else if(value=="QDateTimeEdit")
		return "dte";
	else if(value=="QSpinBox")
		return "sb";
	else if(value=="QComboBox")
		return "cb";
	else if(value=="QDateEdit")
		return "de";
	else if(value=="QDoubleSpinBox")
		return "dsb";
	else if(value=="QTimeEdit")
		return "te";
	else if(value=="KComboModel")
		return "kcm";
	else if(value=="KSelectFile")
		return "ksf";
	return "dsa";
}
