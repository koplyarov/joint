#include <binding/Module.hpp>

#include <joint/devkit/util/CppWrappers.hpp>
#include <joint/devkit/util/JointException.hpp>

#include <binding/JointJavaContext.hpp>
#include <utils/JniError.hpp>
#include <utils/Utils.hpp>


namespace joint {
namespace java {
namespace binding
{

    using namespace devkit;


    Module::Module(JointCore_ManifestHandle moduleManifest)
        : _cls()
    {
        ManifestReader::Read(moduleManifest, _manifest);
        auto location = ManifestReader::GetLocation(moduleManifest);

        auto jvm = JointJavaCoreContext::GetJvm();
        auto env = GetJavaEnv(jvm);

        const auto& jjc = JointJavaContext::ConstInstance();

        ////////////////////////////////////////////////////////////////////////////////

        auto jars = _manifest.GetJars();
        auto class_name_string = JStringLocalRef::StealLocal(env, JAVA_CALL(env->NewStringUTF(_manifest.GetClassName().c_str())));
        auto urls = JObjArrayLocalRef::StealLocal(env, JAVA_CALL(env->NewObjectArray((jsize)jars.size(), jjc.URL_cls.Get(), nullptr)));

        for (size_t i = 0; i < jars.size(); ++i)
        {
            std::string jar_path = location + "/" + jars[i];

            auto url_string = JStringLocalRef::StealLocal(env, JAVA_CALL(env->NewStringUTF(jar_path.c_str())));
            auto file = JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(jjc.File_cls.Get(), jjc.File_ctor_id, url_string.Get())));
            auto uri = JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallObjectMethod(file.Get(), jjc.File_toURI_id)));
            auto url = JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallObjectMethod(uri.Get(), jjc.URI_toURL_id)));

            JAVA_CALL_VOID(env->SetObjectArrayElement(urls.Get(), (jsize)i, url.Get()));
        }

        auto class_loader = JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(jjc.URLClassLoader_cls.Get(), jjc.URLClassLoader_ctor_id, urls.Get())));

        _cls = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->CallObjectMethod(class_loader.Get(), jjc.URLClassLoader_loadClass_id, class_name_string.Get())));
        JOINT_CHECK(_cls, StringBuilder() % "Class " % _manifest.GetClassName() % " not found");
    }


    Module::~Module()
    { }


    JointCore_Error Module::GetRootObject(const char* getterName, JointCore_ObjectAccessor* outObject) JOINT_DEVKIT_NOEXCEPT
    {
        JOINT_CPP_WRAP_BEGIN

        auto jvm = JointJavaCoreContext::GetJvm();
        auto env = GetJavaEnv(jvm);

        auto jm = JointJavaContext::ModuleContext::Make(env, AccessorFromSelf());

        jmethodID root_obj_getter_id = JAVA_CALL(env->GetStaticMethodID(_cls.Get(), getterName, "(Lorg/joint/ModuleContext;)Lorg/joint/JointObject;"));

        auto root_obj = JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallStaticObjectMethod(_cls.Get(), root_obj_getter_id, jm.Get())));
        JointJavaContext::JointObject jo(root_obj);

        *outObject = jo.GetAccessor();
        JOINT_CORE_INCREF_ACCESSOR(*outObject);

        JOINT_CPP_WRAP_END
    }

}}}
