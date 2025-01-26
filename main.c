#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include "prints.h"
#include "load.h"
#include "renderWindow.h"

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

  size_t numRecords = header.numPointRecords;

  PointDataRecord *structs = malloc(numRecords * sizeof(PointDataRecord));

  fseek(fp, header.offsetToPointData, SEEK_SET);

  fread(structs, sizeof(PointDataRecord), numRecords, fp);

  /*printPointDataRecord(&structs[100]);*/

  /*loadToBuffers(structs, header);*/

  int a = 0;

  for(int i = 0; i < header.numPointRecords; i++) {
    structs[i].x -= (header.minX * 100);
    structs[i].y -= (header.minY * 100);
    structs[i].z -= (header.minZ * 100);
    /*if(structs[i].intensity > a) {*/
    /*  a = structs[i].intensity;*/
    /*} else if (structs[i].intensity == 0){*/
    /*  printf("i = %d\n", i);*/
    /*  break;*/
    /*}*/
    /*printf("%d\n", i);*/
  }

  /*int newx = 0, newy = 0, newz = 0;*/
  /**/
  /*for(int i = 0; i < header.numPointRecords; i++) {*/
  /*  if(structs[i].x > newx) newx = structs[i].x;*/
  /*  if(structs[i].y > newy) newy = structs[i].y;*/
  /*  if(structs[i].z > newz) newz = structs[i].z;*/
  /*}*/
  /**/
  /*printf("%d, %d, %d", newx, newy, newz);*/

  /*printPointDataRecord(&structs[100]);*/

  renderWindow(structs, header); 


  return EXIT_SUCCESS;
}
