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

#include "ToyButton.h"
#include "Utils.h"
#include "OSCParser.h"
#include "FadeButton.h"

////////////////////////////////////////////////////////////////////////////////

ToyButtonWidget::ToyButtonWidget(QWidget *parent)
	: ToyWidget(parent)
	, m_Toggle(false)
{
	m_HelpText = tr("Min = Button Up\nMax = Button Down\n\nLeave Min or Max blank to send single edge\n\nLeave both blank to send without arguments\n\nToggle:\nSpecify Min2 and/or Max2 for toggle behavior\n\nOSC Trigger:\nNo Arguments = Click\nArgument(1) = Press\nArgument(0) = Release");
	m_Min2 = m_Max2 = QString();

	m_Widget = new FadeButton(this);
	connect(m_Widget, SIGNAL(pressed()), this, SLOT(onPressed()));
	connect(m_Widget, SIGNAL(released()), this, SLOT(onReleased()));
	
	QPalette pal( m_Widget->palette() );
	m_Color = pal.color(QPalette::Button);
	m_TextColor = pal.color(QPalette::ButtonText);
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonWidget::SetText(const QString &text)
{
	ToyWidget::SetText(text);
	static_cast<FadeButton*>(m_Widget)->setText(m_Text);
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonWidget::SetImagePath(const QString &imagePath)
{
	ToyWidget::SetImagePath(imagePath);
	static_cast<FadeButton*>(m_Widget)->SetImagePath(m_ImagePath);
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonWidget::SetColor(const QColor &color)
{
	ToyWidget::SetColor(color);
	QPalette pal( m_Widget->palette() );
	pal.setColor(QPalette::Button, m_Color);
	m_Widget->setPalette(pal);
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonWidget::SetTextColor(const QColor &textColor)
{
	ToyWidget::SetTextColor(textColor);
	QPalette pal( m_Widget->palette() );
	pal.setColor(QPalette::ButtonText, m_TextColor);
	m_Widget->setPalette(pal);
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonWidget::SetLabel(const QString &label)
{
	static_cast<FadeButton*>(m_Widget)->SetLabel(label);
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonWidget::SetToggle(bool b)
{
	m_Toggle = b;
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonWidget::Recv(const QString &path, const OSCArgument *args, size_t count)
{
	FadeButton *button = static_cast<FadeButton*>(m_Widget);

	bool isFeedback = (path == m_FeedbackPath);
	bool isTrigger = (!isFeedback && path==m_TriggerPath);
	if(isFeedback || isTrigger)
    {
		bool toggle = false;
		bool press = false;
		bool gotAction = GetActionFromOSCArguments(args, count, toggle, press);
		if( isTrigger )
		{
			if( gotAction )
			{
				if( press )
					button->Press();
				else
					button->Release();
			}
			else
			{
				button->Press();
				button->Release();
			}
		}
		else if( HasToggle() )
		{
			if( gotAction )
			else
				SetToggle( !GetToggle() );
		}
		else if( gotAction )
		{
			if( press )
				button->Press(/*user*/false);
			else
				button->Release(/*user*/false);
		}
		else
		{
			button->Press(/*user*/false);
			button->Release(/*user*/false);
		}
    }
    else
    {
        std::string str;
        if(args && count>0)
        {
            if( !args[0].GetString(str) )
                str.clear();
        }

        SetLabel( QString::fromUtf8(str.c_str()) );
    }
}

////////////////////////////////////////////////////////////////////////////////

bool ToyButtonWidget::GetActionFromOSCArguments(const OSCArgument *args, size_t count, bool &toggle, bool &press) const
{
	if(args && count!=0)
	{
		float f = 0;
		if( args[0].GetFloat(f) )
		{
			if(!m_Min.isEmpty() && OSC_IS_ABOUTF(f,m_Min.toFloat()))
			{
				toggle = false;
				press = false;
				return true;
			}
			else if(!m_Max.isEmpty() && OSC_IS_ABOUTF(f,m_Max.toFloat()))
			{
				toggle = false;
				press = true;
				return true;
			}
			else if(!m_Min2.isEmpty() && OSC_IS_ABOUTF(f,m_Min2.toFloat()))
			{
				toggle = true;
				press = false;
				return true;
			}
			else if(!m_Max2.isEmpty() && OSC_IS_ABOUTF(f,m_Max2.toFloat()))
			{
				toggle = true;
				press = true;
				return true;
			}
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonWidget::onPressed()
{
	emit pressed(this);
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonWidget::onReleased()
{
	emit released(this);
	m_Toggle = !m_Toggle;
}

////////////////////////////////////////////////////////////////////////////////

ToyButtonGrid::ToyButtonGrid(Client *pClient, QWidget *parent, Qt::WindowFlags flags)
	: ToyGrid(TOY_BUTTON_GRID, pClient, parent, flags)
{
	SetGridSize( QSize(5,1) );
}

////////////////////////////////////////////////////////////////////////////////

ToyWidget* ToyButtonGrid::CreateWidget()
{
	ToyButtonWidget *w = new ToyButtonWidget(this);
	connect(w, SIGNAL(pressed(ToyButtonWidget*)), this, SLOT(onPressed(ToyButtonWidget*)));
	connect(w, SIGNAL(released(ToyButtonWidget*)), this, SLOT(onReleased(ToyButtonWidget*)));
	return w;
}

////////////////////////////////////////////////////////////////////////////////

bool ToyButtonGrid::SendButtonCommand(ToyButtonWidget *button, bool press)
{
	if(	m_pClient &&
		button &&
		!button->GetPath().isEmpty() )
	{
		bool hasMinMax = button->HasMinOrMax();
		bool hasMinMax2 = button->HasMin2OrMax2();

		if( hasMinMax )
		{
			if(hasMinMax2 && button->GetToggle())
				return SendButtonCommand(button->GetPath(), button->GetMin2(), button->GetMax2(), press);
			else
				return SendButtonCommand(button->GetPath(), button->GetMin(), button->GetMax(), press);
		}
		else if( hasMinMax2 )
			return SendButtonCommand(button->GetPath(), button->GetMin2(), button->GetMax2(), press);
		
		return SendButtonCommand(button->GetPath(), QString(), QString(), press);
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool ToyButtonGrid::SendButtonCommand(const QString &path, const QString &minStr, const QString &maxStr, bool press)
{
	bool shouldSend = false;
	bool forceStrArg = false;
	QString value;

	if( minStr.isEmpty() )
	{
		if( maxStr.isEmpty() )
		{
			// none
			if( press )
				shouldSend = true;
		}
		else
		{
			// max only
			if( press )
			{
				value = maxStr;
				shouldSend = true;
			}
		}
	}
	else if( maxStr.isEmpty() )
	{
		// min only
		if( !press )
		{
			value = minStr;
			shouldSend = true;
		}
	}
	else
	{
		// both
		value = (press ? maxStr : minStr);
		shouldSend = true;
		if(!OSCArgument::IsFloatString(minStr.toUtf8().constData()) || !OSCArgument::IsFloatString(maxStr.toUtf8().constData()))
			forceStrArg = true;	// if either is non-numeric, send both as strings
	}

	if( shouldSend )
	{
		QString path(path);
		bool local = Utils::MakeLocalOSCPath(false, path);
		OSCPacketWriter packetWriter( path.toUtf8().constData() );
		if( !value.isEmpty() )
		{
			QByteArray ba( value.toUtf8() );
			if(!forceStrArg && OSCArgument::IsFloatString(ba.constData()))
				packetWriter.AddFloat32( value.toFloat() );
			else
				packetWriter.AddString( ba.constData() );
		}

		size_t size;
		char *data = packetWriter.Create(size);
		if(data && m_pClient->ToyClient_Send(local,data,size))
			return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonGrid::onPressed(ToyButtonWidget *button)
{
	SendButtonCommand(button, /*press*/true);
}

////////////////////////////////////////////////////////////////////////////////

void ToyButtonGrid::onReleased(ToyButtonWidget *button)
{
	SendButtonCommand(button, /*press*/false);
}

////////////////////////////////////////////////////////////////////////////////
