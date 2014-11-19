#include "fingertipSet.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



FingertipSet::FingertipSet(){}


FingertipSet::FingertipSet( QString basePath_Detections_ForAllCameras_IN, QString myFileString_IndexCheat_IN )
{
        basePath_Detections_ForAllCameras = basePath_Detections_ForAllCameras_IN;

        QFile myFile ( myFileString_IndexCheat_IN );

        myFile.open(QIODevice::ReadOnly);

        if( !myFile.isOpen() )
        {
            qDebug() << "\n\n\n   Animation - ERROR, unable to open **" << myFileString_IndexCheat_IN << "** for IndexCheat Input \n\n\n";
            return;
        }

        QTextStream myStream(&myFile);

        QString dummyDescr;
        int     dummyValuy;

        myStream >> dummyDescr;       myStream >> totalAllignedFrames;      //qDebug() << dummyDescr << totalAllignedFrames;
        myStream >> dummyDescr;       myStream >> dummyValuy;               //qDebug() << dummyDescr << dummyValuy;
        myStream >> dummyDescr;       myStream >> videoOffset;              //qDebug() << dummyDescr << videoOffset;

        myFile.close();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void FingertipSet::read_Curr_Detections_fromFile( int camID, int frameNumber )
{

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        if (  (frameNumber < 0)  ||  (frameNumber >= totalAllignedFrames)  )
        {
          //qDebug() << "\n\n\n   VideoSequence::loadCurrentFrame - OutOfBounds ERROR   \n\n\n";
          //qDebug() << frameNumber << totalAllignedFrames << videoOffset;
            return;
        }

        frameNumber += videoOffset;
        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////

        if (basePath_Detections_ForAllCameras.endsWith("/") == false)
            basePath_Detections_ForAllCameras += "/";

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////

        QString myFileString_DETECTION = basePath_Detections_ForAllCameras + QString::number( camID + 1 ) + "/detections/" + QString::number( frameNumber ).rightJustified(3,'0',false) + ".txt"; // ok

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////

        QFile myFile( myFileString_DETECTION );

        myFile.open(QIODevice::ReadOnly);

        if( !myFile.isOpen() )
        {
          //qDebug() << "FingertipSet::read_Curr_Detections_fromFile - ERROR, unable to open **" << myFileString_DETECTION << "** for Detections Input";
            return;
        }

        QTextStream myStream(&myFile);

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////

        int    NUMB_detections;
        int    dummyINT;
        double dummyDOUBLE;

        ////////////////////////////
        myStream >> NUMB_detections;
        ////////////////////////////
        myStream >> dummyINT;
        myStream >> dummyINT;
        myStream >> dummyINT;
        /////////////////////

        ///////////////////////////////////////////////
        detectionsBag[camID].resize( NUMB_detections );
        ///////////////////////////////////////////////

        for (int ddd=0; ddd<NUMB_detections; ddd++)
        {
            myStream >> detectionsBag[camID][ddd].confidence;
            myStream >> detectionsBag[camID][ddd].topLeft_X;
            myStream >> detectionsBag[camID][ddd].topLeft_Y;
            myStream >> detectionsBag[camID][ddd].height;
            myStream >> detectionsBag[camID][ddd].width;
            myStream >> dummyDOUBLE;
            myStream >> dummyINT;

            detectionsBag[camID][ddd].center_X = (double)detectionsBag[camID][ddd].topLeft_X + (double)detectionsBag[camID][ddd].width  / (double)2;
            detectionsBag[camID][ddd].center_Y = (double)detectionsBag[camID][ddd].topLeft_Y + (double)detectionsBag[camID][ddd].height / (double)2;

            detectionsBag[camID][ddd].centroid_PCL_3D << std::numeric_limits<float>::quiet_NaN(), // will be computed later
                                                         std::numeric_limits<float>::quiet_NaN(), // by packprojecting
                                                         std::numeric_limits<float>::quiet_NaN(); // cropped PCL that lives in there

            detectionsBag[camID][ddd].isAssigned_And_Overlooked = false; // dummy
            detectionsBag[camID][ddd].isAttractedByCenterBB     = false; // dummy
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////

        myFile.close();

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void FingertipSet::READ_and_FILTER_detections_ALL_cameras( int totalCameras, int frameNumber, double detectionConfidenceTHRESH )
{
        for (int camID=0; camID<totalCameras; camID++)
        {
                read_Curr_Detections_fromFile( camID, frameNumber );

                filter_DetectionBag( camID, detectionConfidenceTHRESH );
        }
}


void FingertipSet::filter_DetectionBag( int camID, double detectionConfidenceTHRESH )
{
        for (int ddd=detectionsBag[camID].size()-1; ddd>=0; ddd--)
        {
                if (detectionsBag[camID][ddd].confidence < detectionConfidenceTHRESH)
                {
                    detectionsBag[camID].remove( ddd );
                }
        }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

