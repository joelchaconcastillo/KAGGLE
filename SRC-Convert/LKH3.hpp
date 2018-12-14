#ifndef _INCL_LKH3
#define _INCL_LKH3
void createInputParamKHL3(char * fileInstance, char *tourfile, int id)
{
    FILE *file = fopen(fileInstance, "w");
    // Imprimir matriz
    fprintf(file, "PROBLEM_FILE = %s\n", tourfile);
    fprintf(file, "RUNS = 100000000000000\n");
 //   fprintf(file, "INPUT_TOUR_FILE = %s\n", tourfile);
    fprintf(file, "OUTPUT_TOUR_FILE= %s/../tmp/segmentfile_%d\n",currentDirectory,  id);
    fprintf(file, "MAX_TRIALS=1000\n");
    fprintf(file, "MAX_CANDIDATES=50\n");
   /// fprintf(file, "TOTAL_TIME_LIMIT = 100\n");
    //fprintf(file, "TRACE_LEVEL = 0\n");
    //fprintf(file, "POPULATION_SIZE = 10\n");
    //fprintf(file, "INIT_P = 0.5\n");
 //   fprintf(file, "CANDIDATE_FILE= %s/../Stage2/tmp/lkh-kagglesanta\n", currentDirectory);
 //   fprintf(file, "PI_FILE= %s/../Stage2/tmp/lkh-kagglesanta.pifile\n", currentDirectory);
    fprintf(file, "MOVE_TYPE=5\n");
    fprintf(file, "RUNS=1\n");
 //   fprintf(file, "SEED=1\n");

    fclose(file);

}
void LKH3_Procedure()
{

   //////Build matrix with virtual nodes and construct the input-param file that is required for the KHL tool
    constructMatrix(l, r); // 100 200
    int id = seed; 
    char temporalfile[1000];
    sprintf(temporalfile, "%s/../tmp/%d",currentDirectory, id );
    saveMatrix(temporalfile);

    char temporalconfigureationfile[1000];
    sprintf(temporalconfigureationfile, "%s/../tmp/%d_param", currentDirectory, id);

    char optimizedfile[1000];
    sprintf(optimizedfile, "%s/../tmp/segmentfile_%d", currentDirectory, id);

    createInputParamKHL3(temporalconfigureationfile, temporalfile , id);
    char command[1000];
    sprintf(command, "%s/../bin/LKH3 %s", currentDirectory, temporalconfigureationfile);
  /////Run the KHL tool 
    system(command);
  ////Read the new solution...
    readNewSolution(optimizedfile);
 ///get-back solution...
    transformPath();
    modifySolution();
}
#endif
