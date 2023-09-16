# Binary Loader

Reworking of the “simple binary loader” from *Practical Binary
Analysis*, chapter 4. Their solution lays a solid framework for simple
binary loading and how to use a specialized library in order to load the
content. This solution builds on these efforts by leveraging
object-oriented design and the C++14 STL.

# Rationale

The author’s solution works fine in some versions of 64-bit Linux, such
as the “CTF” VM which supplements the textbook. However, the `bfd.h`
library is not available on all platforms. Further, newer versions of
the library have changed the function signatures, so the book’s version,
as-is, will not work on newer versions of BFD.

This solution leverages object-oriented design patterns and C++14 STL to
produce a more scalable binary loader. The *Loader* interface is meant
to be implemented in whatever way works best for your system or needs.
Here, two versions are offered: a proxy loader, meant for testing the
basic design; and a BFD loader, which follows the original code solution
and aspires to capture the “spirit” of that version.

The client code (*demo.cc*) need only be aware of two interfaces: the
*Binary* and *Loader*. Further, the *PbaLoader* is meant to be a nearly
one-to-one impelementation of the BFD solution provided by the book. 
The design goal was to obfuscate the underlying BFD usage while, at the
same time, leveraging a more object-oriented loading framework.

# Function changes

The following function signatures have changed:

- bfd_section_vma
- bfd_get_section_flags

Their newer counterparts are used in the *BfdLoader*.

# References

- BFD library:
  https://www.rpi.edu/dept/cis/software/g77-mingw32/include/bfd.h
- Get an underlying type from a scoped enum:
  https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11
- Overloading a scoped enum’s built-in functions:
  https://stackoverflow.com/questions/15451382/implementation-of-operators-for-enum-class
- Overloading bitwise operators in C++:
  https://www.geeksforgeeks.org/cpp-bitwise-operator-overloading/
- BFD `config.h` error:
  https://stackoverflow.com/questions/11748035/binutils-bfd-h-wants-config-h-now
- binutils differences: https://stackoverflow.com/a/61223708
