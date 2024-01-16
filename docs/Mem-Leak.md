### Valgrind MemCheck Tool
`valgrind --leak-check=yes ./cppkart`

- Note: Must Temporarily disable ASan for Valgrind to not conflict !

```
** Possible Leaks **
 
 ==14012==    by 0x22F4F7: Shader::Shader(char const*, char const*) (in /home/jasonmzx/Github/cppkart/build/cppkart)
==14012==    by 0x211D03: XJZoomEngine::Run() (in /home/jasonmzx/Github/cppkart/build/cppkart)
==14012==    by 0x22ECCC: main (in /home/jasonmzx/Github/cppkart/build/cppkart)

** Definite Leaks **

==14012== 2,310,000 bytes in 10,000 blocks are definitely lost in loss record 6,895 of 6,897
==14012==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==14012==    by 0x4B9F857: ??? (in /usr/lib/x86_64-linux-gnu/libLinearMath.so.3.05)
==14012==    by 0x223D48: btDefaultMotionState::operator new(unsigned long) (in /home/jasonmzx/Github/cppkart/build/cppkart)
==14012==    by 0x22445C: TerrainPhysics::TerrainPhysics(int, int, std::shared_ptr<float []>, float, float, int, int, float) (in /home/jasonmzx/Github/cppkart/build/cppkart)

---

==14012== 2,310,000 bytes in 10,000 blocks are definitely lost in loss record 6,894 of 6,897
==14012==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==14012==    by 0x4B9F857: ??? (in /usr/lib/x86_64-linux-gnu/libLinearMath.so.3.05)
==14012==    by 0x224609: btHeightfieldTerrainShape::operator new(unsigned long) (in /home/jasonmzx/Github/cppkart/build/cppkart)
==14012==    by 0x2242C3: TerrainPhysics::TerrainPhysics(int, int, std::shared_ptr<float []>, float, float, int, int, float) (in /home/jasonmzx/Github/cppkart/build/cppkart)
```

^^ Leak Fixed !  

