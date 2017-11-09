#ifndef JOINT_DEVKIT_ACCESSORS_VTABLESTORAGE_HPP
#define JOINT_DEVKIT_ACCESSORS_VTABLESTORAGE_HPP


namespace joint {
namespace devkit {
namespace accessors
{

    template < typename VTable_, typename VTableInitializer_ >
    class VTableStorage
    {
    private:
        VTable_    _vTable;

    public:
        VTableStorage()
        { VTableInitializer_::InitVTable(_vTable); }

        const VTable_* GetVTablePtr() const
        { return &_vTable; }
    };


}}}

#endif
