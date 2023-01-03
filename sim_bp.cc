#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_bp.h"
#include <math.h>
#include <iostream>

using namespace std;

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim bimodal 6 gcc_trace.txt
    argc = 4
    argv[0] = "sim"
    argv[1] = "bimodal"
    argv[2] = "6"
    ... and so on
*/


int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    bp_params params;       // look at sim_bp.h header file for the the definition of struct bp_params
    char outcome;           // Variable holds branch outcome
    unsigned long int addr; // Variable holds the address read from input file
    //PREDICTION VALUES
    int miss_pred = 0;
    int no_of_pred = 0;
    float miss_prediction_rate = 0;
    //BIMODAL DECLARATIONS
    int *pc_bm_table;
    int bm_mask;
    int M2_len;
    int index_bimodal;
    params.M2 = 0;
    params.M1 = 0;
    params.K = 0;
    params.N = 0;
    //GSHARE DECLARATIONS
    int *pc_gs_table;
    int gs_mask;
    int M1_len;
    int index_gshare;
    int temp;
    int glob_bra_hist_reg = 0b0;
    int glob_bra_hist_reg_temp = 0b0;
    //HYBRID DECLARATIONS
    int index_hybrid;
    int K_len;
    int *pc_hy_table;
    //DECLARING TAKEN OR NOT TAKEN
    char taken_or_not_taken_bm;
    char taken_or_not_taken_gs;



    if (!(argc == 4 || argc == 5 || argc == 7))
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.bp_name  = argv[1];
    
    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        if(argc != 4)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M2       = strtoul(argv[2], NULL, 10);
        trace_file      = argv[3];
        printf("COMMAND\n%s %s %lu %s\n", argv[0], params.bp_name, params.M2, trace_file);
        /************************BIMODAL ARRAY*****************************/
        M2_len = pow(2,params.M2);
        pc_bm_table= new int [M2_len];

        for (int i=0; i<M2_len; i++)
        {
            pc_bm_table[i] = 2;
        }

    }
    else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        if(argc != 5)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M1       = strtoul(argv[2], NULL, 10);
        params.N        = strtoul(argv[3], NULL, 10);
        trace_file      = argv[4];
        printf("COMMAND\n%s %s %lu %lu %s\n", argv[0], params.bp_name, params.M1, params.N, trace_file);

        /************************GSHARE ARRAY*****************************/
        M1_len = pow(2,params.M1);
            pc_gs_table = new int [M1_len];

            for (int i=0; i<M1_len; i++)
            {
                pc_gs_table[i] = 2;
            } 
    }
    else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        if(argc != 7)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.K        = strtoul(argv[2], NULL, 10);
        params.M1       = strtoul(argv[3], NULL, 10);
        params.N        = strtoul(argv[4], NULL, 10);
        params.M2       = strtoul(argv[5], NULL, 10);
        trace_file      = argv[6];
        printf("COMMAND\n%s %s %lu %lu %lu %lu %s\n", argv[0], params.bp_name, params.K, params.M1, params.N, params.M2, trace_file);
        /************************BIMODAL ARRAY*****************************/
        M2_len = pow(2,params.M2);
        pc_bm_table= new int [M2_len];
        
        for (int i=0; i<M2_len; i++)
        {
            pc_bm_table[i] = 2;
        }

        /************************GSHARE ARRAY*****************************/
        M1_len = pow(2,params.M1);
        pc_gs_table = new int [M1_len];

        for (int i=0; i<M1_len; i++)
        {
            pc_gs_table[i] = 2;
        } 

        /************************HYBRID ARRAY*****************************/
        K_len = pow(2,params.K);
        pc_hy_table = new int [K_len];

        for (int i=0; i<K_len; i++)
        {
            pc_hy_table[i] = 1;
        } 
    }
    else
    {
        printf("Error: Wrong branch predictor name:%s\n", params.bp_name);
        exit(EXIT_FAILURE);
    }
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
    
    char str[2];
    while(fscanf(FP, "%lx %s", &addr, str) != EOF)
    {
        
        outcome = str[0];
        no_of_pred++;
       /* if (outcome == 't')
            printf("%lx %s\n", addr, "t");           // Print and test if file is read correctly
        else if (outcome == 'n')
            printf("%lx %s\n", addr, "n");    */      // Print and test if file is read correctly
        /*************************************
            Add branch predictor code here
        **************************************/

       /****************BIMODAL BRANCH PREDICTOR****************/
       if(strcmp(params.bp_name, "bimodal") == 0)
       {
        addr = addr>>2;
        bm_mask = (M2_len-1);              //doing mask operation to get the actual value
        index_bimodal = addr & bm_mask;

        if (outcome == 't')
        {
            if (pc_bm_table[index_bimodal]<2)
                miss_pred++;

            if(pc_bm_table[index_bimodal]<3)
                pc_bm_table[index_bimodal]++;
            
        }
       else if (outcome == 'n')
        {
            if (pc_bm_table[index_bimodal]>=2)
                miss_pred++;

            if(pc_bm_table[index_bimodal]>0)
                pc_bm_table[index_bimodal]--;
        }
       }
       /****************END OF BIMODAL BRANCH PREDICTOR****************/

       /****************GSHARE BRANCH PREDICTOR****************/
        if(strcmp(params.bp_name, "gshare") == 0)
        {
            if(params.N != 0)
            {   
                addr = addr>>2;
                gs_mask = ((1<< params.M1)-1);
                temp = addr & gs_mask;
                glob_bra_hist_reg_temp = glob_bra_hist_reg<<(params.M1-params.N);
                index_gshare = glob_bra_hist_reg_temp ^ temp;
            }
            else
            {
                addr = addr>>2;
                gs_mask = (M1_len-1);              //doing mask operation to get the actual value
                index_gshare = addr & gs_mask;
            }

        if (outcome == 't')
        {
            if (pc_gs_table[index_gshare]<2)
                miss_pred++;

            if(pc_gs_table[index_gshare]<3)
                pc_gs_table[index_gshare]++;

                glob_bra_hist_reg = glob_bra_hist_reg>>1;
                glob_bra_hist_reg = glob_bra_hist_reg | (1<<(params.N-1));
            
        }
       else if (outcome == 'n')
        {
            if (pc_gs_table[index_gshare]>=2)
                miss_pred++;

            if(pc_gs_table[index_gshare]>0)
                pc_gs_table[index_gshare]--;

                glob_bra_hist_reg = glob_bra_hist_reg>>1;
        }
        }
       /****************END OF GSHARE BRANCH PREDICTOR****************/        
       /****************HYBRID BRANCH PREDICTOR****************/
       if(strcmp(params.bp_name, "hybrid") == 0) 
       {
            addr = addr>>2;
            index_hybrid = addr & ((1<<params.K)-1);
            bm_mask = (M2_len - 1);
            index_bimodal = addr & bm_mask;
            if(params.N != 0)
            {   
                gs_mask = ((1<< params.M1)-1);
                temp = addr & gs_mask;
                glob_bra_hist_reg_temp = glob_bra_hist_reg<<(params.M1-params.N);
                index_gshare = glob_bra_hist_reg_temp ^ temp;
            }
            else
            {
                gs_mask = (M1_len-1);              //doing mask operation to get the actual value
                index_gshare = addr & gs_mask;
            }
            if (pc_bm_table[index_bimodal]>=2)
            taken_or_not_taken_bm = 't';
            else 
            taken_or_not_taken_bm = 'n';

            if(pc_gs_table[index_gshare]>=2)
            taken_or_not_taken_gs = 't';
            else 
            taken_or_not_taken_gs ='n';
        
            if(pc_hy_table[index_hybrid]>=2)
            {
            if (outcome == 't')
            {
                if(taken_or_not_taken_gs == 'n') 
                {
                miss_pred++;
                }
                if(pc_gs_table[index_gshare]<3)
                pc_gs_table[index_gshare]++;
            }
            else if (outcome == 'n')
            {
            if (taken_or_not_taken_gs == 't')
                miss_pred++;

            if(pc_gs_table[index_gshare]>0)
                pc_gs_table[index_gshare]--;
            }
            }

            else if(pc_hy_table[index_hybrid]<2)
            {
            if (outcome == 't')
            {
            if (taken_or_not_taken_bm == 'n')
                miss_pred++;

            if(pc_bm_table[index_bimodal]<3)
                pc_bm_table[index_bimodal]++;            
            }
            else if (outcome == 'n')
            {
            if (taken_or_not_taken_bm == 't')
                miss_pred++;

            if(pc_bm_table[index_bimodal]>0)
                pc_bm_table[index_bimodal]--;}
            }

            if(outcome == 't')
            {
            glob_bra_hist_reg = glob_bra_hist_reg>>1;
            glob_bra_hist_reg = glob_bra_hist_reg | (1<<(params.N-1));
            }
            else  glob_bra_hist_reg = glob_bra_hist_reg>>1;

            if((outcome == taken_or_not_taken_gs) && (outcome != taken_or_not_taken_bm))
            {
            if(pc_hy_table[index_hybrid]<3)
            pc_hy_table[index_hybrid]++;
            }
            else if((outcome != taken_or_not_taken_gs) && (outcome == taken_or_not_taken_bm))
            {
            if(pc_hy_table[index_hybrid]>0)
            pc_hy_table[index_hybrid]--;
            }
       }
    }
       /****************END OF GSHARE BRANCH PREDICTOR****************/   
       /*************************PRINTING*************************/   
    cout<<" OUTPUT"<<endl;
    cout<<"  number of predictions:    "<<no_of_pred<<endl;
    cout<<"  number of mispredictions: "<<miss_pred<<endl;
    miss_prediction_rate = ((float)miss_pred / (float)no_of_pred)*100;
    printf("  misprediction rate:        %0.2f%% \n",miss_prediction_rate);

    if(strcmp(params.bp_name, "bimodal") == 0)              //BIMODAL
        {
         cout<<" FINAL BIMODAL CONTENTS"<<endl;
            for(int i=0; i < M2_len;i++)
                {
                     cout<<i<<"       "<<pc_bm_table[i]<<endl;
                }
        }
    else if(strcmp(params.bp_name, "gshare") == 0)              //GSHARE
        {
         cout<<" FINAL GSHARE CONTENTS"<<endl;
            for(int i=0; i < M1_len;i++)
                {
                    cout<<i<<"       "<<pc_gs_table[i]<<endl;
                }
        }
    else if(strcmp(params.bp_name, "hybrid") == 0)              //HYBRID
        {
         cout<<" FINAL CHOOSER CONTENTS"<<endl;
            for(int i=0; i < K_len;i++)
                {
                    cout<<i<<"       "<<pc_hy_table[i]<<endl;
                }
         cout<<" FINAL GSHARE CONTENTS"<<endl;
            for(int i=0; i < M1_len;i++)
                {
                    cout<<i<<"       "<<pc_gs_table[i]<<endl;
                }
         cout<<" FINAL BIMODAL CONTENTS"<<endl;
            for(int i=0; i < M2_len;i++)
                {
                     cout<<i<<"       "<<pc_bm_table[i]<<endl;
                }
        }
    return 0;

}
