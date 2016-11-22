#include <joint/private/JsonNode.hpp>


namespace joint
{

	JsonNode::Builder JsonNode::Builder::MakeNull()
	{
		Builder result;
		result._node._type = Type::Null;
		return result;
	}


	JsonNode::Builder JsonNode::Builder::MakeBoolean(bool value)
	{
		Builder result;
		result._node._type = Type::Boolean;
		result._node._value.b = value;
		return result;
	}


	JsonNode::Builder JsonNode::Builder::MakeInteger(int64_t value)
	{
		Builder result;
		result._node._type = Type::Integer;
		result._node._value.i = value;
		return result;
	}


	JsonNode::Builder JsonNode::Builder::MakeFloat(double value)
	{
		Builder result;
		result._node._type = Type::Float;
		result._node._value.f = value;
		return result;
	}


	JsonNode::Builder JsonNode::Builder::MakeString(const String& value)
	{
		Builder result;
		result._node._type = Type::String;
		result._node._value.s = new String(value);
		return result;
	}


	JsonNode::Builder JsonNode::Builder::MakeArray()
	{
		Builder result;
		result._node._type = Type::Array;
		result._node._value.a = new Array();
		return result;
	}


	JsonNode::Builder JsonNode::Builder::MakeObject()
	{
		Builder result;
		result._node._type = Type::Object;
		result._node._value.o = new Object();
		return result;
	}


	JsonNode::Array& JsonNode::Builder::AsArray()
	{
		JOINT_CHECK(_node._type == Type::Array, "Invalid json node type");
		return *_node._value.a;
	}


	JsonNode::Object& JsonNode::Builder::AsObject()
	{
		JOINT_CHECK(_node._type == Type::Object, "Invalid json node type");
		return *_node._value.o;
	}

	////////////////////////////////////////////////////////////////////////////////

	JsonNode::JsonNode(Builder&& builder)
	{ *this = std::move(builder._node); }


	JsonNode::~JsonNode()
	{
		switch (_type)
		{
		case Type::String:
			delete _value.s;
			break;
		case Type::Array:
			delete _value.a;
			break;
		case Type::Object:
			delete _value.o;
			break;
		default:
			break;
		}
	}


	JsonNode::JsonNode(JsonNode&& other)
		: _type(other._type), _value(other._value)
	{ other._type = Type::Null; }


	JsonNode& JsonNode::operator = (JsonNode&& other)
	{
		_type = other._type;
		_value = other._value;
		other._type = Type::Null;
		return *this;
	}


	bool JsonNode::AsBoolean() const
	{
		JOINT_CHECK(_type == Type::Boolean, "Invalid json node type");
		return _value.b;
	}


	int64_t JsonNode::AsInteger() const
	{
		JOINT_CHECK(_type == Type::Integer, "Invalid json node type");
		return _value.i;
	}


	double JsonNode::AsFloat() const
	{
		JOINT_CHECK(_type == Type::Float || _type == Type::Integer, "Invalid json node type");
		return _type == Type::Float ? _value.f : (double)_value.i;
	}


	const JsonNode::String& JsonNode::AsString() const
	{
		JOINT_CHECK(_type == Type::String, "Invalid json node type");
		return *_value.s;
	}


	const JsonNode::Array& JsonNode::AsArray() const
	{
		JOINT_CHECK(_type == Type::Array, "Invalid json node type");
		return *_value.a;
	}


	const JsonNode::Object& JsonNode::AsObject() const
	{
		JOINT_CHECK(_type == Type::Object, "Invalid json node type");
		return *_value.o;
	}


	std::ostream& operator << (std::ostream& s, const JsonNode& n)
	{
		bool first_element = true;
		switch (n.GetType())
		{
			case JsonNode::Type::Null:
				s << "null";
				break;
			case JsonNode::Type::Boolean:
				s << (n.AsBoolean() ? "true" : "false");
				break;
			case JsonNode::Type::Integer:
				s << n.AsInteger();
				break;
			case JsonNode::Type::Float:
				s << n.AsFloat();
				break;
			case JsonNode::Type::String:
				s << '"' << n.AsString() << '"';
				break;
			case JsonNode::Type::Array:
				s << "[";
				for (const auto& e : n.AsArray())
				{
					s << (first_element ? " " : ", ") << e;
					first_element = false;
				}
				s << " ]";
				break;
			case JsonNode::Type::Object:
				s << "{";
				for (const auto& e : n.AsObject())
				{
					s << (first_element ? " " : ", ") << "\"" << e.first << "\": " << e.second;
					first_element = false;
				}
				s << " }";
				break;
		}
		return s;
	}

}
