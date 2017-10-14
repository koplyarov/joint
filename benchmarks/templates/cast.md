# C++
| Cast                | Time, ns |
| ------------------- | -------: |
| Native cast         | ${cast.castNative.joint(lang:cpp)[main]} |
| Component-side cast | ${cast.castComponentSide.joint(lang:cpp)[main]} |
| Proxy-side cast     | ${cast.castProxySide.joint(lang:cpp)[main]} |

# C++ (no exceptions)
| Cast                | Time, ns |
| ------------------- | -------: |
| Native cast         | - |
| Component-side cast | ${cast.castComponentSide.joint(lang:cpp_ne)[main]} |
| Proxy-side cast     | ${cast.castProxySide.joint(lang:cpp_ne)[main]} |

# C
| Cast                | Time, ns |
| ------------------- | -------: |
| Native cast         | - |
| Component-side cast | ${cast.castComponentSide.joint(lang:c)[main]} |
| Proxy-side cast     | ${cast.castProxySide.joint(lang:c)[main]} |

# Python
| Cast                | Time, ns |
| ------------------- | -------: |
| Native cast         | - |
| Component-side cast | ${cast.castComponentSide.joint(lang:python)[main]} |
| Proxy-side cast     | ${cast.castProxySide.joint(lang:python)[main]} |

# Java
| Cast                | Time, ns |
| ------------------- | -------: |
| Native cast         | - |
| Component-side cast | ${cast.castComponentSide.joint(lang:java)[main]} |
| Proxy-side cast     | ${cast.castProxySide.joint(lang:java)[main]} |
