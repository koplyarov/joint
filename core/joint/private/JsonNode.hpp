#ifndef JOINT_PRIVATE_JSONNODE_HPP
#define JOINT_PRIVATE_JSONNODE_HPP


#include <joint/devkit/Logger.hpp>
#include <joint/devkit/util/JointException.hpp>

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
			Null,
			Boolean,
			Integer,
			Float,
			String,
			Array,
			Object
		};

		class Builder;

	private:
		using String = std::string;
		using Array = std::vector<JsonNode>;
		using Object = std::map<std::string, JsonNode>;

		union Variant
		{
			bool     b;
			int64_t  i;
			double   f;
			String*  s;
			Array*   a;
			Object*  o;
		};

	private:
		Type        _type = Type::Null;
		Variant     _value;

	public:
		JsonNode() = default;
		JsonNode(Builder&& builder);
		~JsonNode();

		JsonNode(JsonNode&& other);
		JsonNode& operator = (JsonNode&& other);

		JsonNode(const JsonNode&) = delete;
		JsonNode& operator = (const JsonNode&) = delete;

		Type GetType() const { return _type; }

		bool AsBoolean() const;
		int64_t AsInteger() const;
		double AsFloat() const;
		const String& AsString() const;
		const Array& AsArray() const;
		const Object& AsObject() const;
	};

	std::ostream& operator << (std::ostream& s, const JsonNode& n);


	class JsonNode::Builder
	{
		friend class JsonNode;

	private:
		JsonNode    _node;

	public:
		static Builder MakeNull();
		static Builder MakeBoolean(bool value);
		static Builder MakeInteger(int64_t value);
		static Builder MakeFloat(double value);
		static Builder MakeString(const String& value);
		static Builder MakeArray();
		static Builder MakeObject();

		Type GetType() const { return _node.GetType(); }

		Array& AsArray();
		Object& AsObject();
	};

}

#endif
