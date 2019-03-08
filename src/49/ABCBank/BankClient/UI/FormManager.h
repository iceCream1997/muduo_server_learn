#ifndef _FORM_MANAGER_H_
#define _FORM_MANAGER_H_

#include "../JFC/JForm.h"
#include "../../Public/Singleton.h"

#include <string>
#include <map>

using namespace PUBLIC;
using namespace JFC;

namespace UI
{

class FormManager
{
	friend class Singleton<FormManager>;
public:
	void Add(const std::string& name, JForm* form)
	{
		forms_[name] = form;
	}

	JForm* Get(const std::string& name)
	{
		return forms_[name];
	}
private:
	std::map<std::string, JForm*> forms_;
	FormManager();
	~FormManager();
};

}

#endif // _FORM_MANAGER_H_