#define _POSIX_C_SOURCE 200809L

#include "cachelab.h" 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*初始化缓存：每一行的结构为：前8字节为缓存tag（简化实现），然后是B = 2^b字节的数据，然后1字节作为有效位，1字节作为实现lru机制的计数器（这里假设lru计数不会超过255）
    共S = 2^s组，E行，每行8+B+2字节，需分配S*E*line_len字节内存*/
char* initcache(int s,int E,int b)
{
    long S = 2 << (s-1); //根据位数计算组数量和行容量
    long B = 2 << (b-1);
    long line_len = 8 + B + 2; 
    long line_count = S*E;
    
    char* start = (char*)malloc(line_count*line_len); //char*可以操作任意字节
    if(!start)return NULL; //分配失败
    
    char* en = start+8+B;
    for(int i=0;i<line_count;i++)
    {
        (*en) = '0';  //有效位
        (*(en+1)) = '0';  //lru计数
        en += line_len;
    }
    
    return start;
}

/* 模拟一次读/写操作，返回命中情况 */
int operate(char* start,unsigned long addr,int s,int E,int b)
{
    unsigned long tag = addr >> (s+b); // 前64-s-b位 缓存tag
    unsigned long B = 2 << (b-1); 
    unsigned long s_num = (addr << (64 - s - b)) >> (64 - s); // 目标组号
    int line_len = B + 10;
    char* base = start + (s_num*E*line_len); //组基址
    char* temp = base;
    int hit = 0,used = 0,empty_line = 0;

    for(int i=0;i<E;i++) //循环查找目标组
    {
        if((*(base+B+8)) == '1'){  // 检测有效位
            if(!(memcmp(&tag,base,8))){ //检测tag匹配
                *(base+B+9) = '0';  //更新lru计数器
                hit = 1; 
            }
            else{
                (*(base+B+9))++; //lru计数+1
            }
            used++; 
        }   //used 和 empty_line用于检测缓存组是否已满，否则empty_line为miss时加载到的缓存位置
        else{
            empty_line = i;
        }
        base += line_len;
    }
    if(hit)return 0; //命中

    base = temp;
    if(used == E){ // 未命中且缓存组满，需要替换掉某一行
        int rep_line = 0; 
        char max_lru = 0; 
        for(int i=0;i<E;i++) // 循环找lru计数器最大的行
        {
            if(*(base+B+9) > max_lru){ 
                max_lru = *(base+B+9);
                rep_line = i;
            }
            base += line_len; 
        }
        
        char* rep = temp + (rep_line*line_len); //被逐出的行基址
        memcpy(rep,&tag,8);
        *(rep + 9 + B) = '0';
        return 2;
    }

    else{ //未命中但缓存组未满，存入缓存并设置有效位和计数器
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
    while((opt = getopt(argc,argv,"vs:E:b:t:"))!= -1) //解析命令行参数
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
    FILE* f = fopen(t,"r"); //读取trace文件
    if(!f)exit(1);

    char* start = initcache(s,E,b); //初始化模拟缓存
    if(!start)exit(2); 

    char buf[1024];
    int hits = 0,misses = 0,evis = 0;
    while(fgets(buf,sizeof(buf),f)){ //读取一行trace
        
        /*trace行的格式类似于[空格]mode addr size
        mode指示这次操作是读/写/修改（读+写）/取指令，addr是被访问的主存地址，size表示读取的字节数
        若该次访问是取指令，则没有前置空格，本实验只考虑数据缓存。实际上的数据和指令分别缓存在L1数据缓存和L1指令缓存中*/
        
        char mode;
        unsigned long addr;
        int size;
        sscanf(buf," %c %lx,%d",&mode,&addr,&size); //获取参数
        if(mode == 'I')continue;
        if(v)printf("%c %lx,%d",mode,addr,size); 
        
        int res = operate(start,addr,s,E,b); //模拟缓存操作，返回值0代表命中，1代表miss，2代表miss且产生替换
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

        if(mode == 'M'){ //mode M为一次读一次写两次操作
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
