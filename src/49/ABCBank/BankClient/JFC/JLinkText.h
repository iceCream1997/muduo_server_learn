#ifndef _JLINK_TEXT_H_
#define _JLINK_TEXT_H_

#include "JWindow.h"

#include <string>

namespace JFC
{

class JLinkText : public JWindow
{
public:
	JLinkText();
	virtual ~JLinkText();
	JLinkText(SHORT x, SHORT y, SHORT width, SHORT height,
		const std::string& text = std::string(),
		JWindow* parent = jApp->Root());

	virtual void Draw();

private:
	std::string text_;
};

}

#endif // _JLINK_TEXT_H_