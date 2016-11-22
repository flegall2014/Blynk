#include "slider.h"
#include "ui_slider.h"
#include <QPainter>
#include <QDebug>

// Constructor:
Slider::Slider(QWidget *parent) : QWidget(parent), ui(new Ui::Slider),
    m_nMinorTicks(5),
    m_majorTickColor(Qt::gray),
    m_minorTickColor(Qt::gray),
    m_iHorizontalMargin(24),
    m_iLabelAreaHeight(12)
{
    ui->setupUi(this);
    ui->wSlider->setSingleStep(1);
    ui->wSlider->setPageStep(5);
    ui->wSliderLayout->setContentsMargins(m_iHorizontalMargin, 0, m_iHorizontalMargin, 0);
    ui->wTickLayout->setContentsMargins(m_iHorizontalMargin, 0, m_iHorizontalMargin, 0);
    connect(ui->wSlider, &QSlider::valueChanged, this, &Slider::valueChanged);
}

// Destructor:
Slider::~Slider()
{
    delete ui;
}

// Set major ticks labels:
void Slider::setMajorTickLabels(const QStringList &lTickLabels)
{
    m_lTickLabels = lTickLabels;
}

// Set minor ticks:
void Slider::setMinorTicks(int nMinor)
{
    m_nMinorTicks = nMinor;
}

// Set range:
void Slider::setRange(int iMinValue, int iMaxValue)
{
    ui->wSlider->setRange(iMinValue, iMaxValue);
}

// Set value:
void Slider::setValue(int iValue)
{
    ui->wSlider->setValue(iValue);
}

// Set page step:
void Slider::setPageStep(int iPageStep)
{
    ui->wSlider->setPageStep(iPageStep);
}

// Set single step:
void Slider::setSingleStep(int iSingleStep)
{
    ui->wSlider->setSingleStep(iSingleStep);
}

// Paint event:
void Slider::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // Create painter:
    QPainter painter(this);

    // Tick rect:
    QPoint sliderPos = ui->wSlider->pos();
    int iSliderWidth = ui->wSlider->width();
    int iDelta = 6;
    int iRulerX = sliderPos.x()+iDelta;
    int iRulerWidth = iSliderWidth-iDelta;
    QRect tickRect(iRulerX, rect().height()/2, iRulerWidth, rect().height()/2);
    painter.setPen(QPen(m_majorTickColor));
    //painter.drawLine(iRulerX, tickRect.top(), iRulerX+iRulerWidth-1, tickRect.top());

    int iMajorTickHeight = tickRect.height()-m_iLabelAreaHeight;
    int iMinorTickHeight = iMajorTickHeight/2;

    // Draw major ticks:
    int nMajorTicks = m_lTickLabels.size();
    if (nMajorTicks > 1)
    {
        // Major tick interval:
        double dMajorTickInterval = (double)iRulerWidth/(double)(nMajorTicks-1);

        // Draw minor ticks:
        double dMinorTickInterval = dMajorTickInterval/(m_nMinorTicks+1);

        // Draw first & last major ticks:
        QPen majorPen(m_majorTickColor, 2);
        painter.setPen(majorPen);
        painter.drawLine(iRulerX, tickRect.top()+1, iRulerX, tickRect.top()+1+iMajorTickHeight);
        painter.drawLine(iRulerX+iRulerWidth-1, tickRect.top()+1, iRulerX+iRulerWidth-1, tickRect.top()+1+iMajorTickHeight);

        // Draw other major ticks:
        for (int i=1; i<nMajorTicks-1; i++)
            painter.drawLine(i*dMajorTickInterval+iRulerX, tickRect.top()+1, i*dMajorTickInterval+iRulerX, tickRect.top()+1+iMajorTickHeight);

        // Draw minor ticks:
        for (int i=0; i<nMajorTicks-1; i++)
        {
            for (int j=0; j<m_nMinorTicks; j++)
            {
                double x0 = (j+1)*dMinorTickInterval + i*dMajorTickInterval + iRulerX;
                painter.setPen(m_minorTickColor);
                painter.drawLine(x0, tickRect.top()+1, x0, tickRect.top()+1+iMinorTickHeight);
            }
        }

        // Draw labels:
        painter.setPen(m_majorTickColor);
        for (int i=0; i<nMajorTicks; i++)
        {
            double x0 = i*dMajorTickInterval+iRulerX;
            int y0 =  tickRect.top()+1+iMajorTickHeight;
            QRect labelRect(x0-m_iHorizontalMargin, y0, 2*m_iHorizontalMargin, m_iLabelAreaHeight);
            painter.drawText(labelRect, Qt::AlignHCenter|Qt::AlignVCenter, m_lTickLabels[i]);
        }
    }
}
