#ifndef RECORDFORMATS_H_
#define RECORDFORMATS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
  float x;
  float y;
  float z;
  float intensity;
  float r;
  float g;
  float b;
  float ir;
  float ig;
  float ib;

} data;

typedef struct {
  float centerX;
  float centerY;
  float centerZ;

} modelBoundingBox;

#pragma pack(push, 1)

typedef struct {
    char fileSignature[4];
    uint16_t fileSourceID;
    uint16_t globalEncoding;
    uint32_t projectID_GUIDData1;
    uint16_t projectID_GUIDData2;
    uint16_t projectID_GUIDData3;
    uint8_t projectID_GUIDData4[8];
    uint8_t versionMajor;
    uint8_t versionMinor;
    char systemIdentifier[32];
    char generatingSoftware[32];
    uint16_t fileCreationDayOfYear;
    uint16_t fileCreationYear;
    uint16_t headerSize;
    uint32_t offsetToPointData;
    uint32_t numVariableLengthRecords;
    uint8_t pointDataRecordFormat;
    uint16_t pointDataRecordLength;
    uint32_t legacyNumPointRecords;
    uint32_t legacyNumPointByReturn[5];
    double xScaleFactor;
    double yScaleFactor;
    double zScaleFactor;
    double xOffset;
    double yOffset;
    double zOffset;
    double maxX;
    double minX;
    double maxY;
    double minY;
    double maxZ;
    double minZ;
    uint64_t startWaveformDataPacketRecord;
    uint64_t startFirstExtendedVarLengthRecord;
    uint32_t numExtendedVarLengthRecords;
    uint64_t numPointRecords;
    uint64_t numPointsByReturn[15];

} LASFHeader;


typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
    uint16_t intensity;

    uint8_t returnInfo;
    uint8_t classificationFlags;

    uint8_t classification;
    uint8_t userData;
    int16_t scanAngle;
    uint16_t pointSourceID;
    double gpsTime;

} PointDataRecord6;

#pragma pack(pop)

void printDebug(const LASFHeader* a);
/*void printPointDataRecord1(PointDataRecord1 *record);*/
/*void printPointDataRecord2(PointDataRecord2 *record);*/
/*void printPointDataRecord3(PointDataRecord3 *record);*/
/*void printPointDataRecord4(PointDataRecord4 *record);*/
/*void printPointDataRecord5(PointDataRecord5 *record);*/
void printPointDataRecord6(PointDataRecord6 *record);
/*void printPointDataRecord7(PointDataRecord7 *record);*/

#endif
