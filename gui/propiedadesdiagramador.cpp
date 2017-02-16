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

#include "propiedadesdiagramador.h"
#include "model/entity.h"
#include <QSettings>
#include "mainwindow.h"
#include "pantalladiagrama.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
bool PropiedadesDiagramador::requiereReiniciar = false;

PropiedadesDiagramador::PropiedadesDiagramador(){
}
QColor PropiedadesDiagramador::colorColumnasNormales(){
	return Entity::normalCols.color();
}

void PropiedadesDiagramador::setColorColumnasNormales(QColor color){
	Entity::normalCols.setColor(color);
}

QColor PropiedadesDiagramador::colorColumnasForaneas(){
	return Entity::foreignCols.color();
}
void PropiedadesDiagramador::setColorColumnasForaneas(QColor color){
	Entity::foreignCols.setColor(color);
}

QString PropiedadesDiagramador::autorPredeterminado(){
	return PantallaDiagrama::nombreAutorPredeterminado;
}
void PropiedadesDiagramador::setAutorPredeterminado(QString nombre){
	PantallaDiagrama::nombreAutorPredeterminado=nombre;
}

QString PropiedadesDiagramador::formulaNombreColumnasForaneas(){
	return PantallaDiagrama::foreignKeysName;
}
void PropiedadesDiagramador::setFormulaNombreColumnasForaneas(QString nombre){
	PantallaDiagrama::foreignKeysName=nombre;
}

bool PropiedadesDiagramador::autoCargarUltimoDiagrama(){
	return MainWindow::autoCargarUltimoDiagrama;
}

void PropiedadesDiagramador::setAutoCargarUltimoDiagrama(bool value){
	MainWindow::autoCargarUltimoDiagrama = value;
}

void PropiedadesDiagramador::guardarCambios(){
	QSettings settings;
	settings.setValue("cargarAlIniciar", MainWindow::autoCargarUltimoDiagrama);
	settings.setValue("colorColumnaNormal", Entity::normalCols.color());
	settings.setValue("colorColumnaForanea", Entity::foreignCols.color());
	settings.setValue("nombreAutorPredeterminado", PantallaDiagrama::nombreAutorPredeterminado);
	settings.setValue("formulaNombreColumna", PantallaDiagrama::foreignKeysName);
}
void PropiedadesDiagramador::cargarConfiguracion(){
#ifdef WIN32
	//LPWSTR unm;
	TCHAR unm[255];
	DWORD nm=255;
	GetUserNameW((TCHAR *)unm,&nm);
	const QString uname = QString::fromWCharArray(unm);
#else
	const QString uname = getlogin();
#endif
	QSettings settings;
	MainWindow::autoCargarUltimoDiagrama=settings.value("cargarAlIniciar",false).toBool();
	setColorColumnasNormales(settings.value("colorColumnaNormal",QColor(0,0,0)).value<QColor>());
	setColorColumnasForaneas(settings.value("colorColumnaForanea",QColor(149,0,0)).value<QColor>());
	PantallaDiagrama::nombreAutorPredeterminado = settings.value("nombreAutorPredeterminado",uname).toString();
	PantallaDiagrama::foreignKeysName = settings.value("formulaNombreColumna","%t_%c").toString();
	const QString defNAme="<Desconocido>";
	if(PantallaDiagrama::nombreAutorPredeterminado==defNAme){
		PantallaDiagrama::nombreAutorPredeterminado = uname;
		guardarCambios();
	}
}
