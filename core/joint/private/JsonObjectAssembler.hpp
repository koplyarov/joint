#ifndef JOINT_PRIVATE_JSONOBJECTASSEMBLER_HPP
#define JOINT_PRIVATE_JSONOBJECTASSEMBLER_HPP


#include <joint/devkit/log/Logger.hpp>
#include <joint/private/JsonNode.hpp>

#include <stack>


namespace joint
{

    class JsonObjectAssembler
    {
        JOINT_DEVKIT_LOGGER("Joint.Core.JsonObjectAssembler");

        struct StackEntry
        {
            JsonNode::Builder   NodeBuilder;
            std::string         NextPropertyName;

            void AddChild(JsonNode node);
        };

        using NodesStack = std::stack<StackEntry>;

    private:
        NodesStack     _nodesStack;
        JsonNode       _result;

    public:
        void BeginObject();
        void EndObject();

        void BeginArray();
        void EndArray();

        void SetBooleanValue(bool value);
        void SetIntValue(int64_t value);
        void SetFloatValue(double value);
        void SetStringValue(const std::string& value);
        void SetNullValue();

        void SetObjectKey(const std::string& name);

        JsonNode GetAssembledObject() && { return std::move(_result); }
    };

}

#endif
