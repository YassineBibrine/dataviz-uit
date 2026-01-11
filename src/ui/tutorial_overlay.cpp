#include "tutorial_overlay.h"
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QScreen>

TutorialOverlay::TutorialOverlay(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setAttribute(Qt::WA_TranslucentBackground, false);
    setWindowFlags(Qt::FramelessWindowHint);
    
    // Create tooltip widget
    tooltipWidget = new QWidget(this);
    tooltipWidget->setObjectName("tutorialTooltip");
    tooltipWidget->setStyleSheet(R"(
  QWidget#tutorialTooltip {
        background-color: #1a1a2e;
  border: 2px solid #4a9eff;
         border-radius: 12px;
      }
        QLabel {
            color: #ffffff;
     background: transparent;
        }
    QLabel#tutorialTitle {
      font-size: 16px;
            font-weight: bold;
  color: #4a9eff;
        }
        QLabel#tutorialDescription {
            font-size: 13px;
         color: #e0e0e0;
            line-height: 1.4;
        }
        QLabel#stepIndicator {
  font-size: 11px;
 color: #888888;
        }
    QPushButton {
            background-color: #4a9eff;
          color: white;
   border: none;
  border-radius: 6px;
            padding: 8px 16px;
       font-size: 12px;
        font-weight: bold;
        }
        QPushButton:hover {
      background-color: #6ab0ff;
        }
        QPushButton:pressed {
            background-color: #3a8eef;
        }
   QPushButton#skipButton {
       background-color: transparent;
   color: #888888;
        border: 1px solid #555555;
        }
      QPushButton#skipButton:hover {
          background-color: #333344;
  color: #aaaaaa;
        }
        QPushButton#prevButton {
          background-color: #333344;
color: #cccccc;
        }
        QPushButton#prevButton:hover {
   background-color: #444455;
        }
    )");

    QVBoxLayout* tooltipLayout = new QVBoxLayout(tooltipWidget);
    tooltipLayout->setContentsMargins(20, 16, 20, 16);
    tooltipLayout->setSpacing(10);

    // Title
    titleLabel = new QLabel(tooltipWidget);
    titleLabel->setObjectName("tutorialTitle");
    titleLabel->setWordWrap(true);
    tooltipLayout->addWidget(titleLabel);

    // Description
    descriptionLabel = new QLabel(tooltipWidget);
    descriptionLabel->setObjectName("tutorialDescription");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setMinimumWidth(280);
    descriptionLabel->setMaximumWidth(350);
    tooltipLayout->addWidget(descriptionLabel);

    // Step indicator
    stepIndicator = new QLabel(tooltipWidget);
    stepIndicator->setObjectName("stepIndicator");
tooltipLayout->addWidget(stepIndicator);

    // Buttons layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    skipButton = new QPushButton("Skip Tutorial", tooltipWidget);
    skipButton->setObjectName("skipButton");
    skipButton->setCursor(Qt::PointingHandCursor);
 connect(skipButton, &QPushButton::clicked, this, &TutorialOverlay::skip);
  buttonLayout->addWidget(skipButton);

    buttonLayout->addStretch();

  prevButton = new QPushButton("Back", tooltipWidget);
    prevButton->setObjectName("prevButton");
    prevButton->setCursor(Qt::PointingHandCursor);
    connect(prevButton, &QPushButton::clicked, this, &TutorialOverlay::previousStep);
    buttonLayout->addWidget(prevButton);

    nextButton = new QPushButton("Next", tooltipWidget);
    nextButton->setCursor(Qt::PointingHandCursor);
    connect(nextButton, &QPushButton::clicked, this, &TutorialOverlay::nextStep);
    buttonLayout->addWidget(nextButton);

    tooltipLayout->addLayout(buttonLayout);

 tooltipWidget->adjustSize();
    tooltipWidget->hide();

    // Setup fade animation
    opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(opacityEffect);
    opacityEffect->setOpacity(1.0);
}

TutorialOverlay::~TutorialOverlay() = default;

void TutorialOverlay::addStep(QWidget* target, const QString& title, const QString& description, const QString& position)
{
    steps.push_back({target, title, description, position});
}

void TutorialOverlay::start()
{
    if (steps.empty()) return;

    currentStepIndex = 0;
    
    if (parentWidget()) {
 setGeometry(parentWidget()->rect());
        raise();
    }
    
  show();
    showStep(0);
}

void TutorialOverlay::skip()
{
    hide();
    emit tutorialSkipped();
}

void TutorialOverlay::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
 painter.setRenderHint(QPainter::Antialiasing);

    // Draw semi-transparent overlay
    painter.fillRect(rect(), QColor(0, 0, 0, 180));

    if (currentStepIndex >= 0 && currentStepIndex < static_cast<int>(steps.size())) {
        QRect highlightRect = getHighlightRect();
      
        if (!highlightRect.isEmpty()) {
  // Create a path that excludes the highlight area
    QPainterPath path;
path.addRect(rect());
            
            QPainterPath highlightPath;
     highlightPath.addRoundedRect(highlightRect, highlightRadius, highlightRadius);
      
     path = path.subtracted(highlightPath);
        
            // Redraw the overlay with the cutout
       painter.fillRect(rect(), Qt::transparent);
    painter.fillPath(path, QColor(0, 0, 0, 180));

       // Draw glowing border around highlight
          QPen glowPen(QColor(74, 158, 255, 200), 3);
            painter.setPen(glowPen);
    painter.drawRoundedRect(highlightRect, highlightRadius, highlightRadius);
            
     // Draw outer glow
  for (int i = 1; i <= 3; ++i) {
     QPen outerGlow(QColor(74, 158, 255, 80 - i * 20), 2);
             painter.setPen(outerGlow);
      painter.drawRoundedRect(highlightRect.adjusted(-i * 2, -i * 2, i * 2, i * 2), 
      highlightRadius + i, highlightRadius + i);
            }
        }
    }
}

void TutorialOverlay::resizeEvent(QResizeEvent* event)
{
QWidget::resizeEvent(event);
    updateTooltipPosition();
}

void TutorialOverlay::mousePressEvent(QMouseEvent* event)
{
    // Check if click is inside tooltip
    if (tooltipWidget && tooltipWidget->geometry().contains(event->pos())) {
        // Let the tooltip handle it
        return;
    }
    
    // Click outside tooltip area advances to next step
    QRect highlightRect = getHighlightRect();
    if (highlightRect.contains(event->pos())) {
        nextStep();
    }
}

void TutorialOverlay::nextStep()
{
    if (currentStepIndex < static_cast<int>(steps.size()) - 1) {
   currentStepIndex++;
        showStep(currentStepIndex);
    } else {
 // Tutorial complete
        hide();
        emit tutorialCompleted();
    }
}

void TutorialOverlay::previousStep()
{
    if (currentStepIndex > 0) {
   currentStepIndex--;
        showStep(currentStepIndex);
    }
}

void TutorialOverlay::showStep(int index)
{
    if (index < 0 || index >= static_cast<int>(steps.size())) return;

    const TutorialStep& step = steps[index];

    // Update content
    titleLabel->setText(step.title);
    descriptionLabel->setText(step.description);
    stepIndicator->setText(QString("Step %1 of %2").arg(index + 1).arg(steps.size()));

    // Update buttons
    prevButton->setVisible(index > 0);
    nextButton->setText(index == static_cast<int>(steps.size()) - 1 ? "Finish" : "Next");

    // Calculate highlight rect
    currentHighlightRect = getHighlightRect();

    // Position tooltip
    updateTooltipPosition();
    tooltipWidget->show();

    // Trigger repaint
    update();
}

void TutorialOverlay::updateTooltipPosition()
{
    if (currentStepIndex < 0 || currentStepIndex >= static_cast<int>(steps.size())) return;
    if (!tooltipWidget) return;

    const TutorialStep& step = steps[currentStepIndex];
    QRect highlightRect = getHighlightRect();
    
    tooltipWidget->adjustSize();
    QSize tooltipSize = tooltipWidget->size();
    QPoint tooltipPos;

    int margin = 20;

    if (step.position == "center" || highlightRect.isEmpty()) {
        // Center on screen
        tooltipPos = QPoint(
  (width() - tooltipSize.width()) / 2,
   (height() - tooltipSize.height()) / 2
        );
    } else if (step.position == "bottom") {
        tooltipPos = QPoint(
  qBound(margin, highlightRect.center().x() - tooltipSize.width() / 2, width() - tooltipSize.width() - margin),
         qMin(highlightRect.bottom() + margin, height() - tooltipSize.height() - margin)
        );
    } else if (step.position == "top") {
        tooltipPos = QPoint(
        qBound(margin, highlightRect.center().x() - tooltipSize.width() / 2, width() - tooltipSize.width() - margin),
     qMax(margin, highlightRect.top() - tooltipSize.height() - margin)
        );
    } else if (step.position == "left") {
        tooltipPos = QPoint(
            qMax(margin, highlightRect.left() - tooltipSize.width() - margin),
   qBound(margin, highlightRect.center().y() - tooltipSize.height() / 2, height() - tooltipSize.height() - margin)
      );
    } else if (step.position == "right") {
     tooltipPos = QPoint(
            qMin(highlightRect.right() + margin, width() - tooltipSize.width() - margin),
            qBound(margin, highlightRect.center().y() - tooltipSize.height() / 2, height() - tooltipSize.height() - margin)
        );
    }

    tooltipWidget->move(tooltipPos);
}

QRect TutorialOverlay::getHighlightRect() const
{
    if (currentStepIndex < 0 || currentStepIndex >= static_cast<int>(steps.size())) {
        return QRect();
    }

    const TutorialStep& step = steps[currentStepIndex];
    
    if (!step.targetWidget || !step.targetWidget->isVisible()) {
    return QRect();
    }

    // Map widget geometry to overlay coordinates
    QPoint topLeft = step.targetWidget->mapTo(parentWidget(), QPoint(0, 0));
    QRect widgetRect(topLeft, step.targetWidget->size());

    // Add padding
    return widgetRect.adjusted(-highlightPadding, -highlightPadding, highlightPadding, highlightPadding);
}

void TutorialOverlay::animateTransition()
{
    // Future: Add smooth transition animations between steps
}
