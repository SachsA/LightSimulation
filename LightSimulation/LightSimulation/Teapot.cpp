#include "Teapot.h"
#include "teapotdata.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

//set grid = 64, lidTransform = 4x4 identity matrix
Teapot::Teapot(int grid, mat4 lidTransform)
{
	initShader();

    _teapot_vertices = 32 * (grid + 1) * (grid + 1);
    _faces = grid * grid * 32;

    float *v = new float[_teapot_vertices * 3 ];  //vertex positions : vec3
    float *n = new float[_teapot_vertices * 3 ];  //vertex normals : vec3
    float *tc = new float[_teapot_vertices * 2 ]; //texture coordinates : vec2 (we don't use it at this point)
    unsigned int * teapot_elements = new unsigned int[_faces * 6];  //indices for IBO 

	generatePatches(v, n, tc, teapot_elements, grid);

	glGenVertexArrays(1, &_vaoHandle);
	glBindVertexArray(_vaoHandle);

	glGenBuffers(1, &_ibo_teapot);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_teapot);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _faces * 6, teapot_elements, GL_STATIC_DRAW);

	glGenBuffers(1, &_vbo_teapot_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_teapot_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * _teapot_vertices, v, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &_vbo_teapot_normals);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_teapot_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * _teapot_vertices, n, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

    delete [] v;
    delete [] n;
    delete [] teapot_elements;
    delete [] tc;
}

void Teapot::initShader()
{
	shader = new ShaderProgram();
	//shader->initFromFiles("phong.vert", "phong.frag");
	shader->initFromFiles("spot.vert", "spot.frag");

	_Kd = glm::vec3(0.780392, 0.568627, 0.113725);
	_Ka = glm::vec3(0.329412, 0.223529, 0.027451);
	_Ks = glm::vec3(0.992157, 0.941176, 0.807843);
	_shiness = 27.89743616;

	shader->addUniform("Material.Kd");
	shader->addUniform("Material.Ka");
	shader->addUniform("Material.Ks");

	shader->addUniform("mvp");

	shader->addUniform("Material.Shiness");

/*	std::string name;
	name = "Light[";
	for (size_t i = 0; i < 5; i++)
	{
		shader->addUniform(name + std::to_string(i) + "].LightLocation");
		shader->addUniform(name + std::to_string(i) + "].LightIntensity");
	}
*/

	shader->addUniform("SpotLight.LightPosition");
	shader->addUniform("SpotLight.LightIntensity");
	shader->addUniform("SpotLight.LightDirection");
	shader->addUniform("SpotLight.LightExponent");
	shader->addUniform("SpotLight.LightCutoff");
	shader->addUniform("SpotLight.LightInnerCutoff");

	shader->addUniform("NormalMatrix");

	shader->addUniform("ViewMatrix");
	shader->addUniform("ModelViewMatrix");

	shader->use();
	glUniform3fv(shader->uniform("Material.Kd"), 1, glm::value_ptr(_Kd));
	glUniform3fv(shader->uniform("Material.Ka"), 1, glm::value_ptr(_Ka));
	glUniform3fv(shader->uniform("Material.Ks"), 1, glm::value_ptr(_Ks));
	glUniform1f(shader->uniform("Material.Shiness"), _shiness);
	shader->disable();
}

void Teapot::draw()
{
	int size;

	shader->use();
	
	glBindVertexArray(_vaoHandle);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

	shader->disable();
}

void Teapot::generatePatches(float * v, float * n, float * tc, unsigned int* el, int grid) {
    float * B = new float[4*(grid+1)];  // Pre-computed Bernstein basis functions
    float * dB = new float[4*(grid+1)]; // Pre-computed derivitives of basis functions

    int idx = 0, elIndex = 0, tcIndex = 0;

    // Pre-compute the basis functions  (Bernstein polynomials)
    // and their derivatives
    computeBasisFunctions(B, dB, grid);

    // Build each patch
    // The rim
    buildPatchReflect(0, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    // The body
    buildPatchReflect(1, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    buildPatchReflect(2, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    // The lid
    buildPatchReflect(3, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    buildPatchReflect(4, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    // The bottom
    buildPatchReflect(5, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
    // The handle
    buildPatchReflect(6, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
    buildPatchReflect(7, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
    // The spout
    buildPatchReflect(8, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
    buildPatchReflect(9, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);

    delete [] B;
    delete [] dB;
}

void Teapot::moveLid(int grid, float *v, mat4 lidTransform) {

    int start = 3 * 12 * (grid+1) * (grid+1);
    int end = 3 * 20 * (grid+1) * (grid+1);

    for( int i = start; i < end; i+=3 )
    {
        vec4 vert = vec4(v[i], v[i+1], v[i+2], 1.0f );
        vert = lidTransform * vert;
        v[i] = vert.x;
        v[i+1] = vert.y;
        v[i+2] = vert.z;
    }
}

void Teapot::buildPatchReflect(int patchNum,
                                    float *B, float *dB,
                                    float *v, float *n,
                                    float *tc, unsigned int *el,
                                    int &index, int &elIndex, int &tcIndex, int grid,
                                    bool reflectX, bool reflectY)
{
    vec3 patch[4][4];
    vec3 patchRevV[4][4];
    getPatch(patchNum, patch, false);
    getPatch(patchNum, patchRevV, true);

    // Patch without modification
    buildPatch(patch, B, dB, v, n, tc, el,
               index, elIndex, tcIndex, grid, mat3(1.0f), true);

    // Patch reflected in x
    if( reflectX ) {
        buildPatch(patchRevV, B, dB, v, n, tc, el,
                   index, elIndex, tcIndex, grid, mat3(vec3(-1.0f, 0.0f, 0.0f),
                                              vec3(0.0f, 1.0f, 0.0f),
                                              vec3(0.0f, 0.0f, 1.0f) ), false );
    }

    // Patch reflected in y
    if( reflectY ) {
        buildPatch(patchRevV, B, dB, v, n, tc, el,
                   index, elIndex, tcIndex, grid, mat3(vec3(1.0f, 0.0f, 0.0f),
                                              vec3(0.0f, -1.0f, 0.0f),
                                              vec3(0.0f, 0.0f, 1.0f) ), false );
    }

    // Patch reflected in x and y
    if( reflectX && reflectY ) {
        buildPatch(patch, B, dB, v, n, tc, el,
                   index, elIndex, tcIndex, grid, mat3(vec3(-1.0f, 0.0f, 0.0f),
                                              vec3(0.0f, -1.0f, 0.0f),
                                              vec3(0.0f, 0.0f, 1.0f) ), true );
    }
}

void Teapot::buildPatch(vec3 patch[][4],
                           float *B, float *dB,
                           float *v, float *n, float *tc,
                           unsigned int *el,
                           int &index, int &elIndex, int &tcIndex, int grid, mat3 reflect,
                           bool invertNormal)
{
    int startIndex = index / 3;
    float tcFactor = 1.0f / grid;

    for( int i = 0; i <= grid; i++ )
    {
        for( int j = 0 ; j <= grid; j++)
        {
            vec3 pt = reflect * evaluate(i,j,B,patch);
            vec3 norm = reflect * evaluateNormal(i,j,B,dB,patch);
            if( invertNormal )
                norm = -norm;

            v[index] = pt.x;
            v[index+1] = pt.y;
            v[index+2] = pt.z;

            n[index] = norm.x;
            n[index+1] = norm.y;
            n[index+2] = norm.z;

            tc[tcIndex] = i * tcFactor;
            tc[tcIndex+1] = j * tcFactor;

            index += 3;
            tcIndex += 2;
        }
    }

    for( int i = 0; i < grid; i++ )
    {
        int iStart = i * (grid+1) + startIndex;
        int nextiStart = (i+1) * (grid+1) + startIndex;
        for( int j = 0; j < grid; j++)
        {
            el[elIndex] = iStart + j;
            el[elIndex+1] = nextiStart + j + 1;
            el[elIndex+2] = nextiStart + j;

            el[elIndex+3] = iStart + j;
            el[elIndex+4] = iStart + j + 1;
            el[elIndex+5] = nextiStart + j + 1;

            elIndex += 6;
        }
    }
}

void Teapot::getPatch( int patchNum, vec3 patch[][4], bool reverseV )
{
    for( int u = 0; u < 4; u++) {          // Loop in u direction
        for( int v = 0; v < 4; v++ ) {     // Loop in v direction
            if( reverseV ) {
                patch[u][v] = vec3(
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+(3-v)]][0],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+(3-v)]][1],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+(3-v)]][2]
                        );
            } else {
                patch[u][v] = vec3(
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+v]][0],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+v]][1],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+v]][2]
                        );
            }
        }
    }
}

void Teapot::computeBasisFunctions( float * B, float * dB, int grid ) {
    float inc = 1.0f / grid;
    for( int i = 0; i <= grid; i++ )
    {
        float t = i * inc;
        float tSqr = t * t;
        float oneMinusT = (1.0f - t);
        float oneMinusT2 = oneMinusT * oneMinusT;

        B[i*4 + 0] = oneMinusT * oneMinusT2;
        B[i*4 + 1] = 3.0f * oneMinusT2 * t;
        B[i*4 + 2] = 3.0f * oneMinusT * tSqr;
        B[i*4 + 3] = t * tSqr;

        dB[i*4 + 0] = -3.0f * oneMinusT2;
        dB[i*4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
        dB[i*4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
        dB[i*4 + 3] = 3.0f * tSqr;
    }
}


vec3 Teapot::evaluate( int gridU, int gridV, float *B, vec3 patch[][4] )
{
    vec3 p(0.0f,0.0f,0.0f);
    for( int i = 0; i < 4; i++) {
        for( int j = 0; j < 4; j++) {
            p += patch[i][j] * B[gridU*4+i] * B[gridV*4+j];
        }
    }
    return p;
}

vec3 Teapot::evaluateNormal( int gridU, int gridV, float *B, float *dB, vec3 patch[][4] )
{
    vec3 du(0.0f,0.0f,0.0f);
    vec3 dv(0.0f,0.0f,0.0f);

    for( int i = 0; i < 4; i++) {
        for( int j = 0; j < 4; j++) {
            du += patch[i][j] * dB[gridU*4+i] * B[gridV*4+j];
            dv += patch[i][j] * B[gridU*4+i] * dB[gridV*4+j];
        }
    }
    return glm::normalize( glm::cross( du, dv ) );
}