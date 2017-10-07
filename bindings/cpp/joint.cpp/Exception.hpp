#ifndef JOINT_CPP_EXCEPTION_HPP
#define JOINT_CPP_EXCEPTION_HPP


#include <joint/Joint.h>

#include <atomic>
#include <sstream>
#include <string.h>
#include <utility>

#include <joint.cpp/detail/Config.hpp>


#if !DETAIL_JOINT_CPP_NO_EXCEPTIONS
#	include <stdexcept>
#endif


namespace joint
{

#define DETAIL_JOINT_CPP_EXCEPTION_TRY(...) \
		do { \
			JointCore_Error __ret__ = (__VA_ARGS__); \
			if (__ret__ != JOINT_CORE_ERROR_NONE) \
				JOINT_CORE_FATAL("Joint.C++.Exception", #__VA_ARGS__ " failed: %s", JointCore_ErrorToString(__ret__)); \
		} while (false);

	class Exception
#if !DETAIL_JOINT_CPP_NO_EXCEPTIONS
		: public std::exception
#endif
	{
	private:
		JointCore_Exception_Handle        _handle;
		mutable std::atomic<const char*>  _what{nullptr};

	public:
		Exception(const std::string& msg)
		{ DETAIL_JOINT_CPP_EXCEPTION_TRY(JointCore_Exception_Create(msg.c_str(), NULL, 0, &_handle)); }

		Exception(JointCore_Exception_Handle handle)
			: _handle(handle)
		{ }

		Exception(const Exception& other)
			: _handle(other._handle)
		{ JointCore_Exception_IncRef(_handle); }

		Exception& operator=(const Exception& other)
		{
			Exception tmp(other);
			Swap(tmp);
			return *this;
		}

#if DETAIL_JOINT_CPP_MOVE_SUPPORTED
		Exception(Exception&& other)
			: _handle(other._handle)
		{ other._handle = NULL; }

		Exception& operator=(Exception&& other)
		{
			Exception tmp(other);
			Swap(tmp);
			return *this;
		}
#endif

		~Exception() throw()
		{
			if (_handle)
				JointCore_Exception_DecRef(_handle);

			const char* what = _what;
			if (what)
				delete[] what;
		}

		const char* what() const throw()
		{
			const char* what = _what;
			if (!what)
			{
				std::stringstream ss;

				const char* msg;
				DETAIL_JOINT_CPP_EXCEPTION_TRY(JointCore_Exception_GetMessage(_handle, &msg));
				ss << msg;

				JointCore_SizeT bt_size;
				DETAIL_JOINT_CPP_EXCEPTION_TRY(JointCore_Exception_GetBacktraceSize(_handle, &bt_size));

				for (JointCore_SizeT i = 0; i < bt_size; ++i)
				{
					JointCore_Exception_BacktraceEntry e;
					DETAIL_JOINT_CPP_EXCEPTION_TRY(JointCore_Exception_GetBacktraceEntry(_handle, i, &e));

					ss << "\n\tat ";
					if (e.function[0])
						ss << e.function;

					if (e.module[0] || e.filename[0])
					{
						if (e.function[0])
							ss << "(";

						if (e.module[0])
							ss << e.module;
						if (e.filename[0])
						{
							ss << (e.module[0] ? "" : ", ") << e.filename;
							if (e.line != JOINT_CORE_EXCEPTION_INVALID_LINE)
								ss << ":" << e.line;
						}

						if (e.function[0])
							ss << ")";
					}

					if (e.code[0])
					{
						if (e.module[0] || e.function[0] || e.filename[0])
							ss << ": ";
						ss << "'" << e.code << "'";
					}
				}

				std::string s = ss.str();
				char* new_what = new char[s.size() + 1];
				strcpy(new_what, s.c_str());

				if (_what.compare_exchange_strong(what, new_what))
					what = new_what;
				else
					delete[] new_what;
			}
			return what;
		}

		JointCore_Exception_Handle Release()
		{
			JointCore_Exception_Handle handle = _handle;
			_handle = JOINT_CORE_NULL_HANDLE;
			return handle;
		}

	private:
		void Swap(Exception& other)
		{ std::swap(_handle, other._handle); }
	};

//#undef DETAIL_JOINT_CPP_EXCEPTION_TRY

}

#endif
