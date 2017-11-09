#ifndef JOINT_DEVKIT_ACCESSORS_IMPLBASE_HPP
#define JOINT_DEVKIT_ACCESSORS_IMPLBASE_HPP


#include <joint/devkit/Config.hpp>
#include <joint/devkit/accessors/VTableStorage.hpp>
#include <joint/devkit/util/IntrusiveRefCounter.hpp>


namespace joint {
namespace devkit {
namespace accessors
{

    template < typename Derived_, typename VTableType_, typename AccessorType_ >
    class ImplBase : public IntrusiveRefCounter<Derived_>
    {
        using VTableStorage = accessors::VTableStorage<VTableType_, ImplBase>;

    public:
        using VTableType = VTableType_;
        using AccessorType = AccessorType_;

    private:
        static VTableStorage          s_vTable; // TODO: make sure this is not optimized away

    public:
        static void InitVTable(VTableType& vTable)
        {
            vTable.AddRef = &ImplBase::StaticAddRef;
            vTable.Release = &ImplBase::StaticRelease;
            Derived_::InitVTable(vTable);
        }

        static const VTableType* GetVTablePtr()
        { return s_vTable.GetVTablePtr(); }

    protected:
        ~ImplBase() = default;

        static Derived_& DerivedRef(void* instance) JOINT_DEVKIT_NOEXCEPT
        { return *static_cast<Derived_*>(reinterpret_cast<ImplBase*>(instance)); }

        AccessorType AccessorFromSelf() JOINT_DEVKIT_NOEXCEPT
        {
            AccessorType result;
            result.VTable = GetVTablePtr();
            result.Instance = this;
            return result;
        }

    private:
        static void StaticAddRef(void* instance) JOINT_DEVKIT_NOEXCEPT
        { DerivedRef(instance).AddRef(); }

        static void StaticRelease(void* instance) JOINT_DEVKIT_NOEXCEPT
        { DerivedRef(instance).Release(); }
    };


    template < typename Derived_, typename VTableType_, typename AccessorType_ >
    typename ImplBase<Derived_, VTableType_, AccessorType_>::VTableStorage ImplBase<Derived_, VTableType_, AccessorType_>::s_vTable;

}}}

#endif
