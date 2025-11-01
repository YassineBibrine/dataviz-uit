#include "diagnostic_output.h"
#include <QDateTime>

DiagnosticOutput::DiagnosticOutput(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setReadOnly(true);
    setupFormatting();
}

void DiagnosticOutput::appendError(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    appendPlainText(QString("[%1] ERROR: %2").arg(timestamp, message));
}

void DiagnosticOutput::appendWarning(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    appendPlainText(QString("[%1] WARNING: %2").arg(timestamp, message));
}

void DiagnosticOutput::appendInfo(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
 appendPlainText(QString("[%1] INFO: %2").arg(timestamp, message));
}

void DiagnosticOutput::appendMetric(const QString &metricName, const QString &value)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    appendPlainText(QString("[%1] METRIC: %2 = %3").arg(timestamp, metricName, value));
}

void DiagnosticOutput::clear()
{
    QPlainTextEdit::clear();
}

void DiagnosticOutput::setupFormatting()
{
    setFont(QFont("Consolas", 9));
}
