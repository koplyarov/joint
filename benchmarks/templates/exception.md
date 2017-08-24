# C++
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | ${exception.throwNative.joint(module:build/bin/cpp/Benchmarks/Benchmarks.jm)[main]} |
| Component-side throw | ${exception.throwComponentSide.joint(module:build/bin/cpp/Benchmarks/Benchmarks.jm)[main]} |
| Proxy-side throw     | ${exception.throwProxySide.joint(module:build/bin/cpp/Benchmarks/Benchmarks.jm)[main]} |

# C
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | - |
| Component-side throw | ${exception.throwComponentSide.joint(module:build/bin/c/Benchmarks/Benchmarks.jm)[main]} |
| Proxy-side throw     | ${exception.throwProxySide.joint(module:build/bin/c/Benchmarks/Benchmarks.jm)[main]} |

# Python
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | ${exception.throwNative.joint(module:build/bin/python/Benchmarks/Benchmarks.jm)[main]} |
| Component-side throw | ${exception.throwComponentSide.joint(module:build/bin/python/Benchmarks/Benchmarks.jm)[main]} |
| Proxy-side throw     | ${exception.throwProxySide.joint(module:build/bin/python/Benchmarks/Benchmarks.jm)[main]} |

# Java
| Throw                | Time, ns |
| -------------------- | -------: |
| Native throw         | ${exception.throwNative.joint(module:build/bin/java/Benchmarks/Benchmarks.jm)[main]} |
| Component-side throw | ${exception.throwComponentSide.joint(module:build/bin/java/Benchmarks/Benchmarks.jm)[main]} |
| Proxy-side throw     | ${exception.throwProxySide.joint(module:build/bin/java/Benchmarks/Benchmarks.jm)[main]} |
