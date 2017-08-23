# C++
| Cast                | Time, ns |
| ------------------- | -------: |
| Native cast         | ${cast.castNative.joint(module:build/bin/cpp/Benchmarks/Benchmarks.jm)[main]} |
| Component-side cast | ${cast.castComponentSide.joint(module:build/bin/cpp/Benchmarks/Benchmarks.jm)[main]} |
| Proxy-side cast     | ${cast.castProxySide.joint(module:build/bin/cpp/Benchmarks/Benchmarks.jm)[main]} |

# C
| Cast                | Time, ns |
| ------------------- | -------: |
| Native cast         | - |
| Component-side cast | ${cast.castComponentSide.joint(module:build/bin/c/Benchmarks/Benchmarks.jm)[main]} |
| Proxy-side cast     | ${cast.castProxySide.joint(module:build/bin/c/Benchmarks/Benchmarks.jm)[main]} |

# Python
| Cast                | Time, ns |
| ------------------- | -------: |
| Native cast         | - |
| Component-side cast | ${cast.castComponentSide.joint(module:build/bin/python/Benchmarks/Benchmarks.jm)[main]} |
| Proxy-side cast     | ${cast.castProxySide.joint(module:build/bin/python/Benchmarks/Benchmarks.jm)[main]} |

# Java
| Cast                | Time, ns |
| ------------------- | -------: |
| Native cast         | - |
| Component-side cast | ${cast.castComponentSide.joint(module:build/bin/java/Benchmarks/Benchmarks.jm)[main]} |
| Proxy-side cast     | ${cast.castProxySide.joint(module:build/bin/java/Benchmarks/Benchmarks.jm)[main]} |
