#include "../../include/objreader.h"

vector<Mesh> OBJFileReader::readFile(string fileName, char printReading) {
    vector<Mesh> meshes;
    // Checks the extension of the file
    if (fileName.substr(fileName.find_last_of(".") + 1) == "obj") {

        // Read the user-file
        fileReader.open(fileName, std::fstream::in | std::fstream::out);

        // If the file was successfully opened
        if (fileReader.is_open()) {

            // Local string to iterate through the file
            string lineContent;
            // Local vector of doubles to store the vertices read in file
            vector <double> dblVerticesRead;
            // Local vector of Vertices to store a face's vertices
            vector <int> faceVerticesIndices;
            // Variable that stores the index of the las Vertex that was added to a mesh
            int lastVertexIndex;

            if(printReading == 's')
            {
                // Show in console that file was successfully opened
                cout << "**********************************************" << '\n';
                cout << "******** Successfully opened the file ********" << '\n';
                cout << "**********************************************" << "\n\n";
            }
            
            // Store the file's name in fileName attribute
            this->fileName = fileName;

            // Read the file line by line
            while (getline(fileReader, lineContent)) {

                // If the first position of the content on the line read is a comment ('#')
                if (lineContent[0] == '#' && printReading == 's')
                    cout << "<<Found a comment (#)>>" << "\n\n" << lineContent << "\n\n";
                else

                    // Else, read the 3D object's info
                    switch (lineContent[0]) {

                        // Object case
                        case 'o':
                            if(printReading == 's')
                            {
                                cout << "*********************************" << '\n';
                                cout << "****** Found an object (o) ******" << '\n';
                                cout << "*********************************" << "\n\n";
                            }

                            // If there is more than one mesh in file, continue the vertexIndex with last value set
                            if(meshes.size() > 0) {
                                lastVertexIndex = meshes.back().getVertexIndex();
                                meshes.push_back(Mesh(getMeshNameWithLine(lineContent), lastVertexIndex));
                            }
                            else {
                                // Push a new Mesh to the Mesh vector
                                meshes.push_back(Mesh(getMeshNameWithLine(lineContent)));
                                lastVertexIndex = 0;
                            }

                            // Print the name from the new Mesh at Mesh vector
                            if(printReading == 's')
                            {
                                cout << ">> Object identifier: " << meshes.back().getMeshName() << '\n';
                                cout << " --- Successfully stored in vector<Mesh> ---" << "\n\n";
                            }
                            break;

                            // Vertex case
                        case 'v':
                            // Reset the dblVerticesRead
                            dblVerticesRead.clear();

                            if(printReading == 's')
                            {
                                cout << "*********************************" << '\n';
                                cout << "******* Found a vertex (v) ******" << '\n';
                                cout << "*********************************" << "\n\n";
                            }

                            // Get and push a new Vertex to the verticesRead vector
                            dblVerticesRead = getVertexCoordinatesWithLine(lineContent);

                            // Add the Vertex read to the current mesh
                            if(meshes.back().addVertex(dblVerticesRead)) {
                                if (printReading == 's')
                                {
                                    cout << "Vertex: ";
                                    meshes.back().getLastVertex().showCoordinatesFormatted();
                                    cout << "--- Successfully stored in mesh " << meshes.back().getMeshName();
                                    cout << "---" << "\n\n";
                                }
                            } else cout << "~~~ Invalid vertex ~~~" << "\n\n";
                            break;

                            // Face case
                        case 'f':
                            // Reset the faceVertices vector
                            faceVerticesIndices.clear();
                            if(printReading == 's')
                            {
                                cout << "*********************************" << '\n';
                                cout << "******** Found a face (f) *******" << '\n';
                                cout << "*********************************" << "\n\n";
                            }

                            // Get a vector of Vertex that describes the face that is reading
                            faceVerticesIndices = getFaceWithLine(lineContent, meshes.back(), lastVertexIndex);
                            // Create a new face in current mesh with vector faceVertices
                            if(meshes.back().addFace(faceVerticesIndices)) {
                                if(printReading == 's')
                                {
                                    meshes.back().getLastFace().showFaceFormatted();
                                    cout << "--- Successfully stored in mesh " << meshes.back().getMeshName();
                                    cout << "---" << "\n\n";
                                }
                            }
                            else
                                cout << "~~~ Insufficient vertices to make a face! ~~~" << "\n\n";
                            break;
                    }
            }
            // Ask to print info stored in vector<Mesh> as the original file
            if(printReading == 's')
                printInfoAsFile(meshes);

            // Return true when the reading finished
            return meshes;
        }
    } else
    cout << "******** Invalid file extension ********" << '\n';

    // If the file couldn't be read
    // Show message in console
    cout << "******** Failed to read the file ********" << '\n';
    // Return false
    return {};
}

vector<int> OBJFileReader::getFaceWithLine(string currentFileLine, Mesh currentMesh, int lastVrtxIndexInPrevMesh) {
    if(currentFileLine[0] == 'f') {
        // Vector that contains the vertices that conforms the face
        vector<int> verticesOfFace;
        // Index of the vertex at the vector of Vertex in currentMesh
        int vertexIndex;
        // Delete the f id from file
        currentFileLine[0] = ' ';
        // Variable to convert the string numbers to integer variables
        stringstream ssCurrentFileLine(currentFileLine);

        // Read all the vertices that conforms the face
        while(ssCurrentFileLine >> vertexIndex)
            verticesOfFace.push_back(vertexIndex-1);
            //verticesOfFace.push_back(currentMesh.getVertexAddress((vertexIndex-1)-lastVrtxIndexInPrevMesh));

        return verticesOfFace;
    }
    cout << "<<No face char 'f' was found in file>>" << '\n';
    return {};
}

vector<double> OBJFileReader::getVertexCoordinatesWithLine(string currentFileLine) {
    // Validate that currentFileLine is reading a Vertex
    if (currentFileLine[0] == 'v') {

        // Delete the v id from file
        currentFileLine[0] = ' ';
        // Vector that contains the coordinates as doubles
        vector<double> vertexCoordinates;
        // Variable to convert the string numbers to double numbers
        stringstream ssCurrentFileLine(currentFileLine);
        double x, y, z;

        // Read the coordinates from file and add them to the vector of coordinates
        ssCurrentFileLine >> x;
        vertexCoordinates.push_back(x);
        ssCurrentFileLine >> y;
        vertexCoordinates.push_back(y);
        ssCurrentFileLine >> z;
        vertexCoordinates.push_back(z);

        // Return the vector containing the coordinates
        return vertexCoordinates;
    }
    cout << "<<No vertex char 'v' was found in file>>" << '\n';
    return {};
}

string OBJFileReader::getMeshNameWithLine(string currentFileLine) {
    // Validate that currentFileLine is reading an Object
    if (currentFileLine[0] == 'o') {

        // Delete the o id from file
        currentFileLine[0] = ' ';
        stringstream ssCurrentFileLine(currentFileLine);
        // Local meshName
        string meshName;

        // Read the name from file
        ssCurrentFileLine >> meshName;
        // Return the name
        return meshName;
    }
    cout << "<<No object char 'o' was found in file>>" << '\n';
    return nullptr;
}

void OBJFileReader::printInfoAsFile(vector<Mesh> meshes) {
    char userResponse;
    do
    {
        cout << '\n';
        cout << "Want to print the information stored in vector<Mesh> as the original .obj file? (Y/n) ";
        cin >> userResponse;
        userResponse = toupper(userResponse);
    }while(userResponse != 'N' && userResponse != 'Y');
	if(userResponse == 'Y') {
		// Print the meshes
		for (int mesh = 0; mesh < (int)meshes.size(); mesh++) {
		    cout << "o " << meshes[mesh].getMeshName() << '\n';
		    // Print the vertices of mesh
		    for(Vertex vertex : meshes[mesh].getVertices()) {
		        cout << "v ";
		        vertex.showCoordinates();
		    }
		    // Print the faces of mesh
		    for(Face face : meshes[mesh].getFaces()) {
		        cout << "f ";
		        face.showFace();
		    }
		}
	}
}

bool OBJFileReader::validateFile(string fileName) {
    return false;
}

void OBJFileReader::closeFile() {
    fileReader.close();
}