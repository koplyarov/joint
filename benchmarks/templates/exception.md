# C++
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | ${exception.native_throw.joint(lang:cpp)[main]} |
| Component-side throw | ${exception.component_throw.joint(lang:cpp)[main]} |
| Proxy-side throw     | ${exception.proxy_throw.joint(lang:cpp)[main]} |

# C++ (no exceptions)
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | - |
| Component-side throw | ${exception.component_throw.joint(lang:cpp_ne)[main]} |
| Proxy-side throw     | ${exception.proxy_throw.joint(lang:cpp_ne)[main]} |

# C
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | - |
| Component-side throw | ${exception.component_throw.joint(lang:c)[main]} |
| Proxy-side throw     | ${exception.proxy_throw.joint(lang:c)[main]} |

# Python
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | ${exception.native_throw.joint(lang:python)[main]} |
| Component-side throw | ${exception.component_throw.joint(lang:python)[main]} |
| Proxy-side throw     | ${exception.proxy_throw.joint(lang:python)[main]} |

# Java
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | ${exception.native_throw.joint(lang:java)[main]} |
| Component-side throw | ${exception.component_throw.joint(lang:java)[main]} |
| Proxy-side throw     | ${exception.proxy_throw.joint(lang:java)[main]} |
