#pragma once

#include <vector>
//#include "..\\Math\Vector.h"
//
//#include "gl/gl3w.h"
//
//#include "..//glm/glm.hpp"
//#include "..//glm/gtc/type_ptr.hpp"

#include "..//glm/glm.hpp"
#include "..//glm/gtc/matrix_transform.hpp" // glm::rotate
#include "../../glm/gtc/quaternion.hpp"

#include "../Graphics/GraphicsSystem.h" // lines 


namespace NS_COLDEBUGTEST
{

//using NlMath::Vector3D;
//using NlMath::Vec2;
//typedef Vector3D Vec3;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;

const float  PI = 3.14159f;
const float  TWO_PI = 2.0f * PI;
const float  HALF_PI = 0.5f * PI;
const float  QUARTER_PI = 0.25f * PI;
const float  EIGHTH_PI = 0.125f * PI;
const float  SIXTEENTH_PI = 0.0625f * PI;

const int XINDEX = 0;
const int YINDEX = 1;
const int ZINDEX = 2;

const Vec3 XAXIS(1.0f, 0.0f, 0.0f);
const Vec3 YAXIS(0.0f, 1.0f, 0.0f);
const Vec3 ZAXIS(0.0f, 0.0f, 1.0f);

const float  EPSILON = 0.00001f;

static Mat4 Translate(const Vec3& t)
{
	return glm::translate(Mat4(1.0f), t);
}
static Mat4 Rotate(float angle, const Vec3& axis)
{
	return glm::rotate(Mat4(1.0f), angle, axis);
}
static float RoundDecimal(float input)
{
	return std::floor(input * 10000.0f + 0.5f) / 10000.0f;
}
static Vec3 RoundDecimal(const Vec3& input)
{
	return Vec3(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]));
}

struct Vertex
{
	Vertex(Vec3 pos, Vec3 nrm, Vec3 uv) : pos(pos), nrm(nrm), uv(uv) {}

	Vertex() { }

	Vec3 pos, nrm;
	Vec2 uv;
};

typedef std::vector<Vertex> VertexBufferType;
typedef std::vector<int> IndexBufferType;

struct Mesh
{
	Mesh() : numVertices(0), numTris(0), numIndices(0) { }

	VertexBufferType vertexBuffer;
	IndexBufferType indexBuffer;

	int numVertices;
	int numTris;
	int numIndices;
};

void addIndex(Mesh& mesh, int index)
{
	mesh.indexBuffer.push_back(index);
	++mesh.numIndices;

	if (mesh.numIndices % 3 == 0)
		++mesh.numTris;
}

void addVertex(Mesh& mesh, const Vertex& v)
{
	mesh.vertexBuffer.push_back(v);
	++mesh.numVertices;
}

static bool DegenerateTri(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
	return (glm::distance(v0, v1) < EPSILON ||
		glm::distance(v1, v2) < EPSILON ||
		glm::distance(v2, v0) < EPSILON);
}

void BuildIndexBuffer(int stacks, int slices, Mesh& mesh)
{
	//@todo: IMPLEMENT ME
	// geometric modelling pg20 
	// calculating indices for triangles on a plane

	int stride = slices + 1; // number of vertices per row
	int currentRow = 0; // index at start vertex of each row

	int i0 = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;

	for (int i = 0; i < stacks; ++i)
	{
		currentRow = i * stride;
		for (int j = 0; j < slices; ++j)
		{

			/*  You need to compute the indices for the first triangle here */
			/*  ... */
			i0 = currentRow + j;
			i1 = i0 + 1;
			i2 = i1 + stride;

			/*  Ignore degenerate triangle */
			if (!DegenerateTri(mesh.vertexBuffer[i0].pos,
				mesh.vertexBuffer[i1].pos,
				mesh.vertexBuffer[i2].pos))
			{
				/*  Add the indices for the first triangle */
				addIndex(mesh, i0);
				addIndex(mesh, i1);
				addIndex(mesh, i2);
			}


			/*  You need to compute the indices for the second triangle here */
			/*  ... */
			i3 = i2;
			i4 = i3 - 1;
			i5 = i0;

			/*  Ignore degenerate triangle */
			if (!DegenerateTri(mesh.vertexBuffer[i0].pos,
				mesh.vertexBuffer[i1].pos,
				mesh.vertexBuffer[i2].pos))
			{
				/*  Add the indices for the second triangle */
				addIndex(mesh, i3);
				addIndex(mesh, i4);
				addIndex(mesh, i5);
			}
		}
	}
}


Mesh CreatePlane(int stacks, int slices)
{
	Mesh mesh;

	for (int stack = 0; stack <= stacks; ++stack)
	{
		float row = (float)stack / stacks;

		for (int slice = 0; slice <= slices; ++slice)
		{
			float col = (float)slice / slices;

			Vertex v;

			v.pos = Vec3(col - 0.5f, 0.5f - row, 0.0f);
			v.nrm = Vec3(0.0f, 0.0f, 1.0f);
			v.uv = Vec2(col, row);

			addVertex(mesh, v);
		}
	}

	BuildIndexBuffer(stacks, slices, mesh);

	return mesh;
}

Mesh CreateCube(int stacks, int slices)
{
	Mesh planeMesh = CreatePlane(stacks, slices);
	Mesh mesh;

	Vec3 const translateArray[] =
	{
		Vec3(+0.0f, +0.0f, +0.5f), // Z+
		Vec3(+0.0f, +0.0f, -0.5f), // Z-
		Vec3(+0.5f, +0.0f, +0.0f), // X+
		Vec3(-0.5f, +0.0f, +0.0f), // X-
		Vec3(+0.0f, +0.5f, +0.0f), // Y+
		Vec3(+0.0f, -0.5f, +0.0f), // Y-
	};

	Vec2 const rotateArray[] =
	{
		Vec2(+0.0f, +0.0f),             // Z+
		Vec2(+0.0f, (float)+PI),        // Z-
		Vec2(+0.0f, (float)+HALF_PI),   // X+
		Vec2(+0.0f, (float)-HALF_PI),   // X-
		Vec2((float)-HALF_PI, +0.0f),   // Y+
		Vec2((float)+HALF_PI, +0.0f)    // Y-
	};


	/*  Transform the plane to 6 positions to form the faces of the cube */
	for (int i = 0; i < 6; ++i)
	{
		Mat4 transformMat = Translate(translateArray[i]) *
			Rotate(rotateArray[i][YINDEX], YAXIS) *
			Rotate(rotateArray[i][XINDEX], XAXIS);

		for (int j = 0; j < planeMesh.numVertices; ++j)
		{
			Vertex v;
			v.pos = Vec3(transformMat * Vec4(planeMesh.vertexBuffer[j].pos, 1.0));
			v.nrm = Vec3(transformMat * Vec4(planeMesh.vertexBuffer[j].nrm, 1.0));
			v.uv = planeMesh.vertexBuffer[j].uv;

			v.pos = RoundDecimal(v.pos);
			v.nrm = RoundDecimal(v.nrm);

			addVertex(mesh, v);
		}

		for (int j = 0; j < planeMesh.numIndices; ++j)
			addIndex(mesh, planeMesh.indexBuffer[j] + planeMesh.numVertices * i);
	}

	return mesh;
}

Mesh CreateSphere(int stacks, int slices)
{
	//@todo: IMPLEMENT ME
	// geometric modelling pg 29

	// spherical coordinate system
	const float radius = 0.5; // normalised -0.5 to 0.5
	//float alpha = 0; // 0 to 2pi
	//float beta = 0; // -pi/2 to pi/2
	//float x = radius * sin(alpha) * cos(beta);
	//float y = radius * sin(beta);
	//float z = radius * cos(alpha) * cos(beta);
	//float x_n = x / radius;
	//float y_n = y / radius;
	//float z_n = z / radius;

	Mesh mesh;

	for (int i = 0; i <= stacks; ++i)
	{
		float row = (float)i / stacks;
		float beta = PI * (row - 0.5f);

		for (int j = 0; j <= slices; ++j)
		{
			float col = (float)j / slices;
			float alpha = col * TWO_PI;

			// cr8 vertex
			Vertex v;

			// pos
			//x = radius * sin(alpha) * cos(beta);
			//y = radius * sin(beta);
			//z = radius * cos(alpha) * cos(beta);
			v.pos = Vec3(
				radius * sin(alpha) * cos(beta),
				radius * sin(beta),
				radius * cos(alpha) * cos(beta)
			);

			// nrm
			//x_n = x;
			//y_n = y;
			//z_n = z;
			v.nrm = Vec3(v.pos.x, v.pos.y, v.pos.z);
			v.nrm /= radius;

			// tex
			v.uv = Vec2(col, row);

			// add vertex to mesh
			addVertex(mesh, v);
		}
	}

	// build index buffer and return
	BuildIndexBuffer(stacks, slices, mesh);
	return mesh;

	//REMOVE THIS
	//return CreateCube(stacks, slices);
}

Mesh CreateCylinder(int stacks, int slices)
{
	//@todo: IMPLEMENT ME
	// geometric modelling pg 48

	// cylindrical coordinate system
	const float radius = 0.5; // normalised -0.5 to 0.5
	//float alpha = 0; // 0 to 2pi
	//float height = 0; // -0.5 to 0.5
	//float x = radius * sin(alpha);
	//float y = height;
	//float z = radius * cos(alpha);

	Mesh mesh;

	// build tube
	for (int i = 0; i <= stacks; ++i)
	{
		float row = (float)i / stacks;

		for (int j = 0; j <= slices; ++j)
		{
			float col = (float)j / slices;
			float alpha = col * TWO_PI;

			// cr8 vertex
			Vertex v;

			// pos
			v.pos = Vec3(
				radius * sin(alpha),
				row - radius,
				radius * cos(alpha)
			);

			// nrm
			v.nrm = Vec3(
				v.pos.x / radius,
				0.0f,
				v.pos.z / radius
			);

			// tex
			v.uv = Vec2(col, row);

			// add vertex to mesh
			addVertex(mesh, v);
		}
	}

	// build index buffer
	BuildIndexBuffer(stacks, slices, mesh);

	//// build top and bottom caps // pg 56
	//// will not reuse vertices alr generated in tube
	//// top vertex
	//Vertex vTop;
	//// pos
	//vTop.pos = Vec3(0.0f, 0.5f, 0.0f);
	//vTop.nrm = Vec3(0.0f, 0.0f, 1.0f);
	//vTop.uv = Vec2(0.0f, 0.0f);

	//int vTopInd = mesh.vertexBuffer.size();
	//int topSliceIndexStart = stacks * slices;
	//// no of stacks-1 = stacks

	//// for every slice add the index
	//for (int i = 0; i <= slices; ++i)
	//{
	//	addIndex(mesh, vTopInd);
	//	addIndex(mesh, topSliceIndexStart + i);
	//	addIndex(mesh, topSliceIndexStart + i + 1);
	//}

	//// bot vertex
	//Vertex vBot;
	//// pos
	//vBot.pos = Vec3(0.0f, -0.5f, 0.0f);
	//vBot.nrm = Vec3(0.0f, 0.0f, -1.0f);
	//vBot.uv = Vec2(0.0f, 0.0f);

	//int vBotInd = mesh.vertexBuffer.size() + 1;

	//// for every slice add the index
	//for (int i = 0; i <= slices; ++i)
	//{
	//	addIndex(mesh, vBotInd);
	//	addIndex(mesh, i);
	//	addIndex(mesh, i + 1);
	//}

	//// add top and bot vertex to mesh
	//addVertex(mesh, vTop);
	//addVertex(mesh, vBot);

	return mesh;

	//REMOVE THIS
	//return CreateCube(stacks, slices);
}



























void TransformMesh(Mesh& mesh, glm::vec3 tls, glm::vec3 rot, glm::vec3 scl)
{
	// get transform mtx
	glm::mat4 translate = glm::translate(glm::mat4(1.f), tls);

	glm::quat quaternion(glm::radians(rot));
	glm::mat4 rotate = glm::mat4_cast(quaternion);

	glm::mat4 scale = glm::scale(glm::mat4(1.f), scl);

	glm::mat4 tmtx(translate * rotate * scale);

	// apply transform mtx
	for (size_t i = 0; i < mesh.vertexBuffer.size(); ++i)
	{
		glm::vec4 pos(
			mesh.vertexBuffer[i].pos.x,
			mesh.vertexBuffer[i].pos.y,
			mesh.vertexBuffer[i].pos.z,
			1.0f
		);

		pos = tmtx * pos;

		mesh.vertexBuffer[i].pos = glm::vec3(
			pos.x,
			pos.y,
			pos.z
		);

		//mesh.vertexBuffer[i].pos = tmtx * mesh.vertexBuffer[i].pos;
	}
}

void DrawMesh(Mesh& mesh, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), int lod = 0)
{

	// > 0 anti clockwise, < 0 clock wise, == 0 in a line 
	// shoelace formula https://en.wikipedia.org/wiki/Shoelace_formula
	// https://math.stackexchange.com/questions/1324179/how-to-tell-if-3-connected-points-are-connected-clockwise-or-counter-clockwise
	auto backFaceCull = [&](Vec3 v0, Vec3 v1, Vec3 v2)
	{
		//if ((v0.x * v1.y + v1.x * v2.y + v2.x * v0.y) - (v0.x * v2.y + v1.x * v0.y + v2.x * v1.y) > 0)
		//{
		//	return false;
		//}
		//return true;

		// doesnt work for now
		// https://www.gamedev.net/tutorials/programming/graphics/3d-backface-culling-r1088/
		// camera vector dot normal > 0 -> no cull
		// need camera vector to do backface culling

		(void)v0;
		(void)v1;
		(void)v2;

		return true;
	};


	if (abs(lod % 2) == 0)
	{
		for (size_t i = 0; i < mesh.indexBuffer.size(); i += 6)
		{
			int i0 = mesh.indexBuffer.at(i);
			int i1 = mesh.indexBuffer.at(i + 1);
			int i2 = mesh.indexBuffer.at(i + 2);

			int i3 = mesh.indexBuffer.at(i + 3);
			int i4 = mesh.indexBuffer.at(i + 4);
			int i5 = mesh.indexBuffer.at(i + 5);

			Vec3 v0 = mesh.vertexBuffer[i0].pos;
			Vec3 v1 = mesh.vertexBuffer[i1].pos;
			Vec3 v2 = mesh.vertexBuffer[i2].pos;

			Vec3 v3 = mesh.vertexBuffer[i3].pos;
			Vec3 v4 = mesh.vertexBuffer[i4].pos;
			Vec3 v5 = mesh.vertexBuffer[i5].pos;
			
			if (backFaceCull(v0, v1, v2))
			{
				NS_GRAPHICS::SYS_GRAPHICS->DrawLine(v0, v1, color);
				NS_GRAPHICS::SYS_GRAPHICS->DrawLine(v1, v2, color);
				NS_GRAPHICS::SYS_GRAPHICS->DrawLine(v2, v4, color);
			}

			if (backFaceCull(v3, v4, v5))
			{
				NS_GRAPHICS::SYS_GRAPHICS->DrawLine(v4, v0, color);
			}
		}

	}
	else
	{
		for (size_t i = 0; i < mesh.indexBuffer.size(); i += 3)
		{
			int i0 = mesh.indexBuffer.at(i);
			int i1 = mesh.indexBuffer.at(i + 1);
			int i2 = mesh.indexBuffer.at(i + 2);

			Vec3 v0 = mesh.vertexBuffer[i0].pos;
			Vec3 v1 = mesh.vertexBuffer[i1].pos;
			Vec3 v2 = mesh.vertexBuffer[i2].pos;

			if (backFaceCull(v0, v1, v2))
			{
				NS_GRAPHICS::SYS_GRAPHICS->DrawLine(v0, v1, color);
				NS_GRAPHICS::SYS_GRAPHICS->DrawLine(v1, v2, color);
				NS_GRAPHICS::SYS_GRAPHICS->DrawLine(v2, v0, color);
			}
		}
	}
}





} // NS END



