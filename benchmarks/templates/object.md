# C++
|                | void(object), ns | object(), ns | void(null), ns | null(), ns | create, ns |
| -------------- | ------------: | --------: |
| Native         | ${object.invokeNative_void_object.joint(lang:cpp)[main]} | ${object.invokeNative_object_void.joint(lang:cpp)[main]} | ${object.invokeNative_void_null.joint(lang:cpp)[main]} | ${object.invokeNative_null_void.joint(lang:cpp)[main]} | ${object.native_create_object.joint(lang:cpp)[main]} |
| Component side | ${object.invoke_void_object.joint(lang:cpp)[main]} | ${object.invoke_object_void.joint(lang:cpp)[main]} | ${object.invoke_void_null.joint(lang:cpp)[main]} | ${object.invoke_null_void.joint(lang:cpp)[main]} | ${object.create_object.joint(lang:cpp)[main]} |
| Proxy side     | ${object.invokeOutgoing_void_object.joint(lang:cpp)[main]} | ${object.invokeOutgoing_object_void.joint(lang:cpp)[main]} | ${object.invokeOutgoing_void_null.joint(lang:cpp)[main]} | ${object.invokeOutgoing_null_void.joint(lang:cpp)[main]} |

# C
|                | void(object), ns | object(), ns | void(null), ns | null(), ns | create, ns |
| -------------- | ------------: | --------: |
| Native         | - | - | - | - | - |
| Component side | ${object.invoke_void_object.joint(lang:c)[main]} | ${object.invoke_object_void.joint(lang:c)[main]} | ${object.invoke_void_null.joint(lang:c)[main]} | ${object.invoke_null_void.joint(lang:c)[main]} | ${object.create_object.joint(lang:c)[main]} |
| Proxy side     | ${object.invokeOutgoing_void_object.joint(lang:c)[main]} | ${object.invokeOutgoing_object_void.joint(lang:c)[main]} | ${object.invokeOutgoing_void_null.joint(lang:c)[main]} | ${object.invokeOutgoing_null_void.joint(lang:c)[main]} |

# Python
|                | void(object), ns | object(), ns | void(null), ns | null(), ns | create, ns |
| -------------- | ------------: | --------: |
| Native         | ${object.invokeNative_void_object.joint(lang:python)[main]} | ${object.invokeNative_object_void.joint(lang:python)[main]} | ${object.invokeNative_void_null.joint(lang:python)[main]} | ${object.invokeNative_null_void.joint(lang:python)[main]} | ${object.native_create_object.joint(lang:python)[main]} |
| Component side | ${object.invoke_void_object.joint(lang:python)[main]} | ${object.invoke_object_void.joint(lang:python)[main]} | ${object.invoke_void_null.joint(lang:python)[main]} | ${object.invoke_null_void.joint(lang:python)[main]} | ${object.create_object.joint(lang:python)[main]} |
| Proxy side     | ${object.invokeOutgoing_void_object.joint(lang:python)[main]} | ${object.invokeOutgoing_object_void.joint(lang:python)[main]} | ${object.invokeOutgoing_void_null.joint(lang:python)[main]} | ${object.invokeOutgoing_null_void.joint(lang:python)[main]} |

# Java
|                | void(object), ns | object(), ns | void(null), ns | null(), ns | create, ns |
| -------------- | ------------: | --------: |
| Native         | - | - | - | - | - |
| Component side | ${object.invoke_void_object.joint(lang:java)[main]} | ${object.invoke_object_void.joint(lang:java)[main]} | ${object.invoke_void_null.joint(lang:java)[main]} | ${object.invoke_null_void.joint(lang:java)[main]} | ${object.create_object.joint(lang:java)[main]} |
| Proxy side     | ${object.invokeOutgoing_void_object.joint(lang:java)[main]} | ${object.invokeOutgoing_object_void.joint(lang:java)[main]} | ${object.invokeOutgoing_void_null.joint(lang:java)[main]} | ${object.invokeOutgoing_null_void.joint(lang:java)[main]} |
