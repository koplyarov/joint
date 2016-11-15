#ifndef JOINT_DEVKIT_INTERFACEDESCRIPTOR_HPP
#define JOINT_DEVKIT_INTERFACEDESCRIPTOR_HPP


#include <joint/devkit/ArrayHolder.hpp>
#include <joint/devkit/JointException.hpp>
#include <joint/devkit/Logger.hpp>

#include <memory>


namespace joint {
namespace devkit
{

	class ValueMarshaller;


	template < typename BindingInfo_ >
	class TypeDescriptor
	{
		friend class joint::devkit::ValueMarshaller;

		class MemberInfo;

		using ArrayUserData = typename BindingInfo_::ArrayUserData;
		using ObjectUserData = typename BindingInfo_::ObjectUserData;
		using EnumUserData = typename BindingInfo_::EnumUserData;
		using StructUserData = typename BindingInfo_::StructUserData;
		using MemberId = typename BindingInfo_::MemberId;
		using MembersArray = ArrayHolder<MemberInfo>;

		JOINT_DEVKIT_LOGGER("Joint.Devkit.TypeDescriptor")

	private:
		MembersArray                     _members;
		std::unique_ptr<TypeDescriptor>  _arrayElementType;
		Joint_Type                       _jointType;
		size_t                           _membersCount = 0;
		std::unique_ptr<ArrayUserData>   _arrayUserData;
		std::unique_ptr<ObjectUserData>  _objectUserData;
		std::unique_ptr<EnumUserData>    _enumUserData;
		std::unique_ptr<StructUserData>  _structUserData;

	public:
		TypeDescriptor()
		{ _jointType.id = JOINT_TYPE_VOID; }

		template < typename NodeType_, typename DataObtainer_ >
		TypeDescriptor(const NodeType_& node, const DataObtainer_& dataObtainer)
		{
			_jointType.id = JOINT_TYPE_VOID;
			Init(node, dataObtainer);
		}

		template < typename NodeType_, typename DataObtainer_ >
		void Init(const NodeType_& node, const DataObtainer_& dataObtainer)
		{
			_jointType.id = dataObtainer.GetJointTypeId(node);
			switch (_jointType.id)
			{
			case JOINT_TYPE_ENUM:
				_enumUserData.reset(new EnumUserData(dataObtainer.GetEnumUserData(node)));
				break;
			case JOINT_TYPE_STRUCT:
				{
					_structUserData.reset(new StructUserData(dataObtainer.GetStructUserData(node)));
					auto members = dataObtainer.GetMembersNodes(node);
					auto sd = new Joint_StructDescriptor;
					_jointType.payload.structDescriptor = sd;

					_membersCount = members.GetCount();
					sd->membersCount = _membersCount;
					sd->memberTypes = new Joint_Type[_membersCount];
					_members.Reset(new MemberInfo[_membersCount]);

					for (size_t i = 0; i < _membersCount; ++i)
					{
						auto member_node = members.Get(i);
						_members[i].Init(*_structUserData, member_node, dataObtainer);
						sd->memberTypes[i] = _members[i].GetType().GetJointType();
					}
				}
				break;
			case JOINT_TYPE_OBJ:
				_objectUserData.reset(new ObjectUserData(dataObtainer.GetObjectUserData(node)));
				_jointType.payload.interfaceChecksum = dataObtainer.GetInterfaceChecksum(node);
				break;
			case JOINT_TYPE_ARRAY:
				_arrayElementType.reset(new TypeDescriptor);
				_arrayElementType->Init(dataObtainer.GetArrayElementTypeNode(node), dataObtainer);
				_arrayUserData.reset(new ArrayUserData(dataObtainer.GetArrayUserData(node)));
				_jointType.payload.arrayElementType = &_arrayElementType->GetJointType();
				break;
			default:
				break;
			}
		}

		~TypeDescriptor()
		{
			if (_jointType.id == JOINT_TYPE_STRUCT)
			{
				auto sd = _jointType.payload.structDescriptor;
				delete[] sd->memberTypes;
				delete sd;
			}

			_members.Reset();
		}

		TypeDescriptor(const TypeDescriptor&) = delete;
		TypeDescriptor& operator = (const TypeDescriptor&) = delete;

		Joint_Type& GetJointType() { return _jointType; }
		const Joint_Type& GetJointType() const { return _jointType; }
		const MemberInfo& GetMemberInfo(size_t index) const
		{
			if (index >= _membersCount)
				JOINT_THROW("Member index out of range");

			return _members[index];
		}

		const TypeDescriptor& GetArrayElementType() const
		{
			if (!_arrayElementType)
				JOINT_THROW("Array element type is not initialized");
			return *_arrayElementType;
		}
	};


	template < typename BindingInfo_ >
	class TypeDescriptor<BindingInfo_>::MemberInfo
	{
	private:
		MemberId            _memberId;
		TypeDescriptor      _memberType;

	public:
		MemberInfo() : _memberId() { }

		template < typename StructUserData_, typename NodeType_, typename DataObtainer_ >
		void Init(const StructUserData_& structUserData, const NodeType_& node, const DataObtainer_& dataObtainer)
		{
			_memberId = dataObtainer.GetMemberId(structUserData, node);
			_memberType.Init(dataObtainer.GetMemberType(structUserData, node), dataObtainer);
		}

		const TypeDescriptor& GetType() const { return _memberType; }
		const MemberId& GetMemberId() const { return _memberId; }
	};


	template < typename BindingInfo_ >
	class MethodDescriptor
	{
		using MethodUserData = typename BindingInfo_::MethodUserData;
		using TypeDescriptor = joint::devkit::TypeDescriptor<BindingInfo_>;
		using TypeDescriptorArray = ArrayHolder<TypeDescriptor>;

		JOINT_DEVKIT_LOGGER("Joint.Devkit.MethodDescriptor")

	private:
		TypeDescriptor        _retType;
		TypeDescriptorArray   _paramTypes;
		size_t                _paramsCount = 0;
		MethodUserData        _userData;

	public:
		template < typename NodeType_, typename DataObtainer_ >
		void Init(const NodeType_& node, const DataObtainer_& dataObtainer)
		{
			_userData = dataObtainer.GetMethodUserData(node);
			_retType.Init(dataObtainer.GetRetTypeNode(node), dataObtainer);

			auto params_obtainer = dataObtainer.GetParamsNodes(node);
			_paramsCount = params_obtainer.GetCount();

			_paramTypes.Reset(new TypeDescriptor[_paramsCount]);
			for (size_t i = 0; i < _paramsCount; ++i)
				_paramTypes[i].Init(params_obtainer.Get(i), dataObtainer);
		}

		const MethodUserData& GetUserData() const { return _userData; }

		const TypeDescriptor& GetRetType() const { return _retType; }

		const TypeDescriptor& GetParamType(size_t index) const
		{
			if (index >= _paramsCount)
				JOINT_THROW("Parameter index out of range");

			return _paramTypes[index];
		}
	};


	template < typename BindingInfo_ >
	class InterfaceDescriptor
	{
		using MethodDescriptor = joint::devkit::MethodDescriptor<BindingInfo_>;
		using MethodArray = ArrayHolder<MethodDescriptor>;

		JOINT_DEVKIT_LOGGER("Joint.Devkit.InterfaceDescriptor")

	private:
		MethodArray   _methods;
		size_t        _methodsCount = 0;

	public:
		template < typename NodeType_, typename DataObtainer_ >
		InterfaceDescriptor(const NodeType_& node, const DataObtainer_& dataObtainer)
		{
			auto methods_obtainer = dataObtainer.GetMethodsNodes(node);
			_methodsCount = methods_obtainer.GetCount();

			_methods.Reset(new MethodDescriptor[_methodsCount]);
			for (size_t i = 0; i < _methodsCount; ++i)
				_methods[i].Init(methods_obtainer.Get(i), dataObtainer);
		}

		const MethodDescriptor& GetMethod(size_t index) const
		{
			JOINT_ASSERT_EX(GetLogger().GetName(), index < _methodsCount);
			return _methods[index];
		}
	};


}}

#endif
