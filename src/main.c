#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/glad.h"
#include "format6.h"
#include "renderWindow.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

int
main (int argc, char *argv[])
{

  if (argc == 1 || argc > 3 || !strstr (argv[1], ".las"))
    {
      printf (
          "usage: simplelasviewer [path] [--reduce-noise]\n\npath\t\t\tPath "
          "to .las file\n--reduce-noise\t\tAttempt to reduce Y-Scale "
          "miscolorings due to noise. Load time may increase\n");
      return EXIT_FAILURE;
    }

  int noiseFlag = 0;

  if (argc == 3 && !strcmp (argv[2], "--reduce-noise"))
    {
      noiseFlag = 1;
    }

  LASFHeader header;

  FILE *fp = fopen (argv[1], "rb");

  if (fp)
    {
      fread (&header, sizeof (LASFHeader), 1, fp);
    }
  else
    {
      printf ("Failed to read header");
      return EXIT_FAILURE;
    }

  if (header.pointDataRecordFormat != 6)
    {
      printf ("Point data format %d is not supported yet\n",
              header.pointDataRecordFormat);
      return EXIT_FAILURE;
    }

  size_t numRecords = header.numPointRecords;

  PointDataRecord *structs
      = malloc (numRecords * header.pointDataRecordLength);

  fseek (fp, header.offsetToPointData, SEEK_SET);
  fread (structs, sizeof (PointDataRecord), numRecords, fp);

  renderWindow (structs, header, noiseFlag);

  free (structs);

  return EXIT_SUCCESS;
}
