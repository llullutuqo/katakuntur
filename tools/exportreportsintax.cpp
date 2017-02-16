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

#include "exportreportsintax.h"
#include <QHash>
#include <QTextCharFormat>

class ExportReportSintax::ExportReportSintaxPrivate{
public:
	ExportReportSintaxPrivate(){
		correcto.setForeground(Qt::darkGreen);
		engineError.setForeground(Qt::darkYellow);
		error.setForeground(Qt::red);
		error.setFontWeight(QFont::Bold);
		normal.setForeground(Qt::darkGray);
		informacion.setForeground(Qt::blue);
		inicializarReglas();
	}

	void inicializarReglas(){
		ExportReportSintaxPrivate::Regla rule;
		rule.format = correcto;
		rule.pattern = QRegExp("^S: .*");
		reglas.append(rule);
		rule.format = error;
		rule.pattern= QRegExp("^E: .*");
		reglas.append(rule);
		rule.format = informacion;
		rule.pattern = QRegExp("^I: .*");
		reglas.append(rule);
		rule.format = engineError;
		rule.pattern = QRegExp("^B: .*");
		reglas.append(rule);

	}

	struct Regla
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<Regla> reglas;
	QTextCharFormat correcto;
	QTextCharFormat engineError;
	QTextCharFormat error;
	QTextCharFormat normal;
	QTextCharFormat informacion;
};

ExportReportSintax::ExportReportSintax(QTextDocument *parent) :
	QSyntaxHighlighter(parent),d(new ExportReportSintaxPrivate)
{
}

ExportReportSintax::~ExportReportSintax(){
	delete d;
}

void
ExportReportSintax::highlightBlock(const QString &text){
	bool nomatch =true;
	foreach (const ExportReportSintaxPrivate::Regla &rule, d->reglas) {
		QRegExp expression(rule.pattern);
		if(expression.exactMatch(text)){
			int length = expression.matchedLength();
			setFormat(0, length, rule.format);
			nomatch= false;
			break;
		}
	}
	if(nomatch){
		setFormat(0, text.length(), d->normal);
	}
}
