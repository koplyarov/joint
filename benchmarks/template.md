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
|               | time, ns |
| ------------- | -------: |
| Native call   | ${basic.invokeNoParams.joint(binding:python,module:benchmarks)[invokeNoParams_native]} |
| Incoming call | ${basic.invokeNoParams.joint(binding:python,module:benchmarks)[invokeNoParams]} |
| Outgoing call | ${basic.invokeNoParams.joint(binding:python,module:benchmarks)[invokeNoParams_outgoing]} |
