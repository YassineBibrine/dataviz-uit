#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <QPlainTextEdit>

/**
 * @class CodeEditor
 * @brief C++ code editor with syntax highlighting
 * 
 * Provides a specialized text editor for C++ source code with:
 * - Syntax highlighting
 * - Line numbering
 * - Code folding support (future)
 */
class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    
    void loadFile(const QString &filePath);
    void saveFile(const QString &filePath);
    QString getCode() const;
    
    void lineNumberAreaPaintEvent(class QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    void setupSyntaxHighlighting();

private:
    class LineNumberArea *lineNumberArea;
    class CppSyntaxHighlighter *syntaxHighlighter;
};

#endif // CODE_EDITOR_H
