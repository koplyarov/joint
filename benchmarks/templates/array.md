# C
|               | array.Get(), ns | array.Set(), ns |
| ------------- | ------------: | --------: |
| Native call | - | - |
| Proxy-side call | ${array.proxy_array_get_i64.joint(lang:c)[main]} | ${array.proxy_array_set_i64.joint(lang:c)[main]} |

# C++
|               | array.Get(), ns | array.Set(), ns |
| ------------- | ------------: | --------: |
| Native call | - | - |
| Proxy-side call | ${array.proxy_array_get_i64.joint(lang:cpp)[main]} | ${array.proxy_array_set_i64.joint(lang:cpp)[main]} |

# Python
|               | array.Get(), ns | array.Set(), ns |
| ------------- | ------------: | --------: |
| Native call | ${array.native_array_get_i64.joint(lang:python)[main]} | ${array.native_array_set_i64.joint(lang:python)[main]} |
| Proxy-side call | ${array.proxy_array_get_i64.joint(lang:python)[main]} | ${array.proxy_array_set_i64.joint(lang:python)[main]} |

# Java
|               | array.Get(), ns | array.Set(), ns |
| ------------- | ------------: | --------: |
| Proxy-side call | ${array.proxy_array_get_i64.joint(lang:java)[main]} | ${array.proxy_array_set_i64.joint(lang:java)[main]} |
