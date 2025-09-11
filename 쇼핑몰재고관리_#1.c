#include<stdio.h>
int main()
{
    int type,id;
    scanf("%d",&type);
    if(type<1 || type>100)
    {
        printf("Invalid Input");
        return 0;
    }
    int ib[type],ob[type];//ib=inbound ob=outbound
    for(int i=0;i<type;i++)
    {
        scanf("%d",&ib[i]);
    }
    for(int i=0;i<type;i++)
    {
        scanf("%d",&ob[i]);
    }
    scanf("%d",&id);
    printf("%d\n",ib[id-1]-ob[id-1]);
    for(int i=0;i<type;i++)
    {
        printf("%d ",ib[i]-ob[i]);
    }

}