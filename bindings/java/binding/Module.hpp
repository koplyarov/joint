#ifndef BINDINGS_JAVA_BINDING_MODULE_HPP
#define BINDINGS_JAVA_BINDING_MODULE_HPP


#include <joint/devkit/accessors/Module.hpp>
#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/manifest/ManifestReader.hpp>
#include <joint/devkit/util/Holder.hpp>

#include <string>

#include <utils/JPtr.hpp>


namespace joint {
namespace java {
namespace binding
{

    class ModuleManifest : public devkit::ModuleManifestBase
    {
    private:
        std::string                 _className;
        std::vector<std::string>    _jars;

    public:
        std::string GetClassName() const { return _className; }
        std::vector<std::string> GetJars() const { return _jars; }

        template < typename Archive_ >
        void Deserialize(const Archive_& ar)
        {
            devkit::ModuleManifestBase::Deserialize(ar);
            ar.Deserialize("class", _className).Deserialize("jars", _jars);
        }
    };


    class Module : public joint::devkit::accessors::Module<Module>
    {
        JOINT_DEVKIT_LOGGER("Joint.Java.Module")

    private:
        ModuleManifest      _manifest;
        JClassGlobalRef     _cls;

    public:
        Module(JointCore_ManifestHandle moduleManifest);
        ~Module();

        JointCore_Error GetRootObject(const char* getterName, JointCore_ObjectAccessor* outObject) JOINT_DEVKIT_NOEXCEPT;

        const JClassGlobalRef& GetClass() const { return _cls; }
    };

}}}

#endif
