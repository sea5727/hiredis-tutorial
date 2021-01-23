#include<iostream>

int main(int argc, char * argv[]){
    std::cout << "hello world!!" << std::endl;

    int a = 10;
    char b = 'a';
    void * c = NULL;
    const char * d = "Hello World";
    const int e = 1000;
    printf("a:%p\n", &a);
    printf("b:%p\n", &b);
    printf("c:%p\n", &c);
    printf("d:%p\n", &d);
    printf("e:%p\n", &e);

    return 0;
}