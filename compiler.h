#ifndef COMPILER_H
#define COMPILER_H


class Compiler
{
public:
    static Compiler& getInstance(){
        static Compiler instance;
        return instance;
    }

    //TODO: finish these functions
    void Compile();
    void Run();
    void GetCompileInfo();

private:
    Compiler();
    Compiler(Compiler&) = delete;
    Compiler& operator=(Compiler&) = delete;
};

#endif // COMPILER_H
