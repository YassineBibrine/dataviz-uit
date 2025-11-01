#ifndef DIAGNOSTIC_OUTPUT_H
#define DIAGNOSTIC_OUTPUT_H

#include <QPlainTextEdit>

/**
 * @class DiagnosticOutput
 * @brief Displays logs, performance metrics, and error messages
 * 
 * Shows:
 * - Compilation errors and warnings
 * - Runtime execution logs
 * - Performance metrics (comparisons, swaps, time)
 * - Memory statistics
 */
class DiagnosticOutput : public QPlainTextEdit {
    Q_OBJECT

public:
    DiagnosticOutput(QWidget *parent = nullptr);
    
    void appendError(const QString &message);
    void appendWarning(const QString &message);
    void appendInfo(const QString &message);
    void appendMetric(const QString &metricName, const QString &value);
    void clear();

private:
    void setupFormatting();
};

#endif // DIAGNOSTIC_OUTPUT_H
