#include "OBJ_Loader.h"

std::string GetExtension(const std::string& filename)
{
	std::string::size_type idx;

	idx = filename.rfind('.');

	if (idx != std::string::npos)
	{
		return filename.substr(idx + 1);
	}
	else
	{
		// No extension found
	}
}

std::string removeExtension(const std::string& filename) {
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) return filename;
	return filename.substr(0, lastdot);
}

bool LoadModel(char* filename, int& vertexCount, Vertex** ppVertexArray, int& indexCount, unsigned long** ppIndexArray)
{
	

	string ext = GetExtension(string(filename));

	if (ext == "smf")
	{
		if (!LoadSmfModel(filename, vertexCount, ppVertexArray, indexCount, ppIndexArray))
			return false;	
	}
	else if (ext == "obj")
	{
		int vCount, tCount, nCount, fCount;

		if (!ReadFileCounts(filename, vCount, tCount, nCount, fCount))
			return false;

		if (!LoadDataStructures(filename, vCount, tCount, nCount, fCount, vertexCount, ppVertexArray, indexCount, ppIndexArray))
			return false;
	}
	return true;
}

bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;


	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	return true;
}

bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount, int& vertexCounts, Vertex** ppVertexArray, int& indexCount, unsigned long** ppIndexArray)
{
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;


	// Initialize the four data structures.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Also convert to left hand coordinate
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				//Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.

				fin >> faces[faceIndex].vIndex4 >> input2 >> faces[faceIndex].tIndex4 >> input2 >> faces[faceIndex].nIndex4
					>> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2;

				fin.get(input);
				if (input == ' ')
				{

					faces[faceIndex].vIndex1 == input2;
					fin >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
					faces[faceIndex].Count = 4;
					vertexCounts += 4;
					indexCount += 6;

				}
				else if (input == '\n')
				{
					faces[faceIndex].Count = 3;
					vertexCounts += 3;
					indexCount += 3;
				}
				faceIndex++;
			}
		}

		// Start reading the beginning of the next line.
		fin.get(input);

	}

	// Close the file.
	fin.close();

	(*ppVertexArray) = new Vertex[vertexCounts];
	(*ppIndexArray) = new unsigned long[indexCount];

	int vI = 0;
	int iIndex = 0;
	int fCount = 0;
	for (int i = 0; i < faceIndex; i++)
	{
		if (faces[i].Count == 4)
		{
			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;

			InsertData(&((*ppVertexArray)[vI]), &vertices[vIndex], &texcoords[tIndex], &normals[nIndex]);

			vI++;

			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;

			InsertData(&((*ppVertexArray)[vI]), &vertices[vIndex], &texcoords[tIndex], &normals[nIndex]);

			vI++;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;

			InsertData(&((*ppVertexArray)[vI]), &vertices[vIndex], &texcoords[tIndex], &normals[nIndex]);

			vI++;

			vIndex = faces[i].vIndex4 - 1;
			tIndex = faces[i].tIndex4 - 1;
			nIndex = faces[i].nIndex4 - 1;

			InsertData(&((*ppVertexArray)[vI]), &vertices[vIndex], &texcoords[tIndex], &normals[nIndex]);

			vI++;

			(*ppIndexArray)[iIndex] = (*ppIndexArray)[iIndex + 4] = 0 + fCount;
			(*ppIndexArray)[iIndex + 1] = 1 + fCount;
			(*ppIndexArray)[iIndex + 2] = (*ppIndexArray)[iIndex + 3] = 2 + fCount;
			(*ppIndexArray)[iIndex + 5] = 3 + fCount;

			iIndex += 6;
			fCount += 4;
		}
		else
		{
			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;

			InsertData(&((*ppVertexArray)[vI]), &vertices[vIndex], &texcoords[tIndex], &normals[nIndex]);

			vI++;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;

			InsertData(&((*ppVertexArray)[vI]), &vertices[vIndex], &texcoords[tIndex], &normals[nIndex]);

			vI++;

			vIndex = faces[i].vIndex4 - 1;
			tIndex = faces[i].tIndex4 - 1;
			nIndex = faces[i].nIndex4 - 1;

			InsertData(&((*ppVertexArray)[vI]), &vertices[vIndex], &texcoords[tIndex], &normals[nIndex]);

			vI++;

			(*ppIndexArray)[iIndex] = 0 + fCount;
			(*ppIndexArray)[iIndex + 1] = 1 + fCount;
			(*ppIndexArray)[iIndex + 2] = 2 + fCount;

			iIndex += 3;
			fCount += 3;
		}
	}

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}


void InsertData(Vertex* pVertex, VertexType* pPoint, VertexType* pTex, VertexType* pNorm)
{
	pVertex->Pos.x = pPoint->x;
	pVertex->Pos.y = pPoint->y;
	pVertex->Pos.z = pPoint->z;

	pVertex->texCoord.x = pTex->x;
	pVertex->texCoord.y = pTex->y;

	pVertex->Normal.x = pNorm->x;
	pVertex->Normal.y = pNorm->y;
	pVertex->Normal.z = pNorm->z;
}

bool LoadSmfModel(char* filename, int& vertexCount, Vertex** ppVertexArray, int& indexCount, unsigned long** ppIndexArray)
{
	FILE* filePtr;
	SmfHeader head;
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return -3;
	}

	fread(&head, sizeof(SmfHeader), 1, filePtr);

	vertexCount = head.VertexCount;
	indexCount = head.IndexCount;

	(*ppVertexArray) = new Vertex[vertexCount];
	(*ppIndexArray) = new unsigned long[indexCount];
	fseek(filePtr, head.bfOffBits, SEEK_SET);

	fread((*ppVertexArray), sizeof(Vertex), head.VertexCount, filePtr);
	fread((*ppIndexArray), sizeof(unsigned long), head.IndexCount, filePtr);
}