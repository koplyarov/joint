#ifndef JOINT_PRIVATE_JOINTSTRUCTS_H
#define JOINT_PRIVATE_JOINTSTRUCTS_H


#include <joint/Joint.h>
#include <joint/private/Joint_Array.hpp>
#include <joint/private/JsonNode.hpp>

#include <atomic>
#include <string>
#include <vector>


extern "C"
{

	struct Joint_ManifestNode : public joint::JsonNode
	{
		Joint_ManifestNode(Joint_ManifestNode&& other)
			: joint::JsonNode(std::move(other))
		{ }

		Joint_ManifestNode(joint::JsonNode&& other)
			: joint::JsonNode(std::move(other))
		{ }
	};


	struct Joint_Manifest
	{
		const Joint_ManifestNode   rootNode;
		const std::string          location;

		Joint_Manifest(Joint_ManifestNode&& rootNode, std::string location)
			: rootNode(std::move(rootNode)), location(std::move(location))
		{ }
	};

}


inline std::ostream& operator << (std::ostream& s, const JointCore_ManifestHandle m)
{
	if (!m)
		s << "null";
	else
		s << "{ location: " << m->location << ", root: " << m->rootNode << " }";
	return s;
}


#endif
