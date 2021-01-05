// Made by Liana Pigeot - https://github.com/nialna

#include "MCube.h"


// Sets default values for this component's properties
UMCube::UMCube()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	BoxLength = 50.f;
	Threshold = 0.5f;
	Scale = 200.f;


	PointsPerAxis = 30;
	NumPoints = PointsPerAxis * PointsPerAxis * PointsPerAxis;

	Iterations = 10;
	// ...
}


void UMCube::GenerateMap()
{


	GenerateVertices();
	Normals.Init(FVector(0, 0, 1), Vertices.Num());
	Tangents.Init(FRuntimeMeshTangent(0, -1, 0),  Vertices.Num());
	UV.Init(FVector2D(0, 0),  Vertices.Num());
	VertexColors.Init(FColor::White,  Vertices.Num());
	//GenerateTriangles();
	GenerateMesh();
}

void UMCube::GenerateVertices() {
	//Vertices.Init(FVector(0, 0, 0), VerticesArraySize);
	for(int32 y = 0; y < Iterations; y++)
	{
		for(int32 x = 0; x < Iterations; x++)
		{
			for(int32 z = 0; z < Iterations; z++)
			{
				FVector4 Points[8];
				for(int i = 0; i < 8; i++)
				{
					Points[i] = FVector4();
				}
				int Index = GetTriangulationIndexForCube(x, y, z, Points);

				if(edgeTable[Index] != 0)
				{
					for(int32 i = 0; triangulation[Index][i] != -1; i +=3)
					{
						int a0 = cornerIndexAFromEdge[triangulation[Index][i]];
						int b0 = cornerIndexBFromEdge[triangulation[Index][i]];

						int a1 = cornerIndexAFromEdge[triangulation[Index][i+1]];
						int b1 = cornerIndexBFromEdge[triangulation[Index][i+1]];

						int a2 = cornerIndexAFromEdge[triangulation[Index][i+2]];
						int b2 = cornerIndexBFromEdge[triangulation[Index][i+2]];

						const int FirstVertice = Vertices.Add(InterpolateVerts(Points[a2], Points[b2]));
						const int SecondVertice = Vertices.Add(InterpolateVerts(Points[a1], Points[b1]));
						const int ThirdVertice = Vertices.Add(InterpolateVerts(Points[a0], Points[b0]));
						Triangles.Add(FirstVertice);
						Triangles.Add(SecondVertice);
						Triangles.Add(ThirdVertice);
					}
				}
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

void UMCube::ConstructCube(FVector4 (&PointsOut)[8])
{
/* 	int Index = GetTriangulationIndexForCube(x, y, z, Points);
	if (edgeTable[Index] & 1)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[0], Points[1])));
	}
	if (edgeTable[Index] & 2)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[1], Points[2])));
	}
	if (edgeTable[Index] & 4)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[2], Points[3])));
	}
	if (edgeTable[Index] & 8)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[3], Points[0])));
	}
	if (edgeTable[Index] & 16)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[4], Points[5])));
	}
	if (edgeTable[Index] & 32)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[5], Points[6])));
	}
	if (edgeTable[Index] & 64)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[6], Points[7])));
	}
	if (edgeTable[Index] & 128)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[7], Points[4])));
	}
	if (edgeTable[Index] & 256)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[0], Points[4])));
	}
	if (edgeTable[Index] & 512)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[1], Points[5])));
	}
	if (edgeTable[Index] & 1024)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[2], Points[6])));
	}
	if (edgeTable[Index] & 2048)
	{
		Vertices.add(VertexInterp(InterpolateVerts(Points[3], Points[7])));
	}
 */
/* 	for (i=0;triTable[cubeindex][i]!=-1;i+=3)
	{
      Triangles.Add(Vertices[triangulation[Index][i  ]]);
      triangles[ntriang].p[1] = Vertices[triangulation[Index][i+1]];
      triangles[ntriang].p[2] = Vertices[triangulation[Index][i+2]];
   } */
}

FVector UMCube::InterpolateVerts(FVector4& FirstCorner, FVector4& SecondCorner) {
    const float t = (Threshold - FirstCorner.W) / (SecondCorner.W - FirstCorner.W);
	//FirstCorner *= BoxLength;
	//SecondCorner *= BoxLength;
    return FVector(FirstCorner + t * (SecondCorner - FirstCorner)) * BoxLength;
}

int UMCube::GetTriangulationIndexForCube(int32 x, int32 y, int32 z, FVector4 (&PointsOut)[8])
{
	int Index = 0;
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
	float Value = GetNoiseValueForGridCoordinates(x,y,z);
	PointsOut[0] = FVector4(x, y ,z, Value);
	if(Value < Threshold)
	{
		Index |=  1;
	}
	// 1
	Value = GetNoiseValueForGridCoordinates(x + 1, y, z);
	PointsOut[1] = FVector4(x + 1, y , z, Value);
	if(Value < Threshold)
	{
		Index |=  2;
	}
	// 2
	Value = GetNoiseValueForGridCoordinates(x + 1, y + 1, z);
	PointsOut[2] = FVector4(x + 1, y + 1, z, Value);
	if(Value < Threshold)
	{
		Index |=  4;
	}
	// 3
	Value = GetNoiseValueForGridCoordinates(x, y + 1, z);
	PointsOut[3] = FVector4(x , y + 1 , z, Value);
	if(Value < Threshold)
	{
		Index |=  8;
	}
	// 4
	Value = GetNoiseValueForGridCoordinates(x, y, z + 1);
	PointsOut[4] = FVector4(x, y ,z + 1, Value);
	if(Value < Threshold)
	{
		Index |=  16;
	}
	// 5
	Value = GetNoiseValueForGridCoordinates(x + 1,y  ,z + 1);
	PointsOut[5] = FVector4(x + 1,y ,z + 1, Value);
	if(Value < Threshold)
	{
		Index |=  32;
	}
	// 6
	Value = GetNoiseValueForGridCoordinates(x + 1, y + 1, z + 1);
	PointsOut[6] = FVector4(x + 1, y + 1, z + 1, Value);
	if(Value < Threshold)
	{
		Index |= 64;
	}
	// 7
	Value = GetNoiseValueForGridCoordinates(x,y + 1, z + 1);
	PointsOut[7] = FVector4(x ,y + 1, z + 1, Value);
	if(Value < Threshold)
	{
		Index |= 128;
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
		(x * NoiseInputScale),
		(y * NoiseInputScale),
		(z * NoiseInputScale)
	) * Scale;
}

int UMCube::GetIndexForGridCoordinates(int x, int y, int z) {
	return x + y * NoiseSamplesPerLine + z * NoiseSamplesPerLine * NoiseSamplesPerLine;
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

FVector UMCube::GetPositionForGridCoordinates(int x, int y, int z) {
	return FVector(
		x * NoiseResolution,
		y * NoiseResolution,
		z * NoiseResolution
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
/* 	float ab = Mathf.PerlinNoise(x, y);
	float bc = Mathf.PerlinNoise(y, z);
	float ac = Mathf.PerlinNoise(x, z);

	float ba = Mathf.PerlinNoise(y, x);
	float cb = Mathf.PerlinNoise(z, y);
	float ca = Mathf.PerlinNoise(z, x);

	float abc = ab + bc + ac + ba + cb + ca;
	return abc / 6f; */
	return 0.f;
}