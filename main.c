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
#include <gsl/gsl_statistics_float.h>

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

  /*header.maxX *= 100.0f;*/
  /*header.maxY *= 100.0f;*/
  /*header.maxZ *= 100.0f;*/
  /*header.minX *= 100.0f;*/
  /*header.minY *= 100.0f;*/
  /*header.minZ *= 100.0f;*/

  printf("\n\n%f, %f, %f\n\n", header.maxX - header.minX, header.maxY - header.minY, header.maxZ - header.minZ);


  size_t numRecords = header.numPointRecords;

  PointDataRecord *structs = malloc(numRecords * header.pointDataRecordLength);

  /*for(int i = 0; i < numRecords; i++) {*/
  /**/
  /*}*/



  fseek(fp, header.offsetToPointData, SEEK_SET);

  fread(structs, sizeof(PointDataRecord), numRecords, fp);

  printPointDataRecord(&structs[100]);

  renderWindow(structs, header); 

  /*printPointDataRecord(&structs[101]);*/

  free(structs);

  return EXIT_SUCCESS;
}
