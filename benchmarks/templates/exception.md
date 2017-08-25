# C++
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | ${exception.throwNative.joint(lang:cpp)[main]} |
| Component-side throw | ${exception.throwComponentSide.joint(lang:cpp)[main]} |
| Proxy-side throw     | ${exception.throwProxySide.joint(lang:cpp)[main]} |

# C
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | - |
| Component-side throw | ${exception.throwComponentSide.joint(lang:c)[main]} |
| Proxy-side throw     | ${exception.throwProxySide.joint(lang:c)[main]} |

# Python
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | ${exception.throwNative.joint(lang:python)[main]} |
| Component-side throw | ${exception.throwComponentSide.joint(lang:python)[main]} |
| Proxy-side throw     | ${exception.throwProxySide.joint(lang:python)[main]} |

# Java
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | ${exception.throwNative.joint(lang:java)[main]} |
| Component-side throw | ${exception.throwComponentSide.joint(lang:java)[main]} |
| Proxy-side throw     | ${exception.throwProxySide.joint(lang:java)[main]} |
