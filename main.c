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

  FILE* fp = fopen("./USGS_LPC_MT_RavalliGraniteCusterPowder_2019_B19_473046.las", "rb");

  if(fp) {
    fread(&header, sizeof(LASFHeader), 1, fp);
    printDebug(header);
  } else {
    printf("Failed to read header");
    return EXIT_FAILURE;
  }

  size_t numRecords = header.numPointRecords;

  PointDataRecord *structs = malloc(numRecords * header.pointDataRecordLength);


  fseek(fp, header.offsetToPointData, SEEK_SET);

  fread(structs, sizeof(PointDataRecord), numRecords, fp);


  /*for(int i=0; i < numRecords; ++i) {*/
  /*  if(structs[i].classification == 7){*/
  /*    printPointDataRecord(&structs[i]);*/
  /*  }*/
  /*}*/

  /*printPointDataRecord(&structs[100]);*/

  renderWindow(structs, header); 

  /*printPointDataRecord(&structs[101]);*/

  free(structs);

  return EXIT_SUCCESS;
}
