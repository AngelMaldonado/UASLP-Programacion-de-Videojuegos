#include "../../include/geometry.h"
#include "../../include/transformations.h"

Object::Object(){}

Object::Object(vector<Mesh> meshes) {
    this->meshes = meshes;

    // Find a way to compute the center of the object to the fine the object's origin
    origin.x = origin.y = origin.z = 0;
    
    drawOrigin = true;
    brush.lineStyle = GL_LINE_LOOP;
    brush.red = brush.green = brush.blue = 1;
}

vector<Mesh> &Object::getMeshes() {
    return meshes;
}

Coordinates &Object::getOrigin() {
    return origin;
}

void Object::setOrigin(float x, float y, float z) {
    TransformationsHandler::translateObject(*this, x - origin.x, y - origin.y, z - origin.z);
    origin.x = x;
    origin.y = y;
    origin.z = z;
}

void Object::showOrigin() {
    drawOrigin = true;
}

void Object::hideOrigin() {
    drawOrigin = false;
}

void Object::draw() {
    // Local array of primitive coorinates of vertices (x, y, z)
    double* vertices;

    // Get the prp value and store it in a Coordinate structure
    Coordinates prp;
    prp.x = getPRP()[0];
    prp.y = getPRP()[1];
    prp.z = getPRP()[2];

    // For each stored mesh in vector<Mesh>
    for(Mesh mesh : meshes)
    {
        // For each Face that mesh has
        for (Face face : mesh.getFaces()) 
        {
            if(face.visible(prp)) {

                Vertex normalUnitary = face.getNormalVectorAsUnitary();
                IRGB I;
                IRGB R;
                IRGB RbyPRP;
                IRGB NbyL;
                IRGB RbyPRPtoN;
                NbyL.R = normalUnitary.getX() * lightX;
                NbyL.G = normalUnitary.getY() * lightY;
                NbyL.B = normalUnitary.getZ() * lightZ;

                R.R = (2 * NbyL.R * normalUnitary.getX()) - lightX;
                R.G = (2 * NbyL.G * normalUnitary.getY()) - lightY;
                R.B = (2 * NbyL.B * normalUnitary.getZ()) - lightZ;

                RbyPRP.R = R.R * getPRP()[0]; if(RbyPRP.R < 0) RbyPRP.R = 0;
                RbyPRP.G = R.G * getPRP()[1]; if(RbyPRP.G < 0) RbyPRP.G = 0;
                RbyPRP.B = R.B * getPRP()[2]; if(RbyPRP.B < 0) RbyPRP.B = 0;
                RbyPRPtoN.R = RbyPRP.R, RbyPRPtoN.G = RbyPRP.G, RbyPRPtoN.B = RbyPRP.B;

                for(int i = 1; i <= n; i++)
                {
                    RbyPRPtoN.R *= RbyPRP.R;
                    RbyPRPtoN.G *= RbyPRP.G;
                    RbyPRPtoN.B *= RbyPRP.B;
                }

                I.R = Ka * Ia.R + Kd * Id.R * NbyL.R + Ks * Is.R * RbyPRPtoN.R + brush.red;
                I.G = Ka * Ia.G + Kd * Id.G * NbyL.G + Ks * Is.G * RbyPRPtoN.G + brush.green;
                I.B = Ka * Ia.B + Kd * Id.B * NbyL.B + Ks * Is.B * RbyPRPtoN.B + brush.blue;

                //cout << R.R << ' ' << R.G << ' ' << R.B << endl;
                //cout << I.R << ' ' << I.G << ' ' << I.B << endl;
                //normalUnitary.showCoordinatesFormatted();

                // Define color for mesh
                glColor3f (I.R, I.G, I.B);
                //glColor3f(brush.red, brush.green, brush.blue);

                glBegin(brush.lineStyle);
                for(int vertex : face.getVerticesIndices())
                {
                    vertices = mesh.getVertex(vertex).getCoordinates();
                    glVertex3f(vertices[0], vertices[1], vertices[2]);
                }
                glEnd();
            }
        }
    }

    if(drawOrigin) {
        glPointSize(3);
        glColor3f(0, 1, 0);
        glBegin(GL_POINTS);
            glVertex3i(round(origin.x), round(origin.y), round(origin.z));
        glEnd();
    }
}