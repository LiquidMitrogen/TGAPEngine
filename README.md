# TGAPEngine
Copyright 2016 Szymon Biernacki

This game engine is under development currently. It may or may not be usable in this state (It works for me :)). This is an exercise in game programming techniques, if any technique or algorithm catches my eye this is the platform I'm using to check it out. This is why most of it is not well designed, but it serves it's purpose for now. 

Features:
- graphics renderer with a tree scene hierarchy. Easiest use case is just import of a scene from engine binary format and it's ready  to render.
- Blender export scripts for the binary format.
- Binary format .bams (Binary Animation/Mesh Scene) that can hold a whole scene containing meshes. It supports skeletal animations and can be exported from Blender.
- Some supplied GLSL shaders. Engine does not generate shaders on it's own yet.
- Simple particle system that uses instancing.
- GUI that uses billboards in window space. Not interactive yet, but can be GUI elements can be parented into a tree. Can draw fonts using freetype.
- No sound yet! sorry!
- There was a shadow mapping implemented at some point. 

TODO:
fix shadow mapping, add sound, automatic shader builder, interactive GUI buttons etc., particle system overhaul. And much more.

Hiere is a link to my simple portfolio containing two projects made with this engine http://tinyurl.com/jjnzv6v.
