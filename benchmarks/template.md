# C++
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParamsNative.joint(binding:cpp,module:benchmarks-cpp)[time]} |
| Incoming call | ${basic.invokeNoParams.joint(binding:cpp,module:benchmarks-cpp)[time]} |
| Outgoing call | ${basic.invokeNoParamsOutgoing.joint(binding:cpp,module:benchmarks-cpp)[time]} |

# C
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParamsNative.joint(binding:cpp,module:benchmarks-c)[time]} |
| Incoming call | ${basic.invokeNoParams.joint(binding:cpp,module:benchmarks-c)[time]} |
| Outgoing call | ${basic.invokeNoParamsOutgoing.joint(binding:cpp,module:benchmarks-c)[time]} |

# Python
## joint
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParamsNative.joint(binding:python,module:benchmarks)[time]} |
| Incoming call | ${basic.invokeNoParams.joint(binding:python,module:benchmarks)[time]} |
| Outgoing call | ${basic.invokeNoParamsOutgoing.joint(binding:python,module:benchmarks)[time]} |

## swig
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParamsNative.swig(binding:python,module:swig_benchmarks)[time]} |
| Incoming call | ${basic.invokeNoParams.swig(binding:python,module:swig_benchmarks)[time]} |
| Outgoing call | ${basic.invokeNoParamsOutgoing.swig(binding:python,module:swig_benchmarks)[time]} |

# Java
## joint
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParamsNative.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[time]} |
| Incoming call | ${basic.invokeNoParams.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[time]} |
| Outgoing call | ${basic.invokeNoParamsOutgoing.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[time]} |

## swig
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParamsNative.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[time]} |
| Incoming call | ${basic.invokeNoParams.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[time]} |
| Outgoing call | ${basic.invokeNoParamsOutgoing.swig(binding:java,module:`pwd`/build/bin/benchmarks-java-swig.jar:SwigBenchmarks)[time]} |
