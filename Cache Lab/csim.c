#define _POSIX_C_SOURCE 200809L

#include "cachelab.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

long fastpow(long base,long x)
{
    long res = 1;
    while(x > 0)
    {
        if(x & 1){
            res *= base;
        }
        x >>= 1;
        base *= base;
    }
    return res;
}

char* initcache(int s,int E,int b)
{
    long S = fastpow(2,s);
    long B = fastpow(2,b);
    long line_len = 8 + B + 2;
    long line_count = S*E;
    
    char* start = (char*)malloc(line_count*line_len);
    if(!start)return NULL;
    
    char* en = start+8+B;
    for(int i=0;i<line_count;i++)
    {
        (*en) = '0';  //有效位
        (*(en+1)) = '0';  //lru计数
        en += line_len;
    }
    
    return start;
}

int operate(char* start,unsigned long addr,int s,int E,int b)
{
    unsigned long tag = addr >> (s+b);
    unsigned long B = fastpow(2,b);
    unsigned long s_num = (addr << (64 - s - b)) >> (64 - s);
    int line_len = B + 10;
    char* base = start + (s_num*E*line_len);
    char* temp = base;
    int hit = 0,used = 0,empty_line = 0;

    for(int i=0;i<E;i++) //查找
    {
        if((*(base+B+8)) == '1'){
            if(!(memcmp(&tag,base,8))){
                *(base+B+9) = '0';
                hit = 1;
            }
            else{
                (*(base+B+9))++;
            }
            used++;
        }
        else{
            empty_line = i;
        }
        base += line_len;
    }
    if(hit)return 0;

    base = temp;
    if(used == E){
        int rep_line = 0;
        char max_lru = 0;
        for(int i=0;i<E;i++)
        {
            if(*(base+B+9) > max_lru){
                max_lru = *(base+B+9);
                rep_line = i;
            }
            base += line_len;
        }
        
        char* rep = temp + (rep_line*line_len);
        memcpy(rep,&tag,8);
        *(rep + 9 + B) = '0';
        return 2;
    }

    else{
        char *empty = base + (empty_line*line_len);
        memcpy(empty,&tag,8);
        *(empty + 9 + B) = '0';
        *(empty + 8 + B) = '1';
        return 1;
    }
    


}

int main(int argc,char* argv[])
{
    int opt,v = 0,s=0,E=0,b=0;
    char* t = NULL;
    while((opt = getopt(argc,argv,"vs:E:b:t:"))!= -1)
    {
        switch(opt){
            case 'v':{
                v = 1;
                break;
            }
            case 's':{
                s = atoi(optarg);
                break;
            }
            case 'E':{
                E = atoi(optarg);
                break;
            }
            case 'b':{
                b = atoi(optarg);
                break;
            }
            case 't':{
                t = optarg;
            }
        }
    }
    
    if(!t)exit(1);
    FILE* f = fopen(t,"r");
    if(!f)exit(1);

    char* start = initcache(s,E,b);
    if(!start)exit(2);

    char buf[1024];
    int hits = 0,misses = 0,evis = 0;
    while(fgets(buf,sizeof(buf),f)){
        char mode;
        unsigned long addr;
        int size;
        sscanf(buf," %c %lx,%d",&mode,&addr,&size);
        if(mode == 'I')continue;
        if(v)printf("%c %lx,%d",mode,addr,size);
        
        int res = operate(start,addr,s,E,b);
        switch(res){
            case 0:{
                hits++;
                if(v)printf(" hit");
                break;
            }
            case 1:{
                misses++;
                if(v)printf(" miss");
                break;
            }
            case 2:{
                evis++;
                misses++;
                if(v)printf(" miss eviction");
                break;
            }
        }

        if(mode == 'M'){
            res = operate(start,addr,s,E,b);
            switch(res){
                case 0:{
                    hits++;
                    if(v)printf(" hit");
                    break;
                }
                case 1:{
                    misses++;
                    if(v)printf(" miss");
                    break;
                }
                case 2:{
                    evis++;
                    misses++;
                    if(v)printf(" miss eviction");
                    break;
                }
            }
        }


    }

    printSummary(hits, misses, evis);
    return 0;
}
