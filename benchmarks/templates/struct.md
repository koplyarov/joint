# C++
|               | void(i32, i32, i32, i32), ns | void(FlatStruct), ns | FlatStruct(void), ns | void(NestedStruct), ns | NestedStruct(void), ns |
| ------------- | ---------------------------: | -------------------: | -------------------: | ---------------------: | ---------------------: |
| Native call   | ${struct.native_void_noStruct.joint(lang:cpp)[main]} | ${struct.native_void_flatStruct.joint(lang:cpp)[main]} | ${struct.native_flatStruct_void.joint(lang:cpp)[main]} | ${struct.native_void_nestedStruct.joint(lang:cpp)[main]} | ${struct.native_nestedStruct_void.joint(lang:cpp)[main]} |
| Incoming call | ${struct.component_void_noStruct.joint(lang:cpp)[main]} | ${struct.component_void_flatStruct.joint(lang:cpp)[main]} | ${struct.component_flatStruct_void.joint(lang:cpp)[main]} | ${struct.component_void_nestedStruct.joint(lang:cpp)[main]} | ${struct.component_nestedStruct_void.joint(lang:cpp)[main]} |
| Outgoing call | ${struct.proxy_void_noStruct.joint(lang:cpp)[main]} | ${struct.proxy_void_flatStruct.joint(lang:cpp)[main]} | ${struct.proxy_flatStruct_void.joint(lang:cpp)[main]} | ${struct.proxy_void_nestedStruct.joint(lang:cpp)[main]} | ${struct.proxy_nestedStruct_void.joint(lang:cpp)[main]} |

# C++ (no exceptions)
|               | void(i32, i32, i32, i32), ns | void(FlatStruct), ns | FlatStruct(void), ns | void(NestedStruct), ns | NestedStruct(void), ns |
| ------------- | ---------------------------: | -------------------: | -------------------: | ---------------------: | ---------------------: |
| Native call   | ${struct.native_void_noStruct.joint(lang:cpp_ne)[main]} | ${struct.native_void_flatStruct.joint(lang:cpp_ne)[main]} | ${struct.native_flatStruct_void.joint(lang:cpp_ne)[main]} | ${struct.native_void_nestedStruct.joint(lang:cpp_ne)[main]} | ${struct.native_nestedStruct_void.joint(lang:cpp_ne)[main]} |
| Incoming call | ${struct.component_void_noStruct.joint(lang:cpp_ne)[main]} | ${struct.component_void_flatStruct.joint(lang:cpp_ne)[main]} | ${struct.component_flatStruct_void.joint(lang:cpp_ne)[main]} | ${struct.component_void_nestedStruct.joint(lang:cpp_ne)[main]} | ${struct.component_nestedStruct_void.joint(lang:cpp_ne)[main]} |
| Outgoing call | ${struct.proxy_void_noStruct.joint(lang:cpp_ne)[main]} | ${struct.proxy_void_flatStruct.joint(lang:cpp_ne)[main]} | ${struct.proxy_flatStruct_void.joint(lang:cpp_ne)[main]} | ${struct.proxy_void_nestedStruct.joint(lang:cpp_ne)[main]} | ${struct.proxy_nestedStruct_void.joint(lang:cpp_ne)[main]} |

# C
|               | void(i32, i32, i32, i32), ns | void(FlatStruct), ns | FlatStruct(void), ns | void(NestedStruct), ns | NestedStruct(void), ns |
| ------------- | ---------------------------: | -------------------: | -------------------: | ---------------------: | ---------------------: |
| Native call   | ${struct.native_void_noStruct.joint(lang:c)[main]} | ${struct.native_void_flatStruct.joint(lang:c)[main]} | ${struct.native_flatStruct_void.joint(lang:c)[main]} | ${struct.native_void_nestedStruct.joint(lang:c)[main]} | ${struct.native_nestedStruct_void.joint(lang:c)[main]} |
| Incoming call | ${struct.component_void_noStruct.joint(lang:c)[main]} | ${struct.component_void_flatStruct.joint(lang:c)[main]} | ${struct.component_flatStruct_void.joint(lang:c)[main]} | ${struct.component_void_nestedStruct.joint(lang:c)[main]} | ${struct.component_nestedStruct_void.joint(lang:c)[main]} |
| Outgoing call | ${struct.proxy_void_noStruct.joint(lang:c)[main]} | ${struct.proxy_void_flatStruct.joint(lang:c)[main]} | ${struct.proxy_flatStruct_void.joint(lang:c)[main]} | ${struct.proxy_void_nestedStruct.joint(lang:c)[main]} | ${struct.proxy_nestedStruct_void.joint(lang:c)[main]} |

# Java
|               | void(i32, i32, i32, i32), ns | void(FlatStruct), ns | FlatStruct(void), ns | void(NestedStruct), ns | NestedStruct(void), ns |
| ------------- | ---------------------------: | -------------------: | -------------------: | ---------------------: | ---------------------: |
| Native call   | - | - | - | - | - |
| Incoming call | ${struct.component_void_noStruct.joint(lang:java)[main]} | ${struct.component_void_flatStruct.joint(lang:java)[main]} | ${struct.component_flatStruct_void.joint(lang:java)[main]} | ${struct.component_void_nestedStruct.joint(lang:java)[main]} | ${struct.component_nestedStruct_void.joint(lang:java)[main]} |
| Outgoing call | ${struct.proxy_void_noStruct.joint(lang:java)[main]} | ${struct.proxy_void_flatStruct.joint(lang:java)[main]} | ${struct.proxy_flatStruct_void.joint(lang:java)[main]} | ${struct.proxy_void_nestedStruct.joint(lang:java)[main]} | ${struct.proxy_nestedStruct_void.joint(lang:java)[main]} |

# Python
|               | void(i32, i32, i32, i32), ns | void(FlatStruct), ns | FlatStruct(void), ns | void(NestedStruct), ns | NestedStruct(void), ns |
| ------------- | ---------------------------: | -------------------: | -------------------: | ---------------------: | ---------------------: |
| Native call   | ${struct.native_void_noStruct.joint(lang:python)[main]} | ${struct.native_void_flatStruct.joint(lang:python)[main]} | ${struct.native_flatStruct_void.joint(lang:python)[main]} | ${struct.native_void_nestedStruct.joint(lang:python)[main]} | ${struct.native_nestedStruct_void.joint(lang:python)[main]} |
| Incoming call | ${struct.component_void_noStruct.joint(lang:python)[main]} | ${struct.component_void_flatStruct.joint(lang:python)[main]} | ${struct.component_flatStruct_void.joint(lang:python)[main]} | ${struct.component_void_nestedStruct.joint(lang:python)[main]} | ${struct.component_nestedStruct_void.joint(lang:python)[main]} |
| Outgoing call | ${struct.proxy_void_noStruct.joint(lang:python)[main]} | ${struct.proxy_void_flatStruct.joint(lang:python)[main]} | ${struct.proxy_flatStruct_void.joint(lang:python)[main]} | ${struct.proxy_void_nestedStruct.joint(lang:python)[main]} | ${struct.proxy_nestedStruct_void.joint(lang:python)[main]} |
