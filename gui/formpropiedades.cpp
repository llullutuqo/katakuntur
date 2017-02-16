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

#include "formpropiedades.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include "paginas.h"
#include <QSettings>
#include <QDialogButtonBox>
#include "propiedadesdiagramador.h"

class FormPropiedades::PrivateFormPropiedades{
public:
	PrivateFormPropiedades(FormPropiedades * parent){
		m_inter = new QStackedWidget(parent);
		button = new QDialogButtonBox(parent);
		button->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);
		verticalLayout = new QVBoxLayout(parent);
		verticalLayout->setContentsMargins(3, 3, 3, 3);
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		QHBoxLayout *lay = new QHBoxLayout;
		verticalLayout->addLayout(lay);
		verticalLayout->addWidget(button);
		m_lista = new QListWidget(parent);
		m_lista->setMaximumWidth(92);
		m_lista->setViewMode(QListView::IconMode);
		m_lista->setIconSize(QSize(70, 70));
		m_lista->setMovement(QListView::Static);
		m_lista->setSpacing(5);
		m_lista->addItem(new QListWidgetItem(QIcon(":/images/general.png"),trUtf8("General"),m_lista));
		m_lista->addItem(new QListWidgetItem(QIcon(":/images/diagramado.png"),trUtf8("Diagrama"),m_lista));

		lay->addWidget(m_lista);
		lay->addWidget(m_inter);

		m_pg = new PaginaGenerales;
		m_pd = new PaginaDiagramado;

		m_inter->addWidget(m_pg);
		m_inter->addWidget(m_pd);



		cargarGUI();
		connect(m_lista,
				SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
				parent, SLOT(cambiarPagina(QListWidgetItem*,QListWidgetItem*)));
	}
	~PrivateFormPropiedades(){
		delete m_pd;
		delete m_pg;
		delete m_lista;
		delete m_inter;
	}
	void cargarGUI(){
		m_pg->leAutor->setText(PropiedadesDiagramador::autorPredeterminado());
		m_pg->chCargarUltimo->setChecked(PropiedadesDiagramador::autoCargarUltimoDiagrama());
		m_pd->setNormalColor(PropiedadesDiagramador::colorColumnasNormales());
		m_pd->setForeignColor(PropiedadesDiagramador::colorColumnasForaneas());
		m_pd->leFormula->setText(PropiedadesDiagramador::formulaNombreColumnasForaneas());
	}
	void guardarPreferenacias(){
		PropiedadesDiagramador::setAutorPredeterminado(m_pg->leAutor->text());
		PropiedadesDiagramador::setAutoCargarUltimoDiagrama(m_pg->chCargarUltimo->isChecked());
		PropiedadesDiagramador::setColorColumnasNormales(m_pd->normalColor());
		PropiedadesDiagramador::setColorColumnasForaneas(m_pd->foreignColor());
		PropiedadesDiagramador::setFormulaNombreColumnasForaneas(m_pd->leFormula->text());
		PropiedadesDiagramador::guardarCambios();
	}
	QStackedWidget *m_inter;
	QListWidget *m_lista;
	PaginaGenerales *m_pg;
	PaginaDiagramado *m_pd;
	QDialogButtonBox *button;
	QVBoxLayout *verticalLayout;
};


FormPropiedades::FormPropiedades(QWidget *parent) :
	QDialog(parent),
	d(new PrivateFormPropiedades(this))
{
	connect(d->button,SIGNAL(rejected()),this,SLOT(reject()));
	connect(d->button,SIGNAL(accepted()),this,SLOT(guardar()));
	setWindowTitle(trUtf8("Propiedades"));
	setMinimumHeight(250);
}

FormPropiedades::~FormPropiedades()
{
	delete d;
}

void FormPropiedades::cambiarPagina(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (!current)
		current = previous;
	d->m_inter->setCurrentIndex(d->m_lista->row(current));
}
void FormPropiedades::guardar(){
	d->guardarPreferenacias();
	accept();
}
