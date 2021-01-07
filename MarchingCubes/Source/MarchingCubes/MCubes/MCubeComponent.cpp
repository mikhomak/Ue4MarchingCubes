#include "MCubeComponent.h"


UMCubeComponent::UMCubeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BoxLength = 50.f;
	Threshold = 0.5f;
	Scale = 200.f;

	XScale = 30;
	ZScale = 30;
	YScale = 30;

}


void UMCubeComponent::GenerateMarchingCubeMesh(int X_Iterations, int Y_Iterations, int Z_Iterations, float _BoxLength)
{
	/* Making sure we wouldn't crash the game */
	if(RuntimeMesh == nullptr || Noise == nullptr)
	{
		return;
	}
	/* Setting the variables */
	XScale = X_Iterations;
	YScale = Y_Iterations;
	Z_Iterations = ZScale;
	BoxLength = _BoxLength;

	/* Generating all the arrays */
	GenerateMeshData();
	/* Generating the mesh based on the arrays */
	GenerateMesh();
}

void UMCubeComponent::GenerateMeshData() {
	for(int32 y = 0; y < YScale; y++)
	{
		for(int32 x = 0; x < XScale; x++)
		{
			for(int32 z = 0; z < ZScale; z++)
			{
				/* Creating an array for vertices of the current cube */
				FVector4 Points[8];
				/* Filling this array with empty values to not crash the game just in case */
				for(int i = 0; i < 8; i++)
				{
					Points[i] = FVector4();
				}
				/* Creates the index for the triangulation table and filling in Points array*/
				int Index = GetTriangulationIndexForCube(x, y, z, Points);

				/* Cube is entirely in/out of the surface */
				if(edgeTable[Index] != 0)
				{
					for(int32 i = 0; triangulation[Index][i] != -1; i +=3)
					{
						/* Gets the vertices for the configuration */
						int a0 = cornerIndexAFromEdge[triangulation[Index][i]];
						int b0 = cornerIndexBFromEdge[triangulation[Index][i]];

						int a1 = cornerIndexAFromEdge[triangulation[Index][i+1]];
						int b1 = cornerIndexBFromEdge[triangulation[Index][i+1]];

						int a2 = cornerIndexAFromEdge[triangulation[Index][i+2]];
						int b2 = cornerIndexBFromEdge[triangulation[Index][i+2]];

						/* Creates actual vertices on the edges of the cube*/
						FVector FirstVertice = InterpolateVerts(Points[a2], Points[b2]);
						FVector SecondVertice = InterpolateVerts(Points[a1], Points[b1]);
						FVector ThirdVertice = InterpolateVerts(Points[a0], Points[b0]);

						/* Creates indexes for triangles array */
						int FirstVerticeIndex = Vertices.Add(FirstVertice);
						int SecondVerticeIndex = Vertices.Add(SecondVertice);
						int ThirdVerticeIndex = Vertices.Add(ThirdVertice);

						/* Triangles array */
						Triangles.Add(FirstVerticeIndex);
						Triangles.Add(SecondVerticeIndex);
						Triangles.Add(ThirdVerticeIndex);

						/* Creates basic UVs */
						/* We do it 3 times cuz it should be coorelate wth indexes of Vertices array */
						UV.Add(FVector2D(x,y));
						UV.Add(FVector2D(x,y));
						UV.Add(FVector2D(x,y));

						/* Creates basic normals */
						/* If you want to smooth the mesh out you would probably need to find an average of adjacent normals or something like that I dunno, google it */
						/* We do it 3 times cuz it should be coorelate wth indexes of Vertices array */
						FVector Normal = FVector::CrossProduct(FirstVertice - ThirdVertice, SecondVertice - ThirdVertice).GetSafeNormal();
						Normals.Add(Normal);
						Normals.Add(Normal);
						Normals.Add(Normal);

						/* Basic tangets */
						/* We do it 3 times cuz it should be coorelate wth indexes of Vertices array */
						FVector Tanget = (FirstVertice - SecondVertice).GetSafeNormal2D();
						Tangents.Add(Tanget);
						Tangents.Add(Tanget);
						Tangents.Add(Tanget);


					}
				}
			}

		}
	}
}

FVector UMCubeComponent::InterpolateVerts(FVector4& FirstCorner, FVector4& SecondCorner) {
	/* Math shit */
    const float t = (Threshold - FirstCorner.W) / (SecondCorner.W - FirstCorner.W);
    return FVector(FirstCorner + t * (SecondCorner - FirstCorner)) * BoxLength;
}

int UMCubeComponent::GetTriangulationIndexForCube(int x, int y, int z, FVector4 (&PointsOut)[8])
{
	int Index = 0;
	// Iterating through each vertice(clockwise aka 0-X-XY-Y-Z-XZ-XYZ-YZ) of the cube
	// XYZ are the indexes(not world/relative position!!) of the first point of the cube (0)
	// For each vertice we find the Noise Value
	// In case if the vertice is valid (NoiseValue < Threshold) setting 1 to the bit of the vertice to Index(8 bits)
	// EX: 3 and 6 vertices are valid ---> Index = 01001000
	//      4--------5
	//     /|       /|
	//    / |      / |               Z
	//   7--------6  |               |
	//   |  |     |  |               |
	//   |  0-----|--1              []-------X
	//   | /      | /               /
	//   |/       |/               /
	//   3--------2               Y

	// 0
	float Value = GetNoiseValueForCoordinates(x,y,z);
	PointsOut[0] = FVector4(x, y ,z, Value);
	if(Value < Threshold)
	{
		Index |=  1;
	}
	// 1
	Value = GetNoiseValueForCoordinates(x + 1, y, z);
	PointsOut[1] = FVector4(x + 1, y , z, Value);
	if(Value < Threshold)
	{
		Index |=  2;
	}
	// 2
	Value = GetNoiseValueForCoordinates(x + 1, y + 1, z);
	PointsOut[2] = FVector4(x + 1, y + 1, z, Value);
	if(Value < Threshold)
	{
		Index |=  4;
	}
	// 3
	Value = GetNoiseValueForCoordinates(x, y + 1, z);
	PointsOut[3] = FVector4(x , y + 1 , z, Value);
	if(Value < Threshold)
	{
		Index |=  8;
	}
	// 4
	Value = GetNoiseValueForCoordinates(x, y, z + 1);
	PointsOut[4] = FVector4(x, y ,z + 1, Value);
	if(Value < Threshold)
	{
		Index |=  16;
	}
	// 5
	Value = GetNoiseValueForCoordinates(x + 1,y  ,z + 1);
	PointsOut[5] = FVector4(x + 1,y ,z + 1, Value);
	if(Value < Threshold)
	{
		Index |=  32;
	}
	// 6
	Value = GetNoiseValueForCoordinates(x + 1, y + 1, z + 1);
	PointsOut[6] = FVector4(x + 1, y + 1, z + 1, Value);
	if(Value < Threshold)
	{
		Index |= 64;
	}
	// 7
	Value = GetNoiseValueForCoordinates(x,y + 1, z + 1);
	PointsOut[7] = FVector4(x ,y + 1, z + 1, Value);
	if(Value < Threshold)
	{
		Index |= 128;
	}

	return Index;
}

void UMCubeComponent::GenerateMesh()
{
	/* Making sure it wouldn't crash the game */
	if(RuntimeMesh == nullptr)
	{
		return;
	}
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




float UMCubeComponent::GetNoiseValueForCoordinates(int x, int y, int z) {
	/* Filling in the borders (or the border would be empty and it would look odd as shit) */
	if(x == 0 || x >= XScale || y == 0 || y >= YScale || z == 0 || z >= ZScale)
	{
		return 0.f;
	}

	float Res = Noise->GetValue(
		(x * NoiseInputScale),
		(y * NoiseInputScale),
		(z * NoiseInputScale)
	);

	return Res * Scale;
}


void UMCubeComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMCubeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
