#include <joint/private/JsonObjectAssembler.hpp>


namespace joint
{

    void JsonObjectAssembler::StackEntry::AddChild(JsonNode node)
    {
        switch (NodeBuilder.GetType())
        {
        case JsonNode::Type::Array:
            NodeBuilder.AsArray().emplace_back(std::move(node));
            break;
        case JsonNode::Type::Object:
            JOINT_DEVKIT_CHECK(!NextPropertyName.empty(), JOINT_CORE_ERROR_IMPLEMENTATION_ERROR);
            NodeBuilder.AsObject().emplace(NextPropertyName, std::move(node));
            NextPropertyName.clear();
            break;
        default:
            JOINT_DEVKIT_THROW(JOINT_CORE_ERROR_IMPLEMENTATION_ERROR);
        }
    }


    void JsonObjectAssembler::BeginObject()
    { _nodesStack.push(StackEntry{JsonNode::Builder::MakeObject()}); }

    void JsonObjectAssembler::EndObject()
    {
        JsonNode node(std::move(_nodesStack.top().NodeBuilder));
        _nodesStack.pop();

        if (!_nodesStack.empty())
            _nodesStack.top().AddChild(std::move(node));
        else
            _result = std::move(node);
    }


    void JsonObjectAssembler::BeginArray()
    { _nodesStack.push(StackEntry{JsonNode::Builder::MakeArray()}); }

    void JsonObjectAssembler::EndArray()
    {
        JsonNode node(std::move(_nodesStack.top().NodeBuilder));
        _nodesStack.pop();

        if (!_nodesStack.empty())
            _nodesStack.top().AddChild(std::move(node));
        else
            _result = std::move(node);
    }


    void JsonObjectAssembler::SetBooleanValue(bool value)
    { _nodesStack.top().AddChild({JsonNode::Builder::MakeBoolean(value)}); }


    void JsonObjectAssembler::SetIntValue(int64_t value)
    { _nodesStack.top().AddChild({JsonNode::Builder::MakeInteger(value)}); }


    void JsonObjectAssembler::SetFloatValue(double value)
    { _nodesStack.top().AddChild({JsonNode::Builder::MakeFloat(value)}); }


    void JsonObjectAssembler::SetStringValue(const std::string& value)
    { _nodesStack.top().AddChild({JsonNode::Builder::MakeString(value)}); }


    void JsonObjectAssembler::SetNullValue()
    { _nodesStack.top().AddChild({JsonNode::Builder::MakeNull()}); }


    void JsonObjectAssembler::SetObjectKey(const std::string& name)
    {
        JOINT_DEVKIT_CHECK(!_nodesStack.empty(), JOINT_CORE_ERROR_IMPLEMENTATION_ERROR);
        JOINT_DEVKIT_CHECK(_nodesStack.top().NextPropertyName.empty(), JOINT_CORE_ERROR_IMPLEMENTATION_ERROR);
        _nodesStack.top().NextPropertyName = name;
    }


}
