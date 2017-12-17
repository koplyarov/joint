#ifndef JOINT_DEVKIT_MANIFESTREADER_HPP
#define JOINT_DEVKIT_MANIFESTREADER_HPP


#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/JointException.hpp>
#include <joint/devkit/util/Utils.hpp>

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
            JointCore_Error ret = (__VA_ARGS__); \
            if (ret != JOINT_CORE_ERROR_NONE) \
                throw std::runtime_error(std::string(#__VA_ARGS__ " failed: ") + JointCore_ErrorToString(ret)); \
        } while (false)

        template < typename T_, typename Enabler_ = void >
        struct ManifestNodeReader;

        class Archive
        {
        private:
            JointCore_ManifestNodeHandle    _node;

        public:
            Archive(JointCore_ManifestNodeHandle node) : _node(node) { }

            template < typename T_ >
            const Archive& Deserialize(const std::string& key, T_& value) const;
        };

        template < >
        struct ManifestNodeReader<bool, void>
        {
        public:
            static void ReadNode(JointCore_ManifestNodeHandle node, bool& value)
            {
                JointCore_ManifestNodeType type;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
                JOINT_CHECK(type == JOINT_CORE_MANIFEST_NODE_BOOLEAN, JOINT_CORE_ERROR_INVALID_MANIFEST);

                JointCore_Bool real_value;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeBooleanValue(node, &real_value) );
                value = (real_value != 0);
            }
        };

        template < typename T_ >
        struct ManifestNodeReader<T_, typename std::enable_if<std::is_integral<T_>::value>::type>
        {
        public:
            static void ReadNode(JointCore_ManifestNodeHandle node, T_& value)
            {
                JointCore_ManifestNodeType type;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
                JOINT_CHECK(type == JOINT_CORE_MANIFEST_NODE_INTEGER, JOINT_CORE_ERROR_INVALID_MANIFEST);

                int64_t real_value;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeIntegerValue(node, &real_value) );
                value = NoOverflowCast<T_>(real_value);
            }
        };

        template < typename T_ >
        struct ManifestNodeReader<T_, typename std::enable_if<std::is_floating_point<T_>::value>::type>
        {
        public:
            static void ReadNode(JointCore_ManifestNodeHandle node, T_& value)
            {
                JointCore_ManifestNodeType type;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
                JOINT_CHECK(type == JOINT_CORE_MANIFEST_NODE_INTEGER || type == JOINT_CORE_MANIFEST_NODE_FLOAT, JOINT_CORE_ERROR_INVALID_MANIFEST);

                double real_value;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeFloatValue(node, &real_value) );
                value = static_cast<T_>(real_value);
            }
        };

        template < >
        struct ManifestNodeReader<std::string, void>
        {
        public:
            static void ReadNode(JointCore_ManifestNodeHandle node, std::string& value)
            {
                JointCore_ManifestNodeType type;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
                JOINT_CHECK(type == JOINT_CORE_MANIFEST_NODE_STRING, JOINT_CORE_ERROR_INVALID_MANIFEST);

                const char* real_value;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeStringValue(node, &real_value) );
                value = real_value;
            }
        };

        template < typename T_ >
        struct ManifestNodeReader<std::vector<T_>, void>
        {
        public:
            static void ReadNode(JointCore_ManifestNodeHandle node, std::vector<T_>& value)
            {
                JointCore_ManifestNodeType type;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
                JOINT_CHECK(type == JOINT_CORE_MANIFEST_NODE_ARRAY, JOINT_CORE_ERROR_INVALID_MANIFEST);

                JointCore_SizeT count;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeChildrenCount(node, &count) );

                value.clear();
                for (JointCore_SizeT i = 0; i < count; ++i)
                {
                    JointCore_ManifestNodeHandle child_node;
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
            static void ReadNode(JointCore_ManifestNodeHandle node, T_& value)
            {
                JointCore_ManifestNodeType type;
                DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestNodeType(node, &type) );
                JOINT_CHECK(type == JOINT_CORE_MANIFEST_NODE_OBJECT, JOINT_CORE_ERROR_INVALID_MANIFEST);

                value.Deserialize(Archive(node));
            }
        };


        template < typename T_ >
        const Archive& Archive::Deserialize(const std::string& key, T_& value) const
        {
            JointCore_ManifestNodeHandle child_node;
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
        static void Read(JointCore_ManifestHandle manifest, T_& object)
        {
            using namespace detail_ManifestReader;

            JointCore_ManifestNodeHandle root_node;
            DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestRootNode(manifest, &root_node) );

            ManifestNodeReader<T_>::ReadNode(root_node, object);
        }

        static std::string GetLocation(JointCore_ManifestHandle manifest)
        {
            using namespace detail_ManifestReader;

            const char* location = nullptr;
            DETAIL_JOINT_DEVKIT_JOINT_CALL( Joint_GetManifestLocation(manifest, &location) );
            JOINT_CHECK(location, JOINT_CORE_ERROR_INVALID_MANIFEST);
            return location;
        }
    };

}}

#endif
