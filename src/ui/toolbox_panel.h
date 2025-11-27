#pragma once
#include <QWidget>
#include <QListWidget>

class ToolboxPanel : public QWidget {
    Q_OBJECT
public:
    explicit ToolboxPanel(QWidget* parent = nullptr);
    void updateTools(const QString& mode);

signals:
    void toolSelected(QString toolName);

private:
    void setupUI();
    QListWidget* listWidget;
};
