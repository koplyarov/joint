# C++
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParams.joint(binding:cpp,module:benchmarks-cpp)[invokeNoParams_native]} |
| Incoming call | ${basic.invokeNoParams.joint(binding:cpp,module:benchmarks-cpp)[invokeNoParams]} |
| Outgoing call | ${basic.invokeNoParams.joint(binding:cpp,module:benchmarks-cpp)[invokeNoParams_outgoing]} |

# C
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParams.joint(binding:cpp,module:benchmarks-c)[invokeNoParams_native]} |
| Incoming call | ${basic.invokeNoParams.joint(binding:cpp,module:benchmarks-c)[invokeNoParams]} |
| Outgoing call | ${basic.invokeNoParams.joint(binding:cpp,module:benchmarks-c)[invokeNoParams_outgoing]} |

# Python
## joint
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParams.joint(binding:python,module:benchmarks)[invokeNoParams_native]} |
| Incoming call | ${basic.invokeNoParams.joint(binding:python,module:benchmarks)[invokeNoParams]} |
| Outgoing call | ${basic.invokeNoParams.joint(binding:python,module:benchmarks)[invokeNoParams_outgoing]} |

## swig
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParams.swig(binding:python,module:swig_benchmarks)[invokeNoParams_native]} |
| Incoming call | ${basic.invokeNoParams.swig(binding:python,module:swig_benchmarks)[invokeNoParams]} |
| Outgoing call | ${basic.invokeNoParams.swig(binding:python,module:swig_benchmarks)[invokeNoParams_outgoing]} |

# Java
## joint
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParams.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[invokeNoParams_native]} |
| Incoming call | ${basic.invokeNoParams.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[invokeNoParams]} |
| Outgoing call | ${basic.invokeNoParams.joint(binding:java,module:file://`pwd`/build/bin/benchmarks-java.jar:benchmarks.Benchmarks)[invokeNoParams_outgoing]} |

