package main

/*
#include <joint/Joint.h>

typedef struct JointGo_Accessor_s
{
    intptr_t*   VTable;
    intptr_t    Component;
} JointGo_Accessor;

extern JointGo_Accessor GetTests();
*/
import "C"

import "fmt"
import "joint"
import "unsafe"

////////////////////////////////////////////////////////////////////////////////

type SomeObject struct {
    counter  int32
}

func (self *SomeObject) Method() { self.counter++ }
func (self *SomeObject) GetInvokationsCount() int32 { return self.counter; }

func (self *SomeObject) Cast(interfaceId string) joint.Internal_Proxy {
    switch (interfaceId) {
    case joint_IObject_interfaceId: return Make_joint_IObject(self)
    case test_ISomeObject_interfaceId: return Make_test_ISomeObject(self)
    default: return nil
    }
}

////////////////////////////////////////////////////////////////////////////////

type CastComponent017 struct {
}

func (self *CastComponent017) Method1(i int8) int8 { return i }
func (self *CastComponent017) Method3(i int16) int16 { return i }
func (self *CastComponent017) Method4(i uint16) uint16 { return i }
func (self *CastComponent017) Method5(i int32) int32 { return i }
func (self *CastComponent017) Method6(i uint32) uint32 { return i }
func (self *CastComponent017) Method7(i int64) int64 { return i }

func (self *CastComponent017) Cast(interfaceId string) joint.Internal_Proxy {
    switch (interfaceId) {
    case joint_IObject_interfaceId: return Make_joint_IObject(self)
    case test_IInterface0_interfaceId: return Make_test_IInterface0(self)
    case test_IInterface1_interfaceId: return Make_test_IInterface1(self)
    case test_IInterface3_interfaceId: return Make_test_IInterface3(self)
    case test_IInterface4_interfaceId: return Make_test_IInterface4(self)
    case test_IInterface5_interfaceId: return Make_test_IInterface5(self)
    case test_IInterface6_interfaceId: return Make_test_IInterface6(self)
    case test_IInterface7_interfaceId: return Make_test_IInterface7(self)
    default: return nil
    }
}

////////////////////////////////////////////////////////////////////////////////

type Component struct {
}

/// IStarterTests //////////////////////////////////////////////////////////////

func (self *Component) Increment(value int32) int32 { return value + 1 }

/// IBasicTests ////////////////////////////////////////////////////////////////

func (self *Component) AddU8(l uint8, r uint8) uint8        { return l + r }
func (self *Component) AddI8(l int8, r int8) int8           { return l + r }
func (self *Component) AddU16(l uint16, r uint16) uint16    { return l + r }
func (self *Component) AddI16(l int16, r int16) int16       { return l + r }
func (self *Component) AddU32(l uint32, r uint32) uint32    { return l + r }
func (self *Component) AddI32(l int32, r int32) int32       { return l + r }
func (self *Component) AddU64(l uint64, r uint64) uint64    { return l + r }
func (self *Component) AddI64(l int64, r int64) int64       { return l + r }

func (self *Component) AddF32(l float32, r float32) float32 { return l + r }
func (self *Component) AddF64(l float64, r float64) float64 { return l + r }

func (self *Component) And(l bool, r bool) bool             { return l && r } 
func (self *Component) Concat(l string, r string) string    { return l + r }

func (self *Component) CallbackU8(cb test_IBasicTestsCallbackU8, l uint8, r uint8) uint8            { return cb.AddU8(l, r) }
func (self *Component) CallbackI8(cb test_IBasicTestsCallbackI8, l int8, r int8) int8               { return cb.AddI8(l, r) }
func (self *Component) CallbackU16(cb test_IBasicTestsCallbackU16, l uint16, r uint16) uint16       { return cb.AddU16(l, r) }
func (self *Component) CallbackI16(cb test_IBasicTestsCallbackI16, l int16, r int16) int16          { return cb.AddI16(l, r) }
func (self *Component) CallbackU32(cb test_IBasicTestsCallbackU32, l uint32, r uint32) uint32       { return cb.AddU32(l, r) }
func (self *Component) CallbackI32(cb test_IBasicTestsCallbackI32, l int32, r int32) int32          { return cb.AddI32(l, r) }
func (self *Component) CallbackU64(cb test_IBasicTestsCallbackU64, l uint64, r uint64) uint64       { return cb.AddU64(l, r) }
func (self *Component) CallbackI64(cb test_IBasicTestsCallbackI64, l int64, r int64) int64          { return cb.AddI64(l, r) }

func (self *Component) CallbackF32(cb test_IBasicTestsCallbackF32, l float32, r float32) float32    { return cb.AddF32(l, r) }
func (self *Component) CallbackF64(cb test_IBasicTestsCallbackF64, l float64, r float64) float64    { return cb.AddF64(l, r) }

func (self *Component) CallbackBool(cb test_IBasicTestsCallbackBool, l bool, r bool) bool           { return cb.And(l, r) }

func (self *Component) CallbackString(cb test_IBasicTestsCallbackString, l string, r string) string { return cb.Concat(l, r) }

/// IEnumTests /////////////////////////////////////////////////////////////////

func (self *Component) StringRepresentation(e test_Enum) string {
    return fmt.Sprintf("%v", e)
}

func (self *Component) GetNextValueInRing(e test_Enum) test_Enum {
    switch e {
    case test_Enum_Value1: return test_Enum_Value2;
    case test_Enum_Value2: return test_Enum_Value3;
    case test_Enum_Value3: return test_Enum_Value4;
    case test_Enum_Value4: return test_Enum_Value1;
    default: panic("Unknown enum value!");
    }
}

func (self *Component) CallbackStringRepresentation(cb test_IEnumTests, e test_Enum) string {
    return cb.StringRepresentation(e)
}

func (self *Component) CallbackGetNextValueInRing(cb test_IEnumTests, e test_Enum) test_Enum {
    return cb.GetNextValueInRing(e)
}

/// IEnumTests /////////////////////////////////////////////////////////////////

func (self *Component) CastTo1(obj test_IInterface0) test_IInterface1 { return CastTo_test_IInterface1(obj); }
func (self *Component) CastTo2(obj test_IInterface0) test_IInterface2 { return CastTo_test_IInterface2(obj); }
func (self *Component) CastTo3(obj test_IInterface0) test_IInterface3 { return CastTo_test_IInterface3(obj); }
func (self *Component) CastTo4(obj test_IInterface0) test_IInterface4 { return CastTo_test_IInterface4(obj); }
func (self *Component) CastTo5(obj test_IInterface0) test_IInterface5 { return CastTo_test_IInterface5(obj); }
func (self *Component) CastTo6(obj test_IInterface0) test_IInterface6 { return CastTo_test_IInterface6(obj); }
func (self *Component) CastTo7(obj test_IInterface0) test_IInterface7 { return CastTo_test_IInterface7(obj); }

func (self *Component) Create017() test_IInterface0 {
    return Make_test_IInterface0(&CastComponent017{})
}

/// IObjectTests ///////////////////////////////////////////////////////////////

func (self *Component) ReturnNull() test_ISomeObject {
    return nil
}

func (self *Component) CheckNotNull(o test_ISomeObject) bool {
    return o != nil
}

func (self *Component) CallbackReturn(cb test_IObjectTestsCallbackReturn) test_ISomeObject {
    return cb.Return()
}

func (self *Component) CallbackParam(cb test_IObjectTestsCallbackParam, o test_ISomeObject) bool {
    return cb.Method(o)
}

func (self *Component) InvokeObjectMethod(o test_ISomeObject) {
    o.Method()
}

func (self *Component) ReturnSameObject(o test_ISomeObject) test_ISomeObject {
    return o
}

func (self *Component) ReturnNewObject() test_ISomeObject {
    return Make_test_ISomeObject(&SomeObject{0})
}

////////////////////////////////////////////////////////////////////////////////

func (self *Component) Cast(interfaceId string) joint.Internal_Proxy {
    switch (interfaceId) {
    case joint_IObject_interfaceId: return Make_joint_IObject(self)
    case test_IBasicTests_interfaceId: return Make_test_IBasicTests(self)
    case test_ICastTests_interfaceId: return Make_test_ICastTests(self)
    case test_IEnumCallbackTests_interfaceId: return Make_test_IEnumCallbackTests(self)
    case test_IEnumTests_interfaceId: return Make_test_IEnumTests(self)
    case test_IObjectTests_interfaceId: return Make_test_IObjectTests(self)
    case test_IStarterTests_interfaceId: return Make_test_IStarterTests(self)
    default: return nil
    }
}

//export GetTests
func GetTests() C.JointGo_Accessor {
    var component = Make_test_IStarterTests(&Component{})
    var accessor = component.GetAccessor()
    return *(*C.JointGo_Accessor)(unsafe.Pointer(&accessor))
}

func main() {
}
