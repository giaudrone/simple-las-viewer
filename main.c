#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include "prints.h"
#include "renderWindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>


int main() {

  LASFHeader header;

  FILE* fp = fopen("testoutput.las", "rb");

  if(fp) {
    fread(&header, sizeof(LASFHeader), 1, fp);
    printDebug(header);
  } else {
    printf("Failed to read header");
    return EXIT_FAILURE;
  }

  header.maxX *= 100.0f;
  header.maxY *= 100.0f;
  header.maxZ *= 100.0f;
  header.minX *= 100.0f;
  header.minY *= 100.0f;
  header.minZ *= 100.0f;

  size_t numRecords = header.numPointRecords;

  PointDataRecord *structs = malloc(numRecords * header.pointDataRecordLength);

  fseek(fp, header.offsetToPointData, SEEK_SET);

  fread(structs, sizeof(PointDataRecord), numRecords, fp);

  printPointDataRecord(&structs[100]);
/*int minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX;*/
/*int maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN;*/
/**/
/*for (size_t i = 0; i < numRecords; i++) {*/
/*    if (structs[i].x < minX) minX = structs[i].x;*/
/*    if (structs[i].y < minY) minY = structs[i].y;*/
/*    if (structs[i].z < minZ) minZ = structs[i].z;*/
/*    if (structs[i].x > maxX) maxX = structs[i].x;*/
/*    if (structs[i].y > maxY) maxY = structs[i].y;*/
/*    if (structs[i].z > maxZ) maxZ = structs[i].z;*/
/*  }*/

/*printf("Bounds: X[%d, %d], Y[%d, %d], Z[%d, %d]\n", minX, maxX, minY, maxY, minZ, maxZ);*/
/*  for (size_t i = 0; i < numRecords; i++) {*/
/*    float normalizedX = (structs[i].x - header.minX) / (float)(header.maxX - header.minX);*/
/*    float normalizedY = (structs[i].y - header.minY) / (float)(header.maxY - header.minY);*/
/*    float normalizedZ = (structs[i].z - header.minZ) / (float)(header.maxZ - header.minZ);*/
/**/
/*    printf("Point %zu: Normalized Position (X, Y, Z) = (%f, %f, %f)\n", */
/*           i, normalizedX, normalizedY, normalizedZ);*/
/*}*/



  renderWindow(structs, header); 

  /*printPointDataRecord(&structs[101]);*/

  free(structs);

  return EXIT_SUCCESS;
}
