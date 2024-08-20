

void delay(int N){
    for(int i = 0; i < N * 1000; i++);
}


int main(void)
{
    int x = 0;

    while (x < 100)
    {
        x += 1;
        delay(10);
    }
    
    return 0;
}
