#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>   // The GL Header File
#include <GL/gl.h>   // The GL Header File
#include <GLFW/glfw3.h> // The GLFW header
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

using namespace std;

GLuint gProgram[5];
GLint gIntensityLoc;
float gIntensity = 500;
int gWidth = 640, gHeight = 480;

struct Vertex
{
    Vertex(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
    GLfloat x, y, z;
};

struct Texture
{
    Texture(GLfloat inU, GLfloat inV) : u(inU), v(inV) { }
    GLfloat u, v;
};

struct Normal
{
    Normal(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
    GLfloat x, y, z;
};

struct Face
{
	Face(int v[], int t[], int n[]) {
		vIndex[0] = v[0];
		vIndex[1] = v[1];
		vIndex[2] = v[2];
		tIndex[0] = t[0];
		tIndex[1] = t[1];
		tIndex[2] = t[2];
		nIndex[0] = n[0];
		nIndex[1] = n[1];
		nIndex[2] = n[2];
	}
    GLuint vIndex[3], tIndex[3], nIndex[3];
};

vector<Vertex> bunnyVertices;
vector<Texture> bunnyTextures;
vector<Normal> bunnyNormals;
vector<Face> bunnyFaces;
GLuint bunnyVertexAttribBuffer, bunnyIndexBuffer;
int bunnyVertexDataSizeInBytes, bunnyNormalDataSizeInBytes;

vector<Vertex> cubeVertices;
vector<Texture> cubeTextures;
vector<Normal> cubeNormals;
vector<Face> cubeFaces;
GLuint cubeVertexAttribBuffer, cubeIndexBuffer;
int cubeVertexDataSizeInBytes, cubeNormalDataSizeInBytes;

vector<Vertex> quadVertices;
vector<Texture> quadTextures;
vector<Normal> quadNormals;
vector<Face> quadFaces;
GLuint quadVertexAttribBuffer, quadIndexBuffer;
int quadVertexDataSizeInBytes, quadNormalDataSizeInBytes;


GLuint gTextVBO;
GLint gInVertexLoc, gInNormalLoc;

static float going_right = 0; //added
static float speed = 0;
static float initialSpeed = 0;
static float current_position = 0.0f;
static bool pressing_D = false;
static bool pressing_A = false;
static int score = 0;
static int random_box;
static bool reset_box = true;
static bool bunny_dead = false;
static bool startGameAgain = false;
static bool being_happy = false;
static float happy_angle = 0;
static bool draw_cube0 = true;
static bool draw_cube1 = true;
static bool draw_cube2 = true;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;


bool ParseObj(const string& fileName, vector<Vertex> & gVertices, vector<Texture> & gTextures, vector<Normal> & gNormals, vector<Face> & gFaces)
{
    fstream myfile;

    // Open the input 
    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open())
    {
        string curLine;

        while (getline(myfile, curLine))
        {
            stringstream str(curLine);
            GLfloat c1, c2, c3;
            GLuint index[9];
            string tmp;

            if (curLine.length() >= 2)
            {
                if (curLine[0] == '#') // comment
                {
                    continue;
                }
                else if (curLine[0] == 'v')
                {
                    if (curLine[1] == 't') // texture
                    {
                        str >> tmp; // consume "vt"
                        str >> c1 >> c2;
                        gTextures.push_back(Texture(c1, c2));
                    }
                    else if (curLine[1] == 'n') // normal
                    {
                        str >> tmp; // consume "vn"
                        str >> c1 >> c2 >> c3;
                        gNormals.push_back(Normal(c1, c2, c3));
                    }
                    else // vertex
                    {
                        str >> tmp; // consume "v"
                        str >> c1 >> c2 >> c3;
                        gVertices.push_back(Vertex(c1, c2, c3));
                    }
                }
                else if (curLine[0] == 'f') // face
                {
                    str >> tmp; // consume "f"
					char c;
					int vIndex[3],  nIndex[3], tIndex[3];
					str >> vIndex[0]; str >> c >> c; // consume "//"
					str >> nIndex[0]; 
					str >> vIndex[1]; str >> c >> c; // consume "//"
					str >> nIndex[1]; 
					str >> vIndex[2]; str >> c >> c; // consume "//"
					str >> nIndex[2]; 

					assert(vIndex[0] == nIndex[0] &&
						   vIndex[1] == nIndex[1] &&
						   vIndex[2] == nIndex[2]); // a limitation for now

					// make indices start from 0
					for (int c = 0; c < 3; ++c)
					{
						vIndex[c] -= 1;
						nIndex[c] -= 1;
						tIndex[c] -= 1;
					}

                    gFaces.push_back(Face(vIndex, tIndex, nIndex));
                }
                else
                {
                    cout << "Ignoring unidentified line in obj file: " << curLine << endl;
                }
            }

            //data += curLine;
            if (!myfile.eof())
            {
                //data += "\n";
            }
        }

        myfile.close();
    }
    else
    {
        return false;
    }

    
	assert(gVertices.size() == gNormals.size());

    return true;
}

bool ReadDataFromFile(
    const string& fileName, ///< [in]  Name of the shader file
    string&       data)     ///< [out] The contents of the file
{
    fstream myfile;

    // Open the input 
    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open())
    {
        string curLine;

        while (getline(myfile, curLine))
        {
            data += curLine;
            if (!myfile.eof())
            {
                data += "\n";
            }
        }

        myfile.close();
    }
    else
    {
        return false;
    }

    return true;
}

void createVS(GLuint& program, const string& filename)
{
    string shaderSource;

    if (!ReadDataFromFile(filename, shaderSource))
    {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &shader, &length);
    glCompileShader(vs);

    char output[1024] = {0};
    glGetShaderInfoLog(vs, 1024, &length, output);
    printf("VS compile log: %s\n", output);

    glAttachShader(program, vs);
}

void createFS(GLuint& program, const string& filename)
{
    string shaderSource;

    if (!ReadDataFromFile(filename, shaderSource))
    {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &shader, &length);
    glCompileShader(fs);

    char output[1024] = {0};
    glGetShaderInfoLog(fs, 1024, &length, output);
    printf("FS compile log: %s\n", output);

    glAttachShader(program, fs);
}

void initShaders()
{
    gProgram[0] = glCreateProgram();
    gProgram[1] = glCreateProgram();
    gProgram[2] = glCreateProgram();
    gProgram[3] = glCreateProgram();
    gProgram[4] = glCreateProgram();

    createVS(gProgram[0], "vert0.glsl");
    createFS(gProgram[0], "frag0.glsl");

    createVS(gProgram[1], "vert1.glsl");
    createFS(gProgram[1], "frag1.glsl");

    createVS(gProgram[2], "vert_text.glsl");
    createFS(gProgram[2], "frag_text.glsl");

    createVS(gProgram[3], "vertexCubeRed.glsl");
    createFS(gProgram[3], "fragCubeRed.glsl");

    createVS(gProgram[4], "vertexCubeYellow.glsl");
    createFS(gProgram[4], "fragCubeYellow.glsl");

    glBindAttribLocation(gProgram[0], 0, "inVertex");
    glBindAttribLocation(gProgram[0], 1, "inNormal");
    glBindAttribLocation(gProgram[1], 0, "inVertex");
    glBindAttribLocation(gProgram[1], 1, "inNormal");
    glBindAttribLocation(gProgram[2], 2, "vertex");
    glBindAttribLocation(gProgram[3], 0, "inVertex");
    glBindAttribLocation(gProgram[3], 1, "inNormal");
    glBindAttribLocation(gProgram[4], 0, "inVertex");
    glBindAttribLocation(gProgram[4], 1, "inNormal");
    

    glLinkProgram(gProgram[0]);
    glLinkProgram(gProgram[1]);
    glLinkProgram(gProgram[2]);
    glLinkProgram(gProgram[3]);
    glLinkProgram(gProgram[4]);
    glUseProgram(gProgram[0]);

    gIntensityLoc = glGetUniformLocation(gProgram[0], "intensity");
    cout << "gIntensityLoc = " << gIntensityLoc << endl;
    glUniform1f(gIntensityLoc, gIntensity);
}

void initVBO(vector<Vertex> & gVertices, int & gVertexDataSizeInBytes, vector<Normal> & gNormals, int & gNormalDataSizeInBytes, vector<Face> & gFaces, GLuint & gVertexAttribBuffer, GLuint & gIndexBuffer)
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    assert(glGetError() == GL_NONE);

    glGenBuffers(1, &gVertexAttribBuffer);
    glGenBuffers(1, &gIndexBuffer);

    assert(gVertexAttribBuffer > 0 && gIndexBuffer > 0);

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

    gVertexDataSizeInBytes = gVertices.size() * 3 * sizeof(GLfloat);
    gNormalDataSizeInBytes = gNormals.size() * 3 * sizeof(GLfloat);
    int indexDataSizeInBytes = gFaces.size() * 3 * sizeof(GLuint);
    GLfloat* vertexData = new GLfloat [gVertices.size() * 3];
    GLfloat* normalData = new GLfloat [gNormals.size() * 3];
    GLuint* indexData = new GLuint [gFaces.size() * 3];

    float minX = 1e6, maxX = -1e6;
    float minY = 1e6, maxY = -1e6;
    float minZ = 1e6, maxZ = -1e6;

    for (int i = 0; i < gVertices.size(); ++i)
    {
        vertexData[3*i] = gVertices[i].x;
        vertexData[3*i+1] = gVertices[i].y;
        vertexData[3*i+2] = gVertices[i].z;

        minX = std::min(minX, gVertices[i].x);
        maxX = std::max(maxX, gVertices[i].x);
        minY = std::min(minY, gVertices[i].y);
        maxY = std::max(maxY, gVertices[i].y);
        minZ = std::min(minZ, gVertices[i].z);
        maxZ = std::max(maxZ, gVertices[i].z);
    }
    
    std::cout << "minX = " << minX << std::endl;
    std::cout << "maxX = " << maxX << std::endl;
    std::cout << "minY = " << minY << std::endl;
    std::cout << "maxY = " << maxY << std::endl;
    std::cout << "minZ = " << minZ << std::endl;
    std::cout << "maxZ = " << maxZ << std::endl;
  
    for (int i = 0; i < gNormals.size(); ++i)
    {
        normalData[3*i] = gNormals[i].x;
        normalData[3*i+1] = gNormals[i].y;
        normalData[3*i+2] = gNormals[i].z;
    }

    for (int i = 0; i < gFaces.size(); ++i)
    {
        indexData[3*i] = gFaces[i].vIndex[0];
        indexData[3*i+1] = gFaces[i].vIndex[1];
        indexData[3*i+2] = gFaces[i].vIndex[2];
    }


    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, vertexData);
    glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, normalData);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indexData, GL_STATIC_DRAW);

    // done copying; can free now
    delete[] vertexData;
    delete[] normalData;
    delete[] indexData;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));

}

void initFonts(int windowWidth, int windowHeight)
{
    // Set OpenGL options
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(windowWidth), 0.0f, static_cast<GLfloat>(windowHeight));
    glUseProgram(gProgram[2]);
    glUniformMatrix4fv(glGetUniformLocation(gProgram[2], "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // Load font as face
    FT_Face face;
    
    //if (FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationSerif-Italic.ttf", 0, &face))
    if (FT_New_Face(ft, "arial.ttf", 0, &face)) // TODO: CHANGE FOR INEKS
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
                );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    //
    // Configure VBO for texture quads
    //
    glGenBuffers(1, &gTextVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gTextVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init() 
{
	//ParseObj("armadillo.obj");
	ParseObj("bunny.obj", bunnyVertices, bunnyTextures, bunnyNormals, bunnyFaces);
    ParseObj("cube.obj", cubeVertices, cubeTextures, cubeNormals, cubeFaces);
    ParseObj("quad.obj", quadVertices, quadTextures, quadNormals, quadFaces);


    glEnable(GL_DEPTH_TEST);
    initShaders();
    initFonts(gWidth, gHeight);
    initVBO(bunnyVertices, bunnyVertexDataSizeInBytes, bunnyNormals, bunnyNormalDataSizeInBytes, bunnyFaces, bunnyVertexAttribBuffer, bunnyIndexBuffer);
    initVBO(cubeVertices, cubeVertexDataSizeInBytes, cubeNormals, cubeNormalDataSizeInBytes, cubeFaces, cubeVertexAttribBuffer, cubeIndexBuffer);
    initVBO(quadVertices, quadVertexDataSizeInBytes, quadNormals, quadNormalDataSizeInBytes, quadFaces, quadVertexAttribBuffer, quadIndexBuffer);
}

void drawModel(GLuint & gVertexAttribBuffer, GLuint & gIndexBuffer, int & gVertexDataSizeInBytes, vector<Face> & gFaces)
{
	glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));

	glDrawElements(GL_TRIANGLES, gFaces.size() * 3, GL_UNSIGNED_INT, 0);
}

void renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state	
    glUseProgram(gProgram[2]);
    glUniform3f(glGetUniformLocation(gProgram[2], "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, gTextVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)

        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}


void display()
{
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    static float hop = 0;
    static bool rising = true;

    glm::mat4 T,/* R_bunny_happy,*/ R_bunny, R, S, modelMat, modelMatInv, perspMat, R_dead_bunny;
    static float coming = 0;
    static float cube_pos = -30.f;
    static float resetTime = 0.f;

    if(startGameAgain){
        speed = 0;
        current_position = 0;
        hop = 0;
        score = 0;
        coming = 0;
        bunny_dead = false;
        startGameAgain = false;
        cube_pos = -30.f;

        resetTime = glfwGetTime();
        draw_cube0 = draw_cube1 = draw_cube2 = true;
    }

    glUseProgram(gProgram[0]);
    if (!bunny_dead)
    {
        if (pressing_A) current_position -= (0.05 + speed);
        else if (pressing_D) current_position += (0.05 + speed);
        if (current_position < -0.85) current_position = -0.85;
        if (current_position > 0.85) current_position = 0.85;

        T = glm::translate(glm::mat4(1.f), glm::vec3(current_position, -0.7f + hop, -2.f));
       // R_bunny_happy = glm::rotate(glm::mat4(1.f), glm::radians(happy_angle), glm::vec3(0, 1, 0)); // added
        R_bunny = glm::rotate(glm::mat4(1.f), glm::radians(20.f), glm::vec3(1, 0, 0)); // added
        R = glm::rotate(glm::mat4(1.f), glm::radians(270.f + happy_angle), glm::vec3(0, 1, 0));
        S = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1)); // added
        modelMat = T * R_bunny * R * S;
        
        modelMatInv = glm::transpose(glm::inverse(modelMat));
        perspMat = glm::perspective(glm::radians(45.0f), 1.f, 1.0f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(gProgram[0], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
        glUniformMatrix4fv(glGetUniformLocation(gProgram[0], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
        glUniformMatrix4fv(glGetUniformLocation(gProgram[0], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(perspMat));

        drawModel(bunnyVertexAttribBuffer, bunnyIndexBuffer, bunnyVertexDataSizeInBytes, bunnyFaces);

        if (hop >= 0.3) {
            rising = false;
        }
        else if (hop <= 0) {
            rising = true;
        }
        if (rising) {
            hop += (0.02 + speed);
        }
        else {
            hop -= (0.02 + speed);
        }
    }
    else
    {
        T = glm::translate(glm::mat4(1.f), glm::vec3(current_position, -0.7f + hop, -2.f));
        R_dead_bunny = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0, 0, -1)); // added
        R = glm::rotate(glm::mat4(1.f), glm::radians(270.f), glm::vec3(0, 1, 0));
        S = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1)); // added

        modelMat = T * R_dead_bunny * R * S;
        modelMatInv = glm::transpose(glm::inverse(modelMat));
        perspMat = glm::perspective(glm::radians(45.0f), 1.f, 1.0f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(gProgram[0], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
        glUniformMatrix4fv(glGetUniformLocation(gProgram[0], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
        glUniformMatrix4fv(glGetUniformLocation(gProgram[0], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(perspMat));

        drawModel(bunnyVertexAttribBuffer, bunnyIndexBuffer, bunnyVertexDataSizeInBytes, bunnyFaces);
    }

    // DRAW GROUND

    glUseProgram(gProgram[1]);

    T = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.f, -20.f));
    R = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(-1, 0, 0));
    S = glm::scale(glm::mat4(1.f), glm::vec3(1.5f, 20.f, 1.f)); // added
    modelMat = T * R * S;
    modelMatInv = glm::transpose(glm::inverse(modelMat));

    glUniformMatrix4fv(glGetUniformLocation(gProgram[1], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[1], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[1], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(perspMat));
    if (!bunny_dead){
        glUniform1f(glGetUniformLocation(gProgram[1], "speed"), (glfwGetTime()-resetTime)*speed*200); // bu hayattayken çalışmalı
    }
    
    drawModel(quadVertexAttribBuffer, quadIndexBuffer, quadVertexDataSizeInBytes, quadFaces);

    
    if(!bunny_dead){
        cube_pos += coming;
    }
    
    if (cube_pos > -2.f)
    {
        cube_pos = -30.f;
        reset_box = true;
        draw_cube0 = draw_cube1 = draw_cube2 = true;
    }
    //DRAW CUBE 0
    if (reset_box) {
        std::srand(glfwGetTime());
        random_box = std::rand() % 3; // 0,1,2 üretiyor
        reset_box = false;
    }
    if(random_box==0){
        glUseProgram(gProgram[4]); //program with yellow shader
        if (cube_pos > -3.5f && current_position < 0.f + 0.2f && current_position > 0.f - 0.2f) {
            being_happy = true;
            score += 10000;
            draw_cube0 = false;
		}
    }
    else{
        glUseProgram(gProgram[3]); //program with red shader
        if (cube_pos > -3.5f && current_position < 0.f + 0.2f && current_position > 0.f - 0.2f) {
            bunny_dead = true;
            draw_cube0 = false;
		}
    }

    T = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.0f, cube_pos)); // 1. yere birincinin x ini yaz
    R = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(-1, 0, 0));
    S = glm::scale(glm::mat4(1.f), glm::vec3(0.1f, 0.5f, 0.75f)); 
    modelMat = T * R * S;
    modelMatInv = glm::transpose(glm::inverse(modelMat));

    glUniformMatrix4fv(glGetUniformLocation(random_box == 0 ? gProgram[4] : gProgram[3], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(random_box == 0 ? gProgram[4] : gProgram[3], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
    glUniformMatrix4fv(glGetUniformLocation(random_box == 0 ? gProgram[4] : gProgram[3], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(perspMat));

    if (draw_cube0) drawModel(cubeVertexAttribBuffer, cubeIndexBuffer, cubeVertexDataSizeInBytes, cubeFaces);

    //DRAW CUBE 1

    if (random_box == 1) {
        glUseProgram(gProgram[4]); //program with yellow shader
        if (cube_pos > -3.5f && current_position < 1.f + 0.4f && current_position > 1.f - 0.4f) {
            being_happy = true;
            score += 10000;
            draw_cube1 = false;            
        }
    }
    else {
        glUseProgram(gProgram[3]); //program with red shader
        if (cube_pos > -3.5f && current_position < 1.f + 0.4f && current_position > 1.f - 0.4f) {
            bunny_dead = true;
            draw_cube1 = false;
        }
    }

    T = glm::translate(glm::mat4(1.f), glm::vec3(1.f, -1.0f, cube_pos)); // 1. yere birincinin x ini yaz
    R = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(-1, 0, 0));
    S = glm::scale(glm::mat4(1.f), glm::vec3(0.1f, 0.5f, 0.75f));
    modelMat = T * R * S;
    modelMatInv = glm::transpose(glm::inverse(modelMat));

    glUniformMatrix4fv(glGetUniformLocation(random_box == 1 ? gProgram[4] : gProgram[3], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(random_box == 1 ? gProgram[4] : gProgram[3], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
    glUniformMatrix4fv(glGetUniformLocation(random_box == 1 ? gProgram[4] : gProgram[3], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(perspMat));

    if (draw_cube1) drawModel(cubeVertexAttribBuffer, cubeIndexBuffer, cubeVertexDataSizeInBytes, cubeFaces);

    //DRAW CUBE 2

    if (random_box == 2) {
        glUseProgram(gProgram[4]); //program with yellow shader
        if (cube_pos > -3.5f && current_position < -1.f + 0.4f && current_position > -1.f - 0.4f) {
            being_happy = true;
            score += 10000;
            draw_cube2 = false;
        }
    }
    else {
        glUseProgram(gProgram[3]); //program with red shader
        if (cube_pos > -3.5f && current_position < -1.f + 0.4f && current_position > -1.f - 0.4f) {
            bunny_dead = true;
            draw_cube2 = false;
        }
    }

    T = glm::translate(glm::mat4(1.f), glm::vec3(-1.f, -1.0f, cube_pos)); // 1. yere birincinin x ini yaz
    R = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(-1, 0, 0));
    S = glm::scale(glm::mat4(1.f), glm::vec3(0.1f, 0.5f, 0.75f));
    modelMat = T * R * S;
    modelMatInv = glm::transpose(glm::inverse(modelMat));

    glUniformMatrix4fv(glGetUniformLocation(random_box == 2 ? gProgram[4] : gProgram[3], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(random_box == 2 ? gProgram[4] : gProgram[3], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
    glUniformMatrix4fv(glGetUniformLocation(random_box == 2 ? gProgram[4] : gProgram[3], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(perspMat));

    if (draw_cube2) drawModel(cubeVertexAttribBuffer, cubeIndexBuffer, cubeVertexDataSizeInBytes, cubeFaces);



    assert(glGetError() == GL_NO_ERROR);
    if (bunny_dead) renderText("Score: " + std::to_string(score), 30, 900, 1.3f, glm::vec3(1, 0, 0));
    else renderText("Score: " + std::to_string(score), 30, 900, 1.3f, glm::vec3(1, 1, 1));

    assert(glGetError() == GL_NO_ERROR);

	//angle += 0.5;
    speed = (glfwGetTime() - resetTime) * 0.001f;
    if(!bunny_dead){
        score += speed * 5000;
    }
    coming = (glfwGetTime() - resetTime) * 0.013f;

    if(happy_angle >= 360){
        happy_angle = 0;
        being_happy = false;
    } 
    if(being_happy){
        happy_angle += speed * 500;
        //happy_angle += 10;
    }
}

void reshape(GLFWwindow* window, int w, int h)
{
    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    gWidth = w;
    gHeight = h;

    glViewport(0, 0, w, h);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   /* if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        cout << "F pressed" << endl;
        glUseProgram(gProgram[1]);
    }
    else if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        cout << "V pressed" << endl;
        glUseProgram(gProgram[0]);
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        cout << "D pressed" << endl;
        gIntensity /= 1.5;
        cout << "gIntensity = " << gIntensity << endl;
        glUseProgram(gProgram[0]);
        glUniform1f(gIntensityLoc, gIntensity);
    }
    else if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        cout << "B pressed" << endl;
        gIntensity *= 1.5;
        cout << "gIntensity = " << gIntensity << endl;
        glUseProgram(gProgram[0]);
        glUniform1f(gIntensityLoc, gIntensity);
    } */
    if (key == GLFW_KEY_D){
        
        pressing_D = true;
    }
    if (key == GLFW_KEY_A){
        pressing_A = true;
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE){
        pressing_D = false;
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE){
        pressing_A = false;
    }
    if (key == GLFW_KEY_R){
        startGameAgain = true;
        //std::srand(glfwGetTime());
        random_box = std::rand() % 3; // 0,1,2 üretiyor
        reset_box = false;
    }
}

void mainLoop(GLFWwindow* window)
{
    while (!glfwWindowShouldClose(window))
    {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int argc, char** argv)   // Create Main Function For Bringing It All Together
{
    GLFWwindow* window;
    if (!glfwInit())
    {
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(gWidth, gHeight, "Simple Example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    char rendererInfo[512] = {0};
    strcpy(rendererInfo, (const char*) glGetString(GL_RENDERER));
    strcat(rendererInfo, " - ");
    strcat(rendererInfo, (const char*) glGetString(GL_VERSION));
    glfwSetWindowTitle(window, rendererInfo);

    init();

    glfwSetKeyCallback(window, keyboard);
    glfwSetWindowSizeCallback(window, reshape);

    reshape(window, gWidth, gHeight); // need to call this once ourselves
    mainLoop(window); // this does not return unless the window is closed

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

