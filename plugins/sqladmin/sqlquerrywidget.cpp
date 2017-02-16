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

#include "sqlquerrywidget.h"
#include <QPlainTextEdit>
#include "sqlsintax.h"
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlError>
#include <QSplitter>
#include <QHBoxLayout>

class SQLQuerryWidget::SQLQuerryWidgetPrivate{
public:
	SQLQuerryWidgetPrivate(SQLQuerryWidget *base){
		layout = new QHBoxLayout(base);
		splitter = new QSplitter(base);
		tableView = new QTableView(splitter);
		textEdit = new QPlainTextEdit(splitter);
		sintax = new SQLSintax(textEdit->document());
		sintax->setDocument(textEdit->document());
		model = new QSqlQueryModel(base);
		base->setLayout(layout);
		layout->addWidget(splitter);
		layout->setMargin(0);
		splitter->addWidget(textEdit);
		splitter->addWidget(tableView);
		splitter->setOrientation(Qt::Vertical);
	}

	QPlainTextEdit *textEdit;
	SQLSintax * sintax;
	QSqlQueryModel * model;
	QTableView * tableView;
	QHBoxLayout * layout;
	QSplitter *splitter;
};

SQLQuerryWidget::SQLQuerryWidget(QWidget *parent) :
	QWidget(parent), d(new SQLQuerryWidgetPrivate(this))
{
}

void
SQLQuerryWidget::ejecutarSQL(QSqlDatabase dataBase){
	if(d->textEdit){
		QString que=d->textEdit->toPlainText();
		if(que.isEmpty()){
			emit messageEmited(trUtf8("No hay nada para ejecutar."),Warning);
			return;
		};
		d->model->setQuery(que,dataBase);
		if(d->model->query().lastError().isValid()){
			emit messageEmited(trUtf8("Error al ejecutar SQL: ") + d->model->query().lastError().text(),Fatal);
		}
		else{
			emit messageEmited(trUtf8("Ejecutado correctamente."));
		}
		d->tableView->setModel(d->model);
	}
}
