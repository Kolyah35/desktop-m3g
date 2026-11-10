# TODO

## M3GReader
- [x] Prevent major memory leaks
- [x] Replace `abort()` with `assert()`
- [ ] Strict check before every section and object indexes read
- [ ] Define limits for unpacked size, object and vertex count and allocated memory
- [ ] Verify `uncompress()` result
- [ ] Make error messages more informative
- [x] Sort out object ownership

What do I mean under "informative messages":
```
M3G Parse Error
Section: 3
Object: 42 (SkinnedMesh)
Offset: 0x00012AC0
Reason: Reference index 87 is out of range
```

## Scene graph
- [ ] Review `Object3D::duplicate_xxx()` and `Group::duplicate_xxx()`
- [ ] Check for tree cycle in `Group` class
- [ ] Sort out OpenGL resources lifetime (such as `Texture2D`)
- [ ] Optimise `Object3D::find`
- [ ] Improve transformation and picking

## InMemoryReader
- [ ] rewrite and make memory safe

## Graphics3D
- [ ] Finish immediate-mode implementation (`render(Node*, Transform*)`, `bindTarget()`, `clear()`, `addLight()`, etc..)

## SkinnedMesh
- [ ] Optimise render
- [ ] SIMD optimisations?

## MorphingMesh
- [ ] Optimise animation