int get_weights(
      char filename[MAX_FILENAME]
      ,float w01[N_INPUT*N_H1]
      //,float w01[N_INPUT][N_H1]
      ,float b1[N_H1]
      ,float w12[N_H1*N_H2]
      //,float w12[N_H1][N_H2]
      ,float b2[N_H2]
      ,float w23[N_H2*N_OUTPUT]
      //,float w23[N_H2][N_OUTPUT]
      ,float b3[N_OUTPUT]
      ) {
   FILE *readfp;
   if ((readfp = fopen(filename, "rb")) == NULL) {
      printf("Can't open a file.");
      return 1;
   }
   int i,j;
   for (i=0;i<N_INPUT;i++) {
      for (j=0;j<N_H1;j++) {
         fread(&w01[i*N_H1+j], sizeof(w01[i*N_H1+j]), 1, readfp);
         //fread(&w01[i][j], sizeof(w01[i][j]), 1, readfp);
      }
   }
   for (i=0;i<N_H1;i++) {
      fread(&b1[i], sizeof(b1[i]), 1, readfp);
   }
   for (i=0;i<N_H1;i++) {
      for (j=0;j<N_H2;j++) {
         fread(&w12[i*N_H2+j], sizeof(w12[i*N_H2+j]), 1, readfp);
         //fread(&w12[i][j], sizeof(w12[i][j]), 1, readfp);
      }
   }
   for (i=0;i<N_H2;i++) {
      fread(&b2[i], sizeof(b2[i]), 1, readfp);
   }
   for (i=0;i<N_H2;i++) {
      for (j=0;j<N_OUTPUT;j++) {
         fread(&w23[i*N_OUTPUT+j], sizeof(w23[i*N_OUTPUT+j]), 1, readfp);
         //fread(&w23[i][j], sizeof(w23[i][j]), 1, readfp);
      }
   }
   for (i=0;i<N_OUTPUT;i++) {
      fread(&b3[i], sizeof(b3[i]), 1, readfp);
   }
   fclose(readfp);
   return 0;
}

int write_weights(
      char filename[MAX_FILENAME]
      ,float w01[N_INPUT*N_H1]
      //,float w01[N_INPUT][N_H1]
      ,float b1[N_H1]
      ,float w12[N_H1*N_H2]
      //,float w12[N_H1][N_H2]
      ,float b2[N_H2]
      ,float w23[N_H2*N_OUTPUT]
      //,float w23[N_H2][N_OUTPUT]
      ,float b3[N_OUTPUT]
      ) {
   FILE *writefp;
   if ((writefp=fopen(filename,"wb"))==NULL) {
      printf("Can't open a file");
      return 1;
   }
   int i,j;
   for (i=0;i<N_INPUT;i++) {
      for (j=0;j<N_H1;j++) {
         fwrite(&w01[i*N_H1+j],sizeof(w01[i*N_H1+j]),1,writefp);
         //fwrite(&w01[i][j],sizeof(w01[i][j]),1,writefp);
      }
   }
   for (i=0;i<N_H1;i++) {
      fwrite(&b1[i],sizeof(b1[i]),1,writefp);
   }
   for (i=0;i<N_H1;i++) {
      for (j=0;j<N_H2;j++) {
         fwrite(&w12[i*N_H2+j],sizeof(w12[i*N_H2+j]),1,writefp);
         //fwrite(&w12[i][j],sizeof(w12[i][j]),1,writefp);
      }
   }
   for (i=0;i<N_H2;i++) {
      fwrite(&b2[i],sizeof(b2[i]),1,writefp);
   }
   for (i=0;i<N_H2;i++) {
      for (j=0;j<N_OUTPUT;j++) {
         fwrite(&w23[i*N_OUTPUT+j],sizeof(w23[i*N_OUTPUT+j]),1,writefp);
         //fwrite(&w23[i][j],sizeof(w23[i][j]),1,writefp);
      }
   }
   for (i=0;i<N_OUTPUT;i++) {
      fwrite(&b3[i],sizeof(b3[i]),1,writefp);
   }
   fclose(writefp);
   return 0;
}
