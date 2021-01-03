// Made by Liana Pigeot - https://github.com/nialna

#include "MCube.h"


// Sets default values for this component's properties
UMCube::UMCube()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UMCube::GenerateMap()
{
	Normals.Init(FVector(0, 0, 1), VerticesArraySize);
	Tangents.Init(FRuntimeMeshTangent(0, -1, 0), VerticesArraySize);
	UV.Init(FVector2D(0, 0), VerticesArraySize);
	VertexColors.Init(FColor::White, VerticesArraySize);

	GenerateVertices();
	GenerateTriangles();
	GenerateMesh();
}

void UMCube::GenerateVertices() {
	Vertices.Init(FVector(0, 0, 0), VerticesArraySize);
	for(int32 y = 0; y < 10; y++)
	{
		for(int32 x = 0; x < 10; x++)
		{
			for(int32 z = 0; z < 10; z++)
			{
				int32 CubeIndex = 0;
				for(int32 i = 0; i < 8; i++)
				{
					if(cube.values[i] < )
				}
				float Value = Noise->GetValue(x, y, z);
			}

		}
	}

	/* 	for (int y = 0; y < NoiseSamplesPerLine; y++) {
			for (int x = 0; x < NoiseSamplesPerLine; x++) {
				float NoiseResult = GetNoiseValueForGridCoordinates(x, y);
				int index = GetIndexForGridCoordinates(x, y);
				FVector2D Position = GetPositionForGridCoordinates(x, y);
				Vertices[index] = FVector(Position.X, Position.Y, NoiseResult);
				UV[index] = FVector2D(x, y);
			}
		} */
}

int32 UMCube::GetTriangulationIndexForCube(int32 x, int32 y, int32 z)
{
	int32 Index = 0;
	//      4--------5
	//     /|       /|
	//    / |      / |
	//   7--------6  |
	//   |  |     |  |
	//   |  0-----|--1
	//   | /      | /
	//   |/       |/
	//   3--------2

	// 0
	if(GetNoiseValueForGridCoordinates(x,y,z) > Threshold)
	{
		Index |= 1 << 0;
	}
	// 1
	if(GetNoiseValueForGridCoordinates(x + 1, y, z) > Threshold)
	{
		Index |= 1 << 1;
	}
	// 2
	if(GetNoiseValueForGridCoordinates(x + 1, y + 1, z) > Threshold)
	{
		Index |= 1 << 2;
	}
	// 3
	if(GetNoiseValueForGridCoordinates(x,y + 1,z) > Threshold)
	{
		Index |= 1 << 3;
	}
	// 4
	if(GetNoiseValueForGridCoordinates(x,y,z + 1) > Threshold)
	{
		Index |= 1 << 4;
	}
	// 5
	if(GetNoiseValueForGridCoordinates(x + 1,y ,z + 1) > Threshold)
	{
		Index |= 1 << 5;
	}
	// 6
	if(GetNoiseValueForGridCoordinates(x + 1, y + 1, z + 1) > Threshold)
	{
		Index |= 1 << 6;
	}
	// 7
	if(GetNoiseValueForGridCoordinates(x,y + 1, z + 1) > Threshold)
	{
		Index |= 1 << 7;
	}

	return Index;
}


void UMCube::GenerateTriangles() {
	int QuadSize = 6; // This is the number of triangle indexes making up a quad (square section of the grid)
	int NumberOfQuadsPerLine = NoiseSamplesPerLine - 1; // We have one less quad per line than the amount of vertices, since each vertex is the start of a quad except the last ones
														// In our triangles array, we need 6 values per quad
	int TrianglesArraySize = NumberOfQuadsPerLine * NumberOfQuadsPerLine * QuadSize;
	Triangles.Init(0, TrianglesArraySize);

	for (int y = 0; y < NumberOfQuadsPerLine; y++) {
		for (int x = 0; x < NumberOfQuadsPerLine; x++) {
			int QuadIndex = x + y * NumberOfQuadsPerLine;
			int TriangleIndex = QuadIndex * QuadSize;

			// Getting the indexes of the four vertices making up this quad
			int bottomLeftIndex = GetIndexForGridCoordinates(x, y);
			int topLeftIndex = GetIndexForGridCoordinates(x, y + 1);
			int topRightIndex = GetIndexForGridCoordinates(x + 1, y + 1);
			int bottomRightIndex = GetIndexForGridCoordinates(x + 1, y);

			// Assigning the 6 triangle points to the corresponding vertex indexes, by going counter-clockwise.
			Triangles[TriangleIndex] = bottomLeftIndex;
			Triangles[TriangleIndex + 1] = topLeftIndex;
			Triangles[TriangleIndex + 2] = topRightIndex;
			Triangles[TriangleIndex + 3] = bottomLeftIndex;
			Triangles[TriangleIndex + 4] = topRightIndex;
			Triangles[TriangleIndex + 5] = bottomRightIndex;
		}
	}
}


void UMCube::GenerateMesh() {
	RuntimeMesh->CreateSectionFromComponents(0,
		0,
		0,
		Vertices,
		Triangles,
		Normals,
		UV,
		VertexColors,
		Tangents,
		ERuntimeMeshUpdateFrequency::Infrequent,
		true);
}



// Returns the scaled noise value for grid coordinates [x,y]
float UMCube::GetNoiseValueForGridCoordinates(int x, int y) {
	return Noise->GetValue(
		(x * NoiseInputScale) + 0.1,
		(y * NoiseInputScale) + 0.1,
		1.0
	) * NoiseOutputScale;
}

float UMCube::GetNoiseValueForGridCoordinates(int32 x, int32 y, int32 z) {
	return Noise->GetValue(
		(x * NoiseInputScale) + 0.1,
		(y * NoiseInputScale) + 0.1,
		(z * NoiseInputScale) + 0.1
	);
}

int UMCube::GetIndexForGridCoordinates(int x, int y) {
	return x + y * NoiseSamplesPerLine;
}

FVector2D UMCube::GetPositionForGridCoordinates(int x, int y) {
	return FVector2D(
		x * NoiseResolution,
		y * NoiseResolution
	);
}

// Called when the game starts
void UMCube::BeginPlay()
{
	Super::BeginPlay();
	Noise = Cast<UPerlinNoiseComponent>(GetOwner()->GetComponentByClass(UPerlinNoiseComponent::StaticClass()));
	RuntimeMesh = Cast<URuntimeMeshComponentStatic>(GetOwner()->GetComponentByClass(URuntimeMeshComponentStatic::StaticClass()));

}


// Called every frame
void UMCube::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


float UMCube::Perlin3D(float x, float y, float z)
{
	float ab = Mathf.PerlinNoise(x, y);
	float bc = Mathf.PerlinNoise(y, z);
	float ac = Mathf.PerlinNoise(x, z);

	float ba = Mathf.PerlinNoise(y, x);
	float cb = Mathf.PerlinNoise(z, y);
	float ca = Mathf.PerlinNoise(z, x);

	float abc = ab + bc + ac + ba + cb + ca;
	return abc / 6f;
}