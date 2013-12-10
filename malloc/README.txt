Design
======

This malloc implementation uses a static char array as a heap to allocate
dynamic memory for programs. The array is divided into allocated blocks as
needed with the following struct:

struct MemEntry
---------------

* prev/succ: pointers which hold the address of the previous and next MemEntrys,
  or 0 if the current MemEntry is the last one.

* sig: an integer which holds a random value, used to verify that a given
  address does contain a valid MemEntry struct.

* isfree: a char used to indicate if the current block denoted by the
  MemEntry is free to be allocated. 0 = no, 1 = yes.

* size: an integer which indicates the size of the memory block denoted by the
  MemEntry.

On top of standard malloc functionality, this implementation also checks for and
warns of the following common errors and problems:


Invalid Freeing
---------------

When a pointer is passed to free(), several error conditions are checked before
any operations are performed on it. It is first verified point to a memory
location inside the static char array. If it is not inside this then it is
pointing to memory not managed by malloc and free() returns an error. If it does
point to a location inside the array but there is not a MemEntry before it
(checked by verifying that sig has the correct value), then it is also
considered invalid and free() returns an error. If neither of these conditions are
met, then it is a valid pointer and free() continues to the next set of error checks.


Redundant Freeing
-----------------

If a pointer is determined to point to a valid location in the array, but the
MemEntry associated with it shows it is free, then free() returns an error, as
it likely that the caller is attempting to call free() twice in succession on
the same pointer.


Saturation
----------

In the event that malloc() receives a request for memory and there is not a
large enough block available to satisfy the request, it simply returns NULL. It
is up to the caller program to handle this condition gracefully.

Fragmentation
-------------

It is possible that memory can be freed and allocated in such a way that
eventually a request for memory will fail because there is not a large enough
block of contingent memory, even though the total free memory is greater than
the amount requested. To reduce the probability of this happening, this
implementation of malloc first scans through all available blocks of memory, and
then chooses the smallest one available that is still big enough to still
satisfy the request. This strategy prevents preserves larger blocks for as long
as possible.

When any of the error checks indicate that an error has occurred, a message
indicating the type of error, as well as the file and line where it occurred, is
printed to stderr, and the function returns a value to indicate an error. In the
case of malloc() NULL is returned to indicate that allocating memory failed, while
free() returns 0 on success and 1 on failure.

Efficiency
==========

malloc()
--------

When called, malloc() first scans through all available blocks of memory to
choose the smallest one that can satisfy the request. Once a suitable block has
been found (or not found, in which case NULL is returned), it is divided up as
necessary and a pointer is returned. Everything except scanning for the smallest
block is constant work, so the efficiency is O(n), where n is the number of
blocks the array is divided into.


free()
------

When called, free() runs some error checks, merges the current block into the
succeeding and/or preceding blocks if they are free, and then frees the current
block. All of this is constant work, so the efficiency is O(1).
