# 3D Perlin noise using marching cubes algorithm 
![](https://raw.githubusercontent.com/mikhomak/Images/master/MarchinCubes/3D%20Perlin%20Noise.png)

## About:
This component allows you to generate 3D Perlin Noise mesh with collisions in runtime in unreal engine.

## Requirements:

In order for this component to work you will need:
1. Unreal Engine 4 2.5 (2.6 is yet not supported by *RuntimeMeshComponent* so you need to use 2.6 or update *RuntimeMeshComponent* to 2.6)
2. [Runtime mesh component](https://github.com/TriAxis-Games/RuntimeMeshComponent) - In order to create a mesh ([Marketplace](https://www.unrealengine.com/marketplace/en-US/product/runtime-mesh-component))
3. [PerlinNoiseComponent](https://github.com/nialna/PerlinNoiseComponent) - In order to get noise values (This component comes bundled in this repo)

## Installation:
1.  Install the ***RuntimeMeshComponent***  
2.  Add ***libnoise*** library to your UE4 project. You can find how [in this repo](https://github.com/nialna/libnoise-UE4-ready) by [nialna](https://github.com/nialna). P.S. - in the git history of this repo you can find installed version of libnose. 
4.  Copy *PerlinNoiseComponent.h* and *PerlinNoiseComponent.cpp* to your `Source` folder. This component was made by [nialna](https://github.com/nialna), thanks a lot to her!
3.  Copy *MCubeComponent.h* and *MCubeComponent.cpp* to your `Source` folder. (You can also copy *MCubeActor.h* and *MCubeActor.cpp* which is an Actor example)
4.  Add `UMcubeComponent, URuntimeMeshComponentStatic` and `UPerlinNoiseComponent` to your actor. Set `Noise` and `RuntimeMesh` variables for `UMcubeComponent`.
5.  Call `GenerateMarchingCubeMesh()` of `UMcubeComponent` to generate the mesh!

## Screenshots:
![](https://raw.githubusercontent.com/mikhomak/Images/master/MarchinCubes/ScreenShot00011.png)
![](https://raw.githubusercontent.com/mikhomak/Images/master/MarchinCubes/ScreenShot00010.png)
![](https://raw.githubusercontent.com/mikhomak/Images/master/MarchinCubes/ScreenShot00004.png)

## FAQ:
**1) Does the mesh have collisions?**   
YES, but you can disable them in *MCubeComponent.cpp* `    RuntimeMesh->CreateSectionFromComponents(0,
                                             0,
                                             0,
                                             Vertices,
                                             Triangles,
                                             Normals,
                                             UV,
                                             VertexColors,
                                             Tangents,
                                             ERuntimeMeshUpdateFrequency::Infrequent,
                                             true);` by setting the last argument to `false`.

**2) Can you use it as a Terrain?**   
Yeap, but I recomend you for this purpouse to use [Voxel plugin](https://www.unrealengine.com/marketplace/en-US/product/voxel-plugin-free) which is cool as shit

