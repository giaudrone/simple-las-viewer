#include "prints.h"

void printDebug(LASFHeader a) {

  printf("fileSignature: %s\n", a.fileSignature);
  printf("fileSourceID: %u\n", a.fileSourceID);
  printf("globalEncoding: %u\n", a.globalEncoding);
  printf("projectID_GUIDData1: %u\n", a.projectID_GUIDData1);
  printf("projectID_GUIDData2: %u\n", a.projectID_GUIDData2);
  printf("projectID_GUIDData3: %u\n", a.projectID_GUIDData3);
  printf("projectID_GUIDData4: %s\n", a.projectID_GUIDData4);
  printf("legacyNumPointRecords: %d\n", a.legacyNumPointRecords);
  printf("versionMajor: %u\n", a.versionMajor);
  printf("versionMinor: %u\n", a.versionMinor);
  printf("systemIdentifier: %s\n", a.systemIdentifier);
  printf("generatingSoftware: %s\n", a.generatingSoftware);
  printf("fileCreationDayOfYear: %u\n", a.fileCreationDayOfYear);
  printf("fileCreationYear: %u\n", a.fileCreationYear);
  printf("headerSize: %u\n", a.headerSize);
  printf("offsetToPointData: %u\n", a.offsetToPointData);
  printf("numVariableLengthRecords: %u\n", a.numVariableLengthRecords);
  printf("pointDataRecordFormat: %u\n", a.pointDataRecordFormat);
  printf("pointDataRecordLength: %u\n", a.pointDataRecordLength);
  printf("legacyNumPointRecords: %u\n", a.legacyNumPointRecords);
  printf("legacyNumPointByReturn: %u\n", *a.legacyNumPointByReturn);
  printf("xScaleFactor: %f\n", a.xScaleFactor);
  printf("yScaleFactor: %f\n", a.yScaleFactor);
  printf("zScaleFactor: %f\n", a.zScaleFactor);
  printf("xOffset: %f\n", a.xOffset);
  printf("yOffset: %f\n", a.yOffset);
  printf("zOffset: %f\n", a.zOffset);
  printf("maxX: %f\n", a.maxX);
  printf("minX: %f\n", a.minX);
  printf("maxY: %f\n", a.maxY);
  printf("minY: %f\n", a.minY);
  printf("maxZ: %f\n", a.maxZ);
  printf("minZ: %f\n", a.minZ);
  printf("startWaveformDataPacketRecord: %lu\n", a.startWaveformDataPacketRecord);
  printf("startFirstExtendedVarLengthRecord: %lu\n", a.startFirstExtendedVarLengthRecord);
  printf("numExtendedVarLengthRecords: %u\n", a.numExtendedVarLengthRecords);
  printf("numPointRecords: %lu\n", a.numPointRecords);
  printf("numPointsByReturn: %lu\n", *a.numPointsByReturn);
  printf("\n");
} 

void printPointDataRecord(PointDataRecord *record) {
    // Print X, Y, Z
    printf("X: %d\n", record->x);
    printf("Y: %d\n", record->y);
    printf("Z: %d\n", record->z);

    // Print Intensity
    printf("Intensity: %u\n", record->intensity);

    // Debug packed bits in returnInfo
    uint8_t returnNumber = record->returnInfo & 0x0F;  // Extract Return Number (bits 0-3)
    uint8_t numReturns = (record->returnInfo >> 4) & 0x0F;  // Extract Number of Returns (bits 4-7)
    printf("Return Number: %u\n", returnNumber);
    printf("Number of Returns (Given Pulse): %u\n", numReturns);

    // Debug packed bits in classificationFlags
    uint8_t classificationFlags = record->classificationFlags & 0x0F;  // Extract Classification Flags (bits 0-3)
    uint8_t scannerChannel = (record->classificationFlags >> 4) & 0x03;  // Extract Scanner Channel (bits 4-5)
    uint8_t scanDirectionFlag = (record->classificationFlags >> 6) & 0x01;  // Extract Scan Direction Flag (bit 6)
    uint8_t edgeOfFlightLine = (record->classificationFlags >> 7) & 0x01;  // Extract Edge of Flight Line (bit 7)

    printf("Classification Flags: %u\n", classificationFlags);
    printf("Scanner Channel: %u\n", scannerChannel);
    printf("Scan Direction Flag: %u\n", scanDirectionFlag);
    printf("Edge of Flight Line: %u\n", edgeOfFlightLine);

    // Print Classification
    printf("Classification: %u\n", record->classification);

    // Print User Data
    printf("User Data: %u\n", record->userData);

    // Print Scan Angle
    printf("Scan Angle: %d\n", record->scanAngle);

    // Print Point Source ID
    printf("Point Source ID: %u\n", record->pointSourceID);

    // Print GPS Time
    printf("GPS Time: %f\n", record->gpsTime);

    printf("\n");
}
