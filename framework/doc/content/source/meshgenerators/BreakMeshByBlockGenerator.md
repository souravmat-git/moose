# BreakMeshByBlockGenerator

!syntax description /MeshGenerators/BreakMeshByBlockGenerator

## Description

This class implements a MeshGenerator to split a monolithic mesh by blocks similar to what is proposed by VP Nguyen [!cite](Nguyen2014).

To split the mesh, nodes shared by multiple blocks are duplicated N-1 times (where N is the number of blocks sharing a particular node). Each duplicated nodes is assigned to one block and all the elements sharing that node are updated. A new sideset identifying the new interface is added, and it is always linked to elements belonging to blocks with the lower ID.

As an option, the interface can be split into $Q$ different sidesets, where $Q$ is the number of adjacent block pairs. This is achieved by setting `split_interface=true` and is useful when modeling interfaces with different parameters.

## Example Input File Syntax

### Single Interface

!listing test/tests/meshgenerators/break_mesh_by_block_generator/break_mesh_2DJunction_auto.i block=Mesh

### Multiple Interfaces

When `split_interface=true`, the new generated interface is split by block pairs
and named by joining the block names. For instance if one has two neighboring
blocks - one named `wood` and named `steel` with `blockID` equal to `1` and `2`,
respectively - the new interface will be named `wood_steel`. The naming order
follows the block ID order. For this simple example, the new sideset will be on
the block named `wood`. If one block is not named, its name will default to
`Block` plus the `blockID`. For instance, if block 2 is not named the new
interface will be named `wood_Block2`.

!listing test/tests/meshgenerators/break_mesh_by_block_generator/break_mesh_2DJunction_splittrue.i block=Mesh

!syntax parameters /MeshGenerators/BreakMeshByBlockGenerator

!syntax inputs /MeshGenerators/BreakMeshByBlockGenerator
