#include "renderer.h"
#include "gameobject.h"
#include <iostream>
#include <fstream>

CameraModule* RenderManager::activeCamera;

RenderManager::RenderManager()
{
	opaqueRenderList.reserve(2048);
}

RenderManager::~RenderManager(void)
{
}

void RenderManager::MaintainRenderList()
//  Ensure the renderList is sorted in the right order
{
	renderList.sort(RenderManager::MeshComparator);
}

bool RenderManager::AddToRenderer(Mesh &m)
//  Adds specified mesh to the render list
//  If the mesh is brand new it will import the texture and compile the
//  display list needed to draw the mesh, if the mesh or texture has been
//  Used before they will not be reimported/compiled and we will instead use
//  the old versions
{
	std::string fn = m.GetTexturePath();
	std::string meshModel = m.GetMeshSourceFilePath();
	std::string vertexShader = m.GetVertexShader();
	std::string fragmentShader = m.GetFragmentShader();
	GLuint tex;

	//  Only bother compiling a new Display List if one doesn't already exist for this object
	if (!VAOMap.count(m.GetUniqueID())) {
		if (!MeshFileMap.count(meshModel)) {
			//  Compile the object's Display List and remember that this object has been compiled to save repetition
			MeshFileMap[meshModel] = SetupVAO(m);
		} else {
			VAOMap[m.GetUniqueID()] = MeshFileMap[meshModel];
		}

		//  Only bother importing the texture if this texture has not already been imported
		if (!TextureMap.count(fn)) {
			//  Check file extension, included for possible extension to other formats
			if(fn.substr(fn.find_last_of(".") + 1) == "tga") {
				//  Load the texture and put it in our temporary holder 'tex'
				CreateGLTexture(fn.c_str(), tex);
			} else {
				//  If the file type is not recognised then stop
				return false;
			}
			//  Keep a note that this file has been imported to save repetitions
			TextureMap[fn] = tex;
		} else {
			//  If the texture has been imported before then use that old version to save importing again
			tex = TextureMap[fn];
		}
	}

	SetShaders(vertexShader, fragmentShader);

	//  Add our new (or repeated) item to the render list
	if(m.IsTransparent()) {
		renderList.push_back(m.GetUniqueID());
	} else {
		opaqueRenderList.push_back(m.GetUniqueID());
	}

	return true;
}

void RenderManager::BuildDefaultShaderProgram()
{
	if (glIsProgram(defaultShaderProgram) == GL_FALSE) {
		std::string vertex = "default.vertexshader";
		std::string fragment = "default.fragmentshader";

		//  load the two shaders
		LoadShader(vertex);
		LoadShader(fragment);

		//  Create a new program reference and attach our shaders
		GLuint program = glCreateProgram();

		glAttachShader(program, ShaderMap[vertex]);
		glAttachShader(program, ShaderMap[fragment]);
		glLinkProgram(program);


		//  Check the infolog for errors
		GLint result = GL_FALSE;
		int logLength;

		glGetProgramiv(program, GL_LINK_STATUS, &result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> ProgramErrorMessage(logLength);
		glGetProgramInfoLog(program, logLength, NULL, &ProgramErrorMessage[0]);
		std::string output(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
		std::cout << output << std::endl;

		//  If we failed to link then quit before adding the program
		if (result == GL_FALSE) {
			return;
		}
		defaultShaderProgram = program;
	}
}

void RenderManager::AddSkyBox(Mesh &m)
{
	std::string fn = m.GetTexturePath();
	GLuint tex;

	CreateGLTexture(fn.c_str(), tex);
	skyBoxTexture = tex;
	skyBox = SetupVAO(m);
	skyboxShaderProgram = CreateShaderProgram("skybox.vertexshader", "skybox.fragmentshader");
}

void RenderManager::AddTerrainToRenderer(Terrain &t)
{
	std::string fn = t.GetTexturePath();
	GLuint tex;

	CreateGLTexture(fn.c_str(), tex);
	terrainTexture = tex;
	terrain = SetupVAO(t);
	terrainVerts = t.GetNumberOfVerts();
	terrainShaderProgram = CreateShaderProgram(t.GetVertexShader(), t.GetFragmentShader());
}

void RenderManager::RemoveFromRenderer(Mesh m)
//  This function removes a specified mesh from the scene
//  It will not delete the associated texture or display list
{
	if (m.IsTransparent()) {
		std::list<int>::iterator it;
		//  Check through list to find mesh we want to remove
		for(it = renderList.begin(); *it != m.GetUniqueID(); it++) {}
		//  remove it
		renderList.erase(it);
	} else {
		std::vector<int>::iterator it;
		for(it = opaqueRenderList.begin(); *it != m.GetUniqueID(); it++) {}
		//  remove it
		opaqueRenderList.erase(it);
	}
}

void RenderManager::RenderAll()
//  Iterates through all the items in the render list
//  translates the render context to the object's base position then
//  renders the object.
//  Objects are drawn in order, opaque first then transparent, with transparent objects
//  further away being drawn first
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	BuildProjectionMatrix();

	DrawSkyBox();

	DrawTerrain();

	std::vector<int>::iterator vit;
	if (opaqueRenderList.size() > 0) {
		for (vit = opaqueRenderList.begin(); vit != opaqueRenderList.end(); vit++) {
			if (!DrawMesh(*vit)) {
				opaqueRenderList.erase(vit);
			}
		}
	}
	std::list<int>::iterator lit;
	if (renderList.size() > 0) {
		for (lit = renderList.begin(); lit != renderList.end(); lit++) {
			if (!DrawMesh(*lit)) {
				renderList.erase(lit);
			}
		}
	}
}

GLuint RenderManager::SetupVAO(Mesh &m)
{

	//  Create a VertexArrayObject (like a display list for buffer objects) and set it to current
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//  Make our vertex buffer, pass in the vertex data
	GLuint vertBuffer;
	glGenBuffers(1, &vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, m.GetSizeOfVerts(), m.GetVertexArrayBase(), GL_STATIC_DRAW);
	

	//  Make our normal buffer, pass in the normals
	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m.GetSizeOfNormals(), m.GetNormalArrayBase(), GL_STATIC_DRAW);

	//  Make our UV buffer, pass in the UV coords
	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, m.GetSizeOfUVs(), m.GetUVArrayBase(), GL_STATIC_DRAW);

	//  bind the vertex buffer to location 0 (layout(location = 0)) in the shaders
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//  bind the normal buffer to location 1 (layout(location = 1)) in the shaders
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//  bind the vertex buffer to location 2 (layout(location = 2)) in the shaders
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//  Unbind our VAO so we don't mess with it
	glBindVertexArray(0);

	//  Pass the reference out to be used later
	return vao;
}

GLuint RenderManager::SetupVAO(Terrain &t)
{

	//  Create a VertexArrayObject (like a display list for buffer objects) and set it to current
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//  Make our vertex buffer, pass in the vertex data
	GLuint vertBuffer;
	glGenBuffers(1, &vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, t.GetNumberOfVerts()*sizeof(Vector3), t.GetVertexArrayBase(), GL_STATIC_DRAW);
	

	//  Make our normal buffer, pass in the normals
	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, t.GetNumberOfVerts()*sizeof(Vector3), t.GetNormalArrayBase(), GL_STATIC_DRAW);

	//  Make our UV buffer, pass in the UV coords
	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, t.GetNumberOfVerts()*sizeof(Vector2), t.GetUVArrayBase(), GL_STATIC_DRAW);

	//  bind the vertex buffer to location 0 (layout(location = 0)) in the shaders
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//  bind the normal buffer to location 1 (layout(location = 1)) in the shaders
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//  bind the vertex buffer to location 2 (layout(location = 2)) in the shaders
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//  Unbind our VAO so we don't mess with it
	glBindVertexArray(0);

	//  Pass the reference out to be used later
	return vao;
}

bool RenderManager::MeshComparator(int rhs, int lhs)
//  A function for ordering the renderList
//  Places all NON-TRANSPARENT meshes at the start of the list, generally unordered
//  And places TRANSPARENT meshes at the end of the list, ordered by distance from
//  primary camera
{

	//  If both have transparency then find their position relative to the camera and sort based on distance, furthest first
	if(Mesh::GetMeshPointer(rhs)->IsTransparent() == true && Mesh::GetMeshPointer(lhs)->IsTransparent() == true) {
		Vector4 rhsDist = Mesh::GetMeshPointer(rhs)->GetParentPointer()->GetPosition() - activeCamera->GetParent()->GetPosition();
		Vector4 lhsDist = Mesh::GetMeshPointer(lhs)->GetParentPointer()->GetPosition() - activeCamera->GetParent()->GetPosition();

		return rhsDist.LengthSqr() > lhsDist.LengthSqr();
	//  If only one is transparent then place NON-TRANSPARENT one first
	} else if (Mesh::GetMeshPointer(rhs)->IsTransparent() == true || Mesh::GetMeshPointer(lhs)->IsTransparent() == true) {
		//  If rhs is transparent (true) it needs to go last, so false.  If rhs isn't transparent (false) then it needs to go first, so true
		return !Mesh::GetMeshPointer(rhs)->IsTransparent();
	} else {
		//  If both are NON-TRANSPARENT then order doesn't matter
		return true;
	}
}


Matrix4x4 RenderManager::BuildModelMatrix(GameObject g)
{
	Matrix4x4 translation(Matrix4x4::IDENTITY);
	Matrix4x4 rotation(Matrix4x4::IDENTITY);
	Matrix4x4 scale(Matrix4x4::IDENTITY);

	translation.Translation(g.GetPosition());
	rotation = g.GetRotation().GetRotationMatrix();
	scale.Scale(g.GetScale());
	Matrix4x4 result = scale * rotation * translation;

	ConvertToOpenGLMatrix(result, modelMatrix);
	return result;
}

Matrix4x4 RenderManager::BuildViewMatrix()
{
	Vector4 position = activeCamera->GetParent()->GetPosition();
	Vector4 upVector = activeCamera->GetParent()->GetLocalY();
	Vector4 lookAt = position + activeCamera->GetParent()->GetLocalZ();

	Matrix4x4 view;

	view.LookAt(position, lookAt, upVector);
	ConvertToOpenGLMatrix(view, viewMatrix);
	return view;
}

void RenderManager::BuildModelViewMatrix(GameObject g)
{
	Matrix4x4 mvMatrix;
	mvMatrix = BuildModelMatrix(g) * BuildViewMatrix();
	ConvertToOpenGLMatrix(mvMatrix, modelViewMatrix);


	float mv[3][3];
	mv[0][0] = mvMatrix(0,0);
	mv[1][1] = mvMatrix(1,1);
	mv[2][2] = mvMatrix(2,2);
	mv[1][0] = mvMatrix(0,1);
	mv[2][0] = mvMatrix(0,2);
	mv[2][1] = mvMatrix(1,2);
	mv[0][1] = mvMatrix(1,0);
	mv[0][2] = mvMatrix(2,0);
	mv[1][2] = mvMatrix(2,1);

	float det = mv[0][0] * (mv[1][1]*mv[2][2] - mv[1][2]*mv[2][1]) - mv[0][1] * (mv[1][0] * mv[2][2] - mv[1][2] * mv[2][0]) + mv[0][2] * (mv[1][0] * mv[2][1] - mv[1][1] * mv[2][0]);

	mv[0][0] = (mv[1][1]*mv[2][2] - mv[1][2]*mv[2][1])/det;
	mv[0][1] = -(mv[0][2]*mv[2][1] - mv[0][1]*mv[2][2])/det;
	mv[0][2] = (mv[0][1]*mv[1][2] - mv[0][2]*mv[1][1])/det;
	mv[1][0] = -(mv[1][2]*mv[2][0] - mv[1][0]*mv[2][2])/det;
	mv[1][1] = (mv[0][0]*mv[2][2] - mv[0][2]*mv[2][0])/det;
	mv[1][2] = -(mv[0][2]*mv[1][0] - mv[0][0]*mv[1][2])/det;
	mv[2][0] = (mv[1][0]*mv[2][1] - mv[1][1]*mv[2][0])/det;
	mv[2][1] = -(mv[0][1]*mv[2][0] - mv[0][0]*mv[2][1])/det;
	mv[2][2] = (mv[0][0]*mv[1][1] - mv[0][1]*mv[1][0])/det;

	float test = mv[0][0] * (mv[1][1]*mv[2][2] - mv[1][2]*mv[2][1]) - mv[0][1] * (mv[1][0] * mv[2][2] - mv[1][2] * mv[2][0]) + mv[0][2] * (mv[1][0] * mv[2][1] - mv[1][1] * mv[2][0]);


	//  WRONG!

	Matrix4x4 normMatrix(Matrix4x4::IDENTITY);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			normMatrix.elem[i][j] = mv[i][j];
		}
	}

	ConvertToOpenGLMatrix(normMatrix, normalMatrix);
}

void RenderManager::BuildSkyBoxViewMatrix(GameObject g)
{
	g.SetRotate(0.0f, 0.0f, 0.0f);
	Matrix4x4 mat;
	mat = BuildModelMatrix(g) * BuildViewMatrix();
	ConvertToOpenGLMatrix(mat, modelViewMatrix);
}

void RenderManager::BuildProjectionMatrix()
{
	Matrix4x4 projection;
	projection.Projection(activeCamera->GetFieldOfView(), (GLfloat)activeCamera->GetWindowWidth()/(GLfloat)activeCamera->GetWindowHeight(), activeCamera->GetNearClipPlane(), activeCamera->GetFarClipPlane());
	ConvertToOpenGLMatrix(projection, projectionMatrix);
}

void RenderManager::ConvertToOpenGLMatrix(Matrix4x4 m, GLfloat* target)
{
	target[0] = m.elem[0][0];
	target[1] = m.elem[0][1];
	target[2] = m.elem[0][2];
	target[3] = m.elem[0][3];
	target[4] = m.elem[1][0];
	target[5] = m.elem[1][1];
	target[6] = m.elem[1][2];
	target[7] = m.elem[1][3];
	target[8] = m.elem[2][0];
	target[9] = m.elem[2][1];
	target[10] = m.elem[2][2];
	target[11] = m.elem[2][3];
	target[12] = m.elem[3][0];
	target[13] = m.elem[3][1];
	target[14] = m.elem[3][2];
	target[15] = m.elem[3][3];
}

bool RenderManager::LoadShader(std::string fileName)
{
	std::string fn = fileName;
	if (fn.empty()) {
		return false;
	}

	//  Check we haven't already done this file
	if (!ShaderMap.count(fn)) {
		GLuint shader;

		//  Get the file extension so that we know what kind of shader it is
		std::string type = fn.substr(fn.find_last_of(".") + 1);

		//  depending on extension, create a shader ref of the corresponding type
		if (type == "vertexshader") {
			shader = glCreateShader(GL_VERTEX_SHADER);
		} else if (type == "fragmentshader") {
			shader = glCreateShader(GL_FRAGMENT_SHADER);
		} else {
			//  If the file extension is not correct then return an error and stop
			std::cout << "Unrecognised Shader File Extension: " << type << std::endl;
			return false;
		}

		std::string code;
		std::ifstream fileStream(fn, std::ios::in);
		if (!fileStream.is_open()) {
			std::cout << "Could not open shader file : " << fileName << std::endl;
			return false;
		}
		//  Load in the code to a stream
		std::string line = "";
		while(getline(fileStream, line)) {
			code += "\n" + line;
		}
		fileStream.close();

		//  compile that code
		std::cout << "Compiling " << fn << std::endl;
		char const * codePointer = code.c_str();
		glShaderSource(shader, 1, &codePointer, NULL);
		glCompileShader(shader);


		//  Check GLSL compiler output for errors
		GLint result = GL_FALSE;
		int logLength;

		// Check Vertex Shader
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> ShaderErrorMessage(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, &ShaderErrorMessage[0]);
		std::string output(ShaderErrorMessage.begin(), ShaderErrorMessage.end());
		std::cout << output << std::endl;


		//  If we failed to compile the shader quit without adding the the shadermap
		if (result == GL_FALSE) {
			return false;
		}

		ShaderMap[fn] = shader;
	}
	return true;
}

GLuint RenderManager::CreateShaderProgram(std::string vertex, std::string fragment)
{
	//  load the two shaders
	if (!LoadShader(vertex) || !LoadShader(fragment)) {
		BuildDefaultShaderProgram();
		return defaultShaderProgram;
	}

	//  Create a new program reference and attach our shaders
	GLuint program = glCreateProgram();

	glAttachShader(program, ShaderMap[vertex]);
	glAttachShader(program, ShaderMap[fragment]);
	glLinkProgram(program);


	//  Check the infolog for errors
	GLint result = GL_FALSE;
	int logLength;

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> ProgramErrorMessage(logLength);
	glGetProgramInfoLog(program, logLength, NULL, &ProgramErrorMessage[0]);
	std::string output(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
	std::cout << output << std::endl;

	//  If we failed to link then quit and return the default shader
	if (result == GL_FALSE) {
		BuildDefaultShaderProgram();
		return defaultShaderProgram;
	}
	return program;
}

void RenderManager::SetShaders(std::string vertex, std::string fragment)
{
	if (vertex.empty() || fragment.empty()) {
		BuildDefaultShaderProgram();
		currentShaderProgram = defaultShaderProgram;
		return;		
	}
	std::string type1 = vertex.substr(vertex.find_last_of(".") + 1);
	std::string type2 = fragment.substr(vertex.find_last_of(".") + 1);

	std::string name1 = vertex.substr(0, vertex.find_last_of("."));
	std::string name2 = fragment.substr(0, fragment.find_last_of("."));

	std::string fn;

	if (type1 == "vertexshader" && type2 == "fragmentshader") {
		fn = name1 + name2;
	} else if (type1 == "fragmentshader" && type2 == "vertexshader") {
		fn = name2 + name1;
		std::string swap = fragment;
		fragment = vertex;
		vertex = swap;
	} else {
		std::cout  << "Cannot create program from " << vertex << " and " << fragment << " check file names" << std::endl;
		BuildDefaultShaderProgram();
		currentShaderProgram = defaultShaderProgram;
		return;
	}	


	if (!ShaderProgramMap.count(fn)) {
		//  load the two shaders
		if (!LoadShader(vertex) || !LoadShader(fragment)) {
			BuildDefaultShaderProgram();
			currentShaderProgram = defaultShaderProgram;
			return;
		}

		//  Create a new program reference and attach our shaders
		GLuint program = glCreateProgram();

		glAttachShader(program, ShaderMap[vertex]);
		glAttachShader(program, ShaderMap[fragment]);
		glLinkProgram(program);


		//  Check the infolog for errors
		GLint result = GL_FALSE;
		int logLength;

		glGetProgramiv(program, GL_LINK_STATUS, &result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> ProgramErrorMessage(logLength);
		glGetProgramInfoLog(program, logLength, NULL, &ProgramErrorMessage[0]);
		std::string output(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
		std::cout << output << std::endl;

		//  If we failed to link then quit before adding the program
		if (result == GL_FALSE) {
			BuildDefaultShaderProgram();
			currentShaderProgram = defaultShaderProgram;
			return;
		}
		ShaderProgramMap[fn] = program;
	}

	currentShaderProgram = ShaderProgramMap[fn];
}

void RenderManager::DrawSkyBox()
{

	//  Use the default Shader (no lighting)
	glUseProgram(skyboxShaderProgram);

	//  Find the location in gfx card memory of the matrix variables we wish to pass in
	GLuint ProjectMatrixID = glGetUniformLocation(skyboxShaderProgram, "projectionMatrix");
	GLuint ViewMatrixID = glGetUniformLocation(skyboxShaderProgram, "viewMatrix");
	GLuint ModelMatrixID = glGetUniformLocation(skyboxShaderProgram, "modelMatrix");
	GLuint ModelViewMatrixID = glGetUniformLocation(skyboxShaderProgram, "modelViewMatrix");

	//  find the location in gfx card memory of the texture we wish to pass in
	GLuint TextureID  = glGetUniformLocation(skyboxShaderProgram, "texture");

	//  Make our MVP matrices
	BuildProjectionMatrix();
	BuildSkyBoxViewMatrix(*activeCamera->GetParent());

	//  Pass them into the locations we found earlier
	glUniformMatrix4fv(ProjectMatrixID, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(ModelViewMatrixID, 1, GL_FALSE, modelViewMatrix);

	
	//  We're using Texture unit Zero
	glActiveTexture(GL_TEXTURE0);
	//  Put our texture into unit zero
	glBindTexture(GL_TEXTURE_2D, skyBoxTexture);
	// Set our texture variable in the shader to use unit zero
	glUniform1i(TextureID, 0);

	//  draw the skybox
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(skyBox);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//  unbind our shaders and arrays
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}

void RenderManager::DrawTerrain()
{
	glUseProgram(terrainShaderProgram);

	//  Find the location in gfx card memory of the matrix variables we wish to pass in
	GLuint ProjectMatrixID = glGetUniformLocation(terrainShaderProgram, "projectionMatrix");
	GLuint ViewMatrixID = glGetUniformLocation(terrainShaderProgram, "viewMatrix");
	GLuint ModelMatrixID = glGetUniformLocation(terrainShaderProgram, "modelMatrix");
	GLuint ModelViewMatrixID = glGetUniformLocation(terrainShaderProgram, "modelViewMatrix");
	GLuint NormalMatrixID = glGetUniformLocation(terrainShaderProgram, "normalMatrix");
	GLuint LightPositionID = glGetUniformLocation(terrainShaderProgram, "lightpos");

	//  find the location in gfx card memory of the texture we wish to pass in
	GLuint TextureID  = glGetUniformLocation(terrainShaderProgram, "texture");

	//  Make our MVP matrices
	BuildProjectionMatrix();
	BuildModelViewMatrix(base);

	Vector3 lightPos = activeCamera->GetParent()->GetPosition();
	float light[3] = {lightPos.x, lightPos.y, lightPos.z};

	//  Pass them into the locations we found earlier
	glUniformMatrix4fv(ProjectMatrixID, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(ModelViewMatrixID, 1, GL_FALSE, modelViewMatrix);
	glUniformMatrix4fv(NormalMatrixID, 1, GL_FALSE, normalMatrix);
	glUniform3fv(LightPositionID, 1, light);

	//  We're using Texture unit Zero
	glActiveTexture(GL_TEXTURE0);
	//  Put our texture into unit zero
	glBindTexture(GL_TEXTURE_2D, terrainTexture);
	// Set our texture variable in the shader to use unit zero
	glUniform1i(TextureID, 0);

	//  draw the skybox
	glBindVertexArray(terrain);
	glDrawArrays(GL_TRIANGLES, 0, terrainVerts);

	//  unbind our shaders and arrays
	glBindVertexArray(0);
	glUseProgram(0);

}

bool RenderManager::DrawMesh(int meshID)
{
	Mesh* m = Mesh::GetMeshPointer(meshID);
	SetShaders(m->GetVertexShader(), m->GetFragmentShader());

	if (m != NULL) {
		//  Use the default Shader (no lighting)
		glUseProgram(currentShaderProgram);

		//  Find the location in gfx card memory of the matrix variables we wish to pass in
		GLuint ProjectMatrixID = glGetUniformLocation(currentShaderProgram, "projectionMatrix");
		GLuint ViewMatrixID = glGetUniformLocation(currentShaderProgram, "viewMatrix");
		GLuint ModelMatrixID = glGetUniformLocation(currentShaderProgram, "modelMatrix");
		GLuint ModelViewMatrixID = glGetUniformLocation(currentShaderProgram, "modelViewMatrix");
		GLuint NormalMatrixID = glGetUniformLocation(currentShaderProgram, "normalMatrix");
		GLuint LightPositionID = glGetUniformLocation(currentShaderProgram, "lightpos");

		//  find the location in gfx card memory of the texture we wish to pass in
		GLuint TextureID  = glGetUniformLocation(currentShaderProgram, "texture");

		//  Make our MVP matrices
		BuildProjectionMatrix();
		BuildModelViewMatrix(*m->GetParentPointer());

		Vector3 lightPos = activeCamera->GetParent()->GetPosition();
		float light[3] = {lightPos.x, lightPos.y, lightPos.z};

		//  Pass them into the locations we found earlier
		glUniformMatrix4fv(ProjectMatrixID, 1, GL_FALSE, projectionMatrix);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(ModelViewMatrixID, 1, GL_FALSE, modelViewMatrix);
		glUniformMatrix4fv(NormalMatrixID, 1, GL_FALSE, normalMatrix);
		glUniform3fv(LightPositionID, 1, light);

		//  We're using Texture unit Zero
		glActiveTexture(GL_TEXTURE0);
		//  Put our texture into unit zero
		glBindTexture(GL_TEXTURE_2D, TextureMap[m->GetTexturePath()]);
		// Set our texture variable in the shader to use unit zero
		glUniform1i(TextureID, 0);

		//  draw the skybox
		glBindVertexArray(VAOMap[meshID]);
		glDrawArrays(GL_TRIANGLES, 0, m->GetNumberOfVerts());

		//  unbind our shaders and arrays
		glBindVertexArray(0);
		glUseProgram(0);
		return true;
	} else {
		return false;
	}
}
