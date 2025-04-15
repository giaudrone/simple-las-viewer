#include "recordFormats.h"

inline void printDebug(const LASFHeader* header) {

  printf("fileSignature: %s\n", header->fileSignature);
  printf("fileSourceID: %u\n", header->fileSourceID);
  printf("globalEncoding: %u\n", header->globalEncoding);
  printf("projectID_GUIDData1: %u\n", header->projectID_GUIDData1);
  printf("projectID_GUIDData2: %u\n", header->projectID_GUIDData2);
  printf("projectID_GUIDData3: %u\n", header->projectID_GUIDData3);
  printf("projectID_GUIDData4: %s\n", header->projectID_GUIDData4);
  printf("legacyNumPointRecords: %d\n", header->legacyNumPointRecords);
  printf("versionMajor: %u\n", header->versionMajor);
  printf("versionMinor: %u\n", header->versionMinor);
  printf("systemIdentifier: %s\n", header->systemIdentifier);
  printf("generatingSoftware: %s\n", header->generatingSoftware);
  printf("fileCreationDayOfYear: %u\n", header->fileCreationDayOfYear);
  printf("fileCreationYear: %u\n", header->fileCreationYear);
  printf("headerSize: %u\n", header->headerSize);
  printf("offsetToPointData: %u\n", header->offsetToPointData);
  printf("numVariableLengthRecords: %u\n", header->numVariableLengthRecords);
  printf("pointDataRecordFormat: %u\n", header->pointDataRecordFormat);
  printf("pointDataRecordLength: %u\n", header->pointDataRecordLength);
  printf("legacyNumPointRecords: %u\n", header->legacyNumPointRecords);
  printf("legacyNumPointByReturn: %u\n", *header->legacyNumPointByReturn);
  printf("xScaleFactor: %f\n", header->xScaleFactor);
  printf("yScaleFactor: %f\n", header->yScaleFactor);
  printf("zScaleFactor: %f\n", header->zScaleFactor);
  printf("xOffset: %f\n", header->xOffset);
  printf("yOffset: %f\n", header->yOffset);
  printf("zOffset: %f\n", header->zOffset);
  printf("maxX: %f\n", header->maxX);
  printf("minX: %f\n", header->minX);
  printf("maxY: %f\n", header->maxY);
  printf("minY: %f\n", header->minY);
  printf("maxZ: %f\n", header->maxZ);
  printf("minZ: %f\n", header->minZ);
  printf("startWaveformDataPacketRecord: %lu\n", header->startWaveformDataPacketRecord);
  printf("startFirstExtendedVarLengthRecord: %lu\n", header->startFirstExtendedVarLengthRecord);
  printf("numExtendedVarLengthRecords: %u\n", header->numExtendedVarLengthRecords);
  printf("numPointRecords: %lu\n", header->numPointRecords);
  printf("numPointsByReturn: %lu\n", *header->numPointsByReturn);
  printf("\n");
} 

inline void printPointDataRecord6(PointDataRecord6 *record) {
    printf("X: %d\n", record->x);
    printf("Y: %d\n", record->y);
    printf("Z: %d\n", record->z);

    printf("Intensity: %u\n", record->intensity);

    uint8_t returnNumber = record->returnInfo & 0x0F;
    uint8_t numReturns = (record->returnInfo >> 4) & 0x0F;
    printf("Return Number: %u\n", returnNumber);
    printf("Number of Returns (Given Pulse): %u\n", numReturns);

    uint8_t classificationFlags = record->classificationFlags & 0x0F;
    uint8_t scannerChannel = (record->classificationFlags >> 4) & 0x03;
    uint8_t scanDirectionFlag = (record->classificationFlags >> 6) & 0x01;
    uint8_t edgeOfFlightLine = (record->classificationFlags >> 7) & 0x01;

    printf("Classification Flags: %u\n", classificationFlags);
    printf("Scanner Channel: %u\n", scannerChannel);
    printf("Scan Direction Flag: %u\n", scanDirectionFlag);
    printf("Edge of Flight Line: %u\n", edgeOfFlightLine);

    printf("Classification: %u\n", record->classification);

    printf("User Data: %u\n", record->userData);

    printf("Scan Angle: %d\n", record->scanAngle);

    printf("Point Source ID: %u\n", record->pointSourceID);

    printf("GPS Time: %f\n", record->gpsTime);

    printf("\n");
}
