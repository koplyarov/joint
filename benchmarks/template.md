# C++
|               | void(), ns | void(i32), ns | i32(), ns |
| ------------- | ---------: | ------------: | --------: |
| Native call   | ${basic.invokeNative.joint(binding:cpp,module:benchmarks-cpp)[void_void]} | ${basic.invokeNative.joint(binding:cpp,module:benchmarks-cpp)[void_i32]} | ${basic.invokeNative.joint(binding:cpp,module:benchmarks-cpp)[i32_void]} |
| Incoming call | ${basic.invoke.joint(binding:cpp,module:benchmarks-cpp)[void_void]} | ${basic.invoke.joint(binding:cpp,module:benchmarks-cpp)[void_i32]} | ${basic.invoke.joint(binding:cpp,module:benchmarks-cpp)[i32_void]} |
| Outgoing call | ${basic.invokeOutgoing.joint(binding:cpp,module:benchmarks-cpp)[void_void]} | ${basic.invokeOutgoing.joint(binding:cpp,module:benchmarks-cpp)[void_i32]} | ${basic.invokeOutgoing.joint(binding:cpp,module:benchmarks-cpp)[i32_void]} |

# C
|               | void(), ns | void(i32), ns | i32(), ns |
| ------------- | ---------: | ------------: | --------: |
| Native call   | ${basic.invokeNative.joint(binding:cpp,module:benchmarks-c)[void_void]} | ${basic.invokeNative.joint(binding:cpp,module:benchmarks-c)[void_i32]} | ${basic.invokeNative.joint(binding:cpp,module:benchmarks-c)[i32_void]} |
| Incoming call | ${basic.invoke.joint(binding:cpp,module:benchmarks-c)[void_void]} | ${basic.invoke.joint(binding:cpp,module:benchmarks-c)[void_i32]} | ${basic.invoke.joint(binding:cpp,module:benchmarks-c)[i32_void]} |
| Outgoing call | ${basic.invokeOutgoing.joint(binding:cpp,module:benchmarks-c)[void_void]} | ${basic.invokeOutgoing.joint(binding:cpp,module:benchmarks-c)[void_i32]} | ${basic.invokeOutgoing.joint(binding:cpp,module:benchmarks-c)[i32_void]} |

# Python
## joint
|               | void(), ns | void(i32), ns | i32(), ns |
| ------------- | ---------: | ------------: | --------: |
| Native call   | ${basic.invokeNative.joint(binding:python,module:benchmarks)[void_void]} | ${basic.invokeNative.joint(binding:python,module:benchmarks)[void_i32]} | ${basic.invokeNative.joint(binding:python,module:benchmarks)[i32_void]} |
| Incoming call | ${basic.invoke.joint(binding:python,module:benchmarks)[void_void]} | ${basic.invoke.joint(binding:python,module:benchmarks)[void_i32]} | ${basic.invoke.joint(binding:python,module:benchmarks)[i32_void]} |
| Outgoing call | ${basic.invokeOutgoing.joint(binding:python,module:benchmarks)[void_void]} | ${basic.invokeOutgoing.joint(binding:python,module:benchmarks)[void_i32]} | ${basic.invokeOutgoing.joint(binding:python,module:benchmarks)[i32_void]} |

## swig
|               | void(), ns | void(i32), ns | i32(), ns |
| ------------- | ---------: | ------------: | --------: |
| Native call   | ${basic.invokeNative.swig(binding:python,module:swig_benchmarks)[void_void]} | ${basic.invokeNative.swig(binding:python,module:swig_benchmarks)[void_i32]} | ${basic.invokeNative.swig(binding:python,module:swig_benchmarks)[i32_void]} |
| Incoming call | ${basic.invoke.swig(binding:python,module:swig_benchmarks)[void_void]} | ${basic.invoke.swig(binding:python,module:swig_benchmarks)[void_i32]} | ${basic.invoke.swig(binding:python,module:swig_benchmarks)[i32_void]} |
| Outgoing call | ${basic.invokeOutgoing.swig(binding:python,module:swig_benchmarks)[void_void]} | ${basic.invokeOutgoing.swig(binding:python,module:swig_benchmarks)[void_i32]} | ${basic.invokeOutgoing.swig(binding:python,module:swig_benchmarks)[i32_void]} |

# Java
## joint
|               | void(), ns | void(i32), ns | i32(), ns |
| ------------- | ---------: | ------------: | --------: |
| Native call   | ${basic.invokeNative.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_void]} | ${basic.invokeNative.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_i32]} | ${basic.invokeNative.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[i32_void]} |
| Incoming call | ${basic.invoke.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_void]} | ${basic.invoke.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_i32]} | ${basic.invoke.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[i32_void]} |
| Outgoing call | ${basic.invokeOutgoing.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_void]} | ${basic.invokeOutgoing.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_i32]} | ${basic.invokeOutgoing.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[i32_void]} |

## swig
|               | void(), ns | void(i32), ns | i32(), ns |
| ------------- | ---------: | ------------: | --------: |
| Native call   | ${basic.invokeNative.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_void]} | ${basic.invokeNative.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_i32]} | ${basic.invokeNative.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[i32_void]} |
| Incoming call | ${basic.invoke.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_void]} | ${basic.invoke.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_i32]} | ${basic.invoke.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[i32_void]} |
| Outgoing call | ${basic.invokeOutgoing.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_void]} | ${basic.invokeOutgoing.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_i32]} | ${basic.invokeOutgoing.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[i32_void]} |
