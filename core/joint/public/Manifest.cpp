#include <joint/public/Manifest.h>

#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/CppWrappers.hpp>
#include <joint/private/JointStructs.h>
#include <joint/private/JsonNode.hpp>
#include <joint/private/JsonParser.hpp>


using namespace joint;


static const char* LoggerName = "Joint.Core";
JOINT_DEVKIT_LOGGER(LoggerName)

extern "C"
{

	JointCore_Error Joint_ReadManifestFromFile(const char* path, JointCore_ManifestHandle* outManifest)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(outManifest, JOINT_CORE_ERROR_INVALID_PARAMETER);

		GetLogger().Info() << "ReadManifestFromFile(path: " << (path ? path : "null") << ")";
		JsonNode n = JsonParser::Parse(path);

		std::string path_str(path);
		size_t last_slash = path_str.find_last_of("/\\");

		std::string location = (last_slash == std::string::npos) ? "." : path_str.substr(0, last_slash);

		*outManifest = new Joint_Manifest(std::move(n), std::move(location));
		GetLogger().Debug() << "Manifest: " << *outManifest;

		JOINT_CPP_WRAP_END
	}


	void Joint_DeleteManifest(JointCore_ManifestHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		if (handle)
			delete handle;

		JOINT_CPP_WRAP_END_VOID
	}


	JointCore_Error Joint_GetManifestLocation(JointCore_ManifestHandle manifest, const char** outLocation)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(manifest, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outLocation, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outLocation = manifest->location.c_str();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestRootNode(JointCore_ManifestHandle manifest, JointCore_ManifestNodeHandle* outNode)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(manifest, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outNode, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outNode = &manifest->rootNode;

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeType(JointCore_ManifestNodeHandle node, JointCore_ManifestNodeType* outType)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outType, JOINT_CORE_ERROR_INVALID_PARAMETER);

		switch (node->GetType())
		{
		case JsonNode::Type::Null:    *outType = JOINT_CORE_MANIFEST_NODE_NULL; break;
		case JsonNode::Type::Boolean: *outType = JOINT_CORE_MANIFEST_NODE_BOOLEAN; break;
		case JsonNode::Type::Integer: *outType = JOINT_CORE_MANIFEST_NODE_INTEGER; break;
		case JsonNode::Type::Float:   *outType = JOINT_CORE_MANIFEST_NODE_FLOAT; break;
		case JsonNode::Type::String:  *outType = JOINT_CORE_MANIFEST_NODE_STRING; break;
		case JsonNode::Type::Array:   *outType = JOINT_CORE_MANIFEST_NODE_ARRAY; break;
		case JsonNode::Type::Object:  *outType = JOINT_CORE_MANIFEST_NODE_OBJECT; break;
		default: JOINT_THROW(JOINT_CORE_ERROR_INVALID_PARAMETER);
		}

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeBooleanValue(JointCore_ManifestNodeHandle node, JointCore_Bool* outVal)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outVal, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outVal = (JointCore_Bool)node->AsBoolean();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeIntegerValue(JointCore_ManifestNodeHandle node, int64_t* outVal)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outVal, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outVal = node->AsInteger();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeFloatValue(JointCore_ManifestNodeHandle node, double* outVal)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outVal, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outVal = node->AsFloat();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeStringValue(JointCore_ManifestNodeHandle node, const char** outVal)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outVal, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outVal = node->AsString().c_str();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeChildrenCount(JointCore_ManifestNodeHandle node, JointCore_SizeT* outCount)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outCount, JOINT_CORE_ERROR_INVALID_PARAMETER);

		switch (node->GetType())
		{
		case JsonNode::Type::Array:  *outCount = node->AsArray().size(); break;
		case JsonNode::Type::Object: *outCount = node->AsObject().size(); break;
		default: JOINT_THROW(JOINT_CORE_ERROR_INVALID_PARAMETER);
		}

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeArrayElement(JointCore_ManifestNodeHandle node, JointCore_SizeT index, JointCore_ManifestNodeHandle* outNode)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outNode, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outNode = static_cast<JointCore_ManifestNodeHandle>(&node->AsArray().at(index));

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetManifestNodeObjectElementByKey(JointCore_ManifestNodeHandle node, const char* key, JointCore_ManifestNodeHandle* outValue)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(node, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outValue, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outValue = static_cast<JointCore_ManifestNodeHandle>(const_cast<JsonNode*>(&node->AsObject().at(key)));

		JOINT_CPP_WRAP_END
	}

}
