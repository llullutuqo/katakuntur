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

#ifndef PAGINAS_H
#define PAGINAS_H
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QColorDialog>
#include <QToolButton>
class PaginaGenerales : public QWidget{
	Q_OBJECT
public:
	PaginaGenerales(){
		QGroupBox *configGroup = new QGroupBox(tr("Diagramas"));
		leAutor = new QLineEdit;
		leAutor->setToolTip(trUtf8("Escriba el autor predeterminado para los proyectos que se creen."));
		chCargarUltimo = new QCheckBox(trUtf8("Cargar último diagrama editado al iniciar."));
		chCargarUltimo->setToolTip(trUtf8("Carga el último diagrama editado cuando el modelador inicia."));

		QHBoxLayout *cd = new QHBoxLayout;
		cd->addWidget(new QLabel(trUtf8("Autor predeterminado:")));
		cd->addWidget(leAutor);

		QVBoxLayout *configLayout = new QVBoxLayout;
		configLayout->addLayout(cd);
		configLayout->addWidget(chCargarUltimo);
		configGroup->setLayout(configLayout);

		QVBoxLayout *mainLayout = new QVBoxLayout;
		mainLayout->addWidget(configGroup);
		mainLayout->addStretch(1);
		setLayout(mainLayout);
	}
	QLineEdit *leAutor;
	QCheckBox *chCargarUltimo;
};

class PaginaDiagramado : public QWidget{
	Q_OBJECT
public:
	PaginaDiagramado(){
		QGroupBox *configGroup = new QGroupBox(tr("Entidad"));
		show1 = new QWidget(this);
		show2 = new QWidget(this);
		p1= new QToolButton(this);
		p1->setToolTip(trUtf8("Seleccione el color con el que se visualizarán los atributos propios de una entidad."));
		p2= new QToolButton(this);
		p2->setToolTip(trUtf8("Seleccione el color con el que se visualizarán los atributos foráneos a una entidad,\nesto se hará visible cuando se cambie a la vista física."));
		dialog = new QColorDialog(this);
		leFormula = new QLineEdit(this);
		QGridLayout*base = new QGridLayout;
		base->addWidget(new QLabel(trUtf8("Color de atributos normales:"),this),0,0);
		base->addWidget(show1,0,1);
		base->addWidget(p1,0,2);
		base->addWidget(new QLabel(trUtf8("Color de atributos foráneos:"),this),1,0);
		base->addWidget(show2,1,1);
		base->addWidget(p2,1,2);
		connect(p1,SIGNAL(clicked()),this,SLOT(button1()));
		connect(p2,SIGNAL(clicked()),this,SLOT(button2()));

		QVBoxLayout *configLayout = new QVBoxLayout;
		configLayout->addLayout(base);
		configGroup->setLayout(configLayout);

		QHBoxLayout *lay = new QHBoxLayout;
		lay->addWidget(new QLabel(trUtf8("Fórmula para atributos foráneos"),this));
		leFormula->setToolTip(trUtf8("Fórmula para generar los nombres de los atributos foráneos. %t-> nombre de la entidad, %c nombre del atributo."));
		lay->addWidget(leFormula);

		configLayout->addLayout(lay);

		QVBoxLayout *mainLayout = new QVBoxLayout;
		mainLayout->addWidget(configGroup);
		mainLayout->addStretch(1);
		setLayout(mainLayout);
	}
	QColor normalColor(){
		return _1;
	}
	void setNormalColor(QColor color){
		if(_1!=color){
			_1=color;
			show1->setStyleSheet("background-color:" + _1.name());
		}
	}

	QColor foreignColor(){
		return _2;
	}
	void setForeignColor(QColor color){
		if(_2!=color){
			_2=color;
			show2->setStyleSheet("background-color:" + _2.name());
		}
	}

	QToolButton *p1;
	QToolButton *p2;
	QColorDialog *dialog;
	QColor _1;
	QColor _2;
	QWidget *show1;
	QWidget *show2;
	QLineEdit *leFormula;

private slots:
	void button1(){
		if(dialog->exec()){
			setNormalColor(dialog->selectedColor());
		}
	}
	void button2(){
		if(dialog->exec()){
			setForeignColor(dialog->selectedColor());
		}
	}
};

#endif // PAGINAS_H
