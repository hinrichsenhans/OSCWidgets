// Copyright (c) 2016 Electronic Theatre Controls, Inc., http://www.etcconnect.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "FadeButton.h"
#include "Utils.h"

////////////////////////////////////////////////////////////////////////////////

FadeButton::FadeButton(QWidget *parent)
	: QPushButton(parent)
	, m_Click(0)
	, m_Hover(0)
{
	m_ClickTimer = new QTimer(this);
	connect(m_ClickTimer, SIGNAL(timeout()), this, SLOT(onClickTimeout()));
	
	m_HoverTimer = new QTimer(this);
	connect(m_HoverTimer, SIGNAL(timeout()), this, SLOT(onHoverTimeout()));
	
	connect(this, SIGNAL(pressed()), this, SLOT(onPressed()));
	connect(this, SIGNAL(released()), this, SLOT(onReleased()));
	
	setAttribute(Qt::WA_Hover);
	Utils::RegisterTouchWidget(*this);
}

////////////////////////////////////////////////////////////////////////////////

FadeButton::~FadeButton()
{
	SetImagePath( QString() );
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::SetLabel(const QString &label)
{
	if(m_Label != label)
	{
		m_Label = label;
		update();
	}
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::SetImagePath(const QString &imagePath)
{
	if(m_ImagePath != imagePath)
	{
		PMC.Destroy(m_ImagePath);
		m_ImagePath = imagePath;
		PMC.Create(m_ImagePath);
		UpdateImage();
	}
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::UpdateImage()
{
	PMC.GetScaledToFill(m_ImagePath, size(), m_Image);
	update();
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::StartClick()
{
	m_ClickTimestamp.Start();
	m_ClickTimer->start(20);
	onClickTimeout();
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::StopClick()
{
	m_ClickTimer->stop();
	SetClick(0);
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::SetClick(float percent)
{
	if(m_Click != percent)
	{
		m_Click = percent;
		update();
	}
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::Flash()
{
	m_Hover = 1.0;
	m_HoverTimestamp.Start();
	m_HoverTimer->start(20);
	onHoverTimeout();
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::StartHover()
{
	m_HoverTimestamp.Start();
	m_HoverTimer->start(20);
	onHoverTimeout();
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::StopHover()
{
	m_HoverTimer->stop();
	SetHover(0);
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::SetHover(float percent)
{
	if(m_Hover != percent)
	{
		m_Hover = percent;
		update();
	}
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::AutoSizeFont()
{
	QFont fnt( font() );
	int d = qMin(width(), height());
	fnt.setPixelSize( qMax(14,qRound(d*0.2)) );
	setFont(fnt);
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::Press()
{
	if( !isDown() )
	{
		setDown(true);
		emit pressed();
	}
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::Release()
{
	if( isDown() )
	{
		setDown(false);
		emit released();
		emit clicked();
	}
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::resizeEvent(QResizeEvent *event)
{
	AutoSizeFont();
	UpdateImage();
	QPushButton::resizeEvent(event);
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::paintEvent(QPaintEvent* /*event*/)
{
	QRectF r( rect() );
	r.adjust(1, 1, -1, -1);
	
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
	
	float brightness = m_Click;
	if(m_Hover > 0)
		brightness += (m_Hover*0.2f);

	if(brightness > 0)
	{
		QColor color( palette().color(QPalette::Button) );
		qreal t = (brightness * BUTTON_BRIGHTESS);
		color.setRedF( qMin(color.redF()+t,1.0) );
		color.setGreenF( qMin(color.greenF()+t,1.0) );
		color.setBlueF( qMin(color.blueF()+t,1.0) );
		painter.setBrush(color);
	}
	else
		painter.setBrush( palette().color(QPalette::Button) );

	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(r, ROUNDED, ROUNDED);

	if( !m_Image.isNull() )
	{
		painter.setOpacity(1.0-(brightness*0.5));
		painter.drawPixmap(	r.x() + qRound((r.width()-m_Image.width())*0.5),
							r.y() + qRound((r.height()-m_Image.height())*0.5),
							m_Image );
		painter.setOpacity(1.0);
	}
	
	if(m_Hover > 0)
	{
		qreal dy = (-m_Hover * BUTTON_RAISE);
		if(dy != 0)
			r.adjust(0, 0, 0, dy);
	}
	
	QColor textColor( palette().color(QPalette::ButtonText) );
	if( !isEnabled() )
		textColor = textColor.darker(150);

	if( !text().isEmpty() )
	{
		if( m_Label.isEmpty() )
		{
			// just text centered
			painter.setPen(textColor);
			painter.drawText(r, Qt::AlignCenter|Qt::TextWordWrap, text());
		}
		else
		{
			// both text and label text
			QRectF textRect;
			painter.setPen(textColor);
			painter.drawText(r, Qt::AlignCenter|Qt::TextWordWrap|Qt::TextDontPrint, text(), &textRect);

			QRectF labelRect;
			painter.drawText(r, Qt::AlignCenter|Qt::TextWordWrap|Qt::TextDontPrint, m_Label, &labelRect);

			qreal h = (r.height() * 0.5 * (textRect.height()/labelRect.height()));
			painter.drawText(QRectF(r.x(),r.y(),r.width(),h), Qt::AlignCenter|Qt::TextWordWrap, text());
			painter.drawText(QRectF(r.x(),r.y()+h,r.width(),r.height()-h), Qt::AlignCenter|Qt::TextWordWrap, m_Label);
		}
	}
	else if( !m_Label.isEmpty() )
	{
		// just label text centered
		painter.setPen(textColor);
		painter.drawText(r, Qt::AlignCenter|Qt::TextWordWrap, m_Label);
	}
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::onPressed()
{
	StopClick();
	SetClick(1.0f);
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::onReleased()
{
	StartClick();
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::onClickTimeout()
{
	int elapsed = m_ClickTimestamp.GetElapsed();
	float t = (1.0f - elapsed/static_cast<float>(BUTTON_CLICK_MS));
	if(t <= 0)
	{
		t = 0;
		m_ClickTimer->stop();
	}
	SetClick(t);
}

////////////////////////////////////////////////////////////////////////////////

void FadeButton::onHoverTimeout()
{
	float t = (m_HoverTimestamp.GetElapsed() * BUTTON_HOVER_SPEED);
	
	if( underMouse() )
	{
		float hover = (m_Hover + t);
		if(hover >= 1.0f)
		{
			hover = 1.0f;
			m_HoverTimer->stop();
		}
		SetHover(hover);
	}
	else
	{
		float hover = (m_Hover - t);
		if(hover <= 0)
		{
			hover = 0;
			m_HoverTimer->stop();
		}
		SetHover(hover);
	}
}

////////////////////////////////////////////////////////////////////////////////

bool FadeButton::event(QEvent *event)
{
	if( event )
	{
		switch( event->type() )
		{
			case QEvent::HoverEnter:
			case QEvent::HoverLeave:
				StartHover();
				break;

			case QEvent::TouchBegin:
				{
					QTouchEvent *touchEvent = static_cast<QTouchEvent*>(event);
					if( !touchEvent->touchPoints().empty() )
					{
						const QTouchEvent::TouchPoint &touchPoint = touchEvent->touchPoints().front();
						QMouseEvent me(QEvent::MouseButtonPress, touchPoint.pos().toPoint(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
						mousePressEvent(&me);
					}

					event->accept();
				}
				return true;

			case QEvent::TouchUpdate:
				{
					QTouchEvent *touchEvent = static_cast<QTouchEvent*>(event);
					if( !touchEvent->touchPoints().empty() )
					{
						const QTouchEvent::TouchPoint &touchPoint = touchEvent->touchPoints().front();
						QMouseEvent me(QEvent::MouseMove, touchPoint.pos().toPoint(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
						mouseMoveEvent(&me);
					}

					event->accept();
				}
				return true;

			case QEvent::TouchEnd:
				{
					QTouchEvent *touchEvent = static_cast<QTouchEvent*>(event);
					if( !touchEvent->touchPoints().empty() )
					{
						const QTouchEvent::TouchPoint &touchPoint = touchEvent->touchPoints().front();
						QMouseEvent me(QEvent::MouseButtonRelease, touchPoint.pos().toPoint(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
						mouseReleaseEvent(&me);
					}

					event->accept();
				}
				return true;
		}
	}
	
	return QPushButton::event(event);
}

////////////////////////////////////////////////////////////////////////////////
