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

#include "columneditor.h"
#include "models/columnlistmodel.h"
#include "model/attributes.h"
#include <QKeyEvent>

class ColumnEditor::ColumnEditorPrivate{
public:
	ColumnEditorPrivate(){
		m_model = NULL;
	}
	ColumnListModel *m_model;
};

ColumnEditor::ColumnEditor(QWidget *parent) :
	QTableView(parent),d(new ColumnEditorPrivate())
{
}

ColumnEditor::~ColumnEditor()
{
	delete d;
}

void
ColumnEditor::setModel(ColumnListModel *model){
	d->m_model = model;
	QTableView::setModel(model);
}

void
ColumnEditor::keyPressEvent ( QKeyEvent * event ){
	if(event->matches(QKeySequence::New)){
		addRow();
		return;
	}
	QTableView::keyPressEvent(event);
}

void
ColumnEditor::deleteCurrent(){
	/*Se evita crash al momento de eliminar una columna que no existe*/
	QModelIndex index =currentIndex();
	if(!index.isValid())
		return;
	d->m_model->removeRow(index.row());
}

void
ColumnEditor::addRow(){
	int count = d->m_model->columnList()->columnCount() -d->m_model->columnList()->countIneditableColumns(Attributes::Lasts);
	if(d->m_model->insertRow(count))
		setCurrentIndex(d->m_model->indexFromRow(count));
}


void
ColumnEditor::moverArriba(){
	QModelIndex index =currentIndex();
	if(!index.isValid())
		return;
	if(index.row() > d->m_model->columnList()->countIneditableColumns(Attributes::Firsts))
		setCurrentIndex(d->m_model->moverArriba(index.row()));
}

void
ColumnEditor::moverAbajo(){
	QModelIndex index =currentIndex();
	if(!index.isValid())
		return;
	if(index.row()+1<d->m_model->columnList()->columnCount() - d->m_model->columnList()->countIneditableColumns(Attributes::Lasts))
		setCurrentIndex(d->m_model->moverAbajo(index.row()));
}

void
ColumnEditor:: setColumnList(Attributes *cols){
	d->m_model->setColumnList(cols);
}
