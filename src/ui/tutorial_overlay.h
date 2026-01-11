#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <vector>
#include <functional>

struct TutorialStep {
    QWidget* targetWidget;
    QString title;
    QString description;
    QString position; // "top", "bottom", "left", "right", "center"
};

/**
 * @class TutorialOverlay
 * @brief Modern tutorial overlay that highlights UI elements and shows instructions
 */
class TutorialOverlay : public QWidget {
    Q_OBJECT

public:
    explicit TutorialOverlay(QWidget* parent = nullptr);
    ~TutorialOverlay() override;

    void addStep(QWidget* target, const QString& title, const QString& description, const QString& position = "bottom");
    void start();
    void skip();

signals:
    void tutorialCompleted();
    void tutorialSkipped();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void nextStep();
 void previousStep();

private:
    void showStep(int index);
    void updateTooltipPosition();
    void animateTransition();
    QRect getHighlightRect() const;

    std::vector<TutorialStep> steps;
    int currentStepIndex = 0;

    // UI Elements
    QWidget* tooltipWidget = nullptr;
    QLabel* titleLabel = nullptr;
    QLabel* descriptionLabel = nullptr;
    QLabel* stepIndicator = nullptr;
    QPushButton* nextButton = nullptr;
    QPushButton* prevButton = nullptr;
    QPushButton* skipButton = nullptr;

    // Animation
    QPropertyAnimation* fadeAnimation = nullptr;
    QGraphicsOpacityEffect* opacityEffect = nullptr;
    
  // Highlight
    QRect currentHighlightRect;
    int highlightPadding = 8;
    int highlightRadius = 12;
};
