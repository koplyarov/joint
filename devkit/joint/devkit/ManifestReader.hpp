#ifndef JOINT_DEVKIT_MANIFESTREADER_HPP
#define JOINT_DEVKIT_MANIFESTREADER_HPP


#include <joint/devkit/JointException.hpp>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/Utils.hpp>

#include <type_traits>
#include <vector>


namespace joint {
namespace devkit
{

	namespace detail_ManifestReader
	{
		JOINT_DEVKIT_LOGGER("Joint.DevKit.ManifestReader");

#define DETAIL_JOINT_DEVKIT_JOINT_CALL(...) \
		do { \
			Joint_Error ret = (__VA_ARGS__); \
			if (ret != JOINT_ERROR_NONE) \
				throw std::runtime_error(std::string(#__VA_ARGS__ " failed: ") + Joint_ErrorToString(ret)); \
		} while (false)

		template < typename T_, typename Enabler_ = void >
		struct ManifestNodeReader;

		class Archive
		{
		private:
			Joint_ManifestNodeHandle    _node;

		public:
			Archive(Joint_ManifestNodeHandle node) : _node(node) { }

			template < typename T_ >
			const Archive& Deserialize(const std::string& key, T_& value) const;
		};

		template < >
		struct ManifestNodeReader<bool, void>
		{
		public:
			static void ReadNode(Joint_ManifestNodeHandle node, bool& value)
			{
				Joint_ManifestNodeType type;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
				JOINT_CHECK(type == JOINT_MANIFEST_NODE_BOOLEAN, JOINT_ERROR_INVALID_MANIFEST);

				JointCore_Bool real_value;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeBooleanValue(node, &real_value) );
				value = static_cast<bool>(real_value);
			}
		};

		template < typename T_ >
		struct ManifestNodeReader<T_, typename std::enable_if<std::is_integral<T_>::value>::type>
		{
		public:
			static void ReadNode(Joint_ManifestNodeHandle node, T_& value)
			{
				Joint_ManifestNodeType type;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
				JOINT_CHECK(type == JOINT_MANIFEST_NODE_INTEGER, JOINT_ERROR_INVALID_MANIFEST);

				int64_t real_value;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeIntegerValue(node, &real_value) );
				value = NoOverflowCast<T_>(real_value);
			}
		};

		template < typename T_ >
		struct ManifestNodeReader<T_, typename std::enable_if<std::is_floating_point<T_>::value>::type>
		{
		public:
			static void ReadNode(Joint_ManifestNodeHandle node, T_& value)
			{
				Joint_ManifestNodeType type;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
				JOINT_CHECK(type == JOINT_MANIFEST_NODE_INTEGER || type == JOINT_MANIFEST_NODE_FLOAT, JOINT_ERROR_INVALID_MANIFEST);

				double real_value;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeFloatValue(node, &real_value) );
				value = static_cast<T_>(real_value);
			}
		};

		template < >
		struct ManifestNodeReader<std::string, void>
		{
		public:
			static void ReadNode(Joint_ManifestNodeHandle node, std::string& value)
			{
				Joint_ManifestNodeType type;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
				JOINT_CHECK(type == JOINT_MANIFEST_NODE_STRING, JOINT_ERROR_INVALID_MANIFEST);

				const char* real_value;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeStringValue(node, &real_value) );
				value = real_value;
			}
		};

		template < typename T_ >
		struct ManifestNodeReader<std::vector<T_>, void>
		{
		public:
			static void ReadNode(Joint_ManifestNodeHandle node, std::vector<T_>& value)
			{
				Joint_ManifestNodeType type;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
				JOINT_CHECK(type == JOINT_MANIFEST_NODE_ARRAY, JOINT_ERROR_INVALID_MANIFEST);

				Joint_SizeT count;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeChildrenCount(node, &count) );

				value.clear();
				for (Joint_SizeT i = 0; i < count; ++i)
				{
					Joint_ManifestNodeHandle child_node;
					T_ tmp;
					DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeArrayElement(node, i, &child_node) );
					ManifestNodeReader<T_>::ReadNode(child_node, tmp);
					value.emplace_back(std::move(tmp));
				}
			}
		};

		template < typename T_ >
		struct ManifestNodeReader<T_, void>
		{
		public:
			static void ReadNode(Joint_ManifestNodeHandle node, T_& value)
			{
				Joint_ManifestNodeType type;
				DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
				JOINT_CHECK(type == JOINT_MANIFEST_NODE_OBJECT, JOINT_ERROR_INVALID_MANIFEST);

				value.Deserialize(Archive(node));
			}
		};


		template < typename T_ >
		const Archive& Archive::Deserialize(const std::string& key, T_& value) const
		{
			Joint_ManifestNodeHandle child_node;
			DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeObjectElementByKey(_node, key.c_str(), &child_node) );
			ManifestNodeReader<T_>::ReadNode(child_node, value);
			return *this;
		}
	}

	class ModuleManifestBase
	{
	private:
		std::string     _bindingName;

	public:
		std::string GetBindingName() const { return _bindingName; }

		template < typename Archive_ >
		void Deserialize(const Archive_& ar)
		{ ar.Deserialize("binding", _bindingName); }
	};

	class ManifestReader
	{
	public:
		template < typename T_ >
		static void Read(Joint_ManifestHandle manifest, T_& object)
		{
			using namespace detail_ManifestReader;

			Joint_ManifestNodeHandle root_node;
			DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestRootNode(manifest, &root_node) );

			ManifestNodeReader<T_>::ReadNode(root_node, object);
		}

		static std::string GetLocation(Joint_ManifestHandle manifest)
		{
			using namespace detail_ManifestReader;

			const char* location = nullptr;
			DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestLocation(manifest, &location) );
			JOINT_CHECK(location, JOINT_ERROR_INVALID_MANIFEST);
			return location;
		}
	};

}}

#endif
