# C++
|               | void(), ns | void(i32), ns |
| ------------- | ---------: | ------------: |
| Native call   | ${basic.invokeNative.joint(binding:cpp,module:benchmarks-cpp)[void_noParams]} | ${basic.invokeNative.joint(binding:cpp,module:benchmarks-cpp)[void_i32]} |
| Incoming call | ${basic.invoke.joint(binding:cpp,module:benchmarks-cpp)[void_noParams]} | ${basic.invoke.joint(binding:cpp,module:benchmarks-cpp)[void_i32]} |
| Outgoing call | ${basic.invokeOutgoing.joint(binding:cpp,module:benchmarks-cpp)[void_noParams]} | ${basic.invokeOutgoing.joint(binding:cpp,module:benchmarks-cpp)[void_i32]} |

# C
|               | void(), ns | void(i32), ns |
| ------------- | ---------: | ------------: |
| Native call   | ${basic.invokeNative.joint(binding:cpp,module:benchmarks-c)[void_noParams]} | ${basic.invokeNative.joint(binding:cpp,module:benchmarks-c)[void_i32]} |
| Incoming call | ${basic.invoke.joint(binding:cpp,module:benchmarks-c)[void_noParams]} | ${basic.invoke.joint(binding:cpp,module:benchmarks-c)[void_i32]} |
| Outgoing call | ${basic.invokeOutgoing.joint(binding:cpp,module:benchmarks-c)[void_noParams]} | ${basic.invokeOutgoing.joint(binding:cpp,module:benchmarks-c)[void_i32]} |

# Python
## joint
|               | void(), ns | void(i32), ns |
| ------------- | ---------: | ------------: |
| Native call   | ${basic.invokeNative.joint(binding:python,module:benchmarks)[void_noParams]} | ${basic.invokeNative.joint(binding:python,module:benchmarks)[void_i32]} |
| Incoming call | ${basic.invoke.joint(binding:python,module:benchmarks)[void_noParams]} | ${basic.invoke.joint(binding:python,module:benchmarks)[void_i32]} |
| Outgoing call | ${basic.invokeOutgoing.joint(binding:python,module:benchmarks)[void_noParams]} | ${basic.invokeOutgoing.joint(binding:python,module:benchmarks)[void_i32]} |

## swig
|               | void(), ns | void(i32), ns |
| ------------- | ---------: | ------------: |
| Native call   | ${basic.invokeNative.swig(binding:python,module:swig_benchmarks)[void_noParams]} | ${basic.invokeNative.swig(binding:python,module:swig_benchmarks)[void_i32]} |
| Incoming call | ${basic.invoke.swig(binding:python,module:swig_benchmarks)[void_noParams]} | ${basic.invoke.swig(binding:python,module:swig_benchmarks)[void_i32]} |
| Outgoing call | ${basic.invokeOutgoing.swig(binding:python,module:swig_benchmarks)[void_noParams]} | ${basic.invokeOutgoing.swig(binding:python,module:swig_benchmarks)[void_i32]} |

# Java
## joint
|               | void(), ns | void(i32), ns |
| ------------- | ---------: | ------------: |
| Native call   | ${basic.invokeNative.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_noParams]} | ${basic.invokeNative.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_i32]} |
| Incoming call | ${basic.invoke.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_noParams]} | ${basic.invoke.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_i32]} |
| Outgoing call | ${basic.invokeOutgoing.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_noParams]} | ${basic.invokeOutgoing.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[void_i32]} |

## swig
|               | void(), ns | void(i32), ns |
| ------------- | ---------: | ------------: |
| Native call   | ${basic.invokeNative.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_noParams]} | ${basic.invokeNative.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_i32]} |
| Incoming call | ${basic.invoke.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_noParams]} | ${basic.invoke.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_i32]} |
| Outgoing call | ${basic.invokeOutgoing.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_noParams]} | ${basic.invokeOutgoing.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[void_i32]} |
