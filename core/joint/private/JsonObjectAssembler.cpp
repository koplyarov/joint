#include <joint/private/JsonObjectAssembler.hpp>


namespace joint
{

	void JsonObjectAssembler::BeginDict()
	{ GetLogger().Warning() << "BeginDict"; }

	void JsonObjectAssembler::EndDict()
	{ GetLogger().Warning() << "EndDict"; }


	void JsonObjectAssembler::BeginArray()
	{ GetLogger().Warning() << "BeginArray"; }

	void JsonObjectAssembler::EndArray()
	{ GetLogger().Warning() << "EndArray"; }


	void JsonObjectAssembler::SetIntValue(int value)
	{ GetLogger().Warning() << "SetIntValue: " << value; }

	void JsonObjectAssembler::SetFloatValue(float value)
	{ GetLogger().Warning() << "SetFloatValue: " << value; }

	void JsonObjectAssembler::SetStringValue(const std::string& value)
	{ GetLogger().Warning() << "SetStringValue: " << value; }

	void JsonObjectAssembler::SetBoolValue(bool value)
	{ GetLogger().Warning() << "SetBoolValue: " << value; }

	void JsonObjectAssembler::SetNullValue()
	{ GetLogger().Warning() << "SetNullValue"; }

	void JsonObjectAssembler::SetDictKey(const std::string& name)
	{ GetLogger().Warning() << "SetDictKey: " << name; }


}
