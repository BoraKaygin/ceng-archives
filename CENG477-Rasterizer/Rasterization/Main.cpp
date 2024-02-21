#include <iostream>
#include <vector>
#include "Scene.h"
#include "Matrix4.h"
#include "Helpers.h"
#include "ModelingTransformations.h"
#include "ViewingTransformations.h"

using namespace std;

Scene *scene;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Please run the rasterizer as:" << endl
             << "\t./rasterizer <input_file_name>" << endl;
        return 1;
    }
    else
    {
        const char *xmlPath = argv[1];

        scene = new Scene(xmlPath);

        for(auto & mesh : scene->meshes){ // meshleri gez, her bir mesh için:
            Matrix4 modelingTransformationMatrix = getIdentityMatrix();
            for(int j=0; j < mesh->numberOfTransformations; j++){ // transformationları gez, her bir transformation için;
                int transformationID = mesh->transformationIds[j];
                if(mesh->transformationTypes[j] == 't'){
                    Matrix4 translationMatrix = getTranslationMatrix(scene->translations[transformationID-1]);
                    modelingTransformationMatrix = multiplyMatrixWithMatrix(translationMatrix,modelingTransformationMatrix);
                } else if(mesh->transformationTypes[j] == 's'){
                    Matrix4 scalingMatrix = getScalingMatrix(scene->scalings[transformationID-1]);
                    modelingTransformationMatrix = multiplyMatrixWithMatrix(scalingMatrix,modelingTransformationMatrix);
                } else {
                    Matrix4 rotationMatrix = getRotationMatrix(scene->rotations[transformationID-1]);
                    modelingTransformationMatrix = multiplyMatrixWithMatrix(rotationMatrix,modelingTransformationMatrix);
                }
            }
            mesh->modelingTransformationMatrix = modelingTransformationMatrix;
        }

        for(auto & camera : scene-> cameras){
            Matrix4 viewingTransformationMatrix = getCameraTransformationMatrix(camera);
            if(camera->projectionType == 0){ // ortographic
                Matrix4 orthographicProjectionMatrix = getOrthographicProjectionMatrix(camera);
                viewingTransformationMatrix = multiplyMatrixWithMatrix(orthographicProjectionMatrix,viewingTransformationMatrix);
            }
            else{ // perspective
                Matrix4 perspectiveProjectionMatrix = getPerspectiveProjectionMatrix(camera);
                viewingTransformationMatrix = multiplyMatrixWithMatrix(perspectiveProjectionMatrix,viewingTransformationMatrix);
            }
            camera->viewingTransformationMatrix = viewingTransformationMatrix;
            int n_x = camera->horRes, n_y = camera->verRes;
            double M_vp[4][4] = {
                    {n_x/2.0,0,0,(n_x-1)/2.0},
                    {0,n_y/2.0,0,(n_y-1)/2.0},
                    {0,0,0.5,0.5},
                    {0,0,0,1}
            };
            Matrix4 viewportTransformationMatrix = Matrix4(M_vp);
            camera->viewportTransformationMatrix = viewportTransformationMatrix;
        }

        for (int i = 0; i < scene->cameras.size(); i++)
        {
            // initialize image with basic values
            scene->initializeImage(scene->cameras[i]);

            // do forward rendering pipeline operations
            scene->forwardRenderingPipeline(scene->cameras[i]);

            // generate PPM file
            scene->writeImageToPPMFile(scene->cameras[i]);

            // Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
            // Change/remove implementation if necessary.
            //scene->convertPPMToPNG(scene->cameras[i]->outputFilename);
        }

        return 0;
    }
}