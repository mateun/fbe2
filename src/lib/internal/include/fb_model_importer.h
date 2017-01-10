#ifndef FB_MODEL_IMPORTER
#define FB_MODEL_IMPORTER

#include "fb_export_spec.h"
#include <fbxsdk.h>

enum MODEL_FORMAT {
	FBX
};

#ifdef _WIN32
class FB_ENGINE_API ModelImporter
#else
class ModelImporter
#endif
{
private:
	
public:
	Mesh* ImportModelFromFile(std::string fileName, MODEL_FORMAT modelFormat, bool indexed) {
		
		if (!this->SupportsModelFormat(modelFormat)) {
			return nullptr;
		}

		// Initialize the SDK manager. This object handles memory management.
		FbxManager* lSdkManager = FbxManager::Create();

		// Create the IO settings object.
		FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
		lSdkManager->SetIOSettings(ios);

		// Create an importer using the SDK manager.
		FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

		// Use the first argument as the filename for the importer.
		if (!lImporter->Initialize(fileName.c_str(), -1, lSdkManager->GetIOSettings())) {
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
			exit(-1);
		}

		// Create a new scene so that it can be populated by the imported file.
		FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

		// Import the contents of the file into the scene.
		lImporter->Import(lScene);

		// The file is imported; so get rid of the importer.
		lImporter->Destroy();
		
		FbxNode* lRootNode = lScene->GetRootNode();
		if (lRootNode) {

			FbxNode *node = lRootNode->GetChild(0);
			FbxMesh *fbxMesh = node->GetMesh();

			if (indexed) {
				// Pull out the indices
				int nrVertices = fbxMesh->GetPolygonVertexCount();
				GLuint *indices = (GLuint*)malloc(nrVertices * sizeof(int));
				int *fbxVertices = fbxMesh->GetPolygonVertices();
				int controlPointsCount = fbxMesh->GetControlPointsCount();
				for (int i = 0; i < nrVertices; i++) {
					indices[i] = fbxVertices[i];
				}

				// Pull out the positions
				float *positions = (float*)malloc(fbxMesh->GetControlPointsCount() * sizeof(float) * 3);
				FbxVector4 *posArray = fbxMesh->GetControlPoints();
				int posArrayCounter = 0;
				for (int i = 0; i < fbxMesh->GetControlPointsCount() * 3; i += 3) {
					positions[i] = posArray[posArrayCounter].mData[0];
					positions[i + 1] = posArray[posArrayCounter].mData[1];
					positions[i + 2] = posArray[posArrayCounter].mData[2];
					posArrayCounter++;
				}

				// Pull out the uvs
				/*fbxsdk::FbxStringList uvSets;
				fbxMesh->GetUVSetNames(uvSets);
				fbxsdk::FbxArray<FbxVector2> fbxUvs;
				fbxMesh->GetPolygonVertexUVs(uvSets[0].Buffer(), fbxUvs);
				float *uvs = (float*)malloc(fbxUvs.Size() * sizeof(float) * 2);
				int uvCounter = 0;
				for (int p = 0; p < fbxMesh->GetPolygonCount(); p++) {
				for (int v = 0; v < 3; v++) {
				int index = fbxMesh->GetPolygonVertex(p, v);
				FbxVector4 position = fbxMesh->GetControlPointAt(index);

				int uvIndex = fbxMesh->GetTextureU VIndex(p, v);
				FbxVector2 uv;
				bool unmapped = true;
				fbxMesh->GetPolygonVertexUV(p, uvIndex, uvSets[0].Buffer(), uv, unmapped);
				uvs[uvCounter + v] = uv[0];
				uvs[uvCounter + v + 1] = uv[1];
				uvCounter += 2;
				}

				}*/

				// Pull out just the pure uvs
				FbxGeometryElementUV *fbxgeometryElementUV = fbxMesh->GetElementUV(0);
				FbxVector2 *preProcessedUVs = (FbxVector2*)malloc(fbxMesh->GetTextureUVCount() * sizeof(FbxVector2));
				FbxVector2 *processedUVs = (FbxVector2*)malloc(fbxMesh->GetTextureUVCount() * sizeof(FbxVector2));
				float *uvs = (float*)malloc(fbxMesh->GetTextureUVCount() * sizeof(float) * 2);
				for (int i = 0; i < fbxMesh->GetTextureUVCount(); i++) {
					int uvIndex = fbxgeometryElementUV->GetIndexArray().GetAt(i);
					FbxVector2 uv = fbxgeometryElementUV->GetDirectArray().GetAt(uvIndex);
					preProcessedUVs[i] = uv;
					//uvs[uvElementCounter] = uv[0];
					//uvs[uvElementCounter + 1] = uv[1];
					//uvElementCounter += 2;
				}

				// Now map the vertex-index and the uv-index, 
				// so we can just use the vertex-index as our
				// single source of truth for both the vertices (positions)
				// and our uvs. 
				for (int i = 0; i < fbxMesh->GetTextureUVCount(); i++) {
					int vertexIndex = indices[i];
					FbxVector2 uv = preProcessedUVs[i];
					processedUVs[vertexIndex] = uv;
				}

				// Now extract the single elements from the uvs
				int uvElementCounter = 0;
				for (int i = 0; i < fbxMesh->GetTextureUVCount(); i++) {
					uvs[uvElementCounter] = processedUVs[i][0];
					uvs[uvElementCounter + 1] = processedUVs[i][1];
					uvElementCounter += 2;
				}


				Mesh *m = new Mesh(fbxMesh->GetControlPointsCount(), nrVertices, positions, uvs, NULL, indices);
				// TODO free the malloc-ed memory from above!
				free(uvs);
				free(preProcessedUVs);
				free(processedUVs);
				return m;
			}
			else {
				int nrVertices = fbxMesh->GetPolygonVertexCount();
				float *positions = (float*)malloc(nrVertices * sizeof(float) * 3);
				float *normals = (float*)malloc(nrVertices * sizeof(float) * 3);
				
				FbxStringList uvSets;
				fbxMesh->GetUVSetNames(uvSets);
				fbxsdk::FbxArray<FbxVector2> fbxUvs;
				fbxMesh->GetPolygonVertexUVs(uvSets[0].Buffer(), fbxUvs);
				float *uvs = (float*)malloc(fbxUvs.Size() * sizeof(float) * 2);
				int uvCounter = 0;
				int vertexCounter = 0;
				// Walk through all polygons and each 
				// vertex in each polygon.
				FbxGeometryElementUV *fbxgeometryElementUV = fbxMesh->GetElementUV(0);
				for (int p = 0; p < fbxMesh->GetPolygonCount(); p++) {

					// Assuming we only receive triangulated polygons.
					for (int v = 0; v < 3; v++) {
						int index = fbxMesh->GetPolygonVertex(p, v);
						FbxVector4 position = fbxMesh->GetControlPointAt(index);
						positions[vertexCounter] = position.mData[0];
						positions[vertexCounter + 1] = position.mData[1];
						positions[vertexCounter + 2] = position.mData[2];
						

						int uvIndex = fbxMesh->GetTextureUVIndex(p, v);
						FbxVector2 uv = fbxgeometryElementUV->GetDirectArray().GetAt(uvIndex);
						uvs[uvCounter] = uv[0];
						uvs[uvCounter + 1] = uv[1];
						uvCounter += 2;

						FbxVector4 normal;
						fbxMesh->GetPolygonVertexNormal(p, v, normal);
						normals[vertexCounter] = normal.mData[0];
						normals[vertexCounter + 1] = normal.mData[1];
						normals[vertexCounter + 2] = normal.mData[2];
						
						vertexCounter += 3;

					}
				}

				Mesh *m = new Mesh(nrVertices, 0, positions, uvs, normals, NULL);
				// TODO free the malloc-ed memory from above!
				free(uvs);
				free(positions);
				free(normals);

				return m;
			}
		}
		return nullptr;
	}

	bool SupportsModelFormat(MODEL_FORMAT modelFormat) {
		switch (modelFormat)
		{
		case FBX: return true;
		default: return false;
			break;
		}
	}

};


#endif
