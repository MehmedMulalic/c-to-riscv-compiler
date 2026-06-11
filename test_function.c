/*
  Example for handling if-statements and expressions
*/
int main()
{
   int a,b,c,d,res;

   a=1;
   b=2;
   c=3;
   d=4;

   if (a<b)
     {
       res=(a+b+(d-c)+1)-((c-a)+(d-b)); /* Expected: 1*/
     }
   else
     {
       res=2;
     }

   /* gcc: warning implicit declaration, executable generated and correct */
   printf("Expected 1: %d\n", res); /* Expected: 1 */
}
