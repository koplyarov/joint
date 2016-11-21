#ifndef JOINT_PRIVATE_JSONOBJECTASSEMBLER_HPP
#define JOINT_PRIVATE_JSONOBJECTASSEMBLER_HPP


#include <joint/devkit/JointException.hpp>
#include <joint/devkit/Logger.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>


namespace joint
{

	class JsonNode
	{
		JOINT_DEVKIT_LOGGER("Joint.Core.JsonNode");

	public:
		enum class Type
		{
			Integer,
			Float,
			String,
			List,
			Dictionary
		};

	private:
		using String = std::string;
		using List = std::vector<std::unique_ptr<JsonNode>>;
		using Dictionary = std::map<std::string, std::unique_ptr<JsonNode>>;

		union Variant
		{
			int64_t       i;
			double        f;
			String*       s;
			List*         l;
			Dictionary*   d;
		};

	private:
		Type        _type = Type::Integer;
		Variant     _value;

	public:
		JsonNode() = default;
		JsonNode(JsonNode&&) = default;
		JsonNode& operator = (JsonNode&&) = default;

		JsonNode(const JsonNode&) = delete;
		JsonNode& operator = (const JsonNode&) = delete;

		Type GetType() const { return _type; }

		int64_t AsInteger() const
		{
			JOINT_CHECK(_type == Type::Integer, "Invalid json node type");
			return _value.i;
		}

		double AsFloat() const
		{
			JOINT_CHECK(_type == Type::Float || _type == Type::Integer, "Invalid json node type");
			return _type == Type::Float ? _value.f : (double)_value.i;
		}

		const String& AsString() const
		{
			JOINT_CHECK(_type == Type::String, "Invalid json node type");
			return *_value.s;
		}

		const List& AsList() const
		{
			JOINT_CHECK(_type == Type::List, "Invalid json node type");
			return *_value.l;
		}

		const Dictionary& AsDictionary() const
		{
			JOINT_CHECK(_type == Type::Dictionary, "Invalid json node type");
			return *_value.d;
		}
	};


	class JsonObjectAssembler
	{
		JOINT_DEVKIT_LOGGER("Joint.Core.JsonObjectAssembler");

	private:

	public:
		void BeginDict();
		void EndDict();

		void BeginArray();
		void EndArray();

		void SetIntValue(int value);
		void SetFloatValue(float value);
		void SetStringValue(const std::string& value);
		void SetBoolValue(bool value);
		void SetNullValue();

		void SetDictKey(const std::string& name);

		//DeclObjectPtr GetAssembledObject() const;
	};

}

#endif
