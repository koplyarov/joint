# C++
|               | void(object), ns | object(), ns |
| ------------- | ------------: | --------: |
| Native call   | ${object.invokeNative_void_object.joint(lang:cpp)[main]} | ${object.invokeNative_object_void.joint(lang:cpp)[main]} |
| Incoming call | ${object.invoke_void_object.joint(lang:cpp)[main]} | ${object.invoke_object_void.joint(lang:cpp)[main]} |
| Outgoing call | ${object.invokeOutgoing_void_object.joint(lang:cpp)[main]} | ${object.invokeOutgoing_object_void.joint(lang:cpp)[main]} |

# C
|               | void(object), ns | object(), ns |
| ------------- | ------------: | --------: |
| Native call   | - | - |
| Incoming call | ${object.invoke_void_object.joint(lang:c)[main]} | ${object.invoke_object_void.joint(lang:c)[main]} |
| Outgoing call | ${object.invokeOutgoing_void_object.joint(lang:c)[main]} | ${object.invokeOutgoing_object_void.joint(lang:c)[main]} |

# Python
|               | void(object), ns | object(), ns |
| ------------- | ------------: | --------: |
| Native call   | ${object.invokeNative_void_object.joint(lang:python)[main]} | ${object.invokeNative_object_void.joint(lang:python)[main]} |
| Incoming call | ${object.invoke_void_object.joint(lang:python)[main]} | ${object.invoke_object_void.joint(lang:python)[main]} |
| Outgoing call | ${object.invokeOutgoing_void_object.joint(lang:python)[main]} | ${object.invokeOutgoing_object_void.joint(lang:python)[main]} |

# Java
|               | void(object), ns | object(), ns |
| ------------- | ------------: | --------: |
| Native call   | - | - |
| Incoming call | ${object.invoke_void_object.joint(lang:java)[main]} | ${object.invoke_object_void.joint(lang:java)[main]} |
| Outgoing call | ${object.invokeOutgoing_void_object.joint(lang:java)[main]} | ${object.invokeOutgoing_object_void.joint(lang:java)[main]} |

