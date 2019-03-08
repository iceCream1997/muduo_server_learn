#ifndef _JLABEL_H_
#define _JLABEL_H_

#include "JWindow.h"

#include <string>

namespace JFC
{

class JLabel : public JWindow
{
public:
	JLabel();

	JLabel(SHORT x, SHORT y, SHORT width, SHORT height,
		const std::string& text = std::string(),
		JWindow* parent = jApp->Root());

	virtual ~JLabel();

	virtual void Draw();

	void SetText(const std::string& text)
	{
		text_ = text;
	}

private:
	std::string text_;
};

}
#endif // _JLABEL_H_
