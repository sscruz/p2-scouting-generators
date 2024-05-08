#include "firmware/data.h"

float splitting_qqg( float x ){
  return 4*(1+x*x)/(3*(1-x));
}
float splitting_ggg( float x ){
  return 3*(1-x*(1-x))*(1-x*(1-x))/(x*(1-x));
}

float splitting_gqq( float x ){
  int nf=4;
  return nf*(x*x+(1-x)*(1-x))/2;
}

int main( int argc, char **argv)
{
  x_t table_qqg[512];
  x_t table_ggg[512];
  x_t table_gqq[512];

  
  float cutoff=0.1;
  int N_scan=100000;

  float yaxis_qqg[N_scan]; float cum_qqg;
  float yaxis_ggg[N_scan]; float cum_ggg;
  float yaxis_gqq[N_scan]; float cum_gqq;
  float xaxis[N_scan];
  
  for (int i=0; i<N_scan;++i){
    cum_qqg += splitting_qqg( cutoff + float(i)*(1-2*cutoff)/(N_scan-1));
    cum_ggg += splitting_ggg( cutoff + float(i)*(1-2*cutoff)/(N_scan-1));
    cum_gqq += splitting_gqq( cutoff + float(i)*(1-2*cutoff)/(N_scan-1));

    xaxis[i]=cutoff + float(i)*(1-2*cutoff)/(N_scan-1);
    yaxis_qqg[i]=cum_qqg;
    yaxis_ggg[i]=cum_ggg;
    yaxis_gqq[i]=cum_gqq;
  }
  for (int i=0; i<N_scan;++i){
    yaxis_qqg[i]/=cum_qqg;
    yaxis_ggg[i]/=cum_ggg;
    yaxis_gqq[i]/=cum_gqq;
  }

  int maxy_qqg=-1;
  int maxy_ggg=-1;
  int maxy_gqq=-1;
  
  for (int i=0; i<N_scan;++i){
    int y_qqg=floor(yaxis_qqg[i]*512);
    int y_gqq=floor(yaxis_gqq[i]*512);
    int y_ggg=floor(yaxis_ggg[i]*512);
    
    if (y_qqg > maxy_qqg){
      maxy_qqg=y_qqg; // update when theres a step.
      table_qqg[y_qqg]=int( xaxis[i] * (1 << 14) ); // multiplying to go to units of 2^(-14)
      //printf("%d %d\n",  y_qqg, int( xaxis[i] * (1 << 14) ));
    }   
    if (y_gqq > maxy_gqq){
      maxy_gqq=y_gqq; // update when theres a step.
      table_gqq[y_gqq]=int( xaxis[i] * (1 << 14) ); // multiplying to go to units of 2^(-14)
    }   
    if (y_ggg > maxy_ggg){
      maxy_ggg=y_ggg; // update when theres a step.
      table_ggg[y_ggg]=int( xaxis[i] * (1 << 14) ); // multiplying to go to units of 2^(-14)
    }   

  }

  FILE *f_header=fopen("luts.h","a");
  fprintf( f_header, "x_t table_qqg[512]={\n");
  for (int i=0; i<512;++i){
    fprintf( f_header, "   %d,\n", table_qqg[i]);
  }
  fprintf( f_header, "};\n\n");

  fprintf( f_header, "x_t table_gqq[512]={\n");
  for (int i=0; i<512;++i){
    fprintf( f_header, "   %d,\n", table_gqq[i]);
  }
  fprintf( f_header, "};\n\n");

  fprintf( f_header, "x_t table_ggg[512]={\n");
  for (int i=0; i<512;++i){
    fprintf( f_header, "   %d,\n", table_ggg[i]);
  }
  fprintf( f_header, "};\n\n");

  
  fclose(f_header);
  return 0;
  
}

