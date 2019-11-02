# gpmpal
Graphic Point Manipulation Program and library

Still inn design with a simple prototype, but....

It's a library for working with point sequences of any dimension, plus being able to place these in 
containers which can be used for various applications including 2D-4D shapes, kinetic representation, 
spring blobs abd many other things.  It includes sequences and shapes both by reference and indirect
indexing.  Plus nice math and bezier methods that can be mixed into whatever class you may choose to 
create.  Altimately we will add GPU support, but for now we will just add a sequential pipeline.

You can create the sequence objects as mutable lists, fixed length items or in depth groups.  Both Stack
and Heap memory are built in features.  For the classes using heap memory the thinking is for use with
pipelines.

Also point coordantes that are within a point sequence can be indexed outside of their own index range 
which is convenient for dealing with segments and bezier data.

There are built in end classes for bezier sequences and for groups of them, as well as point cahce classes.  
Finally there will be shape classes.  But the source code is written sucg that you can define your own 
classes by inheriting for the base and mixin classes.

[Put example here]

Well... I am going to need advice from more experienced programmers.  So please do be a clitic, or as long 
as your critics are containing advice and solutions.  Thanks.

