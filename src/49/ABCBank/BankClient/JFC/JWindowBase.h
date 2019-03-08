#ifndef _JWINDOW_BASE_H_
#define _JWINDOW_BASE_H_

#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace JFC
{

// 黑色
// 蓝色
// 绿色
// 青色
// 红色
// 洋红
// 棕色
// 淡灰
// 深灰
// 淡蓝
// 淡绿
// 淡青
// 淡红
// 淡洋红
// 黄色
// 白色
#define FCOLOR_BLACK			0
#define FCOLOR_BLUE				FOREGROUND_BLUE
#define FCOLOR_GREEN			FOREGROUND_GREEN
#define FCOLOR_CYAN				FOREGROUND_BLUE | FOREGROUND_GREEN
#define FCOLOR_RED				FOREGROUND_RED
#define FCOLOR_MAGENTA			FOREGROUND_RED | FOREGROUND_BLUE
#define FCOLOR_BLOWN			FOREGROUND_RED | FOREGROUND_GREEN
#define FCOLOR_GRAY				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define FCOLOR_DARKGRAY			FCOLOR_BLACK + FOREGROUND_INTENSITY
#define FCOLOR_LIGHTBLUE		FCOLOR_BLUE + FOREGROUND_INTENSITY
#define FCOLOR_LIGHTGREEN		FCOLOR_GREEN + FOREGROUND_INTENSITY
#define FCOLOR_LIGHTCYAN		FCOLOR_CYAN + FOREGROUND_INTENSITY
#define FCOLOR_LIGHTRED			FCOLOR_RED + FOREGROUND_INTENSITY
#define FCOLOR_LIGHTMAGENTA		FCOLOR_MAGENTA + FOREGROUND_INTENSITY
#define FCOLOR_YELLO			FCOLOR_BLOWN + FOREGROUND_INTENSITY
#define FCOLOR_WHITE			FCOLOR_GRAY + FOREGROUND_INTENSITY

#define BCOLOR_BLACK			0
#define BCOLOR_BLUE				BACKGROUND_BLUE
#define BCOLOR_GREEN			BACKGROUND_GREEN
#define BCOLOR_CYAN				BACKGROUND_BLUE | BACKGROUND_GREEN
#define BCOLOR_RED				BACKGROUND_RED
#define BCOLOR_MAGENTA			BACKGROUND_RED | BACKGROUND_BLUE
#define BCOLOR_BLOWN			BACKGROUND_RED | BACKGROUND_GREEN
#define BCOLOR_GRAY				BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
#define BCOLOR_DARKGRAY			BCOLOR_BLACK + BACKGROUND_INTENSITY
#define BCOLOR_LIGHTBLUE		BCOLOR_BLUE + BACKGROUND_INTENSITY
#define BCOLOR_LIGHTGREEN		BCOLOR_GREEN + BACKGROUND_INTENSITY
#define BCOLOR_LIGHTCYAN		BCOLOR_CYAN + BACKGROUND_INTENSITY
#define BCOLOR_LIGHTRED			BCOLOR_RED + BACKGROUND_INTENSITY
#define BCOLOR_LIGHTMAGENTA		BCOLOR_MAGENTA + BACKGROUND_INTENSITY
#define BCOLOR_YELLO			BCOLOR_BLOWN + BACKGROUND_INTENSITY
#define BCOLOR_WHITE			BCOLOR_GRAY + BACKGROUND_INTENSITY


#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_(n) (48+n)
#define KEY_UP ((224<<8) + 72)			// 57416
#define KEY_DOWN ((224<<8) + 80)		// 57424
#define KEY_LEFT ((224<<8) + 75)		// 57419
#define KEY_RIGHT ((224<<8) + 77)		// 57421
#define KEY_DEL ((224<<8) + 83)			// 57427
#define KEY_BACK 8
#define KEY_TAB 9
#define KEY_HOME ((224<<8) + 71)		// 57415
#define KEY_PGUP ((224<<8) + 73)		// 57417
#define KEY_END ((224<<8) + 79)			// 57423
#define KEY_PGDN ((224<<8) + 81)		// 57425
#define KEY_F11 ((224<<8) + 133)		// 57477
#define KEY_F12 ((224<<8) + 134)		// 57478
#define KEY_F(n) (-58-n)				// -(59~68)
#define KEY_INS ((224<<8) + 82)			// 57425


typedef SMALL_RECT JRECT;

class JWindowBase
{
public:
	JWindowBase();
	JWindowBase(SHORT x, SHORT y, SHORT width, SHORT height);
	virtual ~JWindowBase();

	SHORT Width() const { return width_; }
	SHORT Height() const { return height_; }

	// 设置文本颜色
	void SetTextColor(WORD fColor) { fColor_ = fColor; }

	// 获取文本颜色
	WORD GetTextColor() const { return fColor_; }

	// 设置背景色
	void SetBkColor(WORD bColor) { bColor_ = bColor; }

	// 获取背景色
	WORD GetBkColor() const { return bColor_; }

	// 设置光标位置
	void SetCursorPos(SHORT x, SHORT y);

	// 刷新窗口
	void Refresh();

	// 清除矩形区域
	void FillRect(JRECT rect);

	// 绘制文本（x, y相对于本窗口的位置，并不是相对整个控制台窗口的位置）
	void DrawText(SHORT x, SHORT y, const std::string& text);

	// 在窗口正中间绘制文本
	void DrawText(const std::string& text);

	// 在水平位置画线
	void DrawHLine(SHORT y, SHORT x1, SHORT x2, char ch);

	// 在垂直位置画线
	void DrawVLine(SHORT x, SHORT y1, SHORT y2, char ch);

	// 清除窗口
	void ClearWindow();

private:
	class ScreenBuffer
	{
	public:
		ScreenBuffer()
		{

		}

		~ScreenBuffer()
		{

		}

		void write(SHORT x, SHORT y, CHAR_INFO ci)
		{
			buffer_[y * 80 + x] = ci;
		}

		void write(SHORT x, SHORT y, const std::string& str, WORD fColor, WORD bColor)
		{
			CHAR_INFO ci;
			ci.Attributes = fColor | bColor;
			
			for (size_t i=0; i<str.length(); ++i)
			{
				ci.Char.AsciiChar = str[i];
				write(x+i, y, ci);
			}
		}

		void refresh(SHORT x, SHORT y, SHORT width, SHORT height)
		{
			COORD size = { 80, 25 };
			COORD coord = { x, y };
			JRECT rect = { x, y, x+width-1, y+height-1 };
			WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE),
				buffer_,
				size,
				coord,
				&rect);
		}


		CHAR_INFO buffer_[80 * 25];
	};

public:
	static ScreenBuffer sb_;

protected:
	SHORT x_;			// x坐标
	SHORT y_;			// y坐标
	SHORT width_;		// 宽度
	SHORT height_;		// 高度
	WORD fColor_;		// 前景色（文本颜色）
	WORD bColor_;		// 背景色
};

// 用于保存与还原屏幕某一块区域数据
class ScreenSaver
{
public:
	ScreenSaver(JRECT rect)
		: rect_(rect)
	{
		SHORT w = rect_.Right - rect_.Left + 1;
		SHORT h = rect_.Bottom - rect_.Top + 1;
		buffer_ = new CHAR_INFO[w * h];
	}

	~ScreenSaver()
	{
		delete[] buffer_;
	}

	void Dump()
	{
		SHORT w = rect_.Right - rect_.Left + 1;
		SHORT h = rect_.Bottom - rect_.Top + 1;

		COORD size = {w, h};
		COORD offset = {0, 0};

		ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE),
			buffer_,
			size,
			offset,
			&rect_
			);
	}

	void Restore()
	{
		SHORT w = rect_.Right - rect_.Left + 1;
		SHORT h = rect_.Bottom - rect_.Top + 1;

		COORD size = {w, h};
		COORD offset = {0, 0};


		WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE),
			buffer_,
			size,
			offset,
			&rect_
			);

		for (int i=0; i<h; ++i)
		{
			for (int j=0; j<w; ++j)
			{
				JWindowBase::sb_.write(rect_.Left+j, rect_.Top+i, buffer_[w*i+j]);
			}
		}

	}
private:
	CHAR_INFO* buffer_;
	JRECT rect_;
};

}

#endif // _JWINDOW_BASE_H_
