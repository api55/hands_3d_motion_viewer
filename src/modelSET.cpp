#include <modelSET.h>


ModelSet::ModelSet(){}
ModelSet::ModelSet(
                        QString INPUT_dynamicStringPart_IN,
                        QVector<QString> INPUT_ModelNamesQVect_IN,
                        QString INPUT_EXTENSSS_Mesh_IN,
                        QString INPUT_EXTENSSS_Skeleton_IN,
                        QString INPUT_EXTENSSS_Skin_IN,
                        QString INPUT_EXTENSSS_Limits_IN,
                        QString INPUT_EXTENSSS_VOI_IN
                  )
{

        models.resize( INPUT_ModelNamesQVect_IN.size() );
        models.squeeze();
        ///////////////////////////////////////////////////
        ///////////////////////////////////////////////////
        totalModels = models.size();
        ///////////////////////////////////////////////////
        ///////////////////////////////////////////////////

        for (int modelID=0; modelID<totalModels; modelID++)
        {
                models[modelID] = Model(      INPUT_dynamicStringPart_IN,
                                              INPUT_ModelNamesQVect_IN[modelID],
                                              INPUT_EXTENSSS_Mesh_IN,
                                              INPUT_EXTENSSS_Skeleton_IN,
                                              INPUT_EXTENSSS_Skin_IN,
                                              INPUT_EXTENSSS_Limits_IN,
                                              INPUT_EXTENSSS_VOI_IN
                                       );
        }
        /////////////////
        /////////////////
        total_AxDOFs = 0;
        /////////////////
        /////////////////

        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        has_OnlySkin = true;
        ///////////////////////////////////////////////////
        for (int modelID=0; modelID<totalModels; modelID++)
        {
            if (models[modelID].has_OnlySkin == false)
            {
                /////////////////////
                has_OnlySkin = false;
                /////////////////////
                break;
            }
        }
        ///////////////////////////////////////////////////
        handNUMB = 0;
        ///////////////////////////////////////////////////
        for (int modelID=0; modelID<totalModels; modelID++)
        {
            if (models[modelID].name.contains("Hand"))
            {
                handNUMB++;
            }
        }
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ModelSet::compute_Centroid4D_curr() // the **models[modelID].centroid4D_curr** should be copmuted first !!!
{

    //std::cout << "compute_Centroid4D_curr - MODELSET" << std::endl;

    this->centroid4D_curr << 0,0,0,1;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    int countModelsActive = 0;

    for (int modelID=0; modelID<totalModels; modelID++)
    {
        if (models[modelID].isRenderable)
        {
            ////////////////////
            countModelsActive++;
            ////////////////////
        }
    }

    //////////////////////////////////////////////////////////////// // so that there is a valid centroid
    if (countModelsActive == 0)     countModelsActive = totalModels; // to rotate around with the mouse
    //////////////////////////////////////////////////////////////// // for the GUI viewer

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (int modelID=0; modelID<totalModels; modelID++)
    {
        if (models[modelID].isRenderable || countModelsActive==totalModels)
        {
            this->centroid4D_curr(0) += models[modelID].centroid4D_curr(0);
            this->centroid4D_curr(1) += models[modelID].centroid4D_curr(1);
            this->centroid4D_curr(2) += models[modelID].centroid4D_curr(2);
        }
    }
            this->centroid4D_curr(0) /= (float)countModelsActive;
            this->centroid4D_curr(1) /= (float)countModelsActive;
            this->centroid4D_curr(2) /= (float)countModelsActive;
            this->centroid4D_curr(3)  = 1;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ModelSet::create_VerticesOfInterest( int voiTYPE )
{
    for (int modelID=0; modelID<totalModels; modelID++)
    {
        if (models[modelID].name.contains("Hand"))
        {
            models[modelID].create_VerticesOfInterest( voiTYPE );
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Normally the rest VOIs are found based on the above defined
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// "Source VOIs" through radius search with the use of a KD-tree.
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// In order to relax the dependencies of this viewer-software
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// (.ie the heavy dependency of PCL - http://pointclouds.org/)
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// we store all these VOIs in a txt file and just read this

    for (int modelID=0; modelID<totalModels; modelID++)
    {

            //////////////////////////////////////////////////////
            if (models[modelID].has_OnlySkin == false)   continue;
            //////////////////////////////////////////////////////

            QString pathVOIs = QString("VOIs/VOIs_Model_"+QString::number(modelID)+".txt");

                           ///////////////
            std::ifstream  VOIs_FILE;/////
                           VOIs_FILE.open( pathVOIs.toStdString().data() );
                           ///////////////

                           for (int fingID=0; fingID<models[modelID].VerticesOfInterest_Viz_Indices.size(); fingID++)
                           {
                               int          siz;
                               VOIs_FILE >> siz;

                               for (int iii=0; iii<siz; iii++)
                               {
                                   int                                                            vertexID;
                                   VOIs_FILE >>                                                   vertexID;
                                   models[modelID].VerticesOfInterest_Viz_Indices[fingID].append( vertexID );
                               }
                           }

                           //////////////////
                           VOIs_FILE.close();
                           //////////////////

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ModelSet::compute_VerticesOfInterest_Centroids()
{
        for (int modelID=0; modelID<totalModels; modelID++)
        {
            models[modelID].compute_VerticesOfInterest_Centroids();
        }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int ModelSet::get_1st_Object_s_modelID()
{

        for (int modelID=0; modelID<models.size(); modelID++)
        {
                /////////////////////////////////////////////////////
                if (models[modelID].has_OnlySkin)  continue; ////////
                else                               return    modelID;
                /////////////////////////////////////////////////////
        }

        //////////
        return -1;
        //////////

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

