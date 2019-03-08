#include "JWindowBase.h"

using namespace JFC;

JWindowBase::ScreenBuffer JWindowBase::sb_;

JWindowBase::JWindowBase()
	: x_(0),
	  y_(0),
	  width_(0),
	  height_(0)
{
}

JWindowBase::JWindowBase(SHORT x, SHORT y, SHORT width, SHORT height)
	: x_(x),
	  y_(y),
	  width_(width),
	  height_(height),
	  fColor_(FCOLOR_BLACK),
	  bColor_(BCOLOR_WHITE)
{
}

JWindowBase::~JWindowBase()
{
}


// 设置光标位置
void JWindowBase::SetCursorPos(SHORT x, SHORT y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 刷新窗口
void JWindowBase::Refresh()
{
	sb_.refresh(x_, y_, width_, height_);
}

// 清除矩形区域
void JWindowBase::FillRect(JRECT rect)
{
	SHORT x;
	SHORT y;

	CHAR_INFO ch;
	ch.Char.AsciiChar = ' ';
	ch.Attributes = fColor_ | bColor_;

	for (y = rect.Top; y <= rect.Bottom; ++y)
	{
		for (x = rect.Left; x <= rect.Right; ++x)
		{
			sb_.write(x_+x, y_+y, ch);
		}
	}

}

// 绘制文本（x, y相对于本窗口的位置，并不是相对整个控制台窗口的位置）
void JWindowBase::DrawText(SHORT x, SHORT y, const std::string& text)
{
	sb_.write(x_+x, y_+y, text, fColor_, bColor_);
}

// 在窗口正中间绘制文本
void JWindowBase::DrawText(const std::string& text)
{
	SHORT x = (Width() - text.length()) / 2;
	SHORT y = (Height() - 1) / 2;
	DrawText(x, y, text);
}

// 在水平位置画线
void JWindowBase::DrawHLine(SHORT y, SHORT x1, SHORT x2, char ch)
{
	size_t count;
	SHORT x;
	if (x1 < x2)
	{
		x = x1;
		count = x2 - x1 + 1;
	}
	else
	{
		x = x2;
		count = x1 - x2 + 1;
	}

	std::string str(count, ch);
	DrawText(x, y, str);
}

// 在垂直位置画线
void JWindowBase::DrawVLine(SHORT x, SHORT y1, SHORT y2, char ch)
{
	size_t count;
	SHORT y;
	if (y1 < y2)
	{
		y = y1;
		count = y2 - y1 + 1;
	}
	else
	{
		y = y2;
		count = y1 - y2 + 1;
	}

	CHAR_INFO ci;
	ci.Char.AsciiChar = ch;
	ci.Attributes = fColor_ | bColor_;
	for (size_t i=0; i<count; ++i)
	{
		sb_.write(x_+x, y_+y, ci);
		++y;
	}
}


// 清除窗口
void JWindowBase::ClearWindow()
{
	JRECT rect = { 0, 0, Width()-1, Height()-1 };
	FillRect(rect);
}