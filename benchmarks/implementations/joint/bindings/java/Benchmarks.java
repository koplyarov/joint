package benchmarks;

import org.joint.*;
import adapters.Adapters.*;


class Benchmarks
{

    static class Object
        extends
            AccessorsContainer
        implements
            joint_IObject_impl
    {
        Object()
        {
            joint_IObject.registerAccessors(this);
        }
    }


    static class CastComponent
        extends
            AccessorsContainer
        implements
            benchmarks_ICastInterface1_impl,
            benchmarks_ICastInterface2_impl
    {
        CastComponent()
        {
            benchmarks_ICastInterface1.registerAccessors(this);
            benchmarks_ICastInterface2.registerAccessors(this);
        }
    }


    static class Component
        extends
            AccessorsContainer
        implements
            benchmarks_IBasicBenchmarks_impl,
            benchmarks_IEnumBenchmarks_impl,
            benchmarks_IStructBenchmarks_impl,
            benchmarks_IObjectBenchmarks_impl,
            benchmarks_ICastBenchmarks_impl,
            benchmarks_IExceptionBenchmarks_impl
    {
        private java.util.Random r = new java.util.Random();
        private ModuleContext module;
        public int dummyInt = 0;
        private joint_IObject obj;

        Component(ModuleContext module)
        {
            benchmarks_IBasicBenchmarks.registerAccessors(this);
            benchmarks_IEnumBenchmarks.registerAccessors(this);
            benchmarks_IStructBenchmarks.registerAccessors(this);
            benchmarks_IObjectBenchmarks.registerAccessors(this);
            benchmarks_ICastBenchmarks.registerAccessors(this);
            benchmarks_IExceptionBenchmarks.registerAccessors(this);

            this.module = module;
            this.obj = joint_IObject.makeComponent(module, new Object());
        }

        ///// IBasicBenchmarks /////

        public void NativeVoidToVoid() { dummyInt = r.nextInt(); }

        public void NativeI32ToVoid(int p) { dummyInt = r.nextInt() + p; }
        public int NativeVoidToI32() { dummyInt = r.nextInt(); return dummyInt; }

        public void NativeStringToVoid(String s) { dummyInt = r.nextInt() + s.length(); }
        public String NativeVoidToString3() { return "abc"; }
        public String NativeVoidToString100() { return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; }


        public void VoidToVoid() { }

        public void I32ToVoid(int p) { }
        public int VoidToI32() { return 0; }

        public void StringToVoid(String s) { }
        public String VoidToString3() { return "abc"; }
        public String VoidToString100() { return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; }


        public void MeasureNativeVoidToVoid(long n)
        { for (long i = 0; i < n; ++i) NativeVoidToVoid(); }

        public void MeasureNativeI32ToVoid(long n)
        { for (long i = 0; i < n; ++i) NativeI32ToVoid(42); }

        public void MeasureNativeVoidToI32(long n)
        { for (long i = 0; i < n; ++i) NativeVoidToI32(); }

        public void MeasureNativeString3ToVoid(long n)
        { for (long i = 0; i < n; ++i) NativeStringToVoid("abc"); }

        public void MeasureNativeVoidToString3(long n)
        { for (long i = 0; i < n; ++i) NativeVoidToString3(); }

        public void MeasureNativeString100ToVoid(long n)
        { for (long i = 0; i < n; ++i) NativeStringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"); }

        public void MeasureNativeVoidToString100(long n)
        { for (long i = 0; i < n; ++i) NativeVoidToString100(); }


        public void MeasureOutgoingVoidToVoid(benchmarks_IBasicInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.VoidToVoid(); }

        public void MeasureOutgoingI32ToVoid(benchmarks_IBasicInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.I32ToVoid(0); }

        public void MeasureOutgoingVoidToI32(benchmarks_IBasicInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.VoidToI32(); }

        public void MeasureOutgoingString3ToVoid(benchmarks_IBasicInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.StringToVoid("abc"); }

        public void MeasureOutgoingVoidToString3(benchmarks_IBasicInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.VoidToString3(); }

        public void MeasureOutgoingString100ToVoid(benchmarks_IBasicInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.StringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"); }

        public void MeasureOutgoingVoidToString100(benchmarks_IBasicInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.VoidToString100(); }


        ///// IEnumBenchmarks /////


        public void EnumToVoid(benchmarks_Enum p) { }
        public benchmarks_Enum VoidToEnum() { return benchmarks_Enum.A; }

        public void MeasureNativeEnumToVoid(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureNativeVoidToEnum(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureOutgoingEnumToVoid(benchmarks_IEnumInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.EnumToVoid(benchmarks_Enum.A); }

        public void MeasureOutgoingVoidToEnum(benchmarks_IEnumInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.VoidToEnum(); }


        ///// IObjectBenchmarks /////


        public void ObjectToVoid(joint_IObject p) { }
        public joint_IObject VoidToObject() { return obj; }
        public joint_IObject VoidToNull() { return null; }
        public joint_IObject CreateObject() { return joint_IObject.makeComponent(module, new Object()); }


        public void MeasureNativeObjectToVoid(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureNativeVoidToObject(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureOutgoingObjectToVoid(benchmarks_IObjectInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.ObjectToVoid(obj); }

        public void MeasureOutgoingVoidToObject(benchmarks_IObjectInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.VoidToObject(); }


        public void MeasureNativeNullToVoid(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureNativeVoidToNull(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureOutgoingNullToVoid(benchmarks_IObjectInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.ObjectToVoid(null); }

        public void MeasureOutgoingVoidToNull(benchmarks_IObjectInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.VoidToNull(); }


        public void MeasureNativeCreateObject(long n)
        { throw new RuntimeException("Not implemented"); }


        ///// IStructBenchmarks /////


        public void NoStructToVoid(int a, int b, int c, int d) { }
        public void FlatStructToVoid(benchmarks_FlatStruct p) { }
        public benchmarks_FlatStruct VoidToFlatStruct() { return new benchmarks_FlatStruct(1, 2, 3, 4); }
        public void NestedStructToVoid(benchmarks_NestedStruct p) { }
        public benchmarks_NestedStruct VoidToNestedStruct() { return new benchmarks_NestedStruct(1, new benchmarks_NestedStruct2(2, new benchmarks_NestedStruct3(3, new benchmarks_NestedStruct4(4)))); }


        public void MeasureNativeNoStructToVoid(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureNativeFlatStructToVoid(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureNativeVoidToFlatStruct(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureNativeNestedStructToVoid(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureNativeVoidToNestedStruct(long n)
        { throw new RuntimeException("Not implemented"); }


        public void MeasureOutgoingNoStructToVoid(benchmarks_IStructInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.NoStructToVoid(1, 2, 3, 4); }

        public void MeasureOutgoingFlatStructToVoid(benchmarks_IStructInvokable invokable, long n)
        { benchmarks_FlatStruct s = new benchmarks_FlatStruct(1, 2, 3, 4); for (long i = 0; i < n; ++i) invokable.FlatStructToVoid(s); }

        public void MeasureOutgoingVoidToFlatStruct(benchmarks_IStructInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.VoidToFlatStruct(); }

        public void MeasureOutgoingNestedStructToVoid(benchmarks_IStructInvokable invokable, long n)
        { benchmarks_NestedStruct s = new benchmarks_NestedStruct(1, new benchmarks_NestedStruct2(2, new benchmarks_NestedStruct3(3, new benchmarks_NestedStruct4(4)))); for (long i = 0; i < n; ++i) invokable.NestedStructToVoid(s); }

        public void MeasureOutgoingVoidToNestedStruct(benchmarks_IStructInvokable invokable, long n)
        { for (long i = 0; i < n; ++i) invokable.VoidToNestedStruct(); }


        ///// ICastBenchmarks /////


        public benchmarks_ICastInterface1 GetCastComponent()
        { return benchmarks_ICastInterface1.makeComponent(module, new CastComponent()); }

        public void MeasureNativeCast(long n)
        { throw new RuntimeException("Not implemented"); }

        public void MeasureProxySideCast(benchmarks_ICastInterface1 obj, long n)
        {for (long i = 0; i < n; ++i) benchmarks_ICastInterface2.cast(obj); }


        ///// IExceptionBenchmarks /////


        public void Throw() { throw new RuntimeException("Requested exception"); }

        public void MeasureNativeThrow(long n)
        {
            for (long i = 0; i < n; ++i)
            {
                try { throw new RuntimeException(""); }
                catch (Exception ex) { }
            }
        }

        public void MeasureProxySideThrow(benchmarks_IThrower thrower, long n)
        {
            for (long i = 0; i < n; ++i)
            {
                try { thrower.Throw(); }
                catch (Exception ex) { }
            }
        }
    }


    public static JointObject GetBenchmarks(ModuleContext module)
    { return benchmarks_IBasicBenchmarks.makeComponent(module, new Component(module)).getJointObject(); }
}
