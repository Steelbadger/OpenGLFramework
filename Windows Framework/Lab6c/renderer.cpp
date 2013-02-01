#include "renderer.h"
#include "gameobject.h"

CameraModule* RenderManager::activeCamera;

RenderManager::RenderManager()
{
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
	GLuint tex;

	//  Only bother compiling a new Display List if one doesn't already exist for this object
	if (!UniqueIDToDListMap.count(m.GetUniqueID())) {
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
		//  Compile the object's Display List and remember that this object has been compiled to save repetition
		UniqueIDToDListMap[m.GetUniqueID()] = CompileToDisplayList(m, tex);
	}

	//  Add our new (or repeated) item to the render list
	renderList.push_back(m.GetUniqueID());

	return true;
}

void RenderManager::AddToRenderer(std::vector<Mesh> &meshList)
{
	std::vector<Mesh>::iterator it;

	if (meshList.size() > 0) {
		for (it = meshList.begin(); it != meshList.end(); it++) {
			AddToRenderer(*it);
		}
	}
}

void RenderManager::UpdateCamera()
{
	Vector4 Position = activeCamera->GetParent()->GetPosition();
	Vector4 upVector = activeCamera->GetParent()->GetLocalY();
	Vector4 LookAt = Position + activeCamera->GetParent()->GetLocalZ();

	//Vector4 Position = activeCamera->GetPosition();
	//Vector4 upVector = activeCamera->GetUp();
	//Vector4 LookAt = Position + activeCamera->GetForward();


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//calculate aspect ratio
	gluPerspective(activeCamera->GetFieldOfView(), (GLfloat)activeCamera->GetWindowWidth()/(GLfloat)activeCamera->GetWindowHeight(), activeCamera->GetNearClipPlane(), activeCamera->GetFarClipPlane());
	glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
	glLoadIdentity();
	gluLookAt((GLdouble)Position.x, (GLdouble)Position.y, (GLdouble)Position.z,
				(GLdouble)LookAt.x, (GLdouble)LookAt.y, (GLdouble)LookAt.z,
				(GLdouble)upVector.x, (GLdouble)upVector.y, (GLdouble)upVector.z);
}

void RenderManager::AddSkyBox(Mesh &m)
{
	std::string fn = m.GetTexturePath();
	GLuint tex;

	CreateGLTexture(fn.c_str(), tex);
	skyBoxTexture = tex;
	skyBox = CompileToDisplayList(m, tex);
}

void RenderManager::AddTerrainToRenderer(Terrain &t)
{
	std::string fn = t.GetTexturePath();
	GLuint tex;

//	terrainTexture = t.GetTexture();
//	terrain = t.GetDisplayList();

	CreateGLTexture(fn.c_str(), tex);
	terrainTexture = tex;
	terrain = CompileToDisplayList(t, tex);
}

void RenderManager::RemoveFromRenderer(Mesh m)
//  This function removes a specified mesh from the scene
//  It will not delete the associated texture or display list
{
	std::list<int>::iterator it;

	//  Check through list to find mesh we want to remove
	for(it = renderList.begin(); *it != m.GetUniqueID(); it++) {}

	//  remove it
	renderList.erase(it);
}

void RenderManager::RenderAll()
//  Iterates through all the items in the render list
//  translates the render context to the object's base position then
//  renders the object.
//  Objects are drawn in order, opaque first then transparent, with transparent objects
//  further away being drawn first
{
	std::list<int>::iterator it;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

//	UpdateCamera();

	if (glIsList(skyBox)) {
		glPushMatrix();
			glTranslatef(activeCamera->GetParent()->GetPosition().x, activeCamera->GetParent()->GetPosition().y, activeCamera->GetParent()->GetPosition().z);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			glBindTexture(GL_TEXTURE_2D, skyBoxTexture);
			glCallList(skyBox);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
		glPopMatrix();
	}

	if (glIsList(terrain)) {
		glBindTexture(GL_TEXTURE_2D, terrainTexture);
		glCallList(terrain);
	}

	float thetaY, thetaZ, thetaX;

	if (renderList.size() > 0) {
		for (it = renderList.begin(); it != renderList.end(); it++) {
			//  Get a pointer to the mesh we're about to draw, need this to find it's base position
			Mesh* m = Mesh::GetMeshPointer(*it);

			if (m != NULL) {
				Vector4 localX = m->GetParentPointer()->GetLocalX();
				Vector4 localY = m->GetParentPointer()->GetLocalY();

				//  Work out the rotations around each axis for render context rotations
				thetaY = acos(Vector4(localX.x, 0.0f, localX.z, 1.0f).Dot3(Vector4(1.0f, 0.0f, 0.0f, 1.0f)/Vector4(localX.x, 0.0f, localX.z, 1.0f).Length()))*180.0/Matrix4x4::PI;
				thetaZ = acos(Vector4(localX.x, localX.y, 0.0f, 1.0f).Dot3(Vector4(1.0f, 0.0f, 0.0f, 1.0f)/Vector4(localX.x, localX.y, 0.0f, 1.0f).Length()))*180.0/Matrix4x4::PI;
				thetaX = acos(Vector4(0.0f, localY.y, localY.z, 1.0f).Dot3(Vector4(0.0f, 1.0f, 0.0f, 1.0f)/Vector4(0.0f, localY.y, localY.z, 1.0f).Length()))*180.0/Matrix4x4::PI;

				glPushMatrix();
					glTranslatef(m->GetParentPointer()->GetPosition().x, m->GetParentPointer()->GetPosition().y, m->GetParentPointer()->GetPosition().z);
					glRotatef(thetaY, 0.0f, 1.0f, 0.0f);					
					glRotatef(thetaX, 1.0f, 0.0f, 0.0f);
					glRotatef(thetaZ, 0.0f, 0.0f, 1.0f);
					glBindTexture(GL_TEXTURE_2D, TextureMap[Mesh::GetMeshPointer(*it)->GetTexturePath()]);
					glCallList(UniqueIDToDListMap[*it]);
				glPopMatrix();
			} else {
				renderList.erase(it);
			}
		}
	}
}

GLuint RenderManager::CompileToDisplayList(Mesh &m, GLuint texture)
//  Generate the display list for the specified mesh, pass the GLuint back out for use
{
	GLuint dList = glGenLists(1);

	glNewList(dList,GL_COMPILE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, m.GetVertexArrayBase());
		glNormalPointer(GL_FLOAT, 0, m.GetNormalArrayBase());
		glTexCoordPointer(2,GL_FLOAT,0, m.GetUVArrayBase());
		glDrawArrays(GL_TRIANGLES, 0, m.GetTriangleNumber());
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEndList();

	return dList;
}

GLuint RenderManager::CompileToDisplayList(Terrain &t, GLuint texture)
{
	GLuint dList = glGenLists(1);

	glNewList(dList,GL_COMPILE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, t.GetVertexArrayBase());
		glNormalPointer(GL_FLOAT, 0, t.GetNormalArrayBase());
		glTexCoordPointer(2,GL_FLOAT,0, t.GetUVArrayBase());
		glDrawArrays(GL_TRIANGLES, 0, t.GetNumberOfVerts());
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEndList();

	return dList;
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

