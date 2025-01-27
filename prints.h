#ifndef PRINTS_H_
#define PRINTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
  float x;
  float y;
  float z;
  float intensity;
} data;

#pragma pack(push, 1)

typedef struct {
    char fileSignature[4];            // File Signature ("LASF")
    uint16_t fileSourceID;            // File Source ID
    uint16_t globalEncoding;          // Global Encoding
    uint32_t projectID_GUIDData1;     // Project ID - GUID Data 1
    uint16_t projectID_GUIDData2;     // Project ID - GUID Data 2
    uint16_t projectID_GUIDData3;     // Project ID - GUID Data 3
    uint8_t projectID_GUIDData4[8];   // Project ID - GUID Data 4
    uint8_t versionMajor;             // Version Major
    uint8_t versionMinor;             // Version Minor
    char systemIdentifier[32];        // System Identifier
    char generatingSoftware[32];      // Generating Software
    uint16_t fileCreationDayOfYear;   // File Creation Day of Year
    uint16_t fileCreationYear;        // File Creation Year
    uint16_t headerSize;              // Header Size
    uint32_t offsetToPointData;       // Offset to Point Data
    uint32_t numVariableLengthRecords;// Number of Variable Length Records
    uint8_t pointDataRecordFormat;    // Point Data Record Format
    uint16_t pointDataRecordLength;   // Point Data Record Length
    uint32_t legacyNumPointRecords;   // Legacy Number of Point Records
    uint32_t legacyNumPointByReturn[5]; // Legacy Number of Point by Return
    double xScaleFactor;              // X Scale Factor
    double yScaleFactor;              // Y Scale Factor
    double zScaleFactor;              // Z Scale Factor
    double xOffset;                   // X Offset
    double yOffset;                   // Y Offset
    double zOffset;                   // Z Offset
    double maxX;                      // Max X
    double minX;                      // Min X
    double maxY;                      // Max Y
    double minY;                      // Min Y
    double maxZ;                      // Max Z
    double minZ;                      // Min Z
    uint64_t startWaveformDataPacketRecord; // Start of Waveform Data Packet Record
    uint64_t startFirstExtendedVarLengthRecord; // Start of First Extended Variable Length Record
    uint32_t numExtendedVarLengthRecords; // Number of Extended Variable Length Records
    uint64_t numPointRecords;
    uint64_t numPointsByReturn[15];

} LASFHeader;

void printDebug(LASFHeader a);

#pragma pack(pop)

#pragma pack(push, 1)

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

} PointDataRecord;

#pragma pack(pop)

void printPointDataRecord(PointDataRecord *record);

#endif // !PRINTS_H_
