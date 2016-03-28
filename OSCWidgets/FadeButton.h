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

#pragma once
#ifndef FADE_BUTTON_H
#define FADE_BUTTON_H

#ifndef QT_INCLUDE_H
#include "QtInclude.h"
#endif

#ifndef EOS_TIMER_H
#include "EosTimer.h"
#endif

////////////////////////////////////////////////////////////////////////////////

class FadeButton
	: public QPushButton
{
	Q_OBJECT
	
public:
	FadeButton(QWidget *parent);
	virtual ~FadeButton();

	virtual const QString& GetLabel() const {return m_Label;}
	virtual void SetLabel(const QString &label);
	virtual const QString& GetImagePath() const {return m_ImagePath;}
	virtual void SetImagePath(const QString &imagePath);
    virtual void Press(bool user=true);
    virtual void Release(bool user=true);
	virtual void Flash();
	
private slots:
	void onPressed();
	void onReleased();
	void onClickTimeout();
	void onHoverTimeout();
	
protected:
	float		m_Click;
	QTimer		*m_ClickTimer;
	EosTimer	m_ClickTimestamp;
	float		m_Hover;
	QTimer		*m_HoverTimer;
	EosTimer	m_HoverTimestamp;
	QString		m_Label;
	QString		m_ImagePath;
	QPixmap		m_Image;
	
	virtual void StartClick();
	virtual void StopClick();
	virtual void SetClick(float percent);
	virtual void StartHover();
	virtual void StopHover();
	virtual void SetHover(float percent);
	virtual void AutoSizeFont();
	virtual void UpdateImage();
	virtual void resizeEvent(QResizeEvent *event);
	virtual void paintEvent(QPaintEvent *event);
	virtual bool event(QEvent *event);
};

////////////////////////////////////////////////////////////////////////////////

#endif
