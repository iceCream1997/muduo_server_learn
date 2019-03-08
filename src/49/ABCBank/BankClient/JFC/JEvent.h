#ifndef _JEVENT_H_
#define _JEVENT_H_

namespace JFC
{

class JWindow;

class JEvent
{
public:
	JEvent(int code, JWindow* sender)
		: code_(code),
		  sender_(sender),
		  done_(false)
	{

	}

	JWindow* GetSender() const { return sender_; }
	int GetEventCode() const { return code_; }
	void Done() { done_ = true; }
	bool IsDone() const { return done_ == true; }


private:
	int code_;
	JWindow* sender_;
	bool done_;
};

}

#endif // _JEVENT_H_