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

#include "formabout.h"
#include "ui_formabout.h"
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>

FormAbout::FormAbout(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FormAbout)
{
	ui->setupUi(this);
	ui->label->setText(ui->label->text().arg(KK_VERSION).arg(KK_DATE).arg(KK_PAGE).arg(AUTOR_PAGE));
	QSettings ss;
	language = ss.value("language").toString();
}

FormAbout::~FormAbout()
{
	delete ui;
}

void FormAbout::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void FormAbout::on_pb_donar_clicked()
{
	QDesktopServices::openUrl(QUrl(QString("http://katakuntur.jeanmazuelos.com/%1/donate").arg(language)));
}

void FormAbout::on_pb_megusta_clicked()
{
	QDesktopServices::openUrl(QUrl("https://www.facebook.com/KataKuntur"));
}

void FormAbout::on_pb_web_clicked()
{
	QDesktopServices::openUrl(QUrl(QString("http://katakuntur.jeanmazuelos.com/%1").arg(language)));
}
