# C++
|               | void(i32, i32, i32, i32), ns | void(FlatStruct), ns | FlatStruct(void), ns | void(NestedStruct), ns | NestedStruct(void), ns |
| ------------- | ---------------------------: | -------------------: | -------------------: | ---------------------: | ---------------------: |
| Native call   | ${struct.invokeNative_void_noStruct.joint(lang:cpp)[main]} | ${struct.invokeNative_void_flatStruct.joint(lang:cpp)[main]} | ${struct.invokeNative_flatStruct_void.joint(lang:cpp)[main]} | ${struct.invokeNative_void_nestedStruct.joint(lang:cpp)[main]} | ${struct.invokeNative_nestedStruct_void.joint(lang:cpp)[main]} |
| Incoming call | ${struct.invoke_void_noStruct.joint(lang:cpp)[main]} | ${struct.invoke_void_flatStruct.joint(lang:cpp)[main]} | ${struct.invoke_flatStruct_void.joint(lang:cpp)[main]} | ${struct.invoke_void_nestedStruct.joint(lang:cpp)[main]} | ${struct.invoke_nestedStruct_void.joint(lang:cpp)[main]} |
| Outgoing call | ${struct.invokeOutgoing_void_noStruct.joint(lang:cpp)[main]} | ${struct.invokeOutgoing_void_flatStruct.joint(lang:cpp)[main]} | ${struct.invokeOutgoing_flatStruct_void.joint(lang:cpp)[main]} | ${struct.invokeOutgoing_void_nestedStruct.joint(lang:cpp)[main]} | ${struct.invokeOutgoing_nestedStruct_void.joint(lang:cpp)[main]} |

# C
|               | void(i32, i32, i32, i32), ns | void(FlatStruct), ns | FlatStruct(void), ns | void(NestedStruct), ns | NestedStruct(void), ns |
| ------------- | ---------------------------: | -------------------: | -------------------: | ---------------------: | ---------------------: |
| Native call   | ${struct.invokeNative_void_noStruct.joint(lang:c)[main]} | ${struct.invokeNative_void_flatStruct.joint(lang:c)[main]} | ${struct.invokeNative_flatStruct_void.joint(lang:c)[main]} | ${struct.invokeNative_void_nestedStruct.joint(lang:c)[main]} | ${struct.invokeNative_nestedStruct_void.joint(lang:c)[main]} |
| Incoming call | ${struct.invoke_void_noStruct.joint(lang:c)[main]} | ${struct.invoke_void_flatStruct.joint(lang:c)[main]} | ${struct.invoke_flatStruct_void.joint(lang:c)[main]} | ${struct.invoke_void_nestedStruct.joint(lang:c)[main]} | ${struct.invoke_nestedStruct_void.joint(lang:c)[main]} |
| Outgoing call | ${struct.invokeOutgoing_void_noStruct.joint(lang:c)[main]} | ${struct.invokeOutgoing_void_flatStruct.joint(lang:c)[main]} | ${struct.invokeOutgoing_flatStruct_void.joint(lang:c)[main]} | ${struct.invokeOutgoing_void_nestedStruct.joint(lang:c)[main]} | ${struct.invokeOutgoing_nestedStruct_void.joint(lang:c)[main]} |

# Java
|               | void(i32, i32, i32, i32), ns | void(FlatStruct), ns | FlatStruct(void), ns | void(NestedStruct), ns | NestedStruct(void), ns |
| ------------- | ---------------------------: | -------------------: | -------------------: | ---------------------: | ---------------------: |
| Native call   | - | - | - | - | - |
| Incoming call | ${struct.invoke_void_noStruct.joint(lang:java)[main]} | ${struct.invoke_void_flatStruct.joint(lang:java)[main]} | ${struct.invoke_flatStruct_void.joint(lang:java)[main]} | ${struct.invoke_void_nestedStruct.joint(lang:java)[main]} | ${struct.invoke_nestedStruct_void.joint(lang:java)[main]} |
| Outgoing call | ${struct.invokeOutgoing_void_noStruct.joint(lang:java)[main]} | ${struct.invokeOutgoing_void_flatStruct.joint(lang:java)[main]} | ${struct.invokeOutgoing_flatStruct_void.joint(lang:java)[main]} | ${struct.invokeOutgoing_void_nestedStruct.joint(lang:java)[main]} | ${struct.invokeOutgoing_nestedStruct_void.joint(lang:java)[main]} |

# Python
|               | void(i32, i32, i32, i32), ns | void(FlatStruct), ns | FlatStruct(void), ns | void(NestedStruct), ns | NestedStruct(void), ns |
| ------------- | ---------------------------: | -------------------: | -------------------: | ---------------------: | ---------------------: |
| Native call   | ${struct.invokeNative_void_noStruct.joint(lang:python)[main]} | ${struct.invokeNative_void_flatStruct.joint(lang:python)[main]} | ${struct.invokeNative_flatStruct_void.joint(lang:python)[main]} | ${struct.invokeNative_void_nestedStruct.joint(lang:python)[main]} | ${struct.invokeNative_nestedStruct_void.joint(lang:python)[main]} |
| Incoming call | ${struct.invoke_void_noStruct.joint(lang:python)[main]} | ${struct.invoke_void_flatStruct.joint(lang:python)[main]} | ${struct.invoke_flatStruct_void.joint(lang:python)[main]} | ${struct.invoke_void_nestedStruct.joint(lang:python)[main]} | ${struct.invoke_nestedStruct_void.joint(lang:python)[main]} |
| Outgoing call | ${struct.invokeOutgoing_void_noStruct.joint(lang:python)[main]} | ${struct.invokeOutgoing_void_flatStruct.joint(lang:python)[main]} | ${struct.invokeOutgoing_flatStruct_void.joint(lang:python)[main]} | ${struct.invokeOutgoing_void_nestedStruct.joint(lang:python)[main]} | ${struct.invokeOutgoing_nestedStruct_void.joint(lang:python)[main]} |
