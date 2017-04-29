#ifndef BENCHMARKS_DESCRIPTORS_SWIG_HPP
#define BENCHMARKS_DESCRIPTORS_SWIG_HPP


#include <joint/devkit/JointException.hpp>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/ManifestReader.hpp>
#include <joint/devkit/StringBuilder.hpp>

#include <jni.h>
#include <swig/ISwigBenchmarks.hpp>


#if defined(_DEBUG)
#	undef _DEBUG
#	include <Python.h>
#	define _DEBUG
#else
#	include <Python.h>
#endif


namespace descriptors {
namespace swig
{

	namespace detail
	{
		template < typename T_ >
		T_ SwigJavaCallImpl(JNIEnv* env, T_ val, const char* msg)
		{
			if (env->ExceptionOccurred())
			{
				env->ExceptionDescribe();
				env->ExceptionClear();
				throw std::runtime_error(msg);
			}
			return val;
		}
	}

#define SWIG_JAVA_CALL(...) ::descriptors::swig::detail::SwigJavaCallImpl(env, (__VA_ARGS__), #__VA_ARGS__ " failed at " JOINT_SOURCE_LOCATION)

	struct Desc
	{
		JOINT_DEVKIT_LOGGER("Benchmarks.Swig");

		class Invokable : public IInvokable
		{
		public:
			void VoidToVoid() { }

			void I32ToVoid(int32_t) { }
			int32_t VoidToI32() { return 0; }

			void StringToVoid(const std::string& s) { }
			std::string VoidToString3() { return "abc"; }
			std::string VoidToString100() { return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; }
		};

		class BenchmarkCtx
		{
			class Jvm
			{
				JOINT_DEVKIT_LOGGER("Benchmarks.Swig.Java");

			public:
				JavaVM*   jvm;
				jclass    SwigBenchmarks_cls;

			public:
				static Jvm& Instance(const std::string& jar, const std::string& className)
				{
					static std::string jar_static = jar;
					JOINT_CHECK(jar_static == jar, "Java configuration mismatch!");

					static std::string class_name_static = className;
					JOINT_CHECK(class_name_static == className, "Java configuration mismatch!");

					static Jvm inst(jar, className);
					return inst;
				}

				JNIEnv* GetEnv() const
				{
					JNIEnv* env = nullptr;
					int retcode = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
					JOINT_CHECK(retcode == JNI_OK, ::joint::devkit::StringBuilder() % "jvm->GetEnv failed: " % retcode);
					return env;
				}

			private:
				Jvm(const std::string& jar, const std::string& className)
				{
					using namespace ::joint::devkit;

					std::string class_path_opt = "-Djava.class.path=" + jar;
					std::string lib_path_opt = "-Djava.library.path=/home/koplyarov/work/joint/build/bin";
					JavaVMOption opt[] = {
						{ const_cast<char*>(class_path_opt.c_str()), nullptr },
						{ const_cast<char*>(lib_path_opt.c_str()), nullptr }
					};

					JavaVMInitArgs vm_args = { };
					vm_args.version = 0x00010006;
					jint ret = JNI_GetDefaultJavaVMInitArgs(&vm_args);
					JOINT_CHECK(ret == 0, StringBuilder() % "JNI_GetDefaultJavaVMInitArgs failed: " % ret);
					vm_args.options = opt;
					vm_args.nOptions = sizeof(opt) / sizeof(opt[0]);

					JNIEnv* env = nullptr;
					ret = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), &vm_args);
					JOINT_CHECK(ret == 0, StringBuilder() % "JNI_CreateJavaVM failed: " % ret);
					JOINT_CHECK(jvm, "JNI_CreateJavaVM failed!");

					auto s = SWIG_JAVA_CALL(env->FindClass(className.c_str()));
					SwigBenchmarks_cls = SWIG_JAVA_CALL((jclass)env->NewGlobalRef((jobject)s));
					env->DeleteLocalRef(s);
				}

				~Jvm()
				{
					auto env = GetEnv();
					env->DeleteGlobalRef(SwigBenchmarks_cls);
					jvm->DestroyJavaVM();
				}
			};

			struct PyModuleManifest : public ::joint::devkit::ModuleManifestBase
			{
				std::string     ModuleName;

				template < typename Archive_ >
				void Deserialize(const Archive_& ar)
				{ ar.Deserialize("module", ModuleName); }
			};

			struct JavaModuleManifest : public ::joint::devkit::ModuleManifestBase
			{
				std::string                 Jar;
				std::string                 ClassName;

				template < typename Archive_ >
				void Deserialize(const Archive_& ar)
				{ ar.Deserialize("class", ClassName).Deserialize("jar", Jar); }
			};

		private:
			std::string         _jar;
			std::string         _className;
			PyObject*           _pyBenchmarks = nullptr;
			jobject             _javaBenchmarks = nullptr;

		public:
			BenchmarkCtx(const std::string& moduleManifestPath)
			{
				using namespace ::joint::devkit;

				JointCore_ManifestHandle m;
				JointCore_Error err = Joint_ReadManifestFromFile(moduleManifestPath.c_str(), &m);
				JOINT_CHECK(err == JOINT_CORE_ERROR_NONE, err);

				auto sg = ScopeExit([&]{ Joint_DeleteManifest(m); });

				ModuleManifestBase md;
				ManifestReader::Read(m, md);
				auto binding_name = md.GetBindingName();

				if (binding_name == "python")
				{
					PyModuleManifest md;
					ManifestReader::Read(m, md);

					Py_Initialize();
					PyObject* py_module_name = PyUnicode_FromString(md.ModuleName.c_str());
					PyObject* py_module = PyImport_Import(py_module_name);
					if (!py_module)
						throw std::runtime_error("Could not import " + md.ModuleName);
					const char* getter_name = "GetBenchmarks";
					PyObject* py_getter = PyObject_GetAttrString(py_module, getter_name);
					if (!py_getter)
						throw std::runtime_error("Could not find " + std::string(getter_name) + " function in " + md.ModuleName);
					_pyBenchmarks = PyObject_CallObject(py_getter, nullptr);
					if (!_pyBenchmarks)
						throw std::runtime_error("Could not get benchmarks object!");
				}
				else if (binding_name == "java")
				{
					JavaModuleManifest md;
					ManifestReader::Read(m, md);
					_jar = md.Jar;
					_className = md.ClassName;

					auto& jctx = Jvm::Instance(_jar, _className);
					auto env = jctx.GetEnv();

					jmethodID ctor = SWIG_JAVA_CALL(env->GetMethodID(jctx.SwigBenchmarks_cls, "<init>", "()V"));
					auto b = SWIG_JAVA_CALL(env->NewObject(jctx.SwigBenchmarks_cls, ctor));
					_javaBenchmarks = SWIG_JAVA_CALL(env->NewGlobalRef(b));
				}
				else
					throw std::runtime_error("Binding not supported!");
			}

			~BenchmarkCtx()
			{
				using namespace ::joint::devkit;

				if (_pyBenchmarks)
				{
					Py_DECREF(_pyBenchmarks);
					Py_Finalize();
				}

				if (_javaBenchmarks)
				{
					auto env = Jvm::Instance(_jar, _className).GetEnv();

					env->DeleteGlobalRef(_javaBenchmarks);
				}
			}

			IBenchmarks* CreateBenchmarks() const
			{
				auto result = GetGlobalBenchmarks();
				if (!result)
					throw std::runtime_error("Global benchmarks not set!");
				return result;
			}

			IInvokable* CreateLocalInvokable()
			{ return new Invokable; }
		};


		static std::string GetName() { return "swig"; }
	};

}}

#endif
